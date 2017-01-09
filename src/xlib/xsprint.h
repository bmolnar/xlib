#ifndef _XSPRINT_H
#define _XSPRINT_H

#include <stdarg.h>

#include <xlib/xprint.h>
#include <xlib/xstream.h>

__XLIB_BEGIN_DECL

int xsprintfv(xstream_t *xs, const char *format, va_list ap);
int xsprintf(xstream_t *xs, const char *format, ...);

__XLIB_END_DECL

#endif /* _XSPRINT_H */
