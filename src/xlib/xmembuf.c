#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <string.h>

#include <xlib/xtypes.h>
#include <xlib/xerr.h>
#include <xlib/xmembuf.h>

static size_t
memcpyv(const struct iovec *dstiov, int dstiovlen, const struct iovec *srciov, int srciovlen, size_t count)
{
    int dstidx = 0, srcidx = 0;
    size_t dstpos = 0, srcpos = 0;
    size_t total = 0;
    size_t left;

    while (total < count && dstidx < dstiovlen && srcidx < srciovlen) {
        left = count - total;
        if (left > (srciov[srcidx].iov_len - srcpos))
            left = (srciov[srcidx].iov_len - srcpos);
        if (left > (dstiov[dstidx].iov_len - dstpos))
            left = (dstiov[dstidx].iov_len - dstpos);
        memcpy(((unsigned char *) dstiov[dstidx].iov_base) + dstpos,
               ((unsigned char *) srciov[srcidx].iov_base) + srcpos, left);
        total += left;
        srcpos += left;
        dstpos += left;
        if (srcpos >= srciov[srcidx].iov_len) {
            srcidx++;
            srcpos = 0;
        }
        if (dstpos >= dstiov[dstidx].iov_len) {
            dstidx++;
            dstpos = 0;
        }
    }
    return total;
}

void
xmembuf_init(struct xmembuf *mbuf, void *buf, size_t capacity)
{
    mbuf->mb_buf = buf;
    mbuf->mb_capacity = capacity;
    mbuf->mb_limit = mbuf->mb_capacity;
    mbuf->mb_position = 0;
}

size_t
xmembuf_capacity(struct xmembuf *mbuf)
{
    return mbuf->mb_capacity;
}

size_t
xmembuf_remaining(struct xmembuf *mbuf)
{
    return (mbuf->mb_position < mbuf->mb_limit) ? (mbuf->mb_limit - mbuf->mb_position) : 0;
}

size_t
xmembuf_position(struct xmembuf *mbuf)
{
    return mbuf->mb_position;
}

int
xmembuf_setposition(struct xmembuf *mbuf, size_t position)
{
    if (position > mbuf->mb_limit)
        return -XERR_NOBUFS;
    mbuf->mb_position = position;
    if (position < mbuf->mb_mark)
        mbuf->mb_mark = 0;
    return 0;
}

size_t
xmembuf_limit(struct xmembuf *mbuf)
{
    return mbuf->mb_limit;
}

int
xmembuf_setlimit(struct xmembuf *mbuf, size_t limit)
{
    if (limit > mbuf->mb_capacity)
        return -XERR_NOBUFS;
    mbuf->mb_limit = limit;
    return 0;
}

void
xmembuf_clear(struct xmembuf *mbuf)
{
    mbuf->mb_position = 0;
    mbuf->mb_limit = mbuf->mb_capacity;
    mbuf->mb_mark = 0;
}

void
xmembuf_flip(struct xmembuf *mbuf)
{
    mbuf->mb_limit = mbuf->mb_position;
    mbuf->mb_position = 0;
}

void
xmembuf_mark(struct xmembuf *mbuf)
{
    mbuf->mb_mark = mbuf->mb_position;
}

void
xmembuf_reset(struct xmembuf *mbuf)
{
    mbuf->mb_position = mbuf->mb_mark;
}

void
xmembuf_rewind(struct xmembuf *mbuf)
{
    mbuf->mb_position = mbuf->mb_mark = 0;
}

ssize_t
xmembuf_memptrs(struct xmembuf *mbuf, struct iovec *iov, size_t iovcnt)
{
    iov[0].iov_base = ((unsigned char *) mbuf->mb_buf) + mbuf->mb_position;
    iov[0].iov_len = xmembuf_remaining(mbuf);
    return 1;
}

ssize_t
xmembuf_putxmembuf(struct xmembuf *mbuf, struct xmembuf *src)
{
    struct iovec dstiov[1];
    struct iovec srciov[1];
    size_t nbytes = xmembuf_remaining(src);

    if (nbytes > xmembuf_remaining(mbuf))
        return -XERR_NOBUFS;
    xmembuf_memptrs(mbuf, dstiov, 1);
    xmembuf_memptrs(src, srciov, 1);
    nbytes = memcpyv(dstiov, 1, srciov, 1, nbytes);
    mbuf->mb_position += nbytes;
    src->mb_position += nbytes;
    return (ssize_t) nbytes;
}

ssize_t
xmembuf_putbytes(struct xmembuf *mbuf, const unsigned char *buf, size_t count)
{
    if (mbuf->mb_position + count > mbuf->mb_limit)
        return -XERR_NOBUFS;
    memcpy(((unsigned char *) mbuf->mb_buf) + mbuf->mb_position, buf, count);
    mbuf->mb_position += count;
    return (ssize_t) count;
}

ssize_t
xmembuf_getbytes(struct xmembuf *mbuf, unsigned char *buf, size_t count)
{
    if (mbuf->mb_position + count > mbuf->mb_limit)
        return -XERR_NOBUFS;
    memcpy(buf, ((unsigned char *) mbuf->mb_buf) + mbuf->mb_position, count);
    mbuf->mb_position += count;
    return (ssize_t) count;
}
