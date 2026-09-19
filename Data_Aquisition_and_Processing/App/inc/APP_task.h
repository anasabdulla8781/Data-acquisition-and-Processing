/*
 * APP_task.h
 *
 *  Created on: Mar 27, 2026
 *      Author: ANAS
 */

#ifndef INC_APP_TASK_H_
#define INC_APP_TASK_H_

#include "stdint.h"
#include "FreeRTOS_header.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"



/// Macros
#define BLUE_LED_DONE 0x01
#define GREEN_LED_DONE	0x02
#define BUTTON_PRESSED	0X04


typedef enum
{
	TEMPERATURE_MEASUREMENT,
	PRESSURE_MEASUREMENT,
	BUTTON_PRESS
}measurement_ID;


typedef struct
{
	uint32_t ID;
	uint32_t value;
}measurement;

typedef struct
{
	uint32_t temperature_from_task;
	uint32_t counter_from_isr;
}measurement2;


/// Queue Buffer
extern QueueHandle_t QueueBuffer;

/// Declarations for the semaphore , Mutex , Counting semaphore
extern SemaphoreHandle_t ButtonSemaphore;
extern SemaphoreHandle_t MutexSemaphore;
extern SemaphoreHandle_t CountingSemaphore;

/// Declaration for event group
extern EventGroupHandle_t EventHandle;


extern void app_task_init(void);

/// Differant task declarations
extern void task1s(void * data);
extern void task500ms (void * data);
extern void caliberation_task(void* data);
extern void load_task (void *data);
extern void task200ms(void*data);
extern void task500ms_2(void* data);
extern void task300ms (void * data);
extern void uarttask150ms (void* data);
char* task_state_to_str(eTaskState state);
void Button_task(void* data);
void Print_queue(void* data);
extern void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
void print_notificaiton_temperature(void* data);



extern TaskHandle_t buttontaskHandle;
extern TaskHandle_t task1Handle;

extern uint32_t temperature_measurement;

#endif /* INC_APP_TASK_H_ */
