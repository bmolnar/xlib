#include <stdarg.h>

#include <xlib/xfmt.h>
#include <xlib/xprint.h>
#include <xlib/xstream.h>
#include <xlib/xprint_wrappers.h>

static int
_xstream_op_write(xprint_t *xp, const void *buf, size_t len)
{
    xstream_t *xs = (xstream_t *) XPRINT_PRIV(xp);
    return (int) xstream_write(xs, buf, len);
}
xprint_ops_t _xstream_xpops = {
    .op_write = &_xstream_op_write,
    .op_atwrap = NULL,
};



static int
_wrap_stdio_op_write(xprint_t *xp, const void *buf, size_t len)
{
    FILE *fp = (FILE *) XPRINT_PRIV(xp);
    return (int) fwrite(buf, len, 1, fp);
}
xprint_ops_t _wrap_stdio_xpops = {
    .op_write = &_wrap_stdio_op_write,
    .op_atwrap = NULL,
};
