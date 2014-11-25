#ifndef _XMEMBUF_H
#define _XMEMBUF_H

#include <stdlib.h>

#include <xlib/xtypes.h>

struct xmembuf {
    void * mb_buf;
    size_t mb_capacity;
    size_t mb_limit;
    size_t mb_position;
    size_t mb_mark;
};

void xmembuf_init(struct xmembuf *mbuf, void *buf, size_t capacity);
size_t xmembuf_capacity(struct xmembuf *mbuf);
size_t xmembuf_remaining(struct xmembuf *mbuf);
size_t xmembuf_position(struct xmembuf *mbuf);
int xmembuf_setposition(struct xmembuf *mbuf, size_t position);
size_t xmembuf_limit(struct xmembuf *mbuf);
int xmembuf_setlimit(struct xmembuf *mbuf, size_t limit);
void xmembuf_clear(struct xmembuf *mbuf);
void xmembuf_flip(struct xmembuf *mbuf);
void xmembuf_mark(struct xmembuf *mbuf);
void xmembuf_reset(struct xmembuf *mbuf);
void xmembuf_rewind(struct xmembuf *mbuf);
ssize_t xmembuf_memptrs(struct xmembuf *mbuf, struct iovec *iov, size_t iovcnt);
ssize_t xmembuf_putxmembuf(struct xmembuf *mbuf, struct xmembuf *src);
ssize_t xmembuf_putbytes(struct xmembuf *mbuf, const unsigned char *buf, size_t count);
ssize_t xmembuf_getbytes(struct xmembuf *mbuf, unsigned char *buf, size_t count);

#endif /* _XMEMBUF_H */
