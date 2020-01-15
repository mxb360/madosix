#define __IN_LINUX__

#include <madosix/elf32.h>
#include <madosix/types.h>
#include <madosix/io.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

elf32_header_t elf32_header;


int print_elf32_info(const char *file_name)
{
    int fd;

    if ((fd = open(file_name, O_RDONLY)) < 1) {
        fprintf(stderr, "Error: can not open \"%s\": %s\n\n", file_name, strerror(errno));
        return 1;
    }
    if (read(fd, &elf32_header, sizeof(elf32_header_t)) < 0) {
        fprintf(stderr, "Error: can not read from \"%s\": %s\n\n", file_name, strerror(errno));
        return 1;
    }
    close(fd);

    if (elf32_header.magic != ELF32_MAGIC) {
        fprintf(stderr, "Error: \"%s\" is not a ELF32 file.\n\n", file_name);
        return 1;
    }

    printf("%s:\n", file_name);
    printf("    magic:     %x\n", elf32_header.magic);
    printf("    type:      %u\n", elf32_header.type);
    printf("    machine:   %u\n", elf32_header.machine);
    printf("    version:   %u\n", elf32_header.version);
    printf("    entry:     %#08x\n", elf32_header.entry);
    printf("    phoff:     %u\n", elf32_header.phoff);
    printf("    shoff:     %u\n", elf32_header.shoff);
    printf("    flags:     %u\n", elf32_header.flags);
    printf("    ehsize:    %u\n", elf32_header.ehsize);
    printf("    phentsize: %u\n", elf32_header.phentsize);
    printf("    phnum:     %u\n", elf32_header.phnum);
    printf("    shentsize: %u\n", elf32_header.shentsize);
    printf("    shnum:     %u\n", elf32_header.shnum);
    printf("    shstrndx:  %u\n", elf32_header.shstrndx);
    printf("\n");

    return 0;
}

int main(int argc, char *argv[])
{
    int i, err = 0;

    if (argc < 2) {
        fprintf(stderr, "Error: No input file\n");
        return 1;
    }

    for (i = 1; i < argc; i++) {
        if (print_elf32_info(argv[i]))
            err = 1;
    }

    return err;
}
