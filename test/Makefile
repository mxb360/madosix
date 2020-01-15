# Madosix OS Source File
# 
# Madosix 是一款自制的玩具性操作系统
#
# 这是Madosix系统的测试Makefile文件，用于编译系统库的测试代码
# 以及通过qemu运行
#
# 作者： mxb3600 
# 日期：2020-1 2020-1

MADOSIX_ROOT_DIR=..

include $(MADOSIX_ROOT_DIR)/config.mk

CFLAGS += -I $(MADOSIX_ROOT_DIR)/include


all: elf32_test

clean:
	$(RM) -rf *.out *.o
	$(RM) -rf elf32_test

elf32_test:
	$(CC) $(CFLAGS) elf32_test.c -o elf32_test