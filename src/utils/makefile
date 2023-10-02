# ========== EXECUTABLE NAMES ==========
MAIN = run
STRTEST = str_tst
IOTEST = io_tst

# ========== MODULES AND FLAGS ==========
STRMODULES = driver.o str.o
IOMODULES = driver.o io.o

CFLAGS = -nostdlib
GCOVFLAGS = -fprofile-arcs -ftest-coverage
GTFLAGS = -lgtest -lgtest_main

# ========== CREATE AND RUN EXECUTABLES ==========
$(MAIN): clean driver.o
	gcc driver.o -o $(MAIN)
	./$(MAIN)

$(STRTEST): clean $(STRMODULES)
	gcc $(STRMODULES) -o $(STRTEST)
	./$(STRTEST)

$(IOTEST): clean $(IOMODULES)
	gcc $(CFLAGS) $(IOMODULES) -o $(IOTEST)
	./$(IOTEST)

# ========== CLEAN DIRECTORY AND MAKE ALL COMMAND ==========
clean:
	/bin/rm -f *~ *.o $(STRTEST) $(IOTEST)

all: clean $(STRTEST) $(IOTEST)

# ========== OBJECT FILE CREATION ==========
# C
driver.o: driver.c
	gcc -g -c driver.c

str.o: str.c str.h types.h
	gcc -g -c str.c

io.o: io.c io.h types.h
	gcc -g -c io.c

# x86 ASM

# GCOV TESTING
test_str.o: test_str.c
	gcc -g $(GCOVFLAGS) -c test_str.c