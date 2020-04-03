#include <asm/system.h>
#include <asm/traps.h>
#include <asm/memory.h>
#include <madosix/kernel.h>

struct gatedesc idt[256];
extern uint vectors[];

void trap_init(void)
{
    int i;

    for(i = 0; i < 256; i++)
        SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
    SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);

  lidt(idt, sizeof(idt));
}

extern int sys_syscall_test0(void);
extern int sys_syscall_test1(int a);
extern int sys_syscall_test2(int a, int b);

void trap(struct trapframe *tf)
{
    printk("in syscall: tf->trapno=%d, tf->eax=%d\n", tf->trapno, tf->eax);

    switch (tf->eax)
    {
    case 0:
        tf->eax = sys_syscall_test0();
        break;
    case 1:
        tf->eax = sys_syscall_test1(7);
        break;
    case 2:
        tf->eax = sys_syscall_test2(8, 9);
        break;
    default:
        printk("unkown syscall\n");
        break;
    };
}
