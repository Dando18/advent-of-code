CC = g++-10
WARNINGS = -Wall -pedantic -Werror -Wextra -Wshadow -Wformat=2 -Weffc++
OPTIMIZATION = -O3
STD = -std=c++20
FLAGS = $(WARNINGS) $(OPTIMIZATION) $(STD)

TARGETS = $(patsubst src/%.cc,bin/%,$(wildcard src/*.cc))

all: bin $(TARGETS)

bin/%: src/%.cc 
	$(CC) $(FLAGS) -o $@ $< 

bin:
	mkdir -p bin

clean:
	rm bin/*