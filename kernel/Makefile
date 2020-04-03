# Madosix OS Source File
# 
# Madosix 是一款自制的玩具性操作系统
#
# 这是Madosix系统的内核部分Makefile文件，用于编译kernel下的代码，生成kernel.img
# 以及通过qemu运行
#
# 作者： mxb3600 
# 日期：2020-1 2020-1

MADOSIX_ROOT_DIR=..

include $(MADOSIX_ROOT_DIR)/config.mk

CFLAGS += -I$(MADOSIX_ROOT_DIR)/include
ASFLAGS += -I$(MADOSIX_ROOT_DIR)/include

KERNEL_OBJ = entry.o main.o uart.o

kernel.img: $(KERNEL_OBJ)
	$(LD) $(LDFLAGS)  -T kernel.lds $(KERNEL_OBJ) -o kernel.img
	$(OBJDUMP) -S kernel.img > kernel.asm


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.asm *.d *.img
