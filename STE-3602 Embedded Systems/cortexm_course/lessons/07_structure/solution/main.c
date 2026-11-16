#include <stdint.h>
#include "uart.h"
#include "hal.h"

#define TICK_PERIOD   500000u
#define TICKS_WANTED  5u

volatile uint32_t tick_flag = 0;

void SysTick_Handler(void)
{
    tick_flag = 1;                          /* TODO 3: short and simple */
}

static uint32_t run_polling(void)
{
    uint32_t iterations = 0;
    uint32_t ticks = 0;

    hal_tick_init(TICK_PERIOD, 0);

    /* TODO 1 */
    while (ticks < TICKS_WANTED) {
        iterations++;
        if (hal_tick_elapsed()) {
            ticks++;
        }
    }

    hal_tick_stop();
    return iterations;
}

static uint32_t run_interrupt(void)
{
    uint32_t iterations = 0;
    uint32_t ticks = 0;

    tick_flag = 0;
    hal_tick_init(TICK_PERIOD, 1);

    /* TODO 2 */
    while (ticks < TICKS_WANTED) {
        iterations++;
        hal_sleep();
        if (tick_flag) {
            tick_flag = 0;
            ticks++;
        }
    }

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
