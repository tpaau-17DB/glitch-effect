#!/bin/bash

TO_REMOVE=(
    "aclocal.m4"
    "autom4te.cache"
    "stamp-h1"
    "configure~"
    "src/.deps/"
    "src/.dirstamp"
)

echo "Cleaning up..."

rm -rf "${TO_REMOVE[@]}"

rm -rf *.o *.a *.lo *.la *.so glitch bin
rm -rf src/*.o
