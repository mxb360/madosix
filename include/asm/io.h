#ifndef __MADOSIX_ASM_IO_H__
#define __MADOSIX_ASM_IO_H__

#include <asm/types.h>
#include <asm/config.h>

/* 从指定端口读取一个字节的数据 */
static inline uint8_t inb(uint16_t port)
{
    uint8_t data;

    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

/* 将一个字节的数据写入指定端口 */
static inline void outb(uint16_t port, uint8_t data)
{
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline void outw(ushort port, ushort data)
{
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline void outsl(int port, const void *addr, int cnt)
{
  asm volatile("cld; rep outsl" :
               "=S" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "cc");
}

static inline void insl(int port, void *addr, int cnt)
{
    asm volatile("cld; rep insl" :
                "=D" (addr), "=c" (cnt) :
                "d" (port), "0" (addr), "1" (cnt) :
                "memory", "cc");
}


static inline void stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
               "=D" (addr), "=c" (cnt) :
               "0" (addr), "1" (cnt), "a" (data) :
               "memory", "cc");
}

static inline void stosl(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosl" :
               "=D" (addr), "=c" (cnt) :
               "0" (addr), "1" (cnt), "a" (data) :
               "memory", "cc");
}

static inline uint virt_to_phys(volatile void *address)
{
	return ((uint) (address)) - KERNBASE;
}


static inline void *phys_to_virt(phys_addr_t address)
{
	return ((void *)(((char *) (address)) + KERNBASE));
}



#define outb_p(value,port) \
__asm__ ("outb %%al,%%dx\n" \
		"\tjmp 1f\n" \
		"1:\tjmp 1f\n" \
		"1:"::"a" (value),"d" (port))

#define inb_p(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al\n" \
	"\tjmp 1f\n" \
	"1:\tjmp 1f\n" \
	"1:":"=a" (_v):"d" (port)); \
_v; \
})

#endif
