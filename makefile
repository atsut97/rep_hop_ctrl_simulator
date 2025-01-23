ROOTDIR:=.
INCDIR:=$(ROOTDIR)/include
SRCDIR:=$(ROOTDIR)/src
TESTDIR:=$(ROOTDIR)/test
EXAMPLEDIR:=$(ROOTDIR)/example
MEMCHECK?=n

.PHONY: all
all:
	@cd $(TESTDIR); make
	@cd $(EXAMPLEDIR); make

.PHONY: test
test:
	@cd $(TESTDIR); make test MEMCHECK=$(MEMCHECK)

.PHONY: clean
clean:
	-@rm -f $(ROOTDIR)/*~
	@cd $(TESTDIR); make clean
	@cd $(EXAMPLEDIR); make clean
