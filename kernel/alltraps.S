.globl alltraps

.align 2
alltraps:
  # Build trap frame.
  pushl %ds
  pushl %es
  pushl %fs
  pushl %gs
  pushal
  
  # Set up data segments.
  # movw $(SEG_KDATA<<3), %ax
  # movw %ax, %ds
  # movw %ax, %es

  # Call trap(tf), where tf=%esp
  pushl %esp
  call traps
  addl $4, %esp
  
  # Return falls through to trapret...
trapret:
  popal
  popl %gs
  popl %fs
  popl %es
  popl %ds
  addl $0x8, %esp  # trapno and errcode
  iret
