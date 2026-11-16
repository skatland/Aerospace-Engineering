#include <stdint.h>
#include "uart.h"

/* Set to 1 to see what happens when unprivileged code touches the System Control Space */
#define FAULT_DEMO 0

static inline uint32_t read_control(void)
{
    uint32_t r;
    __asm volatile ("mrs %0, control" : "=r" (r));
    return r;
}

static inline void write_control(uint32_t v)
{
    __asm volatile ("msr control, %0 \n isb" : : "r" (v) : "memory");
}

/* TODO 3 */
__attribute__((naked)) void SVC_Handler(void)
{
    __asm volatile (
        "mrs r0, control \n"
        "bic r0, r0, #1  \n"
        "msr control, r0 \n"
        "isb             \n"
        "bx lr           \n"
    );
}

#if FAULT_DEMO
void HardFault_Handler(void)
{
    uart_puts("HardFault! Unprivileged access to the System Control Space is blocked.\n");
    while (1) { }
}
#endif

int main(void)
{
    uart_puts("Lesson 6: privileged and unprivileged access\n");

    uart_puts("CONTROL at start = ");
    uart_put_hex(read_control());
    uart_puts("   (privileged)\n");

    /* TODO 1 */
    write_control(1);
    uart_puts("after write_control(1) = ");
    uart_put_hex(read_control());
    uart_puts("   (now unprivileged)\n");

    /* TODO 2 */
    write_control(0);
    uart_puts("after write_control(0) = ");
    uart_put_hex(read_control());
    uart_puts("   (write ignored: still unprivileged)\n");

#if FAULT_DEMO
    *(volatile uint32_t *)0xE000E010u = 0;     /* SysTick control register */
    uart_puts("you will not see this line\n");
#endif

    /* TODO 4 */
    uart_puts("calling SVC ...\n");
    __asm volatile ("svc #0");
    uart_puts("after SVC             = ");
    uart_put_hex(read_control());
    uart_puts("   (privileged again)\n");

    while (1) { }
}
