#ifndef __MADOSIX_IO_H__
#define __MADOSIX_IO_H__

#ifndef __IN_LINUX__
/* Use Madosix API */
#include <madosix/types.h>

int open(const char *name, int flag);
int read(int fd, char *buf, size_t size);
int write(int fd, const char *buf, size_t size);
int close(int fd);

/* Use Linux API */
#else
#include <unistd.h>
#include <fcntl.h>
#endif


#endif
