/*
 * APP_task.c
 *
 *  Created on: Mar 27, 2026
 *  Excercise 5 - Case 2 - Make 2 LED tasks of 500ms delay , and add vtask delay in one task and add vtask delay until in another task
 *  			Make a busy loop in both the tasks
 *  Expectation - we should see the lagging behaviour of task with taskdelay
 *      Author: ANAS
 */


#include "APP_task.h"
#include "service.h"

uint32_t counter_task;

/// Adding the handles for the task
TaskHandle_t task1Handle;
TaskHandle_t task2Handle;
TaskHandle_t task_handle_caliberation;
TaskHandle_t task3Handle;
TaskHandle_t taskHandle_load;
TaskHandle_t taskHandle_UART;
TaskHandle_t buttontaskHandle;
TaskHandle_t queuetaskHandle;

/// Variables for checking the state
eTaskState task1_state;
eTaskState task2_state;
eTaskState task3_state;
eTaskState idleState;

volatile uint32_t maxcount = 0;
volatile uint32_t idlecount = 0;

volatile uint8_t led_task = 1;

/// Checking the task is created or not
BaseType_t ret1, ret2,ret3,ret4 , ret5 , ret6;

volatile uint32_t previous_idle_time;
uint32_t load;

char buffer_for_digit[50];
char full_string_buffer[100] = {0};


volatile uint8_t task_deleted = 0;

/// Manually creating the control block and stack for the static task
StaticTask_t task3TCB;
StackType_t task3Stack[256];

/// To check the remaining stack size
UBaseType_t stack_remaining1;
UBaseType_t stack_remaining2;
UBaseType_t stack_remaining3;
UBaseType_t stack_remaining4;

QueueHandle_t QueueBuffer;
SemaphoreHandle_t ButtonSemaphore;
SemaphoreHandle_t CountingSemaphore;
SemaphoreHandle_t MutexSemaphore;
EventGroupHandle_t EventHandle;

measurement sender_measurement;
measurement reciver_measurement;

measurement2 reciver_measurement2;

uint32_t temperature_measurement;
uint32_t pressure_measurement;


void app_task_init(void)
{
	/// Creating the event to store
	EventHandle = xEventGroupCreate();

	/// Creating hte key
	MutexSemaphore = xSemaphoreCreateMutex();

	/// 2 - Make the taskss
//	ret1 = xTaskCreate(task1s , "LED_TASK1" , 256, NULL , 2 , &task1Handle);
//	ret3 = xTaskCreate(print_notificaiton_temperature , "TEMP_NOTIF_FUNC",256,NULL,2,&task3Handle);

//	ret4 = xTaskCreateStatic(task500ms_2 , "LED_TASK3" , 256, NULL , 3 ,task3Stack , &task3TCB);

	ret3 = xTaskCreate(Button_task , "Button_task" , 128, NULL , 3 , &buttontaskHandle);
//	ret5 = xTaskCreate(Print_queue , "PRINT_QUEUE" , 128, NULL , 2 , &queuetaskHandle);
	ret6 = xTaskCreate(task500ms , "TASK1S" , 128, NULL , 2 , &task3Handle);

	/// Make the task - Static task
	ret4 = xTaskCreateStatic(task300ms , "LED_TASK3" , 128, NULL , 2 ,task3Stack , &task3TCB);
	/// Setting up the timer module with proper ARR and PSC
		timer2_set(999 , 7999);
	// 2 - Start the scheduler
	vTaskStartScheduler();
}


/// This task is for load measurement caliberation
void caliberation_task(void* data)
{
	/// 2 - Set the idle count to 0
	idlecount = 0;
	/// 3 - Block this task so that idle task will start running with the idle count 0
	vTaskDelay(pdMS_TO_TICKS(5000));
	/// 4 - Calculate maxcount
	maxcount = idlecount/5;
	idlecount = 0;
	/// 5 - Control came here after after 5s , start other tasks
	ret3 = xTaskCreate(load_task , "LOAD_TASK" , 128, NULL , 1 , &taskHandle_load);
	/// 6 - We dont need this task anymore . Delete it
	//vTaskDelete(NULL);
	vTaskSuspend(NULL);
}


void Print_queue(void* data)
{
	BaseType_t status;
	while (1)
	{
		/// will proceed further only if the data is available
		status = xQueueReceive(QueueBuffer,&reciver_measurement,portMAX_DELAY);

		if (status == pdPASS)
		{
			switch (reciver_measurement.ID)
			{
			case TEMPERATURE_MEASUREMENT:
				itoa_custom((uint32_t)reciver_measurement.value , buffer_for_digit);
				strcat_custom(full_string_buffer ,"Temperature is - ");
				strcat_custom(full_string_buffer ,buffer_for_digit);
				uart_print(full_string_buffer);
				full_string_buffer[0] = '\0';
				break;
			case PRESSURE_MEASUREMENT:
				itoa_custom((uint32_t)reciver_measurement.value , buffer_for_digit);
				strcat_custom(full_string_buffer ,"Pressure is - ");
				strcat_custom(full_string_buffer ,buffer_for_digit);
				uart_print(full_string_buffer);
				full_string_buffer[0] = '\0';
				break;
			case BUTTON_PRESS:
				itoa_custom((uint32_t)reciver_measurement.value , buffer_for_digit);
				strcat_custom(full_string_buffer ,"Button press count is - ");
				strcat_custom(full_string_buffer ,buffer_for_digit);
				uart_print(full_string_buffer);
				full_string_buffer[0] = '\0';
			}
		}
		else
		{
			uart_print("sensor fault");
		}

	}
}


void print_notificaiton_temperature(void* data)
{
	BaseType_t status;
	while (1)
	{
//		/// Wait for the notication with value to come
//		xTaskNotifyWait(0,0,&temperature_measurement,portMAX_DELAY);
		/// print the value we recieved over notification
		/// Making the full string to print over uart

		/// wait for the queue to come
		status = xQueueReceive(QueueBuffer,&reciver_measurement2,portMAX_DELAY);
		if (status == pdPASS)
		{
			strcat_custom(full_string_buffer ,"Temperature is - ");
			/// Making number to ascii
			itoa_custom((uint32_t)reciver_measurement2.temperature_from_task , buffer_for_digit);
			/// Combining all
			strcat_custom(full_string_buffer , buffer_for_digit);
			/// Print the full buffer
			uart_print(full_string_buffer);
			full_string_buffer[0] = '\0';

			strcat_custom(full_string_buffer ,"Timer counter is - ");
			/// Making number to ascii
			itoa_custom((uint32_t)reciver_measurement2.counter_from_isr , buffer_for_digit);
			/// Combining all
			strcat_custom(full_string_buffer , buffer_for_digit);
			/// Print the full buffer
			uart_print(full_string_buffer);
			full_string_buffer[0] = '\0';

		}

	}

}

void task1s(void * data)
{
	uint32_t value;
	measurement2 sender_measurement2;
	while(1)
	{
//		/// Now the task will not run in every 1s , it will wait for the semaphore from isr.
//		xSemaphoreTake(ButtonSemaphore,portMAX_DELAY);

		/// Wait for the notification to come
		xTaskNotifyWait(0,0,&value,portMAX_DELAY);

		/// copied the counter from isr to the struture object
		sender_measurement2.counter_from_isr = value;

		gpio_output_operations (gpiod_ptr , PORTD_GREEN_LED_PIN , SET_TOGGLE);
		temperature_measurement++;

		/// copied the temperature measurement to the structure object
		sender_measurement2.temperature_from_task = temperature_measurement;


		xQueueSend( QueueBuffer, &sender_measurement2, portMAX_DELAY);

		/// Task is completed .. now add everyting to the queue we created and share it

//		/// Completed the measurement . Now share the notification
//		xTaskNotify(task3Handle,temperature_measurement,eSetValueWithOverwrite);
		/// Completed the task .. Block for 1second
//		vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000));
	}
}


void task300ms (void * data)
{
	EventBits_t bits;
	TickType_t lastWakeTime = xTaskGetTickCount();
	while (1)
	{
		/// First collect the event , then only we will block using mutex - Collect the event . But dont clear it .. so other task can also see it
		xEventGroupWaitBits(EventHandle,(BUTTON_PRESSED),pdFALSE,pdTRUE,portMAX_DELAY);
		/// Get the current events
		bits = xEventGroupGetBits(EventHandle);
		if (bits & GREEN_LED_DONE)
		{
			vTaskDelay(pdMS_TO_TICKS(1));
			continue;
		}
		/// Taking the key
		xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
		/// Doing the operations
		uart_print("task300ms - Mid prio task - is Running ");
		gpio_output_operations (gpiod_ptr , PORTD_GREEN_LED_PIN , SET_TOGGLE);
		uart_print("task300ms - Mid prio task - is Completed ");
		xEventGroupSetBits(EventHandle, GREEN_LED_DONE);
		/// Releasing the key
		xSemaphoreGive(MutexSemaphore);
		/// Completed the task .. Block for 10 milli second ( to avoid repeating the task )
		vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));
	}
}

void task500ms (void * data)
{
	EventBits_t bits;
	TickType_t lastWakeTime = xTaskGetTickCount();
	while (1)
	{
		/// First collect the event , then only we will block using mutex - Collect the event . But dont clear it .. so other task can also see it
		xEventGroupWaitBits(EventHandle,(BUTTON_PRESSED),pdFALSE,pdTRUE,portMAX_DELAY);
		/// Get the current events
		bits = xEventGroupGetBits(EventHandle);
		if (bits & BLUE_LED_DONE)
		{
			vTaskDelay(pdMS_TO_TICKS(1));
			continue;
		}
		/// Taking the key
		xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
		uart_print("task500ms - Low prio task - is running");
		/// Adding a delay to see whether the control will go to the high prio task
		for (volatile uint32_t i = 0 ; i<10000 ; i++);
		gpio_output_operations (gpiod_ptr , PORTD_BLUE_LED_PIN , SET_TOGGLE);
		uart_print("task500ms - Low prio task - is Completed ");
		xEventGroupSetBits(EventHandle, BLUE_LED_DONE);
		/// Releasing the key
		xSemaphoreGive(MutexSemaphore);
		/// Completed the task .. Block for 10 milli second ( to avoid repeating the task )
		vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));
	}
}

void task200ms(void*data)
{
	TickType_t lastWakeTime = xTaskGetTickCount();
	while (1)
	{
		gpio_output_operations (gpiod_ptr , PORTD_RED_LED_PIN , SET_TOGGLE);

		pressure_measurement++;

		sender_measurement.value = pressure_measurement;
		sender_measurement.ID = PRESSURE_MEASUREMENT;

		xQueueSend( QueueBuffer, &sender_measurement, portMAX_DELAY);

		vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(200));
	}
}

void uarttask150ms (void* data)
{
	/// Find the time in starting
	TickType_t lastWakeTime = xTaskGetTickCount();
	while (1)
	{
		/// Converting the time to string format
		itoa_custom((uint32_t)lastWakeTime , buffer_for_digit);
		/// Add the strings which needed to be printed
		strcat_custom(full_string_buffer ,"[Time - ");
		strcat_custom(full_string_buffer , buffer_for_digit);
		strcat_custom(full_string_buffer , " Task 1 State is - ");
		strcat_custom(full_string_buffer ,task_state_to_str(task1_state));
		/// Print the full buffer
		uart_print(full_string_buffer);
		full_string_buffer[0] = '\0';

		strcat_custom(full_string_buffer ,"[Time - ");
		strcat_custom(full_string_buffer , buffer_for_digit);
		strcat_custom(full_string_buffer , " Task 2 State is - ");
		strcat_custom(full_string_buffer ,task_state_to_str(task2_state));
		/// Print the full buffer
		uart_print(full_string_buffer);
		full_string_buffer[0] = '\0';

		strcat_custom(full_string_buffer ,"[Time - ");
		strcat_custom(full_string_buffer , buffer_for_digit);
		strcat_custom(full_string_buffer , " Task 3 State is - ");
		strcat_custom(full_string_buffer ,task_state_to_str(task3_state));
		/// Print the full buffer
		uart_print(full_string_buffer);
		full_string_buffer[0] = '\0';

		uart_print(" ");
		uart_print(" ");
		uart_print(" ");

		/// Delaying the task
		vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(150));
	}
}


void load_task (void *data)
{
	uint32_t current_idle_time;
	uint32_t differance;
	TickType_t lastWake = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(1000));
		current_idle_time = idlecount;
		differance = current_idle_time - previous_idle_time;
		previous_idle_time = current_idle_time;
		load = 100 - ((differance*100)/maxcount);
	}
}

void task500ms_2(void* data)
{
	TickType_t lastWakeTime = xTaskGetTickCount();
	while(1)
	{
		xEventGroupWaitBits(EventHandle,(BLUE_LED_DONE | GREEN_LED_DONE),pdTRUE,pdTRUE,portMAX_DELAY);
		uart_print("task500ms - High prio task - is running");
		gpio_output_operations (gpiod_ptr , PORTD_RED_LED_PIN , SET_TOGGLE);
		uart_print("task500ms - High prio task - is completed ");
//		/ Blocking this task .. then only other tasks can run
		vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(500));
	}
}

void Button_task(void* data)
{
	while (1)
	{
		/// Step 1 - Taking the notification from ISR
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		/// Set the event for Button task
		xEventGroupSetBits(EventHandle, BUTTON_PRESSED);
		/// Wait for all the tasks to get complete
		xEventGroupWaitBits(EventHandle,(BLUE_LED_DONE | GREEN_LED_DONE),pdTRUE,pdTRUE,portMAX_DELAY);
		/// Clear the button press
		xEventGroupClearBits(EventHandle,BUTTON_PRESSED);
	}
}
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // You can debug here
    while(1)
    {
    	uart_print(" Stack overflow happened in - ");
    	uart_print(pcTaskName);
    }
}

void vApplicationIdleHook(void)
{
	counter_task++;
	idlecount++;
}

char* task_state_to_str(eTaskState state)
{
    switch (state)
    {
        case eRunning:   return "RUNNING";
        case eReady:     return "READY";
        case eBlocked:   return "BLOCKED";
        case eSuspended: return "SUSPENDED";
        case eDeleted:   return "DELETED";
        default:         return "UNKNOWN";
    }
}
