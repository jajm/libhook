CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c99 -g
MAKE := make
LIBTOOL := libtool
INSTALL := install

PROJECT := libhook
PREFIX := /usr/local
LIBDIR := $(PREFIX)/lib
INCLUDEDIR := $(PREFIX)/include/$(PROJECT)
PKGCONFIGDIR := $(LIBDIR)/pkgconfig

LIBTOOL_CURRENT := 0
LIBTOOL_REVISION := 0
LIBTOOL_AGE := 0
