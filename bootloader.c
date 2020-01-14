#include "x86.h"

#define SECTOR_SIZE  512 

static void read_data_from_sector1(void *dst, uint offset)
{
    while((inb(0x1F7) & 0xC0) != 0x40);

    outb(0x1F2, 1);
    outb(0x1F3, offset);
    outb(0x1F4, offset >> 8);
    outb(0x1F5, offset >> 16);
    outb(0x1F6, (offset >> 24) | 0xE0);
    outb(0x1F7, 0x20); 

    while((inb(0x1F7) & 0xC0) != 0x40);
    insl(0x1F0, dst, SECTOR_SIZE/4);
}

int main(void)
{
    return 0;
}
