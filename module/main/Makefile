MODULE := $(shell basename $(CURDIR))

M_ROOT   := ../..
M_BINDIR := $(M_ROOT)/$(TARGETDIR)
M_INCDIR := $(INCDIR:%=$(M_ROOT)/%)
M_INTDIR := $(M_ROOT)/$(INTDIR)
M_LOGDIR := $(M_ROOT)/$(LOGDIR)
M_OBJDIR := $(M_ROOT)/$(OBJDIR)
M_SRCDIR := source

DEPENDENCY := $(shell find $(M_INCDIR) $(M_INTDIR) $(M_SRCDIR) -type f)

.DELETE_ON_ERROR :
.EXPORT_ALL_VARIABLES :

$(M_OBJDIR)/$(MODULE).o : $(DEPENDENCY)
	@mkdir -p $(M_OBJDIR)
	$(CC) $(CDEFS) -c -x c++ -I $(M_INTDIR) $(M_INCDIR:%=-I %) -o $@ $(filter %.cpp, $(DEPENDENCY))
