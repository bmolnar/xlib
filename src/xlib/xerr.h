#ifndef _XERR_H
#define _XERR_H

#include <stdint.h>

#include <xlib/xtypes.h>

__XLIB_BEGIN_DECL

typedef enum {
    XERR_OK = 0,
    XERR_NOSYS = 1,
    XERR_NOENT = 2,
    XERR_NOMEM = 3,
    XERR_INVAL = 4,
    XERR_NOBUFS = 5,
    XERR_MAXNUM = 6
} xerr_t;

const char *xerr_str(xerr_t xerr);

#define XERR_GETNUM(xerr)  ((int) xerr)
#define XERR_FORNUM(num)   ((((num) >= 0) || ((num) < XERR_MAXNUM)) ? ((xerr_t)(num)) : XERR_MAXNUM)


#define XERRINT(xerr)         (-((int) xerr))
#define XERRINT_ERRNUM(v)     ((-(v) > 0 && -(v) <= XERR_MAXNUM) ? (xerr_t)-(v) : 0)
#define XERRINT_ISERROR(v)    (XERRINT_ERRNUM(v) > 0)


#define __PTRDIFF(a,b)        (((uint8_t *) a) - ((uint8_t *) b))
#define XERRPTR(num)          (((uint8_t *) 0) - num)
#define XERRPTR_ERRNUM(ptr)   ((__PTRDIFF(0,ptr) <= XERR_MAXNUM) ? (xerr_t) __PTRDIFF(0,ptr) : 0)
#define XERRPTR_ISERROR(ptr)  (XERRPTR_ERRNUM(ptr) > 0)

__XLIB_END_DECL

#endif /* _XERR_H */
