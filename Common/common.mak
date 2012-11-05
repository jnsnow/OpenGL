#! /usr/bin/make

ifndef RELDIR
	RELDIR="../.."
endif

CC=g++
CFLAGS=-I $(RELDIR)/include -Wall -pedantic
LDFLAGS=-lGL -lGLU -lGLEW -lglut -lXmu -lX11 -lm
INIT_SHADER = $(RELDIR)/Common/InitShader.o

all: $(BIN)

$(INIT_SHADER): $(RELDIR)/Common/InitShader.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# build any .o from its matching .cpp.
%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

# any thing requires thing.o and initShader.o
%: %.o $(INIT_SHADER)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(INIT_SHADER) example*.o $(BIN) *~