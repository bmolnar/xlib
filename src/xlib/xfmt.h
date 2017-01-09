#ifndef _XLIB_XFMT_H
#define _XLIB_XFMT_H

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdio.h>

#include "xdefs.h"
#include "xtypes.h"

__XLIB_BEGIN_DECL

/*
 * xfmt_flags_t
 */
typedef uint8_t xfmt_flags_t;
#define XFMT_FLAG_ALTFORM (1 << 0)
#define XFMT_FLAG_ZEROPAD (1 << 1)
#define XFMT_FLAG_LEFTADJ (1 << 2)
#define XFMT_FLAG_SPACE   (1 << 3)
#define XFMT_FLAG_PLUS    (1 << 4)

typedef uint8_t xfmt_width_t;
typedef uint8_t xfmt_prec_t;

typedef enum {
    XFMT_LEN_NONE,
    XFMT_LEN_HH,
    XFMT_LEN_H,
    XFMT_LEN_L,
    XFMT_LEN_LL,
    XFMT_LEN_Z,
} xfmt_len_t;


/*
 * xfmt_base_t
 */
extern char xfmt_base16upper[];
extern char xfmt_base16lower[];
typedef enum {
    XFMT_BASE_8,
    XFMT_BASE_10,
    XFMT_BASE_16_LOWER,
    XFMT_BASE_16_UPPER,
} xfmt_base_t;
static inline uint8_t
xfmt_base_radix(xfmt_base_t base)
{
    switch (base) {
    case XFMT_BASE_8:
        return 8;
    case XFMT_BASE_16_LOWER:
    case XFMT_BASE_16_UPPER:
        return 16;
    case XFMT_BASE_10:
    default:
        return 10;
    }
}
static inline char
xfmt_base_digitchar(xfmt_base_t base, uint8_t num)
{
    switch (base) {
    case XFMT_BASE_16_UPPER:
        return xfmt_base16upper[num];
    case XFMT_BASE_16_LOWER:
    case XFMT_BASE_8:
    case XFMT_BASE_10:
    default:
        return xfmt_base16lower[num];
    }
}

/*
 * xfmt_conv_t
 */
typedef char xfmt_conv_t;
static inline xfmt_base_t
xfmt_conv_base(xfmt_conv_t conv)
{
    switch (conv) {
    case 'o':
        return XFMT_BASE_8;
    case 'x':
        return XFMT_BASE_16_LOWER;
    case 'X':
        return XFMT_BASE_16_UPPER;
    case 'd':
    case 'i':
    case 'u':
    default:
        return XFMT_BASE_10;
    }
}
static inline int
xfmt_conv_issigned(xfmt_conv_t conv)
{
    switch (conv) {
    case 'd':
    case 'i':
        return 1;
    case 'o':
    case 'x':
    case 'X':
    case 'u':
    default:
        return 0;
    }
}


/*
 * xfmt_spec_t
 */
struct xfmt_spec {
    xfmt_flags_t  flags;
    xfmt_width_t  width;
    xfmt_prec_t   prec;
    xfmt_len_t    len;
    xfmt_conv_t   conv;
};
typedef struct xfmt_spec xfmt_spec_t;

static inline xfmt_flags_t
xfmt_spec_flags(const struct xfmt_spec *fmtspec)
{
    return fmtspec->flags;
}
static inline xfmt_base_t
xfmt_spec_base(const struct xfmt_spec *fmtspec)
{
    return xfmt_conv_base(fmtspec->conv);
}
static inline int
xfmt_spec_issigned(const struct xfmt_spec *fmtspec)
{
    return xfmt_conv_issigned(fmtspec->conv);
}



#define XFMT_CHAR_ISNULL(c)          ((c) == '\0')
#define XFMT_CHAR_ISBASE8(c)         ((c) >= '0' && (c) <= '7')
#define XFMT_CHAR_ISBASE10(c)        ((c) >= '0' && (c) <= '9')
#define XFMT_CHAR_ISBASE16UALPHA(c)  ((c) >= 'A' && (c) <= 'F')
#define XFMT_CHAR_ISBASE16U(c)       (XFMT_CHAR_ISBASE10(c) || XFMT_CHAR_ISBASE16UALPHA(c))
#define XFMT_CHAR_ISBASE16LALPHA(c)  ((c) >= 'a' && (c) <= 'f')
#define XFMT_CHAR_ISBASE16L(c)       (XFMT_CHAR_ISBASE10(c) || XFMT_CHAR_ISBASE16LALPHA(c))
#define XFMT_CHAR_ISCONVSTART(c)     ((c) == '%')
#define XFMT_CHAR_ISFLAG(c)          ((c) == '#' || (c) == '0' || (c) == '-' || (c) == ' ' || (c) == '+')
#define XFMT_CHAR_ISWIDTHSTART(c)    ((c) >= '1' && (c) <= '9')
#define XFMT_CHAR_ISWIDTHCONT(c)     ((c) >= '0' && (c) <= '9')
#define XFMT_CHAR_ISPRECSTART(c)     ((c) == '.')
#define XFMT_CHAR_ISPRECCONT(c)      ((c) >= '0' && (c) <= '9')
#define XFMT_CHAR_ISLENGTHSTART(c)   ((c) == 'h' || (c) == 'l' || (c) == 'z')
#define XFMT_CHAR_ISLENGTHCONT(c)    ((c) == 'h' || (c) == 'l')
#define XFMT_CHAR_ISCONVSPEC(c)      ((c) == 'd' || (c) == 'i' || (c) == 'o' || (c) == 'u' || \
                                      (c) == 'x' || (c) == 'X' || (c) == 'e' || (c) == 'E' || \
                                      (c) == 'f' || (c) == 'F' || (c) == 'g' || (c) == 'G' || \
                                      (c) == 'a' || (c) == 'A' || (c) == 'c' || (c) == 's' || \
                                      (c) == '@' || (c) == '%')



struct xfmt_handler;
typedef struct xfmt_handler xfmt_handler_t;


/*
 * xfmt_val
 */
typedef int                         xfmt_carg_t;
typedef unsigned long long          xfmt_lluarg_t;
typedef long long                   xfmt_lliarg_t;
typedef unsigned long               xfmt_luarg_t;
typedef long                        xfmt_liarg_t;
typedef unsigned int                xfmt_uarg_t;
typedef int                         xfmt_iarg_t;
typedef size_t                      xfmt_zuarg_t;
typedef ssize_t                     xfmt_ziarg_t;
typedef double                      xfmt_farg_t;
typedef const char *                xfmt_sarg_t;
typedef void *                      xfmt_atarg_arg_t;
struct xfmt_atarg_arglist {
    uint8_t                 argc;
    xfmt_atarg_arg_t        argv[8];
};
typedef struct xfmt_atarg_arglist   xfmt_atarg_arglist_t;
typedef int                        (xfmt_atarg_fn_t)(void *, const void *);
typedef const xfmt_atarg_fn_t *     xfmt_atarg_fnptr_t;
typedef const void *                xfmt_atarg_ptr_t;
struct xfmt_atarg {
    xfmt_atarg_fnptr_t       fn;
    xfmt_atarg_ptr_t         ptr;
};
typedef struct xfmt_atarg xfmt_atarg_t;


typedef unsigned long       xfmt_unum_t;
typedef long                xfmt_inum_t;


typedef union {
    xfmt_uarg_t        v_u;
    xfmt_iarg_t        v_i;
    xfmt_carg_t        v_c;
    xfmt_luarg_t       v_lu;
    xfmt_liarg_t       v_li;
    xfmt_lluarg_t      v_llu;
    xfmt_lliarg_t      v_lli;
    xfmt_zuarg_t       v_zu;
    xfmt_ziarg_t       v_zi;
    xfmt_farg_t        v_f;
    xfmt_sarg_t        v_s;
    xfmt_atarg_t       v_at;
    xfmt_unum_t        v_unum;
    xfmt_inum_t        v_inum;
} xfmt_val_t;

struct xfmt_va_list {
    va_list ap;
};
typedef struct xfmt_va_list xfmt_va_list_t;
#define xfmt_va_copy(xva,va) va_copy(xva.ap,va)
#define xfmt_va_end(xva)     va_end(xva.ap)


/*
 * xfmt_handler
 */
//struct xfmt_handler;
//typedef struct xfmt_handler xfmt_handler_t;

typedef int (xfmt_handler_begin_fn_t)(xfmt_handler_t *xp, const char *pos);
typedef int (xfmt_handler_end_fn_t)(xfmt_handler_t *xp, const char *pos);
typedef int (xfmt_handler_onval_fn_t)(xfmt_handler_t *xp, const char *pos, const char *end, const struct xfmt_spec *fmtspec, const xfmt_val_t *val);
typedef int (xfmt_handler_onchars_fn_t)(xfmt_handler_t *xp, const char *pos, const char *end);

struct xfmt_handler_ops {
    xfmt_handler_begin_fn_t *          op_begin;
    xfmt_handler_end_fn_t *            op_end;
    xfmt_handler_onval_fn_t *          op_onval;
    xfmt_handler_onchars_fn_t *        op_onchars;
};
typedef struct xfmt_handler_ops xfmt_handler_ops_t;

struct xfmt_handler {
    xfmt_handler_ops_t                 xf_ops;
    void *                             xf_priv;
};
typedef struct xfmt_handler xfmt_handler_t;

#define XFMT_HANDLER_INIT(ops,priv) ((xfmt_handler_t) { .xf_ops = (ops), .xf_priv = (priv) })
#define XFMT_HANDLER_OPS(xf)        (&(xf)->xf_ops)
#define XFMT_HANDLER_PRIV(xf)       ((xf)->xf_priv)

/*
 * xfmt
 */
int xfmt_parsev(xfmt_handler_t *xp, const char *format, va_list ap);
int xfmt_parse(xfmt_handler_t *xp, const char *format, ...);

__XLIB_END_DECL

#endif /* _XLIB_XFMT_H */
