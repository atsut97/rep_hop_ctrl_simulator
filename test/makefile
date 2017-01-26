ROOTDIR=..
INCDIR=$(ROOTDIR)/include
SRCDIR=$(ROOTDIR)/src
INCLUDE=-I$(INCDIR)
LINK=-lm -lrt

CC=gcc
CFLAGS=-Wall -O3 $(INCLUDE) -funroll-loops

OBJ=

TARGET=rhc_test_test

all: $(TARGET)

rhc_test_test: rhc_test_test.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LINK)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *~ $(TARGET)
