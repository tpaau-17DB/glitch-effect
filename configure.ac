# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])  # Ensure minimum version of autoconf
AC_INIT([glitch-effect], [1.0], [you@example.com])  # Project name, version, and email
AM_INIT_AUTOMAKE([foreign])  # Initialize Automake with the 'foreign' option
AC_CONFIG_SRCDIR([src/printer.cpp])  # Ensure 'src/printer.cpp' exists and is part of the project
AC_CONFIG_HEADERS([config.h])  # Output header file

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
AC_CHECK_LIB([ncurses], [main])  # Check if the ncurses library is available

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([dup2 setlocale])

# Set up the Makefile to be generated.
AC_CONFIG_FILES([Makefile])

# Generate the output files.
AC_OUTPUT
