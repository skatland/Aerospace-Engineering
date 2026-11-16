#include "uart.h"

int answer = 42;                          /* initialized global   -> .data   */
int empty;                                /* uninitialized global -> .bss    */
int big[100];                             /* TODO 2: 400 bytes more in .bss  */
const char msg[] = "Hello, embedded world!";   /* constant        -> flash (.rodata) */

int main(void)
{
    uart_puts("Lesson 0: the build pipeline\n");
    uart_puts(msg);
    uart_puts("\n");

    uart_puts("answer = ");
    uart_put_dec((uint32_t)answer);
    uart_puts("\n");

    while (1) { }
}
