ROOTDIR=..
INCDIR=$(ROOTDIR)/include
SRCDIR=$(ROOTDIR)/src
INCLUDE=-I$(INCDIR)
LINK=-lm -lrt

CC=gcc
CFLAGS=-Wall -O3 $(INCLUDE) -funroll-loops -MMD -MP
MEMCHECK=n

OBJS_SRC=$(wildcard $(SRCDIR)/*.c)
TEST_SRC=$(wildcard *_test.c)
ALL_SRC =$(OBJS_SRC) $(TEST_SRC)

OBJS=$(OBJS_SRC:.c=.o)
TARGET=$(basename $(TEST_SRC))

DEPS=$(patsubst %.c, %.d, $(ALL_SRC))

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug:
	@echo "OBJS   =" $(OBJS)
	@echo "TARGET =" $(TARGET)
	@echo "DEPS   =" $(DEPS)

.PHONY: test
test: all
ifeq ($(MEMCHECK),y)
# $ make test MEMCHECK=y
	@echo "Running tests in memory check mode..."
	@./run_test.sh --memcheck
else
	@./run_test.sh
endif

%_test: %_test.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(OBJS) $(LINK)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $*.o

.PHONY: clean
clean:
	rm -f *.o *~ $(OBJS) $(DEPS) $(TARGET)

.PRECIOUS: $(OBJS)

-include $(DEPS)
