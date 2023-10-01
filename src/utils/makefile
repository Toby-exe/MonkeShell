# ========== EXECUTABLE NAMES ==========
STRTEST = str_tst
IOTEST = io_tst

# ========== MODULES AND FLAGS ==========
STRMODULES = driver.o str.o
IOMODULES = driver.o io.o

CFLAGS = -nostdlib

# ========== CREATE AND RUN EXECUTABLES ==========
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
