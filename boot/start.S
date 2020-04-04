# Madosix OS Source File
# 
# Madosix 是一款自制的玩具性操作系统
#
# 这是Madosix系统的bootloader汇编文件
# 作用是从16位的实模式跳转到32位的保护模式，并跳转到C代码的main函数，加载内核
# BIOS会从启动盘的第一个扇区的代码（512字节）加载到物理内存0x7c00处并且以实模式执行
#
# 本代码参考自xv6源码里的bootasm.S
#
# 作者： mxb360
# 日期：2020-1 2020-1

# 使用该头文件下的宏来定义临时的GDT（全局描述符表）
#include <asm/memory.h>

# 汇编为16位指令
.code16

# 将代码加载到内存后，从此处开始执行，因此此处地址要为0x7c00（已在链接脚本bootloader.lds里设置）
_start:
    # BIOS 开启了中断，这里要关闭
    # 因为实模式和保护模式的中断处理机制不一样，为了防止出现错误，先关闭中断
    cli                         

    # 将寄存器 DS, ES, SS 清零
    xorw %ax, %ax             # 将%ax置零
    movw %ax, %ds             # -> Data Segment
    movw %ax, %es             # -> Extra Segment
    movw %ax, %ss             # -> Stack Segment

    # 打开A20地址线
    # 8086模式，为向下兼容，A20关闭的情况下，访问超过1MB内存时，会自动回卷
    # 因此即使在保护模式下，不打开A20地址线，也不能访问超过1M的内存空间
    # 对A20信号线进行控制的常用方法是通过设置键盘控制器的端口值
    # 由于键盘的控制器速度很慢， 因此就不能使用键盘控制器对A20 线来进行操作。
    # 为此引进了一个A20 快速门选项（fast gate A20），它使用 IO 端口 0x92来处理 A20 信号线
    # 避免了使用慢速的键盘控制器是操作方式
    inb $0x92, %al
    orb $0x02, %al
    outb %al, $0x92

    # 将临时的GDT的值写到gdt（全局描述符表格寄存器）里
    lgdt gdtdesc
    movl %cr0, %eax

    # 将CR0的PE位置1，进入保护模式
    # 寄存器CR0的第一位是PE位，表示系统运行在实模式还是保护模式
    # PE为0表示实模式，为1表示保护模式，将PE为置一，便运行在保护模式
    orl $0x01, %eax
    movl %eax, %cr0

    # Complete the transition to 32-bit protected mode by using a long jmp
    # to reload %cs and %eip.  The segment descriptors are set up with no
    # translation, so that the mapping is still the identity mapping.
    # 通过使用一个long jmp来重新加载%cs和%eip，完成向32位保护模式的转换。
    # 段描述符没有经过转换，因此映射仍然是身份映射。
    ljmp    $(1<<3), $_start32

# 汇编为32位指令
.code32 

# 下面的代码将在32位的保护模式下运行
_start32:
    # Set up the protected-mode data segment registers
    # 设置保护模式的段寄存器
    movw  $(2<<3), %ax            # Our data segment selector
    movw  %ax, %ds                # -> DS: Data Segment
    movw  %ax, %es                # -> ES: Extra Segment
    movw  %ax, %ss                # -> SS: Stack Segment
    movw  $0, %ax                 # Zero segments not ready for use
    movw  %ax, %fs                # -> FS
    movw  %ax, %gs                # -> GS

    # C函数需要使用栈，因此在进入C函数前，先初始化栈
    # 设置栈指针为0x7c00，即0x7c00之前的地址空间作为栈空间，以供C程序使用
    movl $_start, %esp

    # 调用bootloader C语言部分的boot_main函数，bootloader剩下的工作用C语言完成
    # 正常情况下该函数不会返回
    # C函数boot_main完成以下工作：
    #    1. 将内核从硬盘加载到内存
    #    2. 跳转到内核的人口
    call boot_main
    
# 不应该执行此处
# 如果到此处，说明系统启动失败
# 将程序卡死在此处
spin:
    jmp     spin

.p2align 2                                 # force 4 byte alignment

# 定义临时的GDT（全局描述符表）
# BootLoader只划分了两个段，
# 一个是0~4G的代码段，可执行，可读，
# 另一个是0~4G的数据段，可写，
# 两个段的起始地址都是0，于是进程中的虚拟地址直接等于线性地址。
gdt:
    SEG_NULLASM                             # 空段
    SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)   # 代码段，地址为0-0xffffffff(4G)
    SEG_ASM(STA_W, 0x0, 0xffffffff)         # 数据段，地址为0-0xffffffff(4G)

# GDT
gdtdesc:
    .word (gdtdesc - gdt - 1)               # sizeof(gdt) - 1
    .long gdt                               # address gdt
