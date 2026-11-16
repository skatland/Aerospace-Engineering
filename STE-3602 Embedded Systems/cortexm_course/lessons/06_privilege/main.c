#include <stdint.h>
#include "uart.h"

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

/* TODO 3: the SVC handler runs in handler mode, which is ALWAYS privileged.
           Make it clear bit 0 (nPRIV) of CONTROL and then return. Skeleton:
               mrs r0, control
               bic r0, r0, #1
               msr control, r0
               isb
               bx lr                                                              */
__attribute__((naked)) void SVC_Handler(void)
{
    __asm volatile (
        "bx lr \n"
    );
}

int main(void)
{
    uart_puts("Lesson 6: privileged and unprivileged access\n");

    uart_puts("CONTROL at start = ");
    uart_put_hex(read_control());
    uart_puts("   (bit 0 = nPRIV: 0 means privileged)\n");

    /* TODO 1: drop to unprivileged access: write_control(1); then print CONTROL. */

    /* TODO 2: try to get privilege back directly: write_control(0); then print CONTROL.
               Did it work? */

    /* TODO 4: trigger the SVC with  __asm volatile ("svc #0");  and print CONTROL afterwards. */

    while (1) { }
}
