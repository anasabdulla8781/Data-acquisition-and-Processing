/*
 * M4_FPU.h
 *
 *  Created on: Dec 9, 2025
 *      Author: ANAS
 */

#ifndef INC_M4_FPU_H_
#define INC_M4_FPU_H_



#include <stdint.h>

/*
 * ------------- SYSTEM CONTROL BLOCK BASE -----------------
 * SCB base is fixed for Cortex-M4:
 */
#define SCB_BASE_ADDRESS     (0xE000ED00UL)

/*
 * -------- SCB Register Layout (minimal required) ---------
 * Only CPACR is needed for FPU enabling
 */
typedef struct
{
    volatile uint32_t CPUID;     // 0x00
    volatile uint32_t ICSR;      // 0x04
    volatile uint32_t VTOR;      // 0x08
    volatile uint32_t AIRCR;     // 0x0C
    volatile uint32_t SCR;       // 0x10
    volatile uint32_t CCR;       // 0x14
    volatile uint32_t SHPR1;     // 0x18
    volatile uint32_t SHPR2;     // 0x1C
    volatile uint32_t SHPR3;     // 0x20
    volatile uint32_t SHCRS;     // 0x24
    volatile uint32_t CFSR;      // 0x28
    volatile uint32_t HFSR;      // 0x2C
    volatile uint32_t DFSR;      // 0x30
    volatile uint32_t MMFAR;     // 0x34
    volatile uint32_t BFAR;      // 0x38
    volatile uint32_t AFSR;      // 0x3C
    volatile uint32_t PFR0;      // 0x40
    volatile uint32_t PFR1;      // 0x44
    volatile uint32_t DFR0;      // 0x48
    volatile uint32_t ADR;       // 0x4C
    volatile uint32_t MMFR0;     // 0x50
    volatile uint32_t MMFR1;     // 0x54
    volatile uint32_t MMFR2;     // 0x58
    volatile uint32_t MMFR3;     // 0x5C
    volatile uint32_t ISAR0;     // 0x60
    volatile uint32_t ISAR1;     // 0x64
    volatile uint32_t ISAR2;     // 0x68
    volatile uint32_t ISAR3;     // 0x6C
    volatile uint32_t ISAR4;     // 0x70
    uint32_t RESERVED0[5];       // gap
    volatile uint32_t CPACR;     // 0x88 → COPROCESSOR ACCESS CONTROL REGISTER
} SCB_structure;

/*
 * Create SCB pointer
 */
#define scb_ptr     ((SCB_structure *)SCB_BASE_ADDRESS)


#define application_interrupt_reset_control_Register		(volatile uint32_t* )0xE000ED0C


void FPU_Enable(void);
void software_reset(void);

/*
 *
 *
 */

#endif /* INC_M4_FPU_H_ */
