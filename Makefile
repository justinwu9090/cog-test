CUR=$(shell  pwd)
INC=-I/usr/lib/llvm-14/include -I$(CUR)/tests -I$(CUR)/lib
MORE=-D_GLIBCXX_DEBUG

all: main
	@echo done
main:
	clang++ $(MORE) -lclang $(INC) lib/Methods.cpp main.cpp -o main 
test:
	clang++ $(MORE) -lclang -g -lboost_unit_test_framework $(INC) lib/Methods.cpp tests/Methods_test.cpp -o test
clean:
	@rm -f main test
install:
	sudo apt-get install libclang-dev libboost-all-dev
