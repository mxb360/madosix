
#include <madosix/kernel.h>

int main(void)
{
    uart_init();
    printk("starting madosix kernel ...\n");

    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= i; j++)
            printk("%dx%d=%-2d ", j, i, i * j);
        printk("\n");
    }

    while (1);
}
