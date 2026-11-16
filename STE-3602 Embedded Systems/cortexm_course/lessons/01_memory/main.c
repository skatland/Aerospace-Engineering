#include <stdint.h>
#include "uart.h"

/* Turn an address into something you can read AND write, like a variable:
       REG32(0x20008000) = 5;        x = REG32(0x20008000);
   Read the README to understand every part of this line. */
#define REG32(addr) (*(volatile uint32_t *)(addr))

#define RAM_TEST 0x20008000u      /* a free spot in the middle of RAM */

int main(void)
{
    uart_puts("Lesson 1: memory map and memory access\n");

    /* TODO 1: write 0xDEADBEEF to RAM_TEST with REG32, read it back, print it with uart_put_hex(). */

    /* TODO 2: look at the SAME four bytes one by one:
               volatile uint8_t *p = (volatile uint8_t *)RAM_TEST;
               print p[0], p[1], p[2], p[3]. Which byte comes first? (endianness) */

    /* TODO 3: print the first two words of FLASH: REG32(0x00000000) and REG32(0x00000004).
               What are they? (Think about lesson 3 / the reset sequence.) */

    /* TODO 4 (bit-band): set bit 4 of the word at RAM_TEST to 1 using only ONE write to the
               bit-band alias address. Formula in the README. Then print the word. */

    while (1) { }
}
