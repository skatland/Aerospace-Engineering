#include "uart.h"

/* UART0 data register. Writing a byte here sends it to the terminal.
   'volatile' tells the compiler: this is not normal memory, every access matters. */
#define UART0_DR (*(volatile uint32_t *)0x4000C000u)

void uart_putc(char c)
{
    UART0_DR = (uint32_t)c;
}

void uart_puts(const char *s)
{
    while (*s) {
        uart_putc(*s++);
    }
}

void uart_put_dec(uint32_t n)
{
    char buf[11];
    int i = 0;

    if (n == 0) {
        uart_putc('0');
        return;
    }
    while (n > 0) {
        buf[i++] = (char)('0' + (n % 10));
        n /= 10;
    }
    while (i > 0) {
        uart_putc(buf[--i]);
    }
}

void uart_put_hex(uint32_t v)
{
    uart_puts("0x");
    for (int shift = 28; shift >= 0; shift -= 4) {
        uint32_t d = (v >> shift) & 0xFu;
        uart_putc((char)(d < 10 ? '0' + d : 'A' + (d - 10)));
    }
}

void uart_check(const char *name, int ok)
{
    uart_puts(ok ? "  PASS: " : "  FAIL: ");
    uart_puts(name);
    uart_puts("\n");
}
