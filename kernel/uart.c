// Intel 8250 serial port (UART).
#include <x86/x86.h>
#include <madosix/types.h>
#include <madosix/uart.h>

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
    //ioapicenable(IRQ_COM1, 0);

    uart_puts("madosix: uart init ok ...");
}

void uart_putc(int c)
{
    int i;

    if (!uart)
        return;
    for (i = 0; i < 128 && !(inb(COM1+5) & 0x20); i++)
        ;//microdelay(10);
    outb(COM1+0, c);
}

void uart_puts(const char *s)
{
    while (*s)
        uart_putc(*s++);
    uart_putc('\n');
}
