#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include <xlib/xtypes.h>
#include <xlib/xmembuf.h>
#include <xlib/xalloc.h>
#include <xlib/xstream.h>

#include <xlib/xstream_xmembuf.h>
#ifdef ENABLE_XSTREAM_FILE
#include <xlib/xstream_file.h>
#endif /* ENABLE_XSTREAM_FILE */
#ifdef ENABLE_XSTREAM_STDIO
#include <xlib/xstream_stdio.h>
#endif /* ENABLE_XSTREAM_STDIO */

#include <xlib/xprint.h>
#include <xlib/xprint_wrappers.h>




int
xprint_xmembuf(xprint_t *xp, const struct xmembuf *mb)
{
  return xprintf(xp, "{.mb_buf=%zx, .mb_capacity=%zu, .mb_limit=%zu, .mb_position=%zu, .mb_mark=%zu }",
                 mb->mb_buf, mb->mb_capacity, mb->mb_limit, mb->mb_position, mb->mb_mark);
}




#ifdef HAVE_SYS_STAT_H
int
test_xprint_stat_atarg_fn(xprint_t *xp, const void *ptr)
{
    struct stat *st = (struct stat *) ptr;

    return xprintf(xp, "{.st_dev=%lu, .st_ino=%zu, .st_mode=0x%x, "
                   ".st_nlink=%u, .st_uid=%u, .st_gid=%u, "
                   ".st_rdev=%lu, .st_size=%zd, .st_blksize=%zu, "
                   ".st_blocks=%zu, .st_atime=%lu, .st_mtime=%lu, "
                   ".st_ctime=%lu}",
                   (unsigned long) st->st_dev, (size_t) st->st_ino, (unsigned int) st->st_mode,
                   (unsigned int) st->st_nlink, (unsigned int) st->st_uid, (unsigned int) st->st_gid,
                   (unsigned long) st->st_rdev, (ssize_t) st->st_size, (size_t) st->st_blksize,
                   (size_t) st->st_blocks, (unsigned long) st->st_atime, (unsigned long) st->st_mtime,
                   (unsigned long) st->st_ctime);
}
int
test_xprint_stat(xprint_t *xp)
{
    const char *path = "/tmp";
    struct stat stbuf;
    int rv;

    rv = stat(path, &stbuf);
    if (rv < 0)
    {
        fprintf(stderr, "test_atval_stat: stat(path=\"%s\") failed: %s\n", path, xerr_str(errno));
        return -errno;
    }
    xprintf(xp, "stat(\"%s\") -> %@\n", path, test_xprint_stat_atarg_fn, &stbuf);
    return 0;
}

#endif /* HAVE_SYS_STAT_H */

struct test_atval_info
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
};
static int
_test_xprint_atval_atfn(xprint_t *xp, const void *ptr)
{
    struct test_atval_info *info = (struct test_atval_info *) ptr;
    return xprintf(xp, "{.a=0x%x, .b=0x%x, .c=0x%x}", info->a, info->b, info->c);
}
int
test_xprint_atval(xprint_t *xp)
{
    struct test_atval_info info;

    info.a = 0x1000;
    info.b = 0x2000;
    info.c = 0x3000;

    xprintf(xp, "test_atval: info=%@\n", _test_xprint_atval_atfn, &info);

    return 0;
}



int
test_xprint_conversions(xprint_t *xp)
{
    unsigned char hhuval = ((unsigned char) -1);
    char          hhival = -1;
    unsigned short huval = ((unsigned short) -1);
    short          hival = -1;
    unsigned int    uval = ((unsigned int) -1);
    int             ival = -1;
    unsigned long  luval = ((unsigned long) -1);
    long           lival = -1;
    size_t         zuval = ((size_t) -1);
    ssize_t        zival = -1;
    char            cval = 'a';
    float           fval = 0.5f;
    double         lfval = 0.5;

    xprintf(xp, "hhuval=%hhu, hhival=%hhi\n"
            "huval=%hu, hival=%hi\n"
            "uval=%u, ival=%i\n"
            "luval=%lu, lival=%li\n"
            "zuval=%zu, zival=%zi\n"
            "cval='%c'\n"
            "fval=%f, lfval=%f\n",
            hhuval, hhival,
            huval, hival,
            uval, ival,
            luval, lival,
            zuval, zival,
            cval,
            fval, lfval);

    return 0;
}

int
test_xprint_widths(xprint_t *xp)
{
    unsigned long ulval = 1023;
    float fval = 0.5;

    xprintf(xp, "%%016lx: <%016lx>\n", ulval);
    xprintf(xp, "%%16lx: <%16lx>\n", ulval);
    xprintf(xp, "%%+16lx: <%+16lx>\n", ulval);
    xprintf(xp, "%%+016lx: <%+016lx>\n", ulval);
    xprintf(xp, "%%-16lx: <%-16lx>\n", ulval);
    xprintf(xp, "%%+lx: <%+lx>\n", ulval);

    xprintf(xp, "%%f: <%f>\n", fval);
    xprintf(xp, "%%016f: <%016f>\n", fval);
    xprintf(xp, "%%.6f: <%.6f>\n", fval);
    xprintf(xp, "%%+.6f: <%+.6f>\n", fval);
    xprintf(xp, "%%016.6f: <%016.6f>\n", fval);
    xprintf(xp, "%%16.6f: <%16.6f>\n", fval);
    xprintf(xp, "%%+16.6f: <%+16.6f>\n", fval);
    xprintf(xp, "%%+016.6f: <%+016.6f>\n", fval);
    xprintf(xp, "%%+.1f: <%+.1f>\n", fval);

    return 0;
}
int
test_xprint_all(xprint_t *xp)
{
    int rv;

    rv = test_xprint_conversions(xp);
    if (rv < 0)
    {
        fprintf(stderr, "test_conversions() failed: %d\n", rv);
        return rv;
    }

    rv = test_xprint_widths(xp);
    if (rv < 0)
    {
        fprintf(stderr, "test_widths() failed: %d\n", rv);
        return rv;
    }

    rv = test_xprint_atval(xp);
    if (rv < 0)
    {
        fprintf(stderr, "test_xprintl() failed: %d\n", rv);
        return rv;
    }

#ifdef HAVE_SYS_STAT_H
    rv = test_xprint_stat(xp);
    if (rv < 0)
    {
        fprintf(stderr, "test_xprint_stat() failed: %d\n", rv);
        return rv;
    }
#endif /* HAVE_SYS_STAT_H */

    return 0;
}





int
test_xprint()
{
    xprint_t xp = XPRINT_INIT_STDIO(stdout);

    int rv = test_xprint_all(&xp);
    if (rv < 0)
    {
        fprintf(stderr, "test_xprint_all() failed: %d\n", rv);
        return rv;
    }
    return 0;
}








#ifdef ENABLE_XSTREAM_FILE
int
test_file()
{
    xstream_t *xs;
    int rv;

    xs = xstream_file_open("./output.txt", "w");
    if (xs == NULL)
    {
        fprintf(stderr, "xstream_file_open() failed: %s\n", xerr_str(errno));
        return -errno;
    }

    xprint_t xp = XPRINT_INIT_STREAM(xs);
    rv = test_xprint_all(xp);
    if (rv < 0)
    {
        fprintf(stderr, "test_xprint_all() failed: %d\n", rv);
    }

    xstream_close(xs);
    return 0;
}
#endif /* ENABLE_XSTREAM_FILE */

#ifdef ENABLE_XSTREAM_STDIO
int
test_stdio()
{
    xstream_t *xs;
    int rv;

    xs = xstream_stdio_open(stdout);
    if (xp == NULL)
    {
        fprintf(stderr, "xstream_stdio_open() failed: %s\n", xerr_str(errno));
        return -errno;
    }

    xprint_t xp = XPRINT_INIT_STREAM(xs);
    rv = test_xprint_all(xp);
    if (rv < 0)
    {
        fprintf(stderr, "_test_xsprintf() failed: %d\n", rv);
    }

    return 0;
}
#endif /* ENABLE_XSTREAM_FILE */


/*
 * test_xmembuf
 */
int
test_xmembuf()
{
    char data[4096];
    struct xmembuf mbuf;
    xstream_t *xs;
    int rv;

    xprint_t xpo = XPRINT_INIT_STDIO(stdout);


    xmembuf_init(&mbuf, data, sizeof(data));

    xprint_xmembuf(&xpo, &mbuf);
    xprintf(&xpo, "\n");

    xs = xstream_xmembuf_open("", &mbuf);
    if (xs == NULL)
    {
        fprintf(stderr, "xstream_mem_open() failed: %s\n", xerr_str(errno));
        return -errno;
    }

    xprint_t xp = XPRINT_INIT_XSTREAM(xs);
    rv = test_xprint_all(&xp);
    if (rv < 0)
    {
        fprintf(stderr, "_test_xsprintf() failed: %d\n", rv);
    }

    xprint_xmembuf(&xpo, &mbuf);
    xprintf(&xpo, "\n");

    fprintf(stdout, "\ntest_xmembuf: mbuf.position=%zu, mbuf.data=%s\n", xmembuf_position(&mbuf), data);

    rv = fwrite(data, xmembuf_position(&mbuf), 1, stdout);
    if (rv < 0)
    {
        fprintf(stderr, "fwrite() failed: %s\n", xerr_str(errno));
    }

    return 0;
}




/*
 * test_xalloc
 */
static void *
test_xalloc_op_alloc(xalloc_t *xa, size_t size)
{
    fprintf(stdout, "test_xalloc_op_alloc: size=%zu\n", size);
    return malloc(size);
}
static void
test_xalloc_op_free(xalloc_t *xa, void *ptr)
{
    fprintf(stdout, "test_xalloc_op_free: ptr=%zx\n", (size_t) ptr);
    free(ptr);
}
static xalloc_ops_t test_xalloc_ops = {
    .op_alloc = test_xalloc_op_alloc,
    .op_free = test_xalloc_op_free
};
static int
test_xalloc()
{
    xalloc_t xa = XALLOC_INIT(test_xalloc_ops, NULL);

    int *intp = (int *) xalloc_alloc(&xa, (16 * sizeof(int)));
    if (intp == NULL)
    {
        fprintf(stderr, "xalloc_alloc() failed\n");
    }

    xalloc_free(&xa, intp);
    return 0;
}










int
main(int argc, char *argv[])
{
    int rv;

#ifdef ENABLE_XSTREAM_FILE
    rv = test_file();
    if (rv < 0)
    {
        fprintf(stderr, "test_file() failed: %d\n", rv);
    }
#endif /* ENABLE_XSTREAM_FILE */

#ifdef ENABLE_XSTREAM_STDIO
    rv = test_stdio();
    if (rv < 0)
    {
        fprintf(stderr, "test_stdio() failed: %d\n", rv);
    }
#endif /* ENABLE_XSTREAM_STDIO */

    rv = test_xmembuf();
    if (rv < 0)
    {
        fprintf(stderr, "test_mem() failed: %d\n", rv);
    }

    rv = test_xalloc();
    if (rv < 0)
    {
        fprintf(stderr, "test_xalloc() failed: %d\n", rv);
    }

    rv = test_xprint();
    if (rv < 0)
    {
        fprintf(stderr, "test_xsprintf() failed: %d\n", rv);
    }



    return 0;
}
