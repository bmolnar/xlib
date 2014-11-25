#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <errno.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <xlib/xerr.h>
#include <xlib/xstream.h>

static int
xstream_stdio_open_fn(xstream_t *xs, const char *mode, void *userdata)
{
    XSTREAM_PRIV(xs) = userdata;
    return 0;
}

static ssize_t
xstream_stdio_write_fn(xstream_t *xs, const void *buf, size_t count)
{
    FILE *fp = (FILE *) XSTREAM_PRIV(xs);
    return (ssize_t) fwrite(buf, 1, count, fp);
}

static ssize_t
xstream_stdio_read_fn(xstream_t *xs, void *buf, size_t count)
{
    FILE *fp = (FILE *) XSTREAM_PRIV(xs);
    return (ssize_t) fread(buf, 1, count, fp);
}

static int
xstream_stdio_close_fn(xstream_t *xs)
{
    FILE *fp = (FILE *) XSTREAM_PRIV(xs);
    return fclose(fp);
}

static int
xstream_stdio_flush_fn(xstream_t *xs)
{
    FILE *fp = (FILE *) XSTREAM_PRIV(xs);
    return fflush(fp);
}

static xstream_ops_t xstream_stdio_ops = {
    .open  = &xstream_stdio_open_fn,
    .write = &xstream_stdio_write_fn,
    .read  = &xstream_stdio_read_fn,
    .close = &xstream_stdio_close_fn,
    .flush = &xstream_stdio_flush_fn,
};

xstream_t *
xstream_stdio_open(FILE *fp)
{
    return xstream_open(xstream_stdio_ops, NULL, (void *) fp);
}
