CC=gcc
CFLAGS=-g
DEPS = $(shell find . -type f -name "*.h")
SRC = $(shell find . -type f -name "*.c")
OBJDIR=obj
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))
EXECUTABLE=monkeSh

$(OBJDIR)/%.o: %.c $(DEPS)
	mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(EXECUTABLE)
