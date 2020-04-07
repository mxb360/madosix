#include <asm/uart.h>
#include <madosix/kernel.h>
#include <asm/traps.h>
#include <madosix/string.h>

#define PRINTK_BUF_SIZE    127

#define PRINTK_DEFAULT     0
#define PRINTK_ERROR       1
#define PRINTK_WARNING     2
#define PRINTK_INFO        3
#define PRINTK_DEGBUG      4
#define PRINTK_TYPE_COUNT  5

char printk_buf[PRINTK_TYPE_COUNT][(PRINTK_BUF_SIZE + 1) * 4];
char *end_printk_buf_p[PRINTK_TYPE_COUNT];
const char *type_string[] = {
    "", "\e[31merror: \e[0m", 
    "\e[35mwarning: \e[0m", 
    "\e[32minfo: \e[0m", 
    "\e[36mdebug: \e[0m"};

void vprintk(const char *format, va_list ap)
{
    char _printk_buf[PRINTK_BUF_SIZE + 1], *p = _printk_buf;
    int n = vsnprintf(_printk_buf, PRINTK_BUF_SIZE, format, ap);
    int printk_type = PRINTK_DEFAULT;

    if (p[0] == '<' && p[1] >= '0' && p[1] < '0' + PRINTK_TYPE_COUNT && p[2] == '>') {
        printk_type = p[1] - '0';
        p += 3;
    }
    if (end_printk_buf_p[printk_type] == NULL)
        end_printk_buf_p[printk_type] = printk_buf[printk_type];

    while (*p) {
        *end_printk_buf_p[printk_type]++ = *p;
        if (*p == '\n') {
            *end_printk_buf_p[printk_type] = 0;
            uint32_t t = systick;

            uart_printf("[ %04d.%02d ] %s%s", t / 100, t % 100, type_string[printk_type], printk_buf[printk_type]);
            end_printk_buf_p[printk_type] = printk_buf[printk_type];
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
