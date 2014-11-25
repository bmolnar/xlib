#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

#include <xlib/xerr.h>
#include <xlib/xstream.h>

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
    case XFMT_BASE_16_LOWER:
        return (num < 10) ? ('0' + num) : ('a' + (num-10));
    case XFMT_BASE_16_UPPER:
        return (num < 10) ? ('0' + num) : ('A' + (num-10));
    case XFMT_BASE_8:
    case XFMT_BASE_10:
    default:
        return '0' + num;
    }
}

#define XFMT_FLAG_ALTFORM (1 << 0)
#define XFMT_FLAG_ZEROPAD (1 << 1)
#define XFMT_FLAG_LEFTADJ (1 << 2)
#define XFMT_FLAG_SPACE   (1 << 3)
#define XFMT_FLAG_PLUS    (1 << 4)

typedef enum {
    XFMT_LEN_NONE,
    XFMT_LEN_HH,
    XFMT_LEN_H,
    XFMT_LEN_L,
    XFMT_LEN_LL,
    XFMT_LEN_Z,
} xfmt_len_t;

struct xfmtspec {
    uint8_t     flags;
    uint8_t     width;
    uint8_t     prec;
    xfmt_len_t  len;
    char        conv;
};

static inline xfmt_base_t
xfmtspec_getbase(struct xfmtspec *fmtspec)
{
    switch (fmtspec->conv) {
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

static char convchars[] = "diouxXeEfFgGaAcs@%";

static int
xprintx_isconvchar(char c)
{
    char *ptr;
    for (ptr = &convchars[0]; *ptr != '\0'; ptr++) {
        if (*ptr == c)
            return 1;
    }
    return 0;
}

#define XPRINTX_ISFLAGCHAR(c) ((c) == '#' || (c) == '0' || (c) == '-' || (c) == ' ' || (c) == '+')
#define XPRINTX_ISWIDTHSTART(c) ((c) >= '1' && (c) <= '9')
#define XPRINTX_ISWIDTHCHAR(c) ((c) >= '0' && (c) <= '9')
#define XPRINTX_ISPRECSTART(c) ((c) == '.')
#define XPRINTX_ISPRECCHAR(c) ((c) >= '0' && (c) <= '9')
#define XPRINTX_ISLENGTHSTART(c) ((c) == 'h' || (c) == 'l' || (c) == 'z')
#define XPRINTX_ISLENGTHCHAR(c) ((c) == 'h' || (c) == 'l')
#define XPRINTX_ISCONVCHAR(c) (xprintx_isconvchar((c)))

int
xprintx_getfmtspec(const char *fmtptr, struct xfmtspec *fmtspec, char **next)
{
    char *ptr = (char *) fmtptr;
    unsigned long ulval;

    if (*ptr++ != '%')
        return -1;

    fmtspec->flags = 0;
    while (XPRINTX_ISFLAGCHAR(*ptr)) {
        switch (*ptr++) {
        case '#':
            fmtspec->flags |= XFMT_FLAG_ALTFORM;
            break;
        case '0':
            fmtspec->flags |= XFMT_FLAG_ZEROPAD;
            break;
        case '-':
            fmtspec->flags |= XFMT_FLAG_LEFTADJ;
            break;
        case ' ':
            fmtspec->flags |= XFMT_FLAG_SPACE;
            break;
        case '+':
            fmtspec->flags |= XFMT_FLAG_PLUS;
            break;
        default:
            break;
        }
    }

    fmtspec->width = 0;
    if (XPRINTX_ISWIDTHSTART(*ptr)) {
        ulval = strtoul(ptr, (char **) &ptr, 10);
        if (ulval == ULONG_MAX && errno == ERANGE)
            return -XERR_INVAL;
        fmtspec->width = (uint8_t) ulval;
    }

    fmtspec->prec = 0;
    if (XPRINTX_ISPRECSTART(*ptr)) {
        ptr++;
        ulval = strtoul(ptr, (char **) &ptr, 10);
        if (ulval == ULONG_MAX && errno == ERANGE)
            return -XERR_INVAL;
        fmtspec->prec = (uint8_t) ulval;
    }

    fmtspec->len = XFMT_LEN_NONE;
    if (XPRINTX_ISLENGTHSTART(*ptr)) {
        switch (*ptr++) {
        case 'h':
            if (*ptr == 'h') {
                fmtspec->len = XFMT_LEN_HH;
                ptr++;
            } else {
                fmtspec->len = XFMT_LEN_H;
            }
            break;
        case 'l':
            if (*ptr == 'l') {
                fmtspec->len = XFMT_LEN_LL;
            } else {
                fmtspec->len = XFMT_LEN_L;
            }
            break;
        case 'z':
            fmtspec->len = XFMT_LEN_Z;
            break;
        default:
            return -XERR_INVAL;
        }
    }

    fmtspec->conv = 0;
    if (XPRINTX_ISCONVCHAR(*ptr)) {
        fmtspec->conv = *ptr++;
    } else {
        return -XERR_INVAL;
    }

    *next = ptr;
    return 0;
}

static double
xprintx_rounding(uint8_t digit)
{
    switch(digit) {
    case 0:
        return 0.5;
    case 1:
        return 0.05;
    case 2:
        return 0.005;
    case 3:
        return 0.0005;
    case 4:
        return 0.00005;
    case 5:
        return 0.000005;
    case 6:
        return 0.0000005;
    case 7:
        return 0.00000005;
    default:
        return 0.000000005;
    }
}

static unsigned long
xprintx_pow10ul(uint8_t pow)
{
    switch(pow) {
    case 0:
        return 1;
    case 1:
        return 10;
    case 2:
        return 100;
    case 3:
        return 1000;
    case 4:
        return 10000;
    case 5:
        return 100000;
    case 6:
        return 1000000;
    case 7:
        return 10000000;
    default:
        return 100000000;
    }
}

static int
xprintx_printchar(xstream_t *xp, char c)
{
    return (int) xstream_write(xp, (const void *) &c, sizeof(c));
}

static int
xprintx_printstr(xstream_t *xp, char *str)
{
    return (int) xstream_write(xp, (const void *) str, (size_t) strlen(str));
}

static int
xprintx_printnum(xstream_t *xp, unsigned long num, int8_t sgn, struct xfmtspec *fmtspec)
{
    char buf[3*sizeof(num) + 1];
    char *strp = &buf[sizeof(buf) - 1];
    xfmt_base_t base = xfmtspec_getbase(fmtspec);
    uint8_t radix = xfmt_base_radix(base);
    uint8_t dosign = (sgn < 0 || (fmtspec->flags & XFMT_FLAG_PLUS));
    uint8_t leftspace = 0;
    uint8_t padcnt = 0;
    uint8_t rightspace = 0;
    uint8_t numlen = 0;
    int len = 0;

    *strp = '\0';
    do {
        *--strp = xfmt_base_digitchar(base, (uint8_t)(num % radix));
        num /= radix;
    } while(num);

    numlen = (uint8_t)(&buf[sizeof(buf)-1] - strp) + (dosign ? 1 : 0);

    if (fmtspec->width > numlen) {
        if (fmtspec->flags & XFMT_FLAG_ZEROPAD) {
            padcnt = fmtspec->width - numlen;
            leftspace = 0;
            rightspace = 0;
        } else if (fmtspec->flags & XFMT_FLAG_LEFTADJ) {
            padcnt = 0;
            leftspace = 0;
            rightspace = fmtspec->width - numlen;
        } else {
            padcnt = 0;
            leftspace = fmtspec->width - numlen;
            rightspace = 0;
        }
    }

    while (leftspace-- > 0)
        len += xprintx_printchar(xp, ' ');
    if (dosign)
        len += xprintx_printchar(xp, ((sgn > 0) ? '+' : '-'));
    while (padcnt-- > 0)
        len += xprintx_printchar(xp, '0');
    len += xprintx_printstr(xp, strp);
    while (rightspace-- > 0)
        len += xprintx_printchar(xp, ' ');
    return len;
}

static int
xprintx_printfloat(xstream_t *xp, double num, struct xfmtspec *fmtspec)
{
    uint8_t prec = fmtspec->prec;
    int8_t sgn = (num >= 0.0f) ? 1 : -1;
    unsigned long pow, num_d, num_r;
    uint8_t m;
    int len = 0;

    if (num < 0.0) {
        num = -num;
    }

    num += xprintx_rounding(prec);
    pow = xprintx_pow10ul(prec);
    num_d = (unsigned long) num;
    num_r = ((unsigned long)(num * ((double) pow))) % pow;

    len += xprintx_printnum(xp, num_d, sgn, fmtspec);
    if (prec > 0) {
        len += xprintx_printchar(xp, '.'); 
        while (prec-- > 0) {
            num_r *= 10;
            m = (uint8_t)(num_r / pow);
            len += xprintx_printchar(xp, xfmt_base_digitchar(XFMT_BASE_10, m));
            num_r -= (m * pow);
        }
    }

    return len;
}

struct xprintx_va_list {
    va_list ap;
};

static int
xprintx_printfmtspec(xstream_t *xp, struct xfmtspec *fmtspec, struct xprintx_va_list *xap)
{
    union {
        unsigned int  uival;
        int           ival;
        unsigned long ulval;
        long          lval;
        double        fval;
        char *        strval;
        struct {
            void *at_ptr;
            xprintx_atval_fn_t *at_fn;
        } atval;
    } arg;

    switch (fmtspec->conv) {
    case 'c':
        arg.ival = va_arg(xap->ap, int);
        return xprintx_printchar(xp, (char) arg.ival);
    case 'd':
    case 'i':
        switch (fmtspec->len) {
        case XFMT_LEN_HH:
        case XFMT_LEN_H:
        case XFMT_LEN_NONE:
            arg.lval = (long)(int) va_arg(xap->ap, int);
            break;
        case XFMT_LEN_L:
            arg.lval = va_arg(xap->ap, long);
            break;
        case XFMT_LEN_LL:
            arg.lval = (long)(long long) va_arg(xap->ap, long long);
            break;
        case XFMT_LEN_Z:
            arg.lval = (long)(ssize_t) va_arg(xap->ap, ssize_t);
            break;
        default:
            return -XERR_INVAL;
        }
        return xprintx_printnum(xp, (unsigned long)((arg.lval >= 0) ? arg.lval : -arg.lval), ((arg.lval >= 0) ? 1 : -1), fmtspec);
    case 'o':
        switch (fmtspec->len) {
        case XFMT_LEN_HH:
        case XFMT_LEN_H:
        case XFMT_LEN_NONE:
            arg.ulval = (unsigned long)(unsigned int) va_arg(xap->ap, unsigned int);
            break;
        case XFMT_LEN_L:
            arg.ulval = va_arg(xap->ap, unsigned long);
            break;
        case XFMT_LEN_LL:
            arg.ulval = (unsigned long)(unsigned long long) va_arg(xap->ap, unsigned long long);
            break;
        case XFMT_LEN_Z:
            arg.ulval = (unsigned long)(size_t) va_arg(xap->ap, size_t);
            break;
        default:
            return -XERR_INVAL;
        }
        return xprintx_printnum(xp, arg.ulval, 1, fmtspec);
    case 'u':
        switch (fmtspec->len) {
        case XFMT_LEN_HH:
        case XFMT_LEN_H:
        case XFMT_LEN_NONE:
            arg.ulval = (unsigned long)(unsigned int) va_arg(xap->ap, unsigned int);
            break;
        case XFMT_LEN_L:
            arg.ulval = va_arg(xap->ap, unsigned long);
            break;
        case XFMT_LEN_LL:
            arg.ulval = (unsigned long)(unsigned long long) va_arg(xap->ap, unsigned long long);
            break;
        case XFMT_LEN_Z:
            arg.ulval = (unsigned long)(size_t) va_arg(xap->ap, size_t);
            break;
        default:
            return -XERR_INVAL;
        }
        return xprintx_printnum(xp, arg.ulval, 1, fmtspec);
    case 'x':
        switch (fmtspec->len) {
        case XFMT_LEN_HH:
        case XFMT_LEN_H:
        case XFMT_LEN_NONE:
            arg.ulval = (unsigned long)(unsigned int) va_arg(xap->ap, unsigned int);
            break;
        case XFMT_LEN_L:
            arg.ulval = va_arg(xap->ap, unsigned long);
            break;
        case XFMT_LEN_LL:
            arg.ulval = (unsigned long)(unsigned long long) va_arg(xap->ap, unsigned long long);
            break;
        case XFMT_LEN_Z:
            arg.ulval = (unsigned long)(size_t) va_arg(xap->ap, size_t);
            break;
        default:
            return -XERR_INVAL;
        }
        return xprintx_printnum(xp, arg.ulval, 1, fmtspec);
    case 'f':
        arg.fval = va_arg(xap->ap, double);
        return xprintx_printfloat(xp, arg.fval, fmtspec);
    case 's':
        arg.strval = va_arg(xap->ap, char *);
        return xprintx_printstr(xp, arg.strval);
    case '@':
        arg.atval.at_ptr = va_arg(xap->ap, void *);
        arg.atval.at_fn = (xprintx_atval_fn_t *) va_arg(xap->ap, void *);
        return (*arg.atval.at_fn)(xp, arg.atval.at_ptr);
    case '%':
        return xprintx_printchar(xp, '%');
    default:
        return -XERR_INVAL;
    }
}

int
xprintx_vxprintx(xstream_t *xp, const char *format, va_list ap)
{
    struct xprintx_va_list xap;
    struct xfmtspec fmtspec;
    char *fmtp;
    int len = 0;
    int rv;

    va_copy(xap.ap, ap);

    fmtp = (char *) format;
    while (*fmtp != '\0') {
        if (*fmtp == '%') {
            rv = xprintx_getfmtspec(fmtp, &fmtspec, &fmtp);
            if (rv < 0) {
                goto out;
            }
            rv = xprintx_printfmtspec(xp, &fmtspec, &xap);
            if (rv < 0) {
                goto out;
            }
            len += rv;
        } else {
            rv = xprintx_printchar(xp, *fmtp++);
            if (rv < 0) {
                goto out;
            }
            len += rv;
        }
    }

 out:
    va_end(xap.ap);
    if (rv < 0)
        return rv;
    return len;
}

int
vxprintx(xstream_t *xp, const char *format, va_list ap)
{
    return xprintx_vxprintx(xp, format, ap);
}

int
xprintx(xstream_t *xp, const char *format, ...)
{
    int rv;
    va_list ap;

    va_start(ap, format);
    rv = xprintx_vxprintx(xp, format, ap);
    va_end(ap);

    return rv;
}
