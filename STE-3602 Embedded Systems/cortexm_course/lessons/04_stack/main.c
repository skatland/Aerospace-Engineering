#include <stdint.h>
#include "uart.h"

extern uint32_t _estack;

/* Given: read the stack pointer (we use MSP all the time in this course) */
static inline uint32_t read_sp(void)
{
    uint32_t r;
    __asm volatile ("mov %0, sp" : "=r" (r));
    return r;
}

/* Given: each call to recurse() creates a new stack frame */
static void recurse(int n)
{
    uart_puts("  level ");
    uart_put_dec((uint32_t)n);
    uart_puts(": SP = ");
    uart_put_hex(read_sp());
    uart_puts("\n");

    /* TODO 3: remember the SP from the previous level in a 'static uint32_t last_sp'
               and print how many bytes ONE function call used (last_sp - sp). */

    if (n > 0) {
        recurse(n - 1);
    }
}

int main(void)
{
    uart_puts("Lesson 4: the full-descending stack\n");

    uart_puts("stack started at (_estack) = ");
    uart_put_hex((uint32_t)&_estack);
    uart_puts("\n");
    uart_puts("SP in main                 = ");
    uart_put_hex(read_sp());
    uart_puts("\n");

    /* TODO 1: measure what PUSH and POP do to SP. Use ONE asm block:
         __asm volatile (
             "mov %0, sp \n"          // SP before
             "push {r4-r6} \n"
             "mov %1, sp \n"          // SP after push
             "pop {r4-r6} \n"
             "mov %2, sp \n"          // SP after pop
             : "=r"(before), "=r"(after_push), "=r"(after_pop)
             :
             : "r4", "r5", "r6", "memory");
       Print the three values and the difference. 3 registers = how many bytes? */

    /* TODO 2: which register goes to which address? Use ONE asm block:
         "mov r4, #0x44 \n mov r5, #0x55 \n mov r6, #0x66 \n"
         "push {r4-r6} \n mov %0, sp \n pop {r4-r6} \n"       (output %0 = sp_val)
       After the block, read the words at sp_val[0], sp_val[1], sp_val[2].
       The data is still in memory after the POP - why? */

    uart_puts("recursion:\n");
    recurse(3);

    while (1) { }
}
