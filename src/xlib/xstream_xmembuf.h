#ifndef _XLIB_XSTREAM_XMEMBUF_H
#define _XLIB_XSTREAM_XMEMBUF_H

#include <xlib/xdefs.h>
#include <xlib/xstream.h>
#include <xlib/xmembuf.h>

__XLIB_BEGIN_DECL

xstream_t *xstream_xmembuf_open(const char *mode, struct xmembuf *mbuf);

__XLIB_END_DECL

#endif /* _XLIB_XSTREAM_XMEMBUF_H */
