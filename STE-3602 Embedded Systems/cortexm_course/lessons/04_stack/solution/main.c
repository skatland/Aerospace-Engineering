#include <stdint.h>
#include "uart.h"

extern uint32_t _estack;

static inline uint32_t read_sp(void)
{
    uint32_t r;
    __asm volatile ("mov %0, sp" : "=r" (r));
    return r;
}

static void recurse(int n)
{
    static uint32_t last_sp = 0;
    uint32_t sp = read_sp();

    uart_puts("  level ");
    uart_put_dec((uint32_t)n);
    uart_puts(": SP = ");
    uart_put_hex(sp);
    if (last_sp != 0) {
        uart_puts("   one call used ");
        uart_put_dec(last_sp - sp);
        uart_puts(" bytes");
    }
    uart_puts("\n");
    last_sp = sp;

    if (n > 0) {
        recurse(n - 1);
    }
}

int main(void)
{
    uint32_t before, after_push, after_pop;
    uint32_t sp_val;

    uart_puts("Lesson 4: the full-descending stack\n");

    uart_puts("stack started at (_estack) = ");
    uart_put_hex((uint32_t)&_estack);
    uart_puts("\n");
    uart_puts("SP in main                 = ");
    uart_put_hex(read_sp());
    uart_puts("\n");

    /* TODO 1 */
    __asm volatile (
        "mov %0, sp \n"
        "push {r4-r6} \n"
        "mov %1, sp \n"
        "pop {r4-r6} \n"
        "mov %2, sp \n"
        : "=r" (before), "=r" (after_push), "=r" (after_pop)
        :
        : "r4", "r5", "r6", "memory");

    uart_puts("SP before push   = "); uart_put_hex(before);     uart_puts("\n");
    uart_puts("SP after push    = "); uart_put_hex(after_push); uart_puts("   (");
    uart_put_dec(before - after_push); uart_puts(" bytes lower = 3 words)\n");
    uart_puts("SP after pop     = "); uart_put_hex(after_pop);  uart_puts("   (back to start)\n");

    /* TODO 2 */
    __asm volatile (
        "mov r4, #0x44 \n"
        "mov r5, #0x55 \n"
        "mov r6, #0x66 \n"
        "push {r4-r6} \n"
        "mov %0, sp \n"
        "pop {r4-r6} \n"
        : "=r" (sp_val)
        :
        : "r4", "r5", "r6", "memory");

    volatile uint32_t *w = (volatile uint32_t *)sp_val;
    uint32_t w0 = w[0];
    uint32_t w1 = w[1];
    uint32_t w2 = w[2];
    uart_puts("after POP, memory at old SP still holds:\n");
    uart_puts("  [SP+0] = "); uart_put_hex(w0); uart_puts("  (R4, lowest register -> lowest address)\n");
    uart_puts("  [SP+4] = "); uart_put_hex(w1); uart_puts("  (R5)\n");
    uart_puts("  [SP+8] = "); uart_put_hex(w2); uart_puts("  (R6)\n");

    uart_puts("recursion:\n");
    recurse(3);

    while (1) { }
}
