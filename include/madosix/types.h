#ifndef __MADOSIX_TYPES_H__
#define __MADOSIX_TYPES_H__

#include <asm/config.h>
#include <asm/types.h>

/* 定义布尔类型 */
typedef enum {
    false, true
} bool;

/* NULL */
#ifndef NULL
#define NULL ((void *)0)
#endif

#endif
