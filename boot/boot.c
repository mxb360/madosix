#include <asm/io.h>
#include <madosix/types.h>
#include <madosix/elf32.h>

#define SECTOR_SIZE       512
#define KERNEL_HEADER     0x1000000
#define print_error()    *(uint16_t *)0xb8000 = ('E') | 0x0400

/* 复制硬盘上一个扇区的数据到内存
 * dst:    内存地址
 * offset: 扇区偏移量（扇区编号）
 */
static void copy_sector(void *dest, uint32_t offset)
{
    while((inb(0x1F7) & 0xC0) != 0x40);

    outb(0x1F2, 1);
    outb(0x1F3, offset);
    outb(0x1F4, offset >> 8);
    outb(0x1F5, offset >> 16);
    outb(0x1F6, (offset >> 24) | 0xE0);
    outb(0x1F7, 0x20); 

    while((inb(0x1F7) & 0xC0) != 0x40);
    insl(0x1F0, dest, SECTOR_SIZE/4);
}

/* 复制程序段的数据到内存
 * dst:    内存地址
 * size:   数据大小
 * offset: 数据起始位置在镜像里的偏移量
 */
static void copy_program_section(void *dest, uint32_t size, uint32_t offset)
{
    char *start, *end;

    start = dest;
    end = start + size;
    start -= offset % SECTOR_SIZE;
    offset = offset / SECTOR_SIZE + 1;  /* 将镜像偏移量转化会扇区偏移量，我们的镜像是从扇区1开始的 */

    for(; start < end; start += SECTOR_SIZE, offset++)
        copy_sector(start, offset);
}

/* 内存分布：
 * +------------+
 * |            |
 * +------------+
 * |    内核    |
 * +------------+ 0x100000  <--- 内核起始位置
 * |    设备    |
 * +------------+ 0xa0000
 * |            |
 * +------------+ 0x7d00
 * | BootLoader |
 * +------------+ 0x7c00    <-- BootLoader起始位置
 * |  临时C栈    |
 * +------------+ 0x0000
 *
 *
 * bootloader C语言部分入口函数，在start.S里被调用
 * 该函数将内核镜像加载到内存地址0x1000000处
 * 由于在BootLoader里的GDT配置，此时的虚拟地址等于物理地址
 * 因此，将内核镜像（是一个ELF32格式的文件）里各个段复制到对应的内存对应的地址即可
 * 
 * 但是需要注意的是，内核现在存在内存低地址处，内核加载地址为0x100000，
 * 但是内核中的符号的虚拟地址以0x80100000为开始的，内核虚拟地址在高地址处，现在保护模式下虚拟地址等于物理地址，
 * 内核中所有以地址为目标的跳转都将跳转到物理地址的高地址处，而在那里的都是垃圾数据。
 * 所以，内核在一开始就必须设置页表，以便之后能够正常跳转和寻址。
 */
void boot_main(void)
{
    int index;
    elf32_header_t *kernel_header;
    elf32_program_header_t *program_header_start;
    elf32_program_header_t *program_header;

    /* Kernel镜像文件地址在KERNEL_HEADER(0x1000000)处 */
    kernel_header = (elf32_header_t *)KERNEL_HEADER;

    /* 从kernel镜像里读取ELF32文件头 */
    copy_program_section(kernel_header, sizeof(elf32_header_t), 0);

    /* 如果镜像不是一个ELF32格式，表示没有找到正确的系统镜像，系统启动失败，直接返回 */
    if (!is_elf32_file(kernel_header))
        goto boot_failed;

    /* 读取kernel镜像里的每一个程序段到内存 */
    program_header_start = get_program_header(kernel_header);
    for (index = 0; index < kernel_header->phnum; index++) {
        program_header = program_header_start + index;
        /* 拷贝该程序段到内存 */
        copy_program_section((char *)program_header->paddr, program_header->filesz, program_header->off);
       
        /* 如果有多出的内存空间，则填充0 */
        if (program_header->memsz > program_header->filesz) {
            char *p = (char *)program_header->paddr + program_header->filesz;
            uint32_t size = program_header->memsz - program_header->filesz;
            
            stosb(p, 0, size);
        }
    }

    /* 获取内核的启动函数入口，并启动内核，此函数不会返回*/
    ((void(*)(void))kernel_header->entry)();

boot_failed:
    /* 往显存里输出错误信息 */
    print_error();
}
