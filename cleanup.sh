#!/bin/bash

TO_REMOVE=(
    "aclocal.m4"
    "autom4te.cache"
    "config.h"
    "config.h.in"
    "config.log"
    "config.status"
    "Makefile"
    "depcomp"
    "install-sh"
    "missing"
    "stamp-h1"
    "configure~"
    "compile"
)

echo "Cleaning up..."

rm -rf "${TO_REMOVE[@]}"

rm -rf *.o *.a *.lo *.la *.so glitch bin
rm -rf src/*.o
