CC=gcc
CFLAGS=-Wall -std=c18
# -pedantic -Wextra -Wtype-limits

SRCDIR=src
OBJDIR=obj
BINDIR=bin
# final executable.
EXE=$(BINDIR)/main

# lists of .c, .h source and .o object files
SRCS=$(wildcard $(SRCDIR)/*.c)
HEADERS=$(wildcard $(SRCDIR)/*.h)
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# complie regular version with debugging symbols
all: CFLAGS += -g
all: $(EXE)

# compile release version with optimizations
release: CFLAGS += -o2 -DNDEBUG
release: clean
release: $(EXE)

$(EXE): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

clean: RM+=-r
clean:
	$(RM) $(BINDIR) $(OBJDIR)

run:
	$(EXE) $(file)

# plot a signal stored in the given signal files. wrap
#  the list of files in double quotes: files="<file1> <file2> ..."
plot:
	python3 plot.py $(files)

# run executable with lldb or gdb, whichever exists
debug:
	if command -v lldb &> /dev/null; then \
		lldb $(EXE); \
	elif [ command -v gdb &> /dev/null ]; then \
		gdb $(EXE); \
	else \
		echo "No debugger found"; \
	fi

$(OBJDIR) $(BINDIR):
	mkdir -p $@