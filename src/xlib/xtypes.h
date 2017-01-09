#ifndef _XLIB_XTYPES_H
#define _XLIB_XTYPES_H

#include <xlib/xdefs.h>
#include <xlib/xconfig.h>

__XLIB_BEGIN_DECL

#ifndef HAVE_SIZE_T
typedef unsigned long size_t;
#endif /* HAVE_SIZE_T */

#ifndef HAVE_SSIZE_T
typedef long ssize_t;
#endif /* HAVE_SSIZE_T */

#ifndef HAVE_IOVEC
struct iovec {
    void * iov_base;
    size_t iov_len;
};
#endif /* HAVE_IOVEC */

__XLIB_END_DECL

#endif /* _XLIB_XTYPES_H */
