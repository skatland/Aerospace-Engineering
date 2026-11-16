#include "uart.h"

int answer = 42;                          /* initialized global   -> .data   */
int empty;                                /* uninitialized global -> .bss    */
const char msg[] = "Hello, embedded world!";   /* constant        -> flash (.rodata) */

int main(void)
{
    uart_puts("Lesson 0: the build pipeline\n");
    uart_puts(msg);
    uart_puts("\n");

    /* TODO 1: print the value of 'answer' using uart_put_dec(), followed by a newline. */

    /* TODO 2: add a new global array above main, for example:  int big[100];
               Run 'make sections' before and after. Which section grows? */

    while (1) { }
}
