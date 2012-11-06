#! /usr/bin/make

BIN=fly
RELDIR=..
include ../Common/common.mak
CFLAGS += -O2

# build any binary from a matching .o [...]
fly: fly.o model.o Camera.o

# Make documentation ...
.PHONY: docs
docs: Camera.hpp Camera.cpp Doxyfile
	doxygen