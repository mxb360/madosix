#ifndef __MADOSIX_ASM_MEMORY_H
#define __MADOSIX_ASM_MEMORY_H

#include "_mmu.h"

#define V2P_WO(x) ((x) - KERNBASE)    // same as V2P, but without casts
#define P2V_WO(x) ((x) + KERNBASE)    // same as P2V, but without casts

#endif
