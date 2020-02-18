#include <platform/platform.h>
#include <platform/memlayout.h>
#include <platform/mmu.h>
#include <madosix/types.h>
#include <madosix/uart.h>

int main(void)
{
    uart_init();
    uart_puts("start madosix kernel ...");

    while (1);
}




// The boot page table used in entry.S
// Page directories (and page tables) must start on page boundaries,
// hence the __aligned__ attribute.
// PTE_PS in a page directory entry enables 4Mbyte pages.
__attribute__((__aligned__(PGSIZE)))
pde_t entrypgdir[NPDENTRIES] = {
    // Map VA's [0, 4MB) to PA's [0, 4MB)
    [0] = (0) | PTE_P | PTE_W | PTE_PS,
    // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
    [KERNBASE>>PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS,
};
