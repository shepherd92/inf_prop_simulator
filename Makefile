# project name
PROJECT := simulator

# set to 1 to     require unittests
# set to 0 to not require unittests
UNITTESTS := 1

# suppress Makefile output
.SILENT :

# compiler and linker settings
CXX      := g++
LD       := g++

# std=c++17
# MP:  add phony target for each dependency other than the main file
# MT:  
# MD:  list user and system-defined include files
# MMD: list user-defined include files
CXXFLAGS := -std=c++17 -MMD -MP -Wall -Werror -Wextra -pthread -pedantic-errors -save-temps=obj -DLOGGING
LDFLAGS  := -lstdc++ -m64 -pthread

# directories
ROOT   := .
BLDDIR := $(ROOT)/build
SRCDIR := $(ROOT)/src
DOCDIR := $(ROOT)/doc
LOGDIR := $(BLDDIR)/log

# name of the include directory to be used recursively to search for include files
INCDIRNAME  := inc
TESTDIRNAME := test
MOCKDIRNAME := mock

# definition of extenstions
SRCEXT := cpp
OBJEXT := o

# configuration file to be used when running the binary
CONFIG := config.ini
# list files without unit tests
SRCSNOTTOTEST := $(SRCDIR)/main/main.$(SRCEXT)

TARGETS := __all release debug run relrun gdb valgrind profile coverage doxygen clean

# export variables set to be used by child make processes
.EXPORT_ALL_VARIABLES :
# remove files generated when there was an error
.DELETE_ON_ERROR :
# list phony targets
.PHONY : $(TARGETS)

# do print when directory changes
MAKEFLAGS += --no-print-directory

# add .d to make recognized suffixes
SUFFIXES += .d

###################################################################
###################################################################
ifndef TGTDIR  # set target directory to debug or release
###################################################################

# set target directory
TGTDIR := $(BLDDIR)/debug
release relrun : TGTDIR := $(BLDDIR)/release

# for each target run make again (now target directory is set)
$(TARGETS) :
	@${MAKE} $@

###################################################################
else
###################################################################

BINDIR := $(TGTDIR)/bin
OBJDIR := $(TGTDIR)
BIN    := $(BINDIR)/$(PROJECT)

# find source files
EXESRCS  := $(shell find $(SRCDIR) -name *.$(SRCEXT) -not -path "*/$(TESTDIRNAME)/*")
TESTSRCS := $(shell find $(SRCDIR) -name *.$(SRCEXT)      -path "*/$(TESTDIRNAME)/*")

# create object names from source filenames
EXEOBJS  := $(EXESRCS:%.$(SRCEXT)=$(OBJDIR)/%.$(OBJEXT))
TESTOBJS := $(TESTSRCS:%.$(SRCEXT)=$(OBJDIR)/%.$(OBJEXT))

# create dependency file names from object filenames
DEPS := $(EXESRCS:%.$(SRCEXT)=$(OBJDIR)/%.d)
DEPS += $(TESTSRCS:%.$(SRCEXT)=$(OBJDIR)/%.d)

# automatically create dependency files for object files
-include $(DEPS)

# construct include directory compiler flags
INCDIRS := $(shell find $(ROOT) -iname $(INCDIRNAME) -type d)
CXXFLAGS += $(addprefix -I , $(INCDIRS))

__all : debug

# set target specific compiler and linker flags
debug   : CXXFLAGS += -g3 -pg -O0 --coverage -DDBG
debug   : LDFLAGS  += -pg -lgcov
release : CXXFLAGS += -O3

release debug : $(BIN) | $(BINDIR)

run : debug | $(LOGDIR)
	@echo "Run:    " $(notdir $(BIN)) ...
	$(BIN) $(CONFIG)
	$(shell [ -f gmon.out ] && mv gmon.out $(LOGDIR))

# below target responsible for running binary in release directory
relrun : release | $(LOGDIR)
	@echo "Run:    " gdb $(notdir $(BIN)) ...
	$(BIN) $(CONFIG)
	$(shell [ -f gmon.out ] && mv gmon.out $(LOGDIR))

gdb : debug
	@echo "Run:    " gdb $(notdir $(BIN)) ...
	gdb $(BIN) $(CONFIG)
	$(shell [ -f gmon.out ] && mv gmon.out $(LOGDIR))

valgrind : VALGRIND_PARAMS := -v \
                              --tool=memcheck \
                              --leak-check=full \
                              --show-reachable=yes \
                              --trace-children=yes \
                              --track-origins=yes \
                              --log-file=$(LOGDIR)/valgrind.log
valgrind : debug | $(LOGDIR)
	@echo Run:     valgrind $(notdir $(BIN)) ...
	valgrind $(VALGRIND_PARAMS) $(BIN) $(CONFIG)

profile : run | $(LOGDIR)
	@echo "Run:    " gprof $(BIN) ...
	@echo Profile: $(notdir $@) ...
	gprof -b $(BIN) $(LOGDIR)/gmon.out > $(LOGDIR)/profiler_analysis.txt

# coverage :

doxygen :
	${MAKE} -C $(DOCDIR)

# require tested objects if UNITTESTS is set to 1
#################################
#################################
ifeq ($(UNITTESTS), 1)
#################################

# split objects with and without tests
OBJSNOTTOTEST := $(SRCSNOTTOTEST:%.$(SRCEXT)=$(OBJDIR)/%.$(OBJEXT))
OBJSTOTEST    := $(filter-out $(OBJSNOTTOTEST),$(EXEOBJS))

# construct file names representing tested objects
TESTEDOBJS := $(OBJSTOTEST:%.$(OBJEXT)=%_tested.$(OBJEXT))

# link binary
$(BIN): $(TESTEDOBJS) $(OBJSNOTTOTEST) | $(BINDIR)
	@echo "Link:   " $(notdir $@) ...
	$(CXX) $^ -o $@ $(LDFLAGS)

# test
# allow variables in prerequisites
.SECONDEXPANSION:
# function to create test object name
create_test_name = $(dir $(1))$(TESTDIRNAME)/test_$(notdir $(1))
$(TESTEDOBJS) : %_tested.$(OBJEXT) : %.$(OBJEXT) $$(call create_test_name, $$*.$(OBJEXT)) | $(LOGDIR)
	@echo "Link:   " $(notdir $(call create_test_name, $(basename $<))) ...
	$(CXX) $^ -o $(call create_test_name, $(basename $<)) $(LDFLAGS)
	$(call create_test_name, $(basename $<))
	@echo "Run:    " $(notdir $(call create_test_name, $(basename $<))) ...
	$(shell [ -f gmon.out ] && mv gmon.out $(LOGDIR)/gmon_$(notdir $(<:.$(OBJEXT)=_test)).out)
	@cp $< $@

# construct mock directory compiler flags
MOCKDIRS     := $(shell find $(ROOT) -iname $(MOCKDIRNAME) -type d)
MOCKDIRFLAGS := $(addprefix -I , $(MOCKDIRS))

# compile test objects
$(TESTOBJS) : $(OBJDIR)/%.$(OBJEXT) : %.$(SRCEXT)
	@echo Compile: $(notdir $@) ...
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(MOCKDIRFLAGS) -c $< -o $@
	@rm -f $(@:.$(OBJEXT)=.gcda) $(@:.$(OBJEXT)=.gcno) # remove coverage files

#################################
else  # no unittests are required
#################################

$(BIN): $(EXEOBJS) | $(BINDIR)
	@echo "Link:   " $(notdir $@) ...
	$(CXX) $^ -o $@ $(LDFLAGS)

#################################
endif  # UNITTESTS
#################################
#################################

# compile
$(EXEOBJS) : $(OBJDIR)/%.$(OBJEXT) : %.$(SRCEXT)
	@echo Compile: $(notdir $@) ...
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@rm -f $(@:.$(OBJEXT)=.gcda) $(@:.$(OBJEXT)=.gcno) # remove coverage files

# make directories
$(OBJDIR) $(BINDIR) $(LOGDIR):
	@mkdir -p $@
    
clean :
	@rm -rf *~ core $(BLDDIR)

###################################################################
endif  # TARGETDIR
###################################################################
###################################################################
