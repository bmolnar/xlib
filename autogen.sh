#!/bin/sh

autogen ()
{
    autoreconf -ivf
}

clean ()
{
    rm -rf aclocal.m4 autom4te.cache config-aux configure Makefile.in
}

case $1 in
    clean)
	clean;;
    *)
	autogen;;
esac
