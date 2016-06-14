#ifndef _XPRINT_WRAPPERS_H
#define _XPRINT_WRAPPERS_H

#include <stdarg.h>

#include <xlib/xfmt.h>
#include <xlib/xprint.h>
#include <xlib/xstream.h>

__XLIB_BEGIN_DECL

extern xprint_ops_t _xstream_xpops;
#define XPRINT_INIT_XSTREAM(xs) XPRINT_INIT(_xstream_xpops, xs)

extern xprint_ops_t _wrap_stdio_xpops;
#define XPRINT_INIT_STDIO(fp) XPRINT_INIT(_wrap_stdio_xpops, fp)

__XLIB_END_DECL

#endif /* _XPRINT_WRAPPERS_H */
