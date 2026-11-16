#include <stdint.h>

/* Complete startup code, used by every lesson except lesson 3
   (in lesson 3 YOU write it). Compare with lessons/03_startup/solution/startup.c */

extern uint32_t _estack;   /* start value of the stack pointer (top of RAM)   */
extern uint32_t _sidata;   /* where the initial .data values sit in FLASH      */
extern uint32_t _sdata;    /* start of .data in RAM                            */
extern uint32_t _edata;    /* end of .data in RAM                              */
extern uint32_t _sbss;     /* start of .bss in RAM                             */
extern uint32_t _ebss;     /* end of .bss in RAM                               */

int main(void);
void Reset_Handler(void);
void Default_Handler(void);

/* "weak" = a default that you can override by writing a function with the same name */
void NMI_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)   __attribute__((weak, alias("Default_Handler")));

__attribute__((section(".isr_vector"), used))
void (* const vector_table[])(void) = {
    (void (*)(void))&_estack,  /*  0: initial stack pointer (loaded into SP) */
    Reset_Handler,             /*  1: reset vector          (loaded into PC) */
    NMI_Handler,               /*  2: NMI                                    */
    HardFault_Handler,         /*  3: HardFault                              */
    0, 0, 0, 0, 0, 0, 0,       /*  4-10: MemManage, BusFault, UsageFault, reserved */
    SVC_Handler,               /* 11: SVCall                                 */
    0, 0,                      /* 12-13: DebugMon, reserved                  */
    PendSV_Handler,            /* 14: PendSV                                 */
    SysTick_Handler            /* 15: SysTick                                */
};

void Reset_Handler(void)
{
    /* 1. SP is already set by the hardware (value from address 0x0) */

    /* 2. copy initial values of .data from FLASH to RAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* 3. set .bss to zero */
    for (dst = &_sbss; dst < &_ebss; dst++) {
        *dst = 0;
    }

    /* 4. SystemInit() would set up clocks here (not needed in QEMU) */

    /* 5. start the program */
    main();

    while (1) { }
}

void Default_Handler(void)
{
    while (1) { }
}
