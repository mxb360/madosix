#include <asm/memory.h>

.globl _start

# _start为提供给BootLoader的入口的入口地址，对应内核的entry
# 但是entry的地址是虚拟地址0x80100000（高地址），由于BootLoader无法访问高地址，
# 因此提供给BootLoader的入口应该是低地址（物理地址），通过V2P_WO将虚拟地址转换为物理地址
_start = V2P_WO(entry)

.globl entry

# 该地址为0x80100000，是内核的入口，由BootLoader跳到此处
entry:
    # 设置页表
    #
    # 内核现在位于内存低地址处，内核加载地址为0x100000，
    # 但是内核中的符号的虚拟地址以0x80100000为开始的，内核虚拟地址在高地址处，现在保护模式下虚拟地址等于物理地址，
    # 内核中所有以地址为目标的跳转都将跳转到物理地址的高地址处，而在那里的都是垃圾数据。
    # 所以，内核在一开始就必须设置页表，以便之后能够正常跳转和寻址。
    #
    # 打开4M页面大小扩展
    movl %cr4, %eax
    orl $(CR4_PSE), %eax
    movl %eax, %cr4

    # 页表为entrypgdir，在kernel/pde.c里被定义
    # 当前entrypgdir的值为虚拟地址（高地址）
    # 通过V2P_WO计算出物理地址（在内存里的真实地址，低地址）
    # 注：物理地址 = 虚拟地址-KERNBASE（0x80000000）
    #
    # 该页表只是暂时将内核虚拟地址的4Mb内存映射到物理地址的低内存
    # 在C代码里，会重新设置更为完善的页表
    #
    # 设置页表位置
    movl $(V2P_WO(entrypgdir)), %eax
    movl %eax, %cr3
    
    # 开启分页功能
    movl %cr0, %eax
    orl $(CR0_PG|CR0_WP), %eax
    movl %eax, %cr0

    # 设置内核栈，内核栈大小为4K
    movl $(stack + KSTACKSIZE), %esp

    # 通过jmp跳转到main使eip指向高地址处并在高地址执行代码
    # mov $main, %eax
    # jmp *%eax

    # call 也行？？？
    call main

.comm stack, KSTACKSIZE
