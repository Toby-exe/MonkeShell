# Author(s): Tobias Wondwossen, Jayden Mingle
# Date: 10/19/2021

CC=gcc
CFLAGS=-g
DEPS = $(shell find . -type f -name "*.h")
SRC = $(shell find . -type f -name "*.c")
OBJDIR=obj
MONKE_OBJ = $(patsubst %.c,$(OBJDIR)/%.o, $(filter-out %utils_test.c, $(SRC)))
UTILS_TEST_OBJ = $(patsubst %.c,$(OBJDIR)/%.o, $(filter-out %monkeSh.c %myShell.c, $(SRC)))
MONKE_EXE=monkeSh
UTILS_TEST_EXE=utils_test
TEST_ENV_DIR=tests/testEnv

$(OBJDIR)/%.o: %.c $(DEPS)
	mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(MONKE_EXE): $(MONKE_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(UTILS_TEST_EXE): $(UTILS_TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

testUtils: $(UTILS_TEST_EXE)
	./$(UTILS_TEST_EXE)

monke: $(MONKE_EXE)
	./$(MONKE_EXE)

testEnvMonke: $(MONKE_EXE)
	cp ./$(MONKE_EXE) ./$(TEST_ENV_DIR)/$(MONKE_EXE)

clean:
	rm -f $(OBJ) $(MONKE_EXE) $(UTILS_TEST_EXE)

all: clean $(MONKE_EXE) $(UTILS_TEST_EXE) testEnvMonke
	./$(UTILS_TEST_EXE) && ./$(MONKE_EXE)
