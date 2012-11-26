#! /usr/bin/make

BIN=fly
RELDIR=..
include ../Common/common.mak
#CFLAGS += -O2
ifdef POSTMULT
	CFLAGS += -DPOSTMULT
endif

# build any binary from a matching .o [...]
fly: fly.o model.o Camera.o
# these are in the wrong order I think...?
# if fly and model are edited, you should first compile model, not fly, 
# as model is included in fly.


# Make documentation ...
.PHONY: docs
docs: Camera.hpp Camera.cpp Doxyfile
	doxygen