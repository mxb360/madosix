#include <asm/system.h>
#include <asm/io.h>
#include <asm/traps.h>
#include <asm/memory.h>
#include <madosix/kernel.h>

struct gatedesc idt[256];
extern uint vectors[];

#define HZ    100
#define LATCH (1193180/HZ)

void trap_init(void)
{
    for(int i = 0; i < 256; i++)
        SETGATE(idt[i], 0, SEG_KCODE << 3, vectors[i], 0);
    SETGATE(idt[T_SYSCALL], 1, SEG_KCODE << 3, vectors[T_SYSCALL], DPL_USER);

    lidt(idt, sizeof(idt));
}

extern int sys_syscall_test0(void);
extern int sys_syscall_test1(int a);
extern int sys_syscall_test2(int a, int b);

#define PORT_KEYDAT		0x0060

void traps(struct trapframe *tf)
{
    if (tf->trapno == T_SYSCALL) {
        printk("in system call: eax=%d, ebx=%d, ecx=%d, edx=%d\n", 
                tf->trapno, tf->eax, tf->ebx, tf->ecx, tf->edx);

        switch (tf->eax) {
        case 0:
            tf->eax = sys_syscall_test0();
            break;
        case 1:
            tf->eax = sys_syscall_test1(tf->ebx);
            break;
        case 2:
            tf->eax = sys_syscall_test2(tf->ebx, tf->ecx);
            break;
        default:
            printk("unkown system call\n");
            break;
        };
    } else {
        switch (tf->trapno) {
        case T_IRQ0 + IRQ_TIMER:
            outb(PIC0_OCW2, 0x60 + IRQ_TIMER);
            systick++;
            break;
        case T_IRQ0 + IRQ_KBD: {
            int code = inb(PORT_KEYDAT);
            printk("keyboard event: keycode: %d(0x%02x)\n", code, code);
            outb(PIC0_OCW2, 0x60 + IRQ_KBD);
            break;
        }
        default:
            printk("unknown interrupt: %d(0x%02x)\n", tf->trapno, tf->trapno);
            break;
        }
    }
}

void pic_init(void)
{
	cli();

	outb(PIC0_IMR,  0xff); /* 禁止所有中断 */
	outb(PIC1_IMR,  0xff); /* 禁止所有中断 */

	outb(PIC0_ICW1, 0x11  ); /* 边沿触发模式 */
	outb(PIC0_ICW2, 0x20  ); /* IRQ0-7由INT20-27接收 */
	outb(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2连接 */
	outb(PIC0_ICW4, 0x01  ); /* 无缓冲区模式 */

	outb(PIC1_ICW1, 0x11  ); /* 边沿触发模式 */
	outb(PIC1_ICW2, 0x28  ); /* IRQ8-15由INT28-2f连接 */
	outb(PIC1_ICW3, 2     ); /* PIC1偼IRQ2偵偰愙懕 */
	outb(PIC1_ICW4, 0x01  ); /* 无缓冲区模式 */

	outb(PIC0_IMR,  0xfb  ); /* 11111011 PIC1以外全部禁止 */
	outb(PIC1_IMR,  0xff  ); /* 11111111 禁止所有中断 */
}

volatile uint32_t systick;

void pit_init(void)
{
	outb(PIT_CTRL, 0x34);
	outb(PIT_CNT0, 0x9c);
	outb(PIT_CNT0, 0x2e);

    outb(PIC0_IMR, 0xf8); /* 允许定时（PIT PIC）器和键盘中断(11111000) */
	
	systick = 0;
}
