#ifndef __MADOSIX_TYPES_H__
#define __MADOSIX_TYPES_H__

#include <madosix/config.h>

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef char           int8_t;
typedef unsigned char  uint8_t;
typedef short          int16_t;
typedef unsigned short uint16_t;
typedef int            int32_t;
typedef unsigned       uint32_t;

typedef uint32_t       size_t;
typedef uint32_t       pde_t;

/* 定义布尔类型 */
typedef enum {
    false, true
} bool;

/* NULL */
#ifndef NULL
#define NULL ((void *)0)
#endif

/*  STDARG */
#ifdef USE_STDARG
  #include <stdarg.h>
#else
  typedef char *va_list;
  /* 目的是把sizeof(n)的结果变成至少是sizeof(int)的整倍数，一般用来在结构中实现按int的倍数对齐 */
  #define _INTSIZEOF(n)         ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

  #define va_start(ap, v)       (ap = (va_list)&v + _INTSIZEOF(v))
  #define va_arg(ap, t)         (*(t *)(ap = ap + _INTSIZEOF(t), ap - _INTSIZEOF(t)))
  #define va_end(ap)            (ap = (va_list)0)
#endif

#endif
