#include <asm/io.h>
#include <asm/uart.h>
#include <asm/memory.h>
#include <madosix/kernel.h>

#define COM1    0x3f8

static int uart;    // is there a uart?

void uart_init(void)
{
    // Turn off the FIFO
    outb(COM1+2, 0);

    // 9600 baud, 8 data bits, 1 stop bit, parity off.
    outb(COM1+3, 0x80);    // Unlock divisor
    outb(COM1+0, 115200/9600);
    outb(COM1+1, 0);
    outb(COM1+3, 0x03);    // Lock divisor, 8 data bits.
    outb(COM1+4, 0);
    outb(COM1+1, 0x01);    // Enable receive interrupts.

    // If status is 0xFF, no serial port.
    if (inb(COM1+5) == 0xFF)
        return;
    uart = 1;

    // Acknowledge pre-existing interrupt conditions;
    // enable interrupts.
    inb(COM1+2);
    inb(COM1+0);

    printk("uart init ...\n");
    printk("starting madosix kernel ...\n");
}

#define BACKSPACE 0x100
#define CRTPORT 0x3d4
static ushort *crt = NULL; 

/* 图形界面字符输出 */
static void gaputc(int c)
{
    int pos;

    if (!crt)
        crt = phys_to_virt(0xb8000);  // CGA memory

    // Cursor position: col + 80*row.
    outb(CRTPORT, 14);
    pos = inb(CRTPORT + 1) << 8;
    outb(CRTPORT, 15);
    pos |= inb(CRTPORT + 1);

    if (c == '\n')
        pos += 80 - pos % 80;
    else if (c == BACKSPACE){
        if(pos > 0) --pos;
    } else
        crt[pos++] = (c & 0xff) | 0x0700;  // black on white

    //if(pos < 0 || pos > 25*80)
    //    panic("pos under/overflow");

    if ((pos/80) >= 24){  // Scroll up.
        memmove(crt, crt + 80, sizeof(crt[0]) * 23 * 80);
        pos -= 80;
        memset(crt + pos, 0, sizeof(crt[0]) * (24 * 80 - pos));
    }

    outb(CRTPORT, 14);
    outb(CRTPORT + 1, pos >> 8);
    outb(CRTPORT, 15);
    outb(CRTPORT+1, pos);
    
    crt[pos] = ' ' | 0x0700;
}

void uart_putc(int c)
{
    int i;

    if (!uart)
        return;
    for (i = 0; i < 128 && !(inb(COM1+5) & 0x20); i++)
        ;//microdelay(10);
    outb(COM1+0, c);
    gaputc(c);
}

void uart_put_string(const char *s)
{
    while (*s)
        uart_putc(*s++);
}

void uart_puts(const char *s)
{
    uart_put_string(s);
    uart_putc('\n');
}

void uart_vprintf(const char *format, va_list ap)
{
    char buf[128] = {0}, *s = buf;

	vsnprintf(buf, 128, format, ap);

    while (*s)
        uart_putc(*s++);
}

void uart_printf(const char *format, ...)
{
    va_list ap;
	va_start(ap, format);
    uart_vprintf(format, ap);
	va_end(ap);
}
