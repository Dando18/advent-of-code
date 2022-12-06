CC = g++-10
WARNINGS = -Wall -pedantic -Werror -Wextra -Wshadow -Wformat=2 -Weffc++
OPTIMIZATION = -O3
STD = -std=c++20
INC = -Iinclude
FLAGS = $(WARNINGS) $(OPTIMIZATION) $(STD) $(INC)

TARGETS = $(patsubst src/%.cc,bin/%,$(wildcard src/*.cc))

all: bin $(TARGETS)

bin/%: src/%.cc include/utilities.hpp
	$(CC) $(FLAGS) -o $@ $< 

bin:
	mkdir -p bin

clean:
	rm bin/*