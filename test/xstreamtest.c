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
#include <xlib/xstream.h>

#include <xlib/xstream_xmembuf.h>
#ifdef ENABLE_XSTREAM_FILE
#include <xlib/xstream_file.h>
#endif /* ENABLE_XSTREAM_FILE */
#ifdef ENABLE_XSTREAM_STDIO
#include <xlib/xstream_stdio.h>
#endif /* ENABLE_XSTREAM_STDIO */


#ifdef HAVE_SYS_STAT_H
int
atval_stat(xstream_t *xp, void *ptr)
{
    struct stat *st = (struct stat *) ptr;

    return xprintx(xp, "{.st_dev=%lu, .st_ino=%zu, .st_mode=0x%x, "
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
test_atval_stat(xstream_t *xp)
{
    const char *path = "/tmp";
    struct stat stbuf;
    int rv;

    rv = stat(path, &stbuf);
    if (rv < 0) {
        fprintf(stderr, "test_atval_stat: stat(path=\"%s\") failed: %s\n", path, xerr_str(errno));
        return -errno;
    }

    xprintx(xp, "stat(\"%s\") -> %@\n", path, &stbuf, atval_stat);
    return 0;
}

#endif /* HAVE_SYS_STAT_H */

struct test_atval_info {
    uint32_t a;
    uint32_t b;
    uint32_t c;
};

int
test_atval_info_cb(xstream_t *xp, void *ptr)
{
    struct test_atval_info *info = (struct test_atval_info *) ptr;
    return xprintx(xp, "{.a=0x%x, .b=0x%x, .c=0x%x}", info->a, info->b, info->c);
}

int
test_atval(xstream_t *xp)
{
    struct test_atval_info info;

    info.a = 0x1000;
    info.b = 0x2000;
    info.c = 0x3000;

    xprintx(xp, "test_atval: info=%@\n", &info, test_atval_info_cb);

    return 0;
}



int
test_conversions(xstream_t *xp)
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

    xprintx(xp, "hhuval=%hhu, hhival=%hhi\n"
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
test_widths(xstream_t *xp)
{
    unsigned long ulval = 1023;
    float fval = 0.5;

    xprintx(xp, "%%016lx: <%016lx>\n", ulval);
    xprintx(xp, "%%16lx: <%16lx>\n", ulval);
    xprintx(xp, "%%+16lx: <%+16lx>\n", ulval);
    xprintx(xp, "%%+016lx: <%+016lx>\n", ulval);
    xprintx(xp, "%%-16lx: <%-16lx>\n", ulval);
    xprintx(xp, "%%+lx: <%+lx>\n", ulval);

    xprintx(xp, "%%f: <%f>\n", fval);
    xprintx(xp, "%%016f: <%016f>\n", fval);
    xprintx(xp, "%%.6f: <%.6f>\n", fval);
    xprintx(xp, "%%+.6f: <%+.6f>\n", fval);
    xprintx(xp, "%%016.6f: <%016.6f>\n", fval);
    xprintx(xp, "%%16.6f: <%16.6f>\n", fval);
    xprintx(xp, "%%+16.6f: <%+16.6f>\n", fval);
    xprintx(xp, "%%+016.6f: <%+016.6f>\n", fval);
    xprintx(xp, "%%+.1f: <%+.1f>\n", fval);

    return 0;
}


int
test_xprintx(xstream_t *xp)
{
    int rv;

    rv = test_conversions(xp);
    if (rv < 0) {
        fprintf(stderr, "test_conversions() failed: %d\n", rv);
        return rv;
    }

    rv = test_atval(xp);
    if (rv < 0) {
        fprintf(stderr, "test_atval() failed: %d\n", rv);
        return rv;
    }

#ifdef HAVE_SYS_STAT_H
    rv = test_atval_stat(xp);
    if (rv < 0) {
        fprintf(stderr, "test_atval_stat() failed: %d\n", rv);
        return rv;
    }
#endif /* HAVE_SYS_STAT_H */

    rv = test_widths(xp);
    if (rv < 0) {
        fprintf(stderr, "test_widths() failed: %d\n", rv);
        return rv;
    }

    return 0;
}


#ifdef ENABLE_XSTREAM_FILE
int
test_file()
{
    xstream_t *xp;
    int rv;

    xp = xstream_file_open("./output.txt", "w");
    if (xp == NULL) {
        fprintf(stderr, "xstream_file_open() failed: %s\n", xerr_str(errno));
        return -errno;
    }

    rv = test_xprintx(xp);
    if (rv < 0) {
        fprintf(stderr, "test_xprintx() failed: %d\n", rv);
    }

    xstream_close(xp);
    return 0;
}
#endif /* ENABLE_XSTREAM_FILE */

#ifdef ENABLE_XSTREAM_STDIO
int
test_stdio()
{
    xstream_t *xp;
    int rv;

    xp = xstream_stdio_open(stdout);
    if (xp == NULL) {
        fprintf(stderr, "xstream_stdio_open() failed: %s\n", xerr_str(errno));
        return -errno;
    }

    rv = test_xprintx(xp);
    if (rv < 0) {
        fprintf(stderr, "test_xprintx() failed: %d\n", rv);
    }

    return 0;
}
#endif /* ENABLE_XSTREAM_FILE */

int
test_xmembuf()
{
    char data[4096];
    struct xmembuf mbuf;
    xstream_t *xs;
    int rv;

    xmembuf_init(&mbuf, data, sizeof(data));

    xs = xstream_xmembuf_open("", &mbuf);
    if (xs == NULL) {
        fprintf(stderr, "xstream_mem_open() failed: %s\n", xerr_str(errno));
        return -errno;
    }

    rv = test_xprintx(xs);
    if (rv < 0) {
        fprintf(stderr, "test_xprintx() failed: %d\n", rv);
    }

    fprintf(stdout, "\ntest_xmembuf: mbuf.position=%zu, mbuf.data=%s\n", xmembuf_position(&mbuf), data);

    rv = fwrite(data, xmembuf_position(&mbuf), 1, stdout);
    if (rv < 0) {
        fprintf(stderr, "fwrite() failed: %s\n", xerr_str(errno));
    }


    return 0;
}

int
main(int argc, char *argv[])
{
    int rv;

#ifdef ENABLE_XSTREAM_FILE
    rv = test_file();
    if (rv < 0) {
        fprintf(stderr, "test_file() failed: %d\n", rv);
    }
#endif /* ENABLE_XSTREAM_FILE */

#ifdef ENABLE_XSTREAM_STDIO
    rv = test_stdio();
    if (rv < 0) {
        fprintf(stderr, "test_stdio() failed: %d\n", rv);
    }
#endif /* ENABLE_XSTREAM_STDIO */

    rv = test_xmembuf();
    if (rv < 0) {
        fprintf(stderr, "test_mem() failed: %d\n", rv);
    }

    return 0;
}
