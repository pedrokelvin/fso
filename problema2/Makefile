# Makefile for building demo applications.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
#TARGETS= sum_on_thread sum_on_many_threads sum_malloc
TARGETS= estudantes

CROSS_TOOL =
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -pthread -lm

all: clean $(TARGETS)
	   ./estudantes

$(TARGETS):
	$(CC_C) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(TARGETS)
