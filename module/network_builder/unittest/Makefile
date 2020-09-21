U_ROOT   := ../../..
U_BINDIR := $(U_ROOT)/$(BINDIR)
U_INCDIR := $(INCDIR:%=$(U_ROOT)/%)
U_INTDIR := $(U_ROOT)/$(INTDIR)
U_LOGDIR := $(U_ROOT)/$(LOGDIR)
U_OBJDIR := $(U_ROOT)/$(OBJDIR)
U_SRCDIR := ../source

BIN := $(U_BINDIR)/ut_$(MODULE)

DEPENDENCY := $(shell find . -maxdepth 1 -type f)
OBJS := $(MODULE).o ut_$(MODULE).o logger.o network.o node.o constant_degree_distribution.o poisson_degree_distribution.o power_law_degree_distribution.o uniform_degree_distribution.o

.DELETE_ON_ERROR :
.PHONY : run

run : $(BIN)
	@mkdir -p $(U_LOGDIR)
	$(BIN) > $(U_LOGDIR)/ut_$(MODULE).log 2>&1

$(BIN) : $(OBJS:%=$(U_OBJDIR)/%)
	@mkdir -p $(U_BINDIR)
	$(CC) $^ -o $@ $(LDEFS)

$(U_OBJDIR)/ut_$(MODULE).o : $(DEPENDENCY)
	@mkdir -p $(U_OBJDIR)
	$(CC) $(CDEFS) -c -x c++ -I $(U_INTDIR) $(U_INCDIR:%=-I %) -o $@ $(filter %.cpp, $^)

$(U_OBJDIR)/%.o :
	${MAKE} -C ../../$* $(M_OBJDIR)/$*.o
