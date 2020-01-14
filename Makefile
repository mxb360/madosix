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


include config.mk

BOOT_DIR=bootloader/$(ARCH)

# 默认行为：生成系统镜像文件
all: madosix.img
madosiximg: madosix.img

# 生成镜像文件并运行(需要qemu)
qemu: madosix.img
	$(QEMU) -serial mon:stdio -drive file=madosix.img,format=raw

bootimg: 
	$(MAKE) -C $(BOOT_DIR)

clean:
	$(RM) -rf $(BOOT_DIR)/*.bin $(BOOT_DIR)/*.o $(BOOT_DIR)/*.img $(BOOT_DIR)/*.asm $(BOOT_DIR)/*.elf
	$(RM) -rf madosix.img

madosix.img: bootimg
	$(DD) if=$(BOOT_DIR)/boot.img of=madosix.img bs=512 count=1
