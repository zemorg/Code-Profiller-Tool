COMPILER = gcc
CFLAGS = -g -Wall -I /home/zemor/papi-7.0.1/src
CFLAGS += -pthread

all:
	$(COMPILER) $(CFLAGS) run.c /home/zemor/papi-7.0.1/src/libpapi.a -o run -lm 
