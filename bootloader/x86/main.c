#include <x86/x86.h>
#include <madosix/types.h>
#include <madosix/elf32.h>

#define SECTOR_SIZE    512
#define KERNEL_HEADER  0x1000000

typedef void (*start_kernel_func_t)(void);
static void copy_program_section(void *dest, size_t size, uint32_t offset);

/* bootloader C语言部分入口函数 
 * 在start.S里被调用
 */
void bootloader_main(void)
{
    int index;
    elf32_header_t *kernel_header;
    elf32_program_header_t *program_header_start;
    elf32_program_header_t *program_header;
    start_kernel_func_t start_kernel;

    /* Kernel镜像文件地址在KERNEL_HEADER(0x1000000)处 */
    kernel_header = (elf32_header_t *)KERNEL_HEADER;

    /* 从kernel镜像里读取ELF32文件头 */
    copy_program_section(kernel_header, sizeof(elf32_header_t), 0);

    /* 如果镜像不是一个ELF32格式，表示没有找到正确的系统镜像，系统启动失败，直接返回 */
    if (!is_elf32_file(kernel_header))
        return;

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

    /* 获取内核的启动函数入口 */
    start_kernel = (start_kernel_func_t)kernel_header->entry;
while (1)
{
    /* code */
}

    /* 启动内核，此函数不会返回 */
    start_kernel();
}

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
static void copy_program_section(void *dest, size_t size, uint32_t offset)
{
    char *start, *end;

    start = dest;
    end = start + size;
    start -= offset % SECTOR_SIZE;
    offset = offset / SECTOR_SIZE + 1;  /* 将镜像偏移量转化会扇区偏移量，我们的镜像是从扇区1开始的 */

    for(; start < end; start += SECTOR_SIZE, offset++)
        copy_sector(start, offset);
}
