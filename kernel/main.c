#include <madosix/kernel.h>
#include <asm/system.h>
#include <asm/io.h>
#include <asm/traps.h>


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

int main(void)
{
    uart_init();
    pic_init();
    pit_init();
    seg_init();
    trap_init();

    printk("starting madosix kernel ...\n");

    sti();
    int res;
    
    res = syscall_test0();
    printk("main(): syscall_test0 return: %d\n\n", res);
    res = syscall_test1(7);
    printk("main(): syscall_test1 return: %d\n\n", res);
    res = syscall_test2(6, 9);
    printk("main(): syscall_test2 return: %d\n\n", res);

    while (1) {
        delay(100);
        printk("100 systick: %d\n", systick);
    }
}
