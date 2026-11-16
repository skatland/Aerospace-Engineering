#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

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
    (void (*)(void))&_estack,  /*  0: initial stack pointer */
    Reset_Handler,             /*  1: TODO 1 - the address of Reset_Handler */
    NMI_Handler,
    HardFault_Handler,
    0, 0, 0, 0, 0, 0, 0,
    SVC_Handler,
    0, 0,
    PendSV_Handler,
    SysTick_Handler
};

static void scramble_ram(void)
{
    for (uint32_t *p = &_sdata; p < &_ebss; p++) {
        *p = 0xA5A5A5A5u;
    }
}

void Reset_Handler(void)
{
    scramble_ram();

    /* TODO 2 */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* TODO 3 */
    for (dst = &_sbss; dst < &_ebss; dst++) {
        *dst = 0;
    }

    main();

    while (1) { }
}

void Default_Handler(void)
{
    while (1) { }
}
