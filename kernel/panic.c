#include <madosix/kernel.h>

#define PANIC_BUF_SIZE  128

void _panic(const char *file, const char *func, int line, const char *format, ...)
{
    va_list ap;

    printk("------------->>> KERNEL PANIC <<<----------------\n");
    printk(">>> PANIC from %s:%d:in function \"%s\"\n", file, line, func);
    printk("kernel panic: ");

    va_start(ap, format);
    vprintk(format, ap);
	va_end(ap);

    printk("\n\n");

    while (1);
}
