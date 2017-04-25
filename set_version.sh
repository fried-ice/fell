#!/bin/sh

# Set Version Numbers to all needed files via sed

setVersion() {

# Regex for common version string structure:
#   * one integer ([0-9]* = any coherent string out of decimal numbers)
#   * optionally followed by a (dot and another integer) up to two times
#        (\(\.[0-9]*\)\{0,2\})
#   * a bunch of escapes (\)
readonly VER_REGEX="[0-9]*\(\.[0-9]*\)\{0,2\}"

# Common string representation of version number
# 1.1.1 or 1.1 (if Micro level is 0, do not include it)
if [ $3 == "0" ]; then
    readonly VER_STRING="$1.$2"
else
    readonly VER_STRING="$1.$2.$3"
fi

# src/fell.c
sed -i "s|^#define VERSION_MAJOR \"[0-9]*\"|#define VERSION_MAJOR \"$1\"|" src/fell.c
sed -i "s|^#define VERSION_MINOR \"[0-9]*\"|#define VERSION_MINOR \"$2\"|" src/fell.c
sed -i "s|^#define VERSION_MICRO \"[0-9]*\"|#define VERSION_MICRO \"$3\"|" src/fell.c

# configure.ac
sed -i "s|^AC_INIT(\[fell\], \[$VER_REGEX\],|AC_INIT([fell], [$VER_STRING],|" configure.ac

# man/fell.1
sed -i "s|\"$VER_REGEX\" \"fell man page\"$|\"$VER_STRING\" \"fell man page\"|" man/fell.1

# package/Arch/PKGBUILD
sed -i "s|pkgver=$VER_REGEX|pkgver=$VER_STRING|" package/Arch/PKGBUILD

}

# MAIN

if [ "$#" -ne 3 ]; then
    echo "This script needs 3 arguments. Exiting."
    exit 1
fi

for arg in "$@"
do
    if ! [[ $arg =~ ^[0-9]+$ ]]; then
        echo "Argument \"$arg\" is not a positive integer. Exiting."
        exit 1
    fi
done

setVersion $1 $2 $3
