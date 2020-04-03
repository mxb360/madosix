#ifndef __MADOSIX_KERNEL_H__
#define __MADOSIX_KERNEL_H__

#include <madosix/types.h>
#include <madosix/x86.h>
#include <madosix/string.h>

/* printf */
int vsnprintf(char *buf, int buf_len, const char *format, va_list ap);
int snprintf(char *buf, int n, const char *format, ...);
int vprintf(const char *format, va_list ap);

/* printk */
#include <madosix/uart.h>
#define printk                  uart_printf

#endif
