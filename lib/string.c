#include <asm/io.h>
#include <madosix/string.h>

void *memset(void *dst, int c, size_t n)
{
    stosb(dst, c, n);
    return dst;
}

void *memmove(void *dst, const void *src, size_t n)
{
    const char *s = src;
    char *d = dst;

    if (s < d && s + n > d) {
        s += n;
        d += n;
        while (n-- > 0)
            *--d = *--s;
    } else
        while (n-- > 0)
            *d++ = *s++;

    return dst;
}
