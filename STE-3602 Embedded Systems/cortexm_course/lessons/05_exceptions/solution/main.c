#include <stdint.h>
#include "uart.h"

#define SYST_CSR (*(volatile uint32_t *)0xE000E010u)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014u)
#define SYST_CVR (*(volatile uint32_t *)0xE000E018u)

volatile uint32_t tick_count = 0;

static inline uint32_t read_ipsr(void)
{
    uint32_t r;
    __asm volatile ("mrs %0, ipsr" : "=r" (r));
    return r;
}

void svc_c_handler(uint32_t *frame)
{
    /* TODO 1 */
    uart_puts("  inside SVC_Handler, IPSR = ");
    uart_put_dec(read_ipsr());
    uart_puts("  (11 = SVCall)\n");
    uart_puts("  frame address = "); uart_put_hex((uint32_t)frame); uart_puts("  (this is on the stack)\n");
    uart_puts("  stacked R0    = "); uart_put_hex(frame[0]); uart_puts("\n");
    uart_puts("  stacked R1    = "); uart_put_hex(frame[1]); uart_puts("\n");
    uart_puts("  stacked R2    = "); uart_put_hex(frame[2]); uart_puts("\n");
    uart_puts("  stacked R3    = "); uart_put_hex(frame[3]); uart_puts("\n");
    uart_puts("  stacked R12   = "); uart_put_hex(frame[4]); uart_puts("\n");
    uart_puts("  stacked LR    = "); uart_put_hex(frame[5]); uart_puts("\n");
    uart_puts("  stacked PC    = "); uart_put_hex(frame[6]); uart_puts("  (return address)\n");
    uart_puts("  stacked xPSR  = "); uart_put_hex(frame[7]); uart_puts("\n");
    uart_puts("  Thumb bit (xPSR bit 24) = "); uart_put_dec((frame[7] >> 24) & 1u); uart_puts("\n");
}

__attribute__((naked)) void SVC_Handler(void)
{
    __asm volatile (
        "tst lr, #4      \n"
        "ite eq          \n"
        "mrseq r0, msp   \n"
        "mrsne r0, psp   \n"
        "b svc_c_handler \n"
    );
}

void SysTick_Handler(void)
{
    tick_count++;       /* TODO 3: keep the ISR short */
}

int main(void)
{
    uart_puts("Lesson 5: exceptions\n");
    uart_puts("IPSR in thread mode = ");
    uart_put_dec(read_ipsr());
    uart_puts("\n");

    uart_puts("Part A: SVC\n");
    __asm volatile (
        "mov r0, #0x11 \n"
        "mov r1, #0x22 \n"
        "mov r2, #0x33 \n"
        "mov r3, #0x44 \n"
        "svc #0        \n"
        ::: "r0", "r1", "r2", "r3", "memory");
    uart_puts("back in main, IPSR = ");
    uart_put_dec(read_ipsr());
    uart_puts("\n");

    uart_puts("Part B: SysTick\n");
    /* TODO 2 */
    SYST_RVR = 1000000u - 1u;
    SYST_CVR = 0;
    SYST_CSR = (1u << 2) | (1u << 1) | (1u << 0);

    while (tick_count < 5) {
        __asm volatile ("wfi");
    }
    uart_puts("got 5 ticks\n");

    /* TODO 4 */
    uart_puts("Part C: PRIMASK\n");
    __asm volatile ("cpsid i" ::: "memory");
    uint32_t before = tick_count;
    for (volatile uint32_t i = 0; i < 20000000u; i++) { }
    uint32_t during = tick_count;
    __asm volatile ("cpsie i \n isb" ::: "memory");
    uint32_t after = tick_count;

    uart_puts("  ticks before mask      = "); uart_put_dec(before); uart_puts("\n");
    uart_puts("  ticks while masked     = "); uart_put_dec(during); uart_puts("  (should equal before)\n");
    uart_puts("  ticks after unmasking  = "); uart_put_dec(after);  uart_puts("  (one more: pending is ONE bit)\n");

    SYST_CSR = 0;
    while (1) { }
}
