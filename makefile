CC = g++
WARNINGS = -Wall -pedantic -Werror
OPTIMIZATION = -O3
STD = -std=c++17
FLAGS = $(WARNINGS) $(OPTIMIZATION) $(STD)

TARGETS = $(patsubst src/%.cc,bin/%,$(wildcard src/*.cc))

all: bin $(TARGETS)

bin/%: src/%.cc 
	$(CC) $(FLAGS) -o $@ $< 

bin:
	mkdir -p bin

clean:
	rm bin/*