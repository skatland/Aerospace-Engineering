#ifndef HAL_H
#define HAL_H

#include <stdint.h>

/* A very small Hardware Abstraction Layer (HAL), see lecture 7, slides 14-15.
   The application (main.c) only calls these functions.
   It never touches the SysTick registers directly. */

void hal_tick_init(uint32_t period_cycles, int use_interrupt);
void hal_tick_stop(void);
int  hal_tick_elapsed(void);   /* 1 if the period has passed since the last call (polling) */
void hal_sleep(void);          /* sleep until the next interrupt (WFI)                      */

#endif
