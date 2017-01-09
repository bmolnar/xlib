#ifndef _XLIB_XSTREAM_STDIO_H
#define _XLIB_XSTREAM_STDIO_H

#include <stdio.h>

#include <xlib/xdefs.h>
#include <xlib/xstream.h>

__XLIB_BEGIN_DECL

xstream_t *xstream_stdio_open(FILE *fp);

__XLIB_END_DECL

#endif /* _XLIB_XSTREAM_STDIO_H */
