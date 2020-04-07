#include <madosix/kernel.h>

#define PANIC_BUF_SIZE  127

void _panic(const char *file, const char *func, int line, const char *format, ...)
{
    va_list ap;
    char painc_buf[PANIC_BUF_SIZE + 1];

    eprintk("\e[35m------------->>> KERNEL PANIC <<<----------------\e[0m\n");
    eprintk("\e[35m>>> PANIC from %s:%d:in function \"%s\"\e[0m\n", file, line, func);
    eprintk("\e[31mkernel panic: \e[0m");

    va_start(ap, format);
    vsnprintf(painc_buf, PANIC_BUF_SIZE, format, ap);
	va_end(ap);

    printk(KERN_ERR "%s\n", painc_buf);

    while (1);
}
