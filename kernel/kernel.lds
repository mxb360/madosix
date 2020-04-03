OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
	. = 0x80100000;
	.text : AT(0x100000) { *(.text) }
	.rodata : { *(.rodata) }
	. = ALIGN(0x1000);
	.data : { *(.data) }
	.bss : { *(.bss) }
}
