# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  =

# the build target executable:
EXEC = bdi

CPPFILES = main.cpp Artigo.cpp Hash.cpp

all: $(CPPFILES)
	$(CC) $(CFLAGS) -o $(EXEC) $(CPPFILES) -std=c++11

run:
	./$(EXEC)

clean:
	$(RM) $(EXEC)
