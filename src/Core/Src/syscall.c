#ifndef __clang__
#include <sys/stat.h>
#endif
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hal_usart.h"

__attribute__((weak)) int __io_getchar()
{
    uint8_t ch = 0;
    // USART_Receive(USART1, &ch, 1);
    return ch;
}

__attribute__((weak)) int __io_putchar(int ch)
{
    // USART_Transmit(USART1, (uint8_t *)&ch, 1);
    return ITM_SendChar(ch);
}

char *__env[1] = {0};
char **environ = __env;

#ifndef __clang__
#define FUNC_PREFIX(string) _##string
#else
#define FUNC_PREFIX(string) string
#endif

__attribute__((weak)) int FUNC_PREFIX(read)(int file, char *ptr, int len)
{
    (void)file;
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        *ptr++ = __io_getchar();
    }

    return len;
}

__attribute__((weak)) int FUNC_PREFIX(write)(int file, char *ptr, int len)
{
    (void)file;
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}

int FUNC_PREFIX(close)(int file)
{
    (void)file;
    return -1;
}

void _exit(int status)
{
    (void)status;
    while (1)
    {
    } /* Make sure we hang here */
}

int remove(const char *path)
{
    (void)path;
    return -1;
}

void *FUNC_PREFIX(sbrk)(size_t incr)
{
    extern char _heap_start;
    extern char _heap_end;
    (void)incr;
    void *ret = 0;
    static char *heap_top = &_heap_start;
    char *new_heap_top = heap_top + incr;
    if (new_heap_top > &_heap_end)
    {
        return (void *)(-1);
    }
    if (new_heap_top < &_heap_start)
        abort();
    ret = (void *)heap_top;
    heap_top = new_heap_top;
    return ret;
}

#ifndef __clang__
int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _fstat(int file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}
#endif