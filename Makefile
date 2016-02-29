.PHONY: all clean

programs = hello binarytrees fasta fannkuchredux nbody lissa

all: $(programs)

clean:
	-rm -f $(programs)

prefix = /opt/moxie-elf

CPPFLAGS = \
	-I$(prefix)/moxie-elf/include \
	-I$(prefix)/moxie-elf/include/c++/5.3.0 \
	-I$(prefix)/moxie-elf/include/c++/5.3.0/moxie-elf \
	-I$(prefix)/lib/gcc/moxie-elf/5.3.0/include

LDFLAGS = \
	-L$(prefix)/moxie-elf/lib \
	-Wl,-T,$(prefix)/moxie-elf/lib/sim.ld

LIBS = -lsim -lc -lm

CC = $(prefix)/bin/moxie-elf-gcc
CXX = $(prefix)/bin/moxie-elf-g++
CXXFLAGS = -std=c++14

hello: hello.cpp
	$(CXX) -g $(CXXFLAGS) $(CPPFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

binarytrees: binarytrees.c
	$(CC) -g $(CPPFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

fasta: fasta.c
	$(CC) -g -O1 $(CPPFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

fannkuchredux: fannkuchredux.c
	$(CC) -g -std=c99 -O3 $(CPPFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

nbody: nbody.cpp
	$(CXX) -g $(CXXFLAGS) $(CPPFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

lissa: lissa.c sim-fbwrite.S sim-msleep.S
	$(CC) -g -std=c99 -O3 $(CPPFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@
