#include <stdint.h>
#include "uart.h"

#define REG32(addr) (*(volatile uint32_t *)(addr))

#define RAM_TEST 0x20008000u

/* Bit-band alias address of one bit in the SRAM bit-band region (0x20000000-0x200FFFFF) */
#define BITBAND_SRAM(addr, bit) \
    (0x22000000u + (((addr) - 0x20000000u) * 32u) + ((bit) * 4u))

int main(void)
{
    uart_puts("Lesson 1: memory map and memory access\n");

    /* TODO 1 */
    REG32(RAM_TEST) = 0xDEADBEEFu;
    uart_puts("word at RAM_TEST = ");
    uart_put_hex(REG32(RAM_TEST));
    uart_puts("\n");

    /* TODO 2: expected order EF BE AD DE (little endian: lowest byte at lowest address) */
    volatile uint8_t *p = (volatile uint8_t *)RAM_TEST;
    for (int i = 0; i < 4; i++) {
        uart_puts("byte at offset ");
        uart_put_dec((uint32_t)i);
        uart_puts(" = ");
        uart_put_hex(p[i]);
        uart_puts("\n");
    }

    /* TODO 3: word 0 = initial SP (0x20010000), word 1 = reset vector (odd = Thumb bit) */
    uart_puts("flash word 0 = ");
    uart_put_hex(REG32(0x00000000u));
    uart_puts("\n");
    uart_puts("flash word 1 = ");
    uart_put_hex(REG32(0x00000004u));
    uart_puts("\n");

    /* TODO 4: set bit 4 with one write to the alias address */
    REG32(RAM_TEST) = 0;
    REG32(BITBAND_SRAM(RAM_TEST, 4)) = 1;
    uart_puts("after bit-band write, word = ");
    uart_put_hex(REG32(RAM_TEST));
    uart_puts("   (expected 0x00000010)\n");

    while (1) { }
}
