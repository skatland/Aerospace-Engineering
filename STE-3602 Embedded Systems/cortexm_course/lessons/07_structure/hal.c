#include "hal.h"

#define SYST_CSR (*(volatile uint32_t *)0xE000E010u)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014u)
#define SYST_CVR (*(volatile uint32_t *)0xE000E018u)

void hal_tick_init(uint32_t period_cycles, int use_interrupt)
{
    SYST_CSR = 0;                         /* stop the timer while we configure it */
    SYST_RVR = period_cycles - 1u;
    SYST_CVR = 0;
    SYST_CSR = (1u << 2)                              /* CLKSOURCE: processor clock */
             | (use_interrupt ? (1u << 1) : 0u)       /* TICKINT: request interrupt */
             | (1u << 0);                             /* ENABLE                     */
}

void hal_tick_stop(void)
{
    SYST_CSR = 0;
}

int hal_tick_elapsed(void)
{
    /* COUNTFLAG (bit 16) is set when the counter reached 0. Reading CSR clears it. */
    return (int)((SYST_CSR >> 16) & 1u);
}

void hal_sleep(void)
{
    __asm volatile ("wfi");
}
