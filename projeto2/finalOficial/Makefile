# Makefile for building demo applications.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
#TARGETS= sum_on_thread sum_on_many_threads sum_malloc
TARGETS= trabalhoFinal

CROSS_TOOL =
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc


all: clean $(TARGETS)
		 clear
	   ./trabalhoFinal addresses.txt

$(TARGETS):
	$(CC_C) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(TARGETS)
