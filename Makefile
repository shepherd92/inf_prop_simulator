PROJECT_NAME := simulator
CC    := g++
CDEFS := -std=c++17 -Wall -Werror -Wextra -pthread -pedantic-errors -DLOGGING
LDEFS := -m64 -pthread

ROOT      := .
BUILDDIR  := $(ROOT)/build

.PHONY : all release debug run relrun gdb valgrind profile coverage doxygen clean
.EXPORT_ALL_VARIABLES :

MAKEFLAGS += --no-print-directory

###################################################################
ifndef TARGETDIR
###################################################################

TARGETDIR := NULL

all : debug

debug run gdb valgrind coverage profile : TARGETDIR := $(BUILDDIR)/debug
release relrun                          : TARGETDIR := $(BUILDDIR)/release

debug run relrun gdb valgrind coverage profile release : .EXPORT_ALL_VARIABLES

debug release clean :
	@${MAKE} $@

run gdb valgrind coverage : debug
	@${MAKE} $@

relrun : release
	@${MAKE} run

profile : run
	@${MAKE} $@

###################################################################
else
###################################################################

INTDIR := $(ROOT)/interface
MODDIR := $(ROOT)/module
INCDIR := $(shell find $(MODDIR) -name 'include' -type d)
DOCDIR := $(ROOT)/dox

MODULES := $(shell find $(MODDIR) -mindepth 1 -maxdepth 1 -type d -printf "%f\n")

BINDIR := $(TARGETDIR)/bin
OBJDIR := $(TARGETDIR)/obj
LOGDIR := $(BUILDDIR)/log
BIN    := $(BINDIR)/$(PROJECT_NAME)

CONFIG := config.ini

VALGRIND_PARAMS := -v \
                   --tool=memcheck \
                   --leak-check=full \
                   --show-reachable=yes \
                   --trace-children=yes \
                   --track-origins=yes \
                   --log-file=$(LOGDIR)/valgrind.log

debug   : CDEFS += -g -pg -O0 --coverage -DDBG
debug   : LDEFS += -pg -lgcov
release : CDEFS += -O3

release debug : $(BIN) | $(TARGETDIR)

$(MODULES) :
	${MAKE} -C $(MODDIR)/$@

$(BIN) : $(MODULES)
	@mkdir -p $(TARGETDIR)
	$(CC) $(addprefix $(OBJDIR)/,$(addsuffix .o,$^)) -o $@ $(LDEFS)

run :
	@mkdir -p $(LOGDIR)
	$(BIN) $(CONFIG)
	$(shell [ -f gmon.out ] && mv gmon.out $(LOGDIR))

gdb :
	gdb $(BIN) $(CONFIG)

valgrind :
	@mkdir -p $(LOGDIR)
	valgrind $(VALGRIND_PARAMS) $(BIN) $(CONFIG)

profile :
	@mkdir -p $(LOGDIR)
	gprof -b $(BIN) $(LOGDIR)/gmon.out > $(LOGDIR)/profiler_analysis.txt

coverage :

doxygen :
	${MAKE} -C $(DOCDIR)

clean :
	@rm -rf *~ core $(BUILDDIR)

###################################################################
endif # TARGETDIR
###################################################################
