#ifndef __MADOSIX_KERNEL_H__
#define __MADOSIX_KERNEL_H__

#include <madosix/types.h>
#include <madosix/string.h>

/*  stdarg */
typedef char *va_list;
#define _va_round(type)     (((sizeof(type) + sizeof(int)-1) / sizeof(int)) * sizeof(int))
#define va_start(ap, list)  (ap = ((char *) &(list) + _va_round(list)))
#define va_arg(ap, type)    (ap += _va_round(type), *((type *)(ap - _va_round(type))))
#define va_end(ap)          ((void)0)

/* vsprintf */
int vsnprintf(char *buf, int buf_len, const char *format, va_list ap);
int vsprintf(char *buf, const char *format, va_list ap);
int snprintf(char *buf, int n, const char *format, ...);
int sprintf(char *buf, const char *format, ...);

/* printk */
void vprintk(const char *format, va_list ap);
void printk(const char *format, ...);

/* panic */
void _panic(const char *file, const char *func, int line, const char *format, ...);
#define panic(format, ...)  _panic(__FILE__, __func__, __LINE__, format, ##__VA_ARGS__)

#endif
