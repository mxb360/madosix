#ifndef __MADOSIX_UART_H__
#define __MADOSIX_UART_H__

void uart_init(void);
void uart_putc(int c);
void uart_puts(const char *s);

#endif
