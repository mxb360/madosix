#ifndef __MADOSIX_UART_H__
#define __MADOSIX_UART_H__

#include <madosix/types.h>

void uart_init(void);
void uart_putc(int c);
void uart_puts(const char *s);

#endif
