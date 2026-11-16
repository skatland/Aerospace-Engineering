#include <stdint.h>
#include "uart.h"
#include "hal.h"

#define TICK_PERIOD   500000u     /* clock cycles between two ticks */
#define TICKS_WANTED  5u

volatile uint32_t tick_flag = 0;  /* set by the ISR, read by the main loop */

/* TODO 3: write the ISR the way the slides say: short and simple. Only set the flag. */
void SysTick_Handler(void)
{
}

/* Approach (a) in Task 9: polling (superloop). Returns how many times the loop body ran. */
static uint32_t run_polling(void)
{
    uint32_t iterations = 0;

    hal_tick_init(TICK_PERIOD, 0);          /* no interrupt, we ask the hardware instead */

    /* TODO 1: loop until TICKS_WANTED ticks have been seen.
               Every time around the loop: iterations++ and ask hal_tick_elapsed(). */

    hal_tick_stop();
    return iterations;
}

/* Approach (b) in Task 9: interrupt sets a flag, main loop sleeps in between. */
static uint32_t run_interrupt(void)
{
    uint32_t iterations = 0;

    tick_flag = 0;
    hal_tick_init(TICK_PERIOD, 1);          /* interrupt on every tick */

    /* TODO 2: loop until TICKS_WANTED ticks have been seen.
               Every time around the loop: iterations++, hal_sleep(), then check tick_flag
               (clear it when you have seen it). */

    hal_tick_stop();
    return iterations;
}

int main(void)
{
    uart_puts("Lesson 7: software structure\n");

    uint32_t poll_iter = run_polling();
    uint32_t irq_iter  = run_interrupt();

    uart_puts("polling:   loop body ran "); uart_put_dec(poll_iter); uart_puts(" times\n");
    uart_puts("interrupt: loop body ran "); uart_put_dec(irq_iter);  uart_puts(" times\n");

    uart_check("both versions saw their ticks", poll_iter > 0 && irq_iter > 0);
    uart_check("polling wakes the CPU far more often", poll_iter > irq_iter * 10u);

    while (1) { }
}
