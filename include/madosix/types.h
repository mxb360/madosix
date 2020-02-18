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

//typedef uint32_t       size_t;
typedef uint32_t       pde_t;

/* 定义布尔类型 */
typedef enum {
    false, true
} bool;

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif
