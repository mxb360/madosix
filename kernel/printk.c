#include <asm/uart.h>
#include <madosix/kernel.h>
#include <asm/traps.h>
#include <madosix/string.h>

#define PRINTK_BUF_SIZE 255

char printk_buf[(PRINTK_BUF_SIZE + 1) * 4];
char *end_printk_buf_p = printk_buf;

void vprintk(const char *format, va_list ap)
{
    char _printk_buf[PRINTK_BUF_SIZE + 1];
    char *p = _printk_buf;
    int n = vsnprintf(_printk_buf, PRINTK_BUF_SIZE, format, ap);

    while (*p) {
        *end_printk_buf_p++ = *p;
        if (*p == '\n') {
            *end_printk_buf_p = 0;
            uint32_t t = systick;
            uart_printf("[ %04d.%02d ] %s", t / 100, t % 100, printk_buf);
            end_printk_buf_p = printk_buf;
        }
        p++;
    }
}

void printk(const char *format, ...)
{
    va_list ap;
	va_start(ap, format);
    vprintk(format, ap);
	va_end(ap);
}
