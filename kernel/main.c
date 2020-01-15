#include <madosix/uart.h>

int main(void)
{
    uart_init();
    uart_puts("hello, world!");

    while (1);
}
