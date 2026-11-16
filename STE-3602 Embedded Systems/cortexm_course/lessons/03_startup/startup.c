#include <stdint.h>

/* YOUR startup code. Fill in the three TODOs.
   If nothing prints when you run it, the vector table is wrong: run 'make vectors'. */

extern uint32_t _estack;   /* start value of the stack pointer (top of RAM) */
extern uint32_t _sidata;   /* where the initial .data values sit in FLASH    */
extern uint32_t _sdata;    /* start of .data in RAM                          */
extern uint32_t _edata;    /* end of .data in RAM                            */
extern uint32_t _sbss;     /* start of .bss in RAM                           */
extern uint32_t _ebss;     /* end of .bss in RAM                             */

int main(void);
void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)   __attribute__((weak, alias("Default_Handler")));

__attribute__((section(".isr_vector"), used))
void (* const vector_table[])(void) = {
    (void (*)(void))&_estack,  /*  0: initial stack pointer                        */
    0,                         /*  1: TODO 1: the reset vector (what goes here?)   */
    NMI_Handler,               /*  2: NMI                                          */
    HardFault_Handler,         /*  3: HardFault                                    */
    0, 0, 0, 0, 0, 0, 0,       /*  4-10: other fault handlers, reserved            */
    SVC_Handler,               /* 11: SVCall                                       */
    0, 0,                      /* 12-13: DebugMon, reserved                        */
    PendSV_Handler,            /* 14: PendSV                                       */
    SysTick_Handler            /* 15: SysTick                                      */
};

/* Simulates a real chip after power-on: RAM contains random garbage.
   (In the emulator RAM would otherwise start as all zeros, which hides bugs.) */
static void scramble_ram(void)
{
    for (uint32_t *p = &_sdata; p < &_ebss; p++) {
        *p = 0xA5A5A5A5u;
    }
}

void Reset_Handler(void)
{
    scramble_ram();

    /* TODO 2: copy the initial values of .data from FLASH (starts at _sidata)
               to RAM (_sdata up to _edata), one 32-bit word at a time. */

    /* TODO 3: set RAM from _sbss up to _ebss to zero. */

    main();

    while (1) { }
}

void Default_Handler(void)
{
    while (1) { }
}
