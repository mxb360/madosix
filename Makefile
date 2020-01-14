# Madosix OS Source File
# 
# Madosix 是一款自制的玩具性操作系统
#
# 这是Madosix系统的根Makefile文件，用于编译系统源码，构建完整系统镜像
#
# 作者： mxb3600 
# 日期：2020-1 2020-1

##################################################################################


# 编译器配置
COMPILE_PREFIX=

GCC=$(COMPILE_PREFIX)gcc
LD=$(COMPILE_PREFIX)ld
OBJCOPY=$(COMPILE_PREFIX)objcopy
OBJDUMP=$(COMPILE_PREFIX)objdump

##################################################################################

# 默认行为：生成系统镜像文件
all: madosix.img

# 生成镜像文件并运行(需要qemu)
qemu: madosix.img
	qemu-system-i386 -serial mon:stdio -drive file=madosix.img,format=raw

# 清理构建生成的所有文件
clean:
	rm -rf *.o *.bin *.elf *.img *.asm

##################################################################################

bootloader.bin: bootloader.c bootloader.S bootloader.lds
	$(GCC) -m32 -c bootloader.S -o bootloader.s.o
	$(GCC) -m32 -c bootloader.c -o bootloader.c.o
	$(LD)  -T bootloader.lds -o bootloader.elf bootloader.s.o bootloader.c.o
	$(OBJDUMP) -S bootloader.elf > bootloader.asm
	$(OBJCOPY) -S -O binary -j .text -j .rodata bootloader.elf bootloader.bin
	./bootloader.pl bootloader.bin

madosix.img: bootloader.bin
	dd if=bootloader.bin of=madosix.img bs=512 count=1


.PHONY=clean
