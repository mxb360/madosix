#ifndef __MADOSIX_ELF32_H__
#define __MADOSIX_ELF32_H__

#include <madosix/types.h>

#define ELF32_MAGIC 0x464C457FU  

/* 32位ELF文件头 */
typedef struct elf32_header {
    uint32_t magic;             // 魔数
    uint8_t  ident[12];         //
    uint16_t type;              // 类型
    uint16_t machine;           // 系统架构
    uint32_t version;           // 版本
    uint32_t entry;             // 程序入口
    uint32_t phoff;             // start of program headers
    uint32_t shoff;             // start of section headers
    uint32_t flags;             // 标志
    uint16_t ehsize;            // 文件头的大小
    uint16_t phentsize;         // 程序头大小
    uint16_t phnum;             // number of program headers
    uint16_t shentsize;         // 节头大小
    uint16_t shnum;             // number of section headers
    uint16_t shstrndx;          // 字符串表段索引
} elf32_header_t;

#endif
