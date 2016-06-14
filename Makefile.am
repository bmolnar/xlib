AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libx.la
noinst_PROGRAMS = xstreamtest

libx_ladir = $(includedir)/xlib
libx_la_HEADERS = \
	include/xlib/xerr.h \
	include/xlib/xmembuf.h \
	include/xlib/xiovec.h \
	include/xlib/xfmt.h \
	include/xlib/xalloc.h \
	include/xlib/xstream.h \
	include/xlib/xstream_xmembuf.h \
	include/xlib/xprint.h \
	include/xlib/xprint_wrappers.h \
	include/xlib/xlib.h
libx_la_CFLAGS = -Wall -ggdb
libx_la_CPPFLAGS = -I$(srcdir)/src/xlib -I$(srcdir)/include
libx_la_SOURCES = \
	src/xlib/xerr.c \
	src/xlib/xmembuf.c \
	src/xlib/xfmt.c \
	src/xlib/xalloc.c \
	src/xlib/xstream.c \
	src/xlib/xstream_xmembuf.c \
	src/xlib/xprint.c \
	src/xlib/xprint_wrappers.c
libx_la_LDFLAGS =
libx_la_LIBADD =

if ENABLE_XSTREAM_FILE
libx_la_SOURCES += src/xlib/xstream_file.c
libx_la_HEADERS += include/xlib/xstream_file.h
endif

if ENABLE_XSTREAM_STDIO
libx_la_SOURCES += src/xlib/xstream_stdio.c
libx_la_HEADERS += include/xlib/xstream_stdio.h
endif




TESTS = xstreamtest
xstreamtest_CFLAGS = -Wall -ggdb
xstreamtest_CPPFLAGS = -I$(srcdir)/include
xstreamtest_SOURCES = test/xstreamtest.c
xstreamtest_LDFLAGS = 
xstreamtest_LDADD = libx.la

