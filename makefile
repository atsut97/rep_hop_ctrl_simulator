ROOTDIR:=.
INCDIR:=$(ROOTDIR)/include
SRCDIR:=$(ROOTDIR)/src
TESTDIR:=$(ROOTDIR)/test
EXAMPLEDIR:=$(ROOTDIR)/example

all:
	@cd $(TESTDIR); make
	@cd $(EXAMPLEDIR); make

clean:
	-@rm -f $(ROOTDIR)/*~
	@cd $(TESTDIR); make clean
	@cd $(EXAMPLEDIR); make clean
