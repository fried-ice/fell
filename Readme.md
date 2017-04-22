# FELL - the Furious shELL
[![Build Status](https://travis-ci.org/fried-ice/connect_four.svg?branch=master)](https://travis-ci.org/fried-ice/connect_four)

## Overview
Fell is a simple shell programm.
I use it to aproach Linux API basics and understand the usage of GNU Automake and CMake build systems.

This program is most likely filled with bugs and security problems, so use it at your own risk.

At the moment, you only can start programs by absolute paths - more to come.

## Build instructions
Build using either GNU Autoconf or CMake:

```shell
mkdir build
cd build
cmake ..
make
make install
```

```shell
./prepare.sh
./configure
make
make install
```
## Packaging
As i like to learn more about packaging for several GNU/Linux Distributions, there will be some Package Builds.

As of the time of writing, there is an Arch Linux PackageBuild ready for use.
