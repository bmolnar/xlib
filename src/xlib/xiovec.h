#ifndef _XLIB_XIOVEC_H
#define _XLIB_XIOVEC_H

#include <stdint.h>

__XLIB_BEGIN_DECL

struct xiovec {
    void * iov_base;
    size_t iov_len;
};

__XLIB_END_DECL

#endif /* _XLIB_XIOVEC_H */
