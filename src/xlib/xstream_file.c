#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <xlib/xtypes.h>
#include <xlib/xstream.h>
#include <xlib/xstream_file.h>

static int
xstream_file_oflags(const char *mode)
{
    if (strcmp(mode, "r") == 0)
        return (O_RDONLY);
    else if (strcmp(mode, "r+") == 0)
        return (O_RDWR);
    else if (strcmp(mode, "w") == 0)
        return (O_WRONLY | O_TRUNC | O_CREAT);
    else if (strcmp(mode, "w+") == 0)
        return (O_RDWR | O_TRUNC | O_CREAT);
    else if (strcmp(mode, "a") == 0)
        return (O_WRONLY | O_APPEND | O_CREAT);
    else if (strcmp(mode, "a+") == 0)
        return (O_RDWR | O_APPEND | O_CREAT);
    else
        return (O_RDONLY);
}

static mode_t
xstream_file_omode(const char *mode)
{
    return (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

static int
xstream_file_open_fn(xstream_t *xs, const char *mode, void *userdata)
{
    const char *path = (const char *) userdata;
    int oflags = xstream_file_oflags(mode);
    mode_t omode = xstream_file_omode(mode);
    int fd;

    fd = open(path, oflags, omode);
    if (fd < 0)
        return -errno;

    XSTREAM_PRIV(xs) = (void *)(size_t) fd;
    return 0;
}

static ssize_t
xstream_file_write_fn(xstream_t *xs, const void *buf, size_t count)
{
    int fd = (int)(size_t) XSTREAM_PRIV(xs);
    return write(fd, buf, count);
}

static ssize_t
xstream_file_read_fn(xstream_t *xs, void *buf, size_t count)
{
    int fd = (int)(size_t) XSTREAM_PRIV(xs);
    return read(fd, buf, count);
}

static int
xstream_file_close_fn(xstream_t *xs)
{
    int fd = (int)(size_t) XSTREAM_PRIV(xs);
    close(fd);
    return 0;
}

static int
xstream_file_flush_fn(xstream_t *xs)
{
    int fd = (int)(size_t) XSTREAM_PRIV(xs);
    int rv;

    rv = fsync(fd);
    return (rv < 0) ? -errno : 0;
}

static xstream_ops_t xstream_file_ops = {
    .open  = &xstream_file_open_fn,
    .write = &xstream_file_write_fn,
    .read  = &xstream_file_read_fn,
    .close = &xstream_file_close_fn,
    .flush = &xstream_file_flush_fn,
};

xstream_t *
xstream_file_open(const char *path, const char *mode)
{
    return xstream_open(xstream_file_ops, mode, (void *) path);
}
