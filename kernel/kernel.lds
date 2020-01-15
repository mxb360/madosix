OUTPUT_FORMAT(elf32-i386) 
OUTPUT_ARCH(i386)         
ENTRY(main)

SECTIONS {                
	. = 0x80100000;
	.text : AT(0x100000) {
		*(.text .stub .text.* )
	}

	.rodata : {
		*(.rodata .rodata.* )
	}                  
						                          
	/DISCARD/ : {         
	 }                     
}
