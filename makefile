CC = g++
WARNINGS = -Wall -pedantic -Werror
OPTIMIZATION = -O3
STD = -std=c++17
FLAGS = $(WARNINGS) $(OPTIMIZATION) $(STD)

TARGETS = $(patsubst %.cc,bin/%,$(wildcard *.cc))

all: bin $(TARGETS)

bin/%: %.cc 
	$(CC) $(FLAGS) -o $@ $< 

bin:
	mkdir -p bin

clean:
	rm bin/*