CXX=clang++
CXX_FLAGS=-std=c++20 -Iincludes -Wall -Wextra -Werror -O0 -gdwarf-4

exec: bin/exec
tests: bin/tests

bin/exec: ./src/dna_forensics.cc ./includes/utilities.hpp ./src/utilities.cc ./includes/functions.hpp ./src/functions.cc 
	$(CXX) $(CXX_FLAGS) ./src/dna_forensics.cc ./src/utilities.cc ./src/functions.cc -o $@

bin/tests: ./tests/tests.cc ./includes/utilities.hpp ./src/utilities.cc ./includes/functions.hpp ./src/functions.cc
	$(CXX) $(CXX_FLAGS) ./tests/tests.cc ./src/utilities.cc ./src/functions.cc -o $@

.DEFAULT_GOAL := exec
.PHONY: exec tests clean

clean:
	rm -rf bin/*
