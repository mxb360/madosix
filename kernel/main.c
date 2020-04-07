#include <madosix/kernel.h>
#include <asm/system.h>
#include <asm/io.h>
#include <asm/traps.h>
#include <asm/uart.h>


extern void trap_init(void);
extern void pic_init(void);
extern void seg_init(void);

extern int syscall_test0(void);
extern int syscall_test1(int a);
extern int syscall_test2(int a, int b);


void delay(uint32_t t)
{
    uint32_t current = systick;
    while (current + t > systick);
}

void syscall_test(void)
{
    printk("syscall test ...\n");

    int res;

    res = syscall_test0();
    printk("main(): syscall_test0 return: %d\n\n", res);
    res = syscall_test1(7);
    printk("main(): syscall_test1 return: %d\n\n", res);
    res = syscall_test2(6, 9);
    printk("main(): syscall_test2 return: %d\n\n", res);
}

void timer_test(void)
{
    delay(100);
    printk("systick: %d\n", systick);
}

extern char __kernel_end[];

int main(void)
{
    uart_init();
    pic_init();
    pit_init();
    seg_init();
    trap_init();
    sti();

    iprintk("kernel base: 0x%x\n", KERNBASE);
    dprintk("kernel end addr: 0x%x\n", __kernel_end);
    wprintk("kernel size: %d\n", (int)__kernel_end - KERNBASE - 0x100000);
    *(int *)0 = 0;
    syscall_test();

    while (1) {
        timer_test();
    }
}

