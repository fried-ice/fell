#!/bin/sh

# Use this script to prepare the build environment and generate docs

aclocal
autoconf
autoheader
automake --add-missing

# There has to be smarter solution, but  I haven´t figured out now
# 	First: Search for all ".md" files and execute pandoc on them, creating a man page with the extension .md.tmp
# 	Second: Rename all those files removing the.md.tmp file endings (the last 7 characters)

command -v pandoc >/dev/null 2>&1 || { echo >&2 "Pandoc is required for building man pages. Aborting."; exit 1; }

find ./man -type f -name "*.md" -exec pandoc "{}" -s -t man -o "{}.tmp" \;
find ./man -type f -regextype sed -regex ".*/.*\.[0-9].md.tmp$" | while read f; do mv "$f" "${f%???????}"; done
