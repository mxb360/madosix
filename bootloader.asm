
bootloader.elf：     文件格式 elf32-i386


Disassembly of section .text:

00007c00 <_start>:
    7c00:	fa                   	cli    
    7c01:	31 c0                	xor    %eax,%eax
    7c03:	8e d8                	mov    %eax,%ds
    7c05:	8e c0                	mov    %eax,%es
    7c07:	8e d0                	mov    %eax,%ss
    7c09:	e4 92                	in     $0x92,%al
    7c0b:	0c 02                	or     $0x2,%al
    7c0d:	e6 92                	out    %al,$0x92
    7c0f:	0f 01 16             	lgdtl  (%esi)
    7c12:	5c                   	pop    %esp
    7c13:	7c 0f                	jl     7c24 <_start32+0x1>
    7c15:	20 c0                	and    %al,%al
    7c17:	66 83 c8 01          	or     $0x1,%ax
    7c1b:	0f 22 c0             	mov    %eax,%cr0
    7c1e:	ea                   	.byte 0xea
    7c1f:	23 7c 08 00          	and    0x0(%eax,%ecx,1),%edi

00007c23 <_start32>:
    7c23:	66 b8 10 00          	mov    $0x10,%ax
    7c27:	8e d8                	mov    %eax,%ds
    7c29:	8e c0                	mov    %eax,%es
    7c2b:	8e d0                	mov    %eax,%ss
    7c2d:	66 b8 00 00          	mov    $0x0,%ax
    7c31:	8e e0                	mov    %eax,%fs
    7c33:	8e e8                	mov    %eax,%gs
    7c35:	bc 00 7c 00 00       	mov    $0x7c00,%esp
    7c3a:	e8 50 01 00 00       	call   7d8f <main>

00007c3f <spin>:
    7c3f:	eb fe                	jmp    7c3f <spin>
    7c41:	8d 76 00             	lea    0x0(%esi),%esi

00007c44 <gdt>:
	...
    7c4c:	ff                   	(bad)  
    7c4d:	ff 00                	incl   (%eax)
    7c4f:	00 00                	add    %al,(%eax)
    7c51:	9a cf 00 ff ff 00 00 	lcall  $0x0,$0xffff00cf
    7c58:	00                   	.byte 0x0
    7c59:	92                   	xchg   %eax,%edx
    7c5a:	cf                   	iret   
	...

00007c5c <gdtdesc>:
    7c5c:	17                   	pop    %ss
    7c5d:	00 44 7c 00          	add    %al,0x0(%esp,%edi,2)
	...

00007c62 <inb>:
    7c62:	55                   	push   %ebp
    7c63:	89 e5                	mov    %esp,%ebp
    7c65:	83 ec 14             	sub    $0x14,%esp
    7c68:	8b 45 08             	mov    0x8(%ebp),%eax
    7c6b:	66 89 45 ec          	mov    %ax,-0x14(%ebp)
    7c6f:	0f b7 45 ec          	movzwl -0x14(%ebp),%eax
    7c73:	89 c2                	mov    %eax,%edx
    7c75:	ec                   	in     (%dx),%al
    7c76:	88 45 ff             	mov    %al,-0x1(%ebp)
    7c79:	0f b6 45 ff          	movzbl -0x1(%ebp),%eax
    7c7d:	c9                   	leave  
    7c7e:	c3                   	ret    

00007c7f <insl>:
    7c7f:	55                   	push   %ebp
    7c80:	89 e5                	mov    %esp,%ebp
    7c82:	57                   	push   %edi
    7c83:	53                   	push   %ebx
    7c84:	8b 55 08             	mov    0x8(%ebp),%edx
    7c87:	8b 4d 0c             	mov    0xc(%ebp),%ecx
    7c8a:	8b 45 10             	mov    0x10(%ebp),%eax
    7c8d:	89 cb                	mov    %ecx,%ebx
    7c8f:	89 df                	mov    %ebx,%edi
    7c91:	89 c1                	mov    %eax,%ecx
    7c93:	fc                   	cld    
    7c94:	f3 6d                	rep insl (%dx),%es:(%edi)
    7c96:	89 c8                	mov    %ecx,%eax
    7c98:	89 fb                	mov    %edi,%ebx
    7c9a:	89 5d 0c             	mov    %ebx,0xc(%ebp)
    7c9d:	89 45 10             	mov    %eax,0x10(%ebp)
    7ca0:	90                   	nop
    7ca1:	5b                   	pop    %ebx
    7ca2:	5f                   	pop    %edi
    7ca3:	5d                   	pop    %ebp
    7ca4:	c3                   	ret    

00007ca5 <outb>:
    7ca5:	55                   	push   %ebp
    7ca6:	89 e5                	mov    %esp,%ebp
    7ca8:	83 ec 08             	sub    $0x8,%esp
    7cab:	8b 55 08             	mov    0x8(%ebp),%edx
    7cae:	8b 45 0c             	mov    0xc(%ebp),%eax
    7cb1:	66 89 55 fc          	mov    %dx,-0x4(%ebp)
    7cb5:	88 45 f8             	mov    %al,-0x8(%ebp)
    7cb8:	0f b6 45 f8          	movzbl -0x8(%ebp),%eax
    7cbc:	0f b7 55 fc          	movzwl -0x4(%ebp),%edx
    7cc0:	ee                   	out    %al,(%dx)
    7cc1:	90                   	nop
    7cc2:	c9                   	leave  
    7cc3:	c3                   	ret    

00007cc4 <read_sect>:
    7cc4:	55                   	push   %ebp
    7cc5:	89 e5                	mov    %esp,%ebp
    7cc7:	90                   	nop
    7cc8:	68 f7 01 00 00       	push   $0x1f7
    7ccd:	e8 90 ff ff ff       	call   7c62 <inb>
    7cd2:	83 c4 04             	add    $0x4,%esp
    7cd5:	0f b6 c0             	movzbl %al,%eax
    7cd8:	25 c0 00 00 00       	and    $0xc0,%eax
    7cdd:	83 f8 40             	cmp    $0x40,%eax
    7ce0:	75 e6                	jne    7cc8 <read_sect+0x4>
    7ce2:	6a 01                	push   $0x1
    7ce4:	68 f2 01 00 00       	push   $0x1f2
    7ce9:	e8 b7 ff ff ff       	call   7ca5 <outb>
    7cee:	83 c4 08             	add    $0x8,%esp
    7cf1:	8b 45 0c             	mov    0xc(%ebp),%eax
    7cf4:	0f b6 c0             	movzbl %al,%eax
    7cf7:	50                   	push   %eax
    7cf8:	68 f3 01 00 00       	push   $0x1f3
    7cfd:	e8 a3 ff ff ff       	call   7ca5 <outb>
    7d02:	83 c4 08             	add    $0x8,%esp
    7d05:	8b 45 0c             	mov    0xc(%ebp),%eax
    7d08:	c1 e8 08             	shr    $0x8,%eax
    7d0b:	0f b6 c0             	movzbl %al,%eax
    7d0e:	50                   	push   %eax
    7d0f:	68 f4 01 00 00       	push   $0x1f4
    7d14:	e8 8c ff ff ff       	call   7ca5 <outb>
    7d19:	83 c4 08             	add    $0x8,%esp
    7d1c:	8b 45 0c             	mov    0xc(%ebp),%eax
    7d1f:	c1 e8 10             	shr    $0x10,%eax
    7d22:	0f b6 c0             	movzbl %al,%eax
    7d25:	50                   	push   %eax
    7d26:	68 f5 01 00 00       	push   $0x1f5
    7d2b:	e8 75 ff ff ff       	call   7ca5 <outb>
    7d30:	83 c4 08             	add    $0x8,%esp
    7d33:	8b 45 0c             	mov    0xc(%ebp),%eax
    7d36:	c1 e8 18             	shr    $0x18,%eax
    7d39:	83 c8 e0             	or     $0xffffffe0,%eax
    7d3c:	0f b6 c0             	movzbl %al,%eax
    7d3f:	50                   	push   %eax
    7d40:	68 f6 01 00 00       	push   $0x1f6
    7d45:	e8 5b ff ff ff       	call   7ca5 <outb>
    7d4a:	83 c4 08             	add    $0x8,%esp
    7d4d:	6a 20                	push   $0x20
    7d4f:	68 f7 01 00 00       	push   $0x1f7
    7d54:	e8 4c ff ff ff       	call   7ca5 <outb>
    7d59:	83 c4 08             	add    $0x8,%esp
    7d5c:	90                   	nop
    7d5d:	68 f7 01 00 00       	push   $0x1f7
    7d62:	e8 fb fe ff ff       	call   7c62 <inb>
    7d67:	83 c4 04             	add    $0x4,%esp
    7d6a:	0f b6 c0             	movzbl %al,%eax
    7d6d:	25 c0 00 00 00       	and    $0xc0,%eax
    7d72:	83 f8 40             	cmp    $0x40,%eax
    7d75:	75 e6                	jne    7d5d <read_sect+0x99>
    7d77:	68 80 00 00 00       	push   $0x80
    7d7c:	ff 75 08             	pushl  0x8(%ebp)
    7d7f:	68 f0 01 00 00       	push   $0x1f0
    7d84:	e8 f6 fe ff ff       	call   7c7f <insl>
    7d89:	83 c4 0c             	add    $0xc,%esp
    7d8c:	90                   	nop
    7d8d:	c9                   	leave  
    7d8e:	c3                   	ret    

00007d8f <main>:
    7d8f:	55                   	push   %ebp
    7d90:	89 e5                	mov    %esp,%ebp
    7d92:	b8 00 00 00 00       	mov    $0x0,%eax
    7d97:	5d                   	pop    %ebp
    7d98:	c3                   	ret    
