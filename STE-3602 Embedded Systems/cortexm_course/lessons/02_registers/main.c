#include <stdint.h>
#include "uart.h"

/* TODO 1-4: implement the four bit helpers. They return wrong answers on purpose right now. */
static uint32_t set_bit(uint32_t value, uint32_t bit)    { return value; }
static uint32_t clear_bit(uint32_t value, uint32_t bit)  { return value; }
static uint32_t toggle_bit(uint32_t value, uint32_t bit) { return value; }
static uint32_t test_bit(uint32_t value, uint32_t bit)   { return 0; }

/* Given example: read a special register with the MRS instruction */
static inline uint32_t read_control(void)
{
    uint32_t r;
    __asm volatile ("mrs %0, control" : "=r" (r));
    return r;
}

/* TODO 5: implement these the same way as read_control (use "ipsr", "msp", "primask") */
static inline uint32_t read_ipsr(void)    { return 0xFFFFFFFFu; }
static inline uint32_t read_msp(void)     { return 0; }
static inline uint32_t read_primask(void) { return 0xFFFFFFFFu; }

/* TODO 6: run "cmp a, b" and return the APSR (flags) right after it:
           __asm volatile ("cmp %1, %2 \n mrs %0, apsr" : "=r"(r) : "r"(a), "r"(b) : "cc");  */
static uint32_t apsr_after_cmp(uint32_t a, uint32_t b)
{
    (void)a; (void)b;
    return 0;
}

int main(void)
{
    uart_puts("Lesson 2: registers and bits\n");

    uart_puts("Bit helpers:\n");
    uart_check("set_bit(0, 3) == 8",          set_bit(0, 3) == 8);
    uart_check("clear_bit(0xFF, 0) == 0xFE",  clear_bit(0xFF, 0) == 0xFE);
    uart_check("toggle_bit(5, 1) == 7",       toggle_bit(5, 1) == 7);
    uart_check("toggle_bit(7, 1) == 5",       toggle_bit(7, 1) == 5);
    uart_check("test_bit(0x10, 4) == 1",      test_bit(0x10, 4) == 1);
    uart_check("test_bit(0x10, 3) == 0",      test_bit(0x10, 3) == 0);

    uart_puts("Special registers (we are in thread mode, privileged):\n");
    uart_puts("  CONTROL = "); uart_put_hex(read_control());  uart_puts("\n");
    uart_puts("  IPSR    = "); uart_put_hex(read_ipsr());     uart_puts("\n");
    uart_puts("  MSP     = "); uart_put_hex(read_msp());      uart_puts("\n");
    uart_puts("  PRIMASK = "); uart_put_hex(read_primask());  uart_puts("\n");
    uart_check("IPSR == 0 (not inside an exception handler)", read_ipsr() == 0);
    uart_check("MSP is near the top of RAM", read_msp() > 0x2000F000u && read_msp() <= 0x20010000u);
    uart_check("PRIMASK == 0 (interrupts not masked)", read_primask() == 0);

    uart_puts("Flags in APSR (bit 31 N, 30 Z, 29 C, 28 V):\n");
    uint32_t f1 = apsr_after_cmp(5, 5);
    uint32_t f2 = apsr_after_cmp(3, 5);
    uart_check("cmp 5,5 sets Z (equal)",              test_bit(f1, 30) == 1);
    uart_check("cmp 5,5 sets C (no borrow)",          test_bit(f1, 29) == 1);
    uart_check("cmp 3,5 sets N (result negative)",    test_bit(f2, 31) == 1);
    uart_check("cmp 3,5 clears C (borrow happened)",  test_bit(f2, 29) == 0);

    while (1) { }
}
