#!/bin/sh

# Use this script to prepare the build environment

aclocal
autoconf
autoheader
automake --add-missing


