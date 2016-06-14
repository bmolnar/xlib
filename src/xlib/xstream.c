#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <xlib/xerr.h>
#include <xlib/xstream.h>

static int
xstream_do_open(xstream_t *xs, const char *mode, void *userdata)
{
    xstream_ops_t *ops = XSTREAM_OPS(xs);
    if (ops->op_open == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*ops->op_open)(xs, mode, userdata);
}
static int
xstream_do_close(xstream_t *xs)
{
    xstream_ops_t *ops = XSTREAM_OPS(xs);
    if (ops->op_close == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*ops->op_close)(xs);
}
static ssize_t
xstream_do_write(xstream_t *xs, const void *buf, size_t sz)
{
    xstream_ops_t *ops = XSTREAM_OPS(xs);
    if (ops->op_write == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*ops->op_write)(xs, buf, sz);
}
static ssize_t
xstream_do_read(xstream_t *xs, void *buf, size_t sz)
{
    xstream_ops_t *ops = XSTREAM_OPS(xs);
    if (ops->op_read == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*ops->op_read)(xs, buf, sz);
}
static int
xstream_do_flush(xstream_t *xs)
{
    xstream_ops_t *ops = XSTREAM_OPS(xs);
    if (ops->op_flush == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*ops->op_flush)(xs);
}


/*
 * public interface
 */

int
xstream_init(xstream_t *xs, xstream_ops_t ops)
{
    xs->xs_ops = ops;
    return 0;
}

void
xstream_cleanup(xstream_t *xs)
{
}

xstream_t *
xstream_create(xstream_ops_t ops)
{
    xstream_t *xs;
    int rv = 0;

    xs = malloc(sizeof(*xs));
    if (xs == NULL) {
        errno = XERR_NOMEM;
        return NULL;
    }

    rv = xstream_init(xs, ops);
    if (rv < 0) {
        free(xs);
        errno = -rv;
        return NULL;
    }

    return xs;
}

void
xstream_destroy(xstream_t *xs)
{
    xstream_cleanup(xs);
    free(xs);
}





xstream_t *
xstream_open(xstream_ops_t ops, const char *mode, void *userdata)
{
    xstream_t *xs;
    int rv;

    xs = xstream_create(ops);
    if (xs == NULL)
    {
        return xs;
    }
    rv = xstream_do_open(xs, mode, userdata);
    if (rv < 0 && rv != -XERR_NOSYS)
    {
        free(xs);
        errno = -rv;
        return NULL;
    }
    return xs;
}

int
xstream_close(xstream_t *xs)
{
    int rv;

    rv = xstream_do_close(xs);
    xstream_destroy(xs);
    return rv;
}

ssize_t
xstream_write(xstream_t *xs, const void *buf, size_t sz)
{
    return xstream_do_write(xs, buf, sz);
}

ssize_t
xstream_read(xstream_t *xs, void *buf, size_t sz)
{
    return xstream_do_read(xs, buf, sz);
}

int
xstream_flush(xstream_t *xs)
{
    return xstream_do_flush(xs);
}
