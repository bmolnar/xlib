#ifndef _XPRINTF_H
#define _XPRINTF_H

#include <stdarg.h>

#include <xlib/xtypes.h>
#include <xlib/xerr.h>

__XLIB_BEGIN_DECL

struct xstream;
typedef struct xstream xstream_t;

typedef int     (xstream_open_fn_t)(xstream_t *xp, const char *mode, void *userdata);
typedef ssize_t (xstream_write_fn_t)(xstream_t *xp, const void *buf, size_t size);
typedef ssize_t (xstream_read_fn_t)(xstream_t *xp, void *buf, size_t size);
typedef int     (xstream_flush_fn_t)(xstream_t *xp);
typedef int     (xstream_close_fn_t)(xstream_t *xp);

typedef struct xstream_ops {
    xstream_open_fn_t * open;
    xstream_write_fn_t *write;
    xstream_read_fn_t * read;
    xstream_flush_fn_t *flush;
    xstream_close_fn_t *close;
} xstream_ops_t;

struct xstream {
    xstream_ops_t xs_ops;
    void *        xs_priv;
};

/* xstream */
#define XSTREAM_PRIV(xs) ((xs)->xs_priv)

xstream_t *xstream_open(xstream_ops_t ops, const char *mode, void *userdata);
int xstream_close(xstream_t *xs);
ssize_t xstream_write(xstream_t *xs, const void *buf, size_t sz);
ssize_t xstream_read(xstream_t *xs, void *buf, size_t sz);
int xstream_flush(xstream_t *xs);

/* xprintx */
typedef int (xprintx_atval_fn_t)(xstream_t *, void *);
int vxprintx(xstream_t *xp, const char *format, va_list ap);
int xprintx(xstream_t *xp, const char *format, ...);

__XLIB_END_DECL

#endif /* _XPRINTF_H */
