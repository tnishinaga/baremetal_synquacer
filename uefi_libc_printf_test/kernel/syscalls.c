#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdint.h>

#include "memmap.h"
#include "config.h"

int _system(const char *);
int _rename(const char *, const char *);
int _isatty(int);
void _raise(void);
int _unlink(const char *);
int _link(const char *, const char *);
void * _sbrk(ptrdiff_t);
pid_t _getpid(void);
int _kill(int, int);
void _exit(int);
int _close(int);
int _swiclose(int);
int _open(const char *, int, ...);
int _swiopen(const char *, int);
int _write(int, const void *, size_t);
int _swiwrite(int, const void *, size_t);
_off_t _lseek(int, _off_t, int);
_off_t _swilseek(int, _off_t, int);
int _read(int, void *, size_t);
int _swiread(int, void *, size_t);


void * _sbrk(ptrdiff_t diff)
{
    extern void *__bss_end;
    static uint8_t *heap_head = (uint8_t *)&__bss_end;
    uint8_t *heap_head_prev = (uint8_t *)heap_head;

    heap_head_prev = heap_head;
    heap_head += diff;

    return heap_head_prev;
}

int _write(int fd, const void *data, size_t size)
{
    (void)fd;
    uint8_t *byte_data = (uint8_t *)data;
    for (size_t i = 0; i < size; i++) {
        pl011_putc((int)byte_data[i], STDIO);
    }
    return 0;
}

int _isatty(int fd)
{
    (void)fd;
    return 0;
}

int _fstat (int fd, struct stat * st)
{
    (void)fd;
    (void)st;
    return 0;
}


void _exit(int status)
{
    (void)status;
    // loop
    while(1) {
        __asm volatile("wfi");
    }
    return;
}

int _close(int file)
{
    (void)file;
    return 0;
}

_off_t _lseek(int file, _off_t ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read(int file, void *ptr, size_t len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

// int _system(const char *s)
// {
//     return 0;
// }

// int _rename(const char *oldpath, const char *newpath)
// {
//     return 0;
// }

// clock_t _times(struct tms *tp)
// {
//     return 0;
// }

// int _gettimeofday(struct timeval *tp, void *tzvp)
// {
//     return 0;
// }

// void _raise(void)
// {
//     return;
// }

// int _unlink(const char *path)
// {
//     return 0;
// }

// int _link(const char *__path1, const char *__path2)
// {
//     return 0;
// }

// int _stat(const char *fname, struct stat *st)
// {
//     return 0;
// }

// pid_t _getpid(void)
// {
//     return 0;
// }

// int _kill(int pid, int sig)
// {
//     return 0;
// }

// int _swiclose(int file)
// {
//     return 0;
// }

// int _open(const char *path, int flags, ...)
// {
//     return 0;
// }

// int _swiopen(const char *path, int flags)
// {
//     return 0;
// }

// int _swiwrite(int file, const void *ptr, size_t len)
// {
//     return 0;
// }

// _off_t _swilseek(int file, _off_t ptr, int dir)
// {
//     return 0;
// }

// int _swiread(int file, void *ptr, size_t len)
// {
//     return 0;
// }


