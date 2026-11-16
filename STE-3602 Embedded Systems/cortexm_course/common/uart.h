#ifndef UART_H
#define UART_H

#include <stdint.h>

/* Tiny output driver for UART0 on the QEMU board "lm3s6965evb".
   Everything you print ends up in your VS Code terminal. */

void uart_putc(char c);
void uart_puts(const char *s);
void uart_put_dec(uint32_t n);   /* prints n in decimal, e.g. 42          */
void uart_put_hex(uint32_t v);   /* prints v as 0x........ (8 hex digits) */
void uart_check(const char *name, int ok);   /* prints PASS: name / FAIL: name */

#endif
