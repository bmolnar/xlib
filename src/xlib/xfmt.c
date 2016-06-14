#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>

#include <xlib/xerr.h>
#include <xlib/xfmt.h>

char xfmt_base16upper[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
char xfmt_base16lower[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

static int
xfmt_handler_begin(xfmt_handler_t *xp, const char *pos)
{
    xfmt_handler_ops_t *ops = XFMT_HANDLER_OPS(xp);
    if (ops->op_begin == NULL)
    {
        return -XERR_NOSYS;
    }
    return ops->op_begin(xp, pos);
}
static int
xfmt_handler_end(xfmt_handler_t *xp, const char *pos)
{
    xfmt_handler_ops_t *ops = XFMT_HANDLER_OPS(xp);
    if (ops->op_end == NULL)
    {
        return -XERR_NOSYS;
    }
    return ops->op_end(xp, pos);
}
static int
xfmt_handler_onval(xfmt_handler_t *xp, const char *pos, const char *end, const struct xfmt_spec *fmtspec, const xfmt_val_t *val)
{
    xfmt_handler_ops_t *ops = XFMT_HANDLER_OPS(xp);
    if (ops->op_onval == NULL)
    {
        return -XERR_NOSYS;
    }
    return ops->op_onval(xp, pos, end, fmtspec, val);
}
static int
xfmt_handler_onchars(xfmt_handler_t *xp, const char *pos, const char *end)
{
    xfmt_handler_ops_t *ops = XFMT_HANDLER_OPS(xp);
    if (ops->op_onchars == NULL)
    {
        return -XERR_NOSYS;
    }
    return ops->op_onchars(xp, pos, end);
}
static int
xfmt_handler_onconv(xfmt_handler_t *xp, const char *pos, const char *end, const struct xfmt_spec *fmtspec, struct xfmt_va_list *xap)
{
    xfmt_val_t arg;

    switch (fmtspec->conv)
    {
    case 'c':
        arg.v_c = (xfmt_carg_t) va_arg(xap->ap, xfmt_carg_t);
        break;
    case 'd':
    case 'i':
        switch (fmtspec->len)
        {
        case XFMT_LEN_HH:
        case XFMT_LEN_H:
        case XFMT_LEN_NONE:
            arg.v_inum = (xfmt_inum_t) va_arg(xap->ap, xfmt_iarg_t);
            break;
        case XFMT_LEN_L:
            arg.v_inum = (xfmt_inum_t) va_arg(xap->ap, xfmt_liarg_t);
            break;
        case XFMT_LEN_LL:
            arg.v_inum = (xfmt_inum_t) va_arg(xap->ap, xfmt_lliarg_t);
            break;
        case XFMT_LEN_Z:
            arg.v_inum = (xfmt_inum_t) va_arg(xap->ap, xfmt_ziarg_t);
            break;
        default:
            return -XERR_INVAL;
        }
        break;
    case 'o':
    case 'u':
    case 'x':
        switch (fmtspec->len)
        {
        case XFMT_LEN_HH:
        case XFMT_LEN_H:
        case XFMT_LEN_NONE:
            arg.v_unum = (xfmt_unum_t) va_arg(xap->ap, xfmt_uarg_t);
            break;
        case XFMT_LEN_L:
            arg.v_unum = (xfmt_unum_t) va_arg(xap->ap, xfmt_luarg_t);
            break;
        case XFMT_LEN_LL:
            arg.v_unum = (xfmt_unum_t) va_arg(xap->ap, xfmt_lluarg_t);
            break;
        case XFMT_LEN_Z:
            arg.v_unum = (xfmt_unum_t) va_arg(xap->ap, xfmt_zuarg_t);
            break;
        default:
            return -XERR_INVAL;
        }
        break;
    case 'f':
        arg.v_f = (xfmt_farg_t) va_arg(xap->ap, xfmt_farg_t);
        break;
    case 's':
        arg.v_s = (xfmt_sarg_t) va_arg(xap->ap, xfmt_sarg_t);
        break;
    case '@':
        arg.v_at.fn = (const xfmt_atarg_fn_t *) va_arg(xap->ap, void *);
        arg.v_at.ptr = (void *) va_arg(xap->ap, void *);
        break;
    case '%':
        return xfmt_handler_onchars(xp, end-1, end);
    default:
        return -XERR_INVAL;
    }
    return xfmt_handler_onval(xp, pos, end, fmtspec, &arg);
}

static int
xfmt_getfmtspec(const char *fmtptr, struct xfmt_spec *fmtspecp, char **nextpp)
{
    const char *posp = fmtptr;
    struct xfmt_spec fmtspec;
    unsigned long ulval;
    int rv = 0;

    if (!XFMT_CHAR_ISCONVSTART(*posp))
    {
        rv = -XERR_INVAL;
        goto out;
    }
    posp++;

    fmtspec.flags = 0;
    for (; XFMT_CHAR_ISFLAG(*posp); posp++)
    {
        switch (*posp)
        {
        case '#':
            fmtspec.flags |= XFMT_FLAG_ALTFORM;
            break;
        case '0':
            fmtspec.flags |= XFMT_FLAG_ZEROPAD;
            break;
        case '-':
            fmtspec.flags |= XFMT_FLAG_LEFTADJ;
            break;
        case ' ':
            fmtspec.flags |= XFMT_FLAG_SPACE;
            break;
        case '+':
            fmtspec.flags |= XFMT_FLAG_PLUS;
            break;
        default:
            break;
        }
    }

    fmtspec.width = 0;
    if (XFMT_CHAR_ISWIDTHSTART(*posp))
    {
        ulval = strtoul(posp, (char **) &posp, 10);
        if (ulval == ULONG_MAX && errno == ERANGE)
        {
            rv = -XERR_INVAL;
            goto out;
        }
        fmtspec.width = (uint8_t) ulval;
    }

    fmtspec.prec = 0;
    if (XFMT_CHAR_ISPRECSTART(*posp))
    {
        posp++;
        ulval = strtoul(posp, (char **) &posp, 10);
        if (ulval == ULONG_MAX && errno == ERANGE)
        {
            rv = -XERR_INVAL;
            goto out;
        }
        fmtspec.prec = (uint8_t) ulval;
    }

    fmtspec.len = XFMT_LEN_NONE;
    if (XFMT_CHAR_ISLENGTHSTART(*posp))
    {
        switch (*(posp++))
        {
        case 'h':
            if (*posp == 'h')
            {
                fmtspec.len = XFMT_LEN_HH;
                posp++;
            }
            else
            {
                fmtspec.len = XFMT_LEN_H;
            }
            break;
        case 'l':
            if (*posp == 'l')
            {
                fmtspec.len = XFMT_LEN_LL;
                posp++;
            }
            else
            {
                fmtspec.len = XFMT_LEN_L;
            }
            break;
        case 'z':
            fmtspec.len = XFMT_LEN_Z;
            break;
        default:
            rv = -XERR_INVAL;
            goto out;
        }
    }

    fmtspec.conv = 0;
    if (!XFMT_CHAR_ISCONVSPEC(*posp))
    {
        rv = -XERR_INVAL;
        goto out;
    }
    fmtspec.conv = *(posp++);

    rv = (int)(posp - fmtptr);
 out:
    if (rv >= 0)
    {
        *fmtspecp = fmtspec;
    }
    if (nextpp != NULL)
    {
        *nextpp = (char *) posp;
    }
    return rv;
}

int
xfmt_parsev(xfmt_handler_t *xp, const char *format, va_list ap)
{
    struct xfmt_va_list xap;
    struct xfmt_spec fmtspec;
    char *fmtp = (char *) format;
    char *nextp;
    int rv = 0;

    va_copy(xap.ap, ap);

    xfmt_handler_begin(xp, fmtp);
    while (!XFMT_CHAR_ISNULL(*fmtp))
    {
        if (XFMT_CHAR_ISCONVSTART(*fmtp))
        {
            rv = xfmt_getfmtspec(fmtp, &fmtspec, &nextp);
            if (rv < 0)
            {
                goto out;
            }
            rv = xfmt_handler_onconv(xp, fmtp, nextp, &fmtspec, &xap);
            if (rv < 0)
            {
                goto out;
            }
            fmtp = nextp;
        }
        else
        {
            nextp = fmtp;
            while (!XFMT_CHAR_ISNULL(*nextp) && !XFMT_CHAR_ISCONVSTART(*nextp))
            {
                nextp++;
            }
            if ((nextp - fmtp) > 0)
            {
                rv = xfmt_handler_onchars(xp, fmtp, nextp);
                if (rv < 0)
                {
                    goto out;
                }
            }
            fmtp = nextp;
        }
    }

    rv = (int)(fmtp - format);
 out:
    xfmt_handler_end(xp, fmtp);
    va_end(xap.ap);
    return rv;
}
int
xfmt_parse(xfmt_handler_t *xp, const char *format, ...)
{
    int rv;
    va_list ap;

    va_start(ap, format);
    rv = xfmt_parsev(xp, format, ap);
    va_end(ap);

    return rv;
}
