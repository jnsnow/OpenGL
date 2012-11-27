#! /usr/bin/make -f

# John Huston
# Adapted from My Generic Makefile,
# Written September 20, 2011
# Modified November 27, 2012

ifndef RELDIR
	RELDIR=..
endif

COMMON_DIR = $(RELDIR)/Common
CLASSES_DIR = $(RELDIR)/Classes

UNAME := $(shell uname)
EXT=cpp
CC=g++
CFLAGS=-Wall -pedantic -ggdb -I $(RELDIR)/include
EXECS=fly
SOURCES=fly.cpp model.cpp $(CLASSES_DIR)/Camera.cpp $(CLASSES_DIR)/vec.cpp $(CLASSES_DIR)/mat.cpp $(COMMON_DIR)/InitShader.cpp
OBJS=$(subst .$(EXT),.o,$(SOURCES))
DEPS=$(subst .$(EXT),.d,$(SOURCES))
ifeq ($(UNAME), Linux)
LDFLAGS= -lGL -lGLU -lGLEW -lglut -lXmu -lX11 -lm
endif
ifeq ($(UNAME), Darwin)
LDFLAGS= -framework Carbon -framework OpenGL -framework GLUT
endif
ifdef POSTMULT
	CFLAGS += -DPOSTMULT
endif

# This .PHONY is required to prevent Makefile's built-in rules from processing it.
.PHONY: all
all: $(EXECS)

# Include all of the dependencies.
-include $(DEPS)

# Are the dependency files up to date? Are they missing? (re)Generate them.
%.d: %.cpp
	@echo "Building Dependency for $<"
	@$(CC) -M $(CFLAGS) $< > $@.$$$$;			\
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;	\
	rm -f $@.$$$$

# Any *.o can be built from its matching sourcefile.
%.o:
	$(CC) $(CFLAGS) -c -o $@ $*.$(EXT)

$(EXECS): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *~ $(OBJS) $(EXECS)

realclean: clean
	rm -f *~
	find $(RELDIR) -name '*.d' -exec rm '{}' \;

# Make documentation ...
.PHONY: docs
docs: $(SOURCES) $(RELDIR)/Doxygen/Doxyfile
	doxygen