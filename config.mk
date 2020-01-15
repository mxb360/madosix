# Madosix OS Source File
# 
# Madosix 是一款自制的玩具性操作系统
#
# 这是Madosix系统的Makefile配置文件，将被所有的Makefile文件包含
#
# 作者： mxb3600 
# 日期：2020-1 2020-1

# 编译器配置
COMPILE_PREFIX=
ARCH=x86

CC=$(COMPILE_PREFIX)gcc 
LD=$(COMPILE_PREFIX)ld

CFLAGS = -m32 -Wall -N
LDFLAGS =

OBJCOPY=$(COMPILE_PREFIX)objcopy
OBJDUMP=$(COMPILE_PREFIX)objdump

BUILD_BOOTIMG=perl
DD=dd
RM=rm

QEMU=qemu-system-i386
