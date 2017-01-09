#include <stdarg.h>

#include <xlib/xstream.h>
#include <xlib/xprint.h>

static int
_xsprint_op_write(xprint_t *xp, const void *buf, size_t len)
{
    xstream_t *xs = (xstream_t *) XPRINT_PRIV(xp);
    return (int) xstream_write(xs, buf, len);
}
static xprint_ops_t _xsprint_xpops = {
    .op_write = &_xsprint_op_write,
    .op_atwrap = NULL,
};
int
xsprintfv(xstream_t *xs, const char *format, va_list ap)
{
    xprint_t xp = XPRINT_INIT(_xsprint_xpops, xs);
    return xprint_printfv(&xp, format, ap);
}
int
xsprintf(xstream_t *xs, const char *format, ...)
{
    xprint_t xp = XPRINT_INIT(_xsprint_xpops, xs);
    int rv;
    va_list ap;

    va_start(ap, format);
    rv = xprint_printfv(&xp, format, ap);
    va_end(ap);

    return rv;
}

