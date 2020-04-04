#include <asm/system.h>
#include <madosix/kernel.h>

#define syscall0(res)

int sys_syscall_test0(void)
{
    printk("in sys_syscall_test0\n");
    printk("hello, world!\n");
    return 0;
}

int sys_syscall_test1(int a)
{
    printk("in sys_syscall_test1\n");
    printk("arg: a=%d, ret=%d\n", a, a * a);
    return a * a;
}

int sys_syscall_test2(int a, int b)
{
    printk("in sys_syscall_test2\n");
    printk("arg: a=%d,b=%d ret: %d\n", a, b, a * b);
    return a * b;
}

/////////////////////////////////////////////////////////////

int syscall_test0(void)
{
    long res; 
    __asm__ volatile ("int $0x80" : "=a" (res)  : "0" (10)); 
    return  res; 
}

int syscall_test1(int a) 
{ 
    long res; 
    __asm__ volatile ("int $0x80" : "=a" (res) : "0" (11),"b" (a)); 
    return res;
}

int syscall_test2(int a, int b) 
{ 
    long res; 
    __asm__ volatile ("int $0x80" : "=a" (res) : "0" (12),"b" (a),"c" (b)); 
    return res;
}
