#ifndef _KFIFO_H
#define _KFIFO_H

struct kfifo {
    unsigned char *buffer;    /* the buffer holding the data */
    unsigned int size;    /* the size of the allocated buffer */
    unsigned int in;    /* data is added at offset (in % size) */
    unsigned int out;    /* data is extracted from off. (out % size) */
};

// min
#define min(a, b) (((a) < (b)) ? (a) : (b))

struct kfifo *kfifo_alloc(unsigned char *buffer, unsigned int size);
void kfifo_free(struct kfifo *fifo);
unsigned int kfifo_put(struct kfifo *fifo, unsigned char *buffer, unsigned int len);
unsigned int kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len);
unsigned int kfifo_len(struct kfifo *fifo);

#endif
