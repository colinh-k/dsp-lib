CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -Wextra -Wtype-limits

SRCDIR=src
OBJDIR=obj
BINDIR=bin
# final executable.
EXE=$(BINDIR)/ctex

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

$(OBJDIR) $(BINDIR):
	mkdir -p $@