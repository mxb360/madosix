# Madosix OS Source File
# 
# Madosix 是一款自制的玩具性操作系统
#
# 这是Madosix系统的根Makefile文件，用于编译系统源码，构建完整系统镜像madosix.img
# 以及通过qemu运行
# 
# 主要操作：
#     make 					# 生成madosix.img系统镜像
#     make all				# 生成madosix.img系统镜像
#     make madosiximg		# 生成madosix.img系统镜像
#
#     make bootimg          # 生成boot.img镜像，位于bootloader/
#     
#     make clean            # 清空编译生成的所有中间文件和目标文件
#
#     make qemu             # 通过qemu虚拟机运行madosix.img系统镜像
#
# 作者： mxb3600 
# 日期：2020-1 2020-1


#include config.mk
# 编译器配置
COMPILE_PREFIX =

# 编译运行工具配置
CC = $(COMPILE_PREFIX)gcc 
LD = $(COMPILE_PREFIX)ld
AS = $(TOOLPREFIX)gas
OBJCOPY = $(COMPILE_PREFIX)objcopy
OBJDUMP = $(COMPILE_PREFIX)objdump
BUILD_BOOTIMG = perl
DD = dd
RM = rm -rf
QEMU = qemu-system-i386

# 编译参数配置
CFLAGS = -fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -Werror -fno-omit-frame-pointer -fno-stack-protector -Iinclude
ASFLAGS = -m32 -gdwarf-2 -Wa,-divide -Iinclude
LDFLAGS += -m elf_i386

CFLAGS += -Wno-unused-function -Wno-unused-variable

BOOT_DIR = boot
KERNEL_DIR = kernel
TEST_DIR = test
LIB_DIR = lib

KERNEL_OBJ = $(KERNEL_DIR)/main.o		\
			 $(KERNEL_DIR)/pde.o		\
			 $(KERNEL_DIR)/uart.o		\

LIB_OBJ =    $(LIB_DIR)/string.o		\
			 $(LIB_DIR)/vsnprintf.o		\

madosiximg: madosix.img

bootimg: boot.img

kernelimg: kernel.img

madosix.img: boot.img kernel.img
	$(DD) if=/dev/zero of=madosix.img count=10000
	$(DD) if=boot.img of=madosix.img conv=notrunc
	$(DD) if=kernel.img of=madosix.img seek=1 conv=notrunc

boot.img: $(BOOT_DIR)/main.c $(BOOT_DIR)/start.S $(BOOT_DIR)/boot.lds Makefile
	$(CC) $(CFLAGS) -c $(BOOT_DIR)/start.S -o $(BOOT_DIR)/start.o
	$(CC) $(CFLAGS) -c  $(BOOT_DIR)/main.c -o $(BOOT_DIR)/main.o
	$(LD) $(LDFLAGS) -T $(BOOT_DIR)/boot.lds -o $(BOOT_DIR)/boot.elf $(BOOT_DIR)/start.o $(BOOT_DIR)/main.o
	$(OBJDUMP) -S $(BOOT_DIR)/boot.elf > $(BOOT_DIR)/boot.asm
	$(OBJCOPY) -S -O binary -j .text  $(BOOT_DIR)/boot.elf boot.img
	$(BUILD_BOOTIMG) $(BOOT_DIR)/bootbuild.pl boot.img

kernel.img: $(KERNEL_DIR)/entry.o $(KERNEL_DIR)/kernel.lds $(KERNEL_OBJ) $(LIB_OBJ) Makefile
	$(LD) $(LDFLAGS) -T $(KERNEL_DIR)/kernel.lds $(KERNEL_DIR)/entry.o $(KERNEL_OBJ) $(LIB_OBJ) -o kernel.img
	$(OBJDUMP) -S kernel.img > $(KERNEL_DIR)/kernel.asm

$(KERNEL_DIR)/entry.o: $(KERNEL_DIR)/entry.S
	$(CC) -c $< -o $@ $(ASFLAGS)

-include $(KERNEL_DIR)/*.d
-include $(LIB_DIR)/*.d

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# 生成镜像文件并运行(需要qemu)
qemu: madosix.img
	$(QEMU) -serial mon:stdio -drive file=madosix.img,format=raw -m 512

clean:
	$(RM) madosix.img kernel.img boot.img
	$(RM) $(KERNEL_DIR)/*.o $(KERNEL_DIR)/*.elf $(KERNEL_DIR)/*.asm $(KERNEL_DIR)/*.d 
	$(RM) $(BOOT_DIR)/*.o $(BOOT_DIR)/*.elf $(BOOT_DIR)/*.asm $(BOOT_DIR)/*.d
	$(RM) $(LIB_DIR)/*.o $(LIB_DIR)/*.d
