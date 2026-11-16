#include <stdint.h>
#include "uart.h"

extern uint32_t _estack;      /* symbol from the linker script: top of RAM */

int counter = 5;              /* .data: has an initial value */
int table[4] = {1, 2, 3, 4};  /* .data */
int zero_var;                 /* .bss: no initial value, must become 0 */
int zero_array[8];            /* .bss */

int main(void)
{
    volatile uint32_t *flash = (volatile uint32_t *)0x00000000u;
    int all_zero = 1;

    uart_puts("Lesson 3: startup code\n");

    for (int i = 0; i < 8; i++) {
        if (zero_array[i] != 0) {
            all_zero = 0;
        }
    }

    uart_check(".data scalar: counter == 5",        counter == 5);
    uart_check(".data array:  table[3] == 4",       table[3] == 4);
    uart_check(".bss scalar:  zero_var == 0",       zero_var == 0);
    uart_check(".bss array:   all 8 elements == 0", all_zero);
    uart_check("vector table word 0 == &_estack",   flash[0] == (uint32_t)&_estack);

    uart_puts("address of counter  = "); uart_put_hex((uint32_t)&counter);  uart_puts("\n");
    uart_puts("address of zero_var = "); uart_put_hex((uint32_t)&zero_var); uart_puts("\n");
    uart_puts("vector table word 1 = "); uart_put_hex(flash[1]);            uart_puts("  (Reset_Handler, odd = Thumb)\n");

    while (1) { }
}
