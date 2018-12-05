#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "kfifo.h"


//判断x是否是2的次方
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))


void __kfifo_reset(struct kfifo *fifo)
{
    fifo->in = fifo->out = 0;
}

void kfifo_reset(struct kfifo *fifo)
{
    __kfifo_reset(fifo);
}

unsigned int __kfifo_len(struct kfifo *fifo)
{
    return fifo->in - fifo->out;
}

unsigned int kfifo_len(struct kfifo *fifo)
{
    unsigned int ret;

    ret = __kfifo_len(fifo);

    return ret;
}

struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size)
{
    struct kfifo *fifo;

    fifo = malloc(sizeof(struct kfifo));
    if (!fifo)
        return NULL;

    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = fifo->out = 0;

    return fifo;
}

struct kfifo *kfifo_alloc(unsigned char *buffer, unsigned int size)
{
    //unsigned char *buffer;
    struct kfifo *ret;
#if 0	
    if (size & (size - 1)) {
        size = is_power_of_2(size);
    }
#endif

    //buffer = malloc(size);
    if (!buffer)
        return NULL;

    ret = kfifo_init(buffer, size);
#if 0
    if ((unsigned long)ret<=0)
    {
        free(buffer);
    }
#endif

    return ret;
}

void kfifo_free(struct kfifo *fifo)
{
#if 0
    free(fifo->buffer);
#endif
    free(fifo);
}

unsigned int __kfifo_put(struct kfifo *fifo, unsigned char *buffer, unsigned int len)
{
    unsigned int l;

    len = min(len, fifo->size - fifo->in + fifo->out);

    l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
    memcpy(fifo->buffer, buffer + l, len - l);

    fifo->in += len;

    return len;
}

unsigned int kfifo_put(struct kfifo *fifo,
                     unsigned char *buffer, unsigned int len)
{
    unsigned int ret;

    ret = __kfifo_put(fifo, buffer, len);

    return ret;
}

unsigned int __kfifo_get(struct kfifo *fifo,
             unsigned char *buffer, unsigned int len)
{
    unsigned int l;

    len = min(len, fifo->in - fifo->out);

    l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
		memcpy(buffer + l, fifo->buffer, len - l);

    fifo->out += len;
    return len;
}

unsigned int kfifo_get(struct kfifo *fifo,
                     unsigned char *buffer, unsigned int len)
{
    unsigned int ret;

    ret = __kfifo_get(fifo, buffer, len);

    return ret;
}
