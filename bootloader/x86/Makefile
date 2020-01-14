# Madosix OS Source File
# 
# Madosix 是一款自制的玩具性操作系统
#
# 这是Madosix系统的bootloader部分的Makefile文件，用于编译bootloader，生成boot.img
#
# 作者： mxb3600 
# 日期：2020-1 2020-1


MADOSIX_ROOT_DIR=../..

include $(MADOSIX_ROOT_DIR)/config.mk

CFLAGS += -I $(MADOSIX_ROOT_DIR)/include

boot.img: main.c start.S bootloader.lds
	$(CC) $(CFLAGS) -c start.S -o start.o
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(LD) $(LDFLAGS) -T bootloader.lds -o boot.elf start.o main.o
	$(OBJDUMP) -S boot.elf > boot.asm
	$(OBJCOPY) -S -O binary -j .text  boot.elf boot.img
	$(BUILD_BOOTIMG) buildbootimg.pl boot.img
