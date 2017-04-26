% fell(1) 0.2 | Fell Manual
% Fried Ice
% 26 April 2017

# NAME

**fell** – the **F**urious sh**ELL**

# SYNOPSIS

**fell** [*options*]

# DESCRIPTION

__fell__ is a simple shell created for understanding the linux way of creating new processes by forking and executing. It is **not** supposed to deliver every day usage shell experience and is most likely filled with bugs and security problems.

# OPTIONS

-h, --help
:   Display a friendly help message and exit.

-v, --version
:   Display version information and exit.

# USAGE
**fell** currently supports the following features:

**cd** *path*
:   Switch current working directory (relative or absolute).

**exit**
:    Close the shell.

**programm**
:   Execute a binary program in the $PATH.

**path/to/programm**
:   Execute a binary programm (relative or absolute).

# SEE ALSO
fork (3), exex (3), waitpid (3)

# BUGS
Probably a lot.
