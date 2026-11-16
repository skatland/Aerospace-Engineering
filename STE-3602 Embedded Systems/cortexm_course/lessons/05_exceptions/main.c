#include <stdint.h>
#include "uart.h"

/* SysTick registers (part of the System Control Space, see lecture 5/6) */
#define SYST_CSR (*(volatile uint32_t *)0xE000E010u)   /* control and status   */
#define SYST_RVR (*(volatile uint32_t *)0xE000E014u)   /* reload value         */
#define SYST_CVR (*(volatile uint32_t *)0xE000E018u)   /* current value        */

volatile uint32_t tick_count = 0;

static inline uint32_t read_ipsr(void)
{
    uint32_t r;
    __asm volatile ("mrs %0, ipsr" : "=r" (r));
    return r;
}

/* ---------- Part A: SVC and what the processor stacks on exception entry ---------- */

/* Called from SVC_Handler below. 'frame' points to the 8 words the hardware pushed. */
void svc_c_handler(uint32_t *frame)
{
    /* TODO 1: print IPSR (read_ipsr()) and the 8 stacked words, in the order they
               are stored from the lowest address up:  R0, R1, R2, R3, R12, LR, PC, xPSR.
               Also print bit 24 of the stacked xPSR (the Thumb bit). */
    (void)frame;
}

/* Given: finds out which stack was used (MSP or PSP) and passes the frame pointer to C.
   Explained in the README. */
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

/* ---------- Part B: SysTick interrupt ---------- */

void SysTick_Handler(void)
{
    /* TODO 3: count one tick */
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

    uart_puts("Part B: SysTick (waits for 5 ticks - hangs until TODO 2 and 3 are done)\n");
    /* TODO 2: configure SysTick
               SYST_RVR = 1000000 - 1;   (reload value)
               SYST_CVR = 0;             (clear current value)
               SYST_CSR = bit 2 (CLKSOURCE) | bit 1 (TICKINT, request interrupt) | bit 0 (ENABLE) */

    while (tick_count < 5) {
        __asm volatile ("wfi");      /* sleep until an interrupt arrives */
    }
    uart_puts("got 5 ticks\n");

    /* TODO 4: mask interrupts with PRIMASK and see what happens to the ticks:
         __asm volatile ("cpsid i" ::: "memory");     // PRIMASK = 1
         ... save tick_count, busy-wait:  for (volatile uint32_t i = 0; i < 20000000u; i++) { }
         ... save tick_count again, THEN unmask:
         __asm volatile ("cpsie i \n isb" ::: "memory");
         ... read tick_count a third time. Print all three. */

    SYST_CSR = 0;     /* stop SysTick */
    while (1) { }
}
