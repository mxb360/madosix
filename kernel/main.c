#include <madosix/kernel.h>
#include <asm/system.h>

extern void trap_init(void);

extern int syscall_test0(void);
extern int syscall_test1(int a);
extern int syscall_test2(int a, int b);

int main(void)
{
    uart_init();
    trap_init();

    printk("starting madosix kernel ...\n");

    int res;
    
    res = syscall_test0();
    printk("main(): syscall_test0 return: %d\n", res);

    res = syscall_test1(1);
    printk("main(): syscall_test1 return: %d\n", res);

    res = syscall_test2(1, 2);
    printk("main(): syscall_test2 return: %d\n", res);

    while (1);
}
