CC = g++-10
WARNINGS = -Wall -pedantic -Werror -Wextra -Wshadow -Wformat=2 -Weffc++ -Wfloat-equal -Wstrict-overflow=4
OPTIMIZATION = -O3 -march=native
STD = -std=c++20
INC = -Iinclude -isystem ../tpl/gif-h
FLAGS = $(WARNINGS) $(OPTIMIZATION) $(STD) $(INC)

TARGETS = $(patsubst src/%.cc,bin/%,$(wildcard src/*.cc))

all: bin $(TARGETS)

bin/%: src/%.cc include/utilities.hpp
	$(CC) $(FLAGS) -o $@ $< 

bin:
	mkdir -p bin

clean:
	rm bin/*