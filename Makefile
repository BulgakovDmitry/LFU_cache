.SILENT:

#--------------------------------------------------------------------------------------------------
CXX = g++
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
SRC   := src/
HPP   := headers/
LIB   := myLib/
BUILD := build/
TESTS := tests/
OBJ   := $(BUILD)obj/
BIN   := $(BUILD)bin/
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
INCLUDE_FLAGS := -I$(LIB) -I$(HPP) 
SANITAZER     := -fsanitize=address

FLAGS := -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations \
	     -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wctor-dtor-privacy -Wempty-body -Wfloat-equal          \
	     -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd  \
	     -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel     \
	     -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override            \
	     -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros              \
	     -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector        \
	     $(INCLUDE_FLAGS)                                                                                                             \
	     $(SANITAZER)
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
LFU_OBJ   := $(OBJ)cacheFunc.o $(OBJ)cacheDump.o
MYLIB_OBJ := $(OBJ)myLib.o
TEST_OBJ  := $(OBJ)tests.o $(LFU_OBJ) $(MYLIB_OBJ)
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
.PHONY: all
all: lfu

.PHONY: run
run: lfu
	./$(BIN)lfu.out 

.PHONY: run_
run_: lfu
	./$(BIN)lfu.out  --interface


.PHONY: tests
test: tests
	./$(BIN)tests.out
#--------------------------------------------------------------------------------------------------
     

#--------------------------------------------------------------------------------------------------
.PHONY: clean
clean:
	rm -f $(OBJ)*
	rm -f $(BIN)*
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
.PHONY: help
help:
	printf '%s\n'                                                 \
	"Main goals:" 									              \
	"  make         - compile all"                                \
	"  make run_    - compile and run {lfu.out} with --interface" \
	"  make run     - compile and run {lfu.out}"                  \
	"  make testRun - compile and run tests {tests.out}"          \
	"  make lfu     - compile LFU algorithm"                      \
	"  make test    - compile tests"                              \
	"  make clean   - clean build/obj/ and build/bin/" 
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
.PHONY: lfu 
lfu: $(LFU_OBJ) $(MYLIB_OBJ) $(OBJ)main.o
	$(CXX) $^ -o $(BIN)lfu.out $(FLAGS)

.PHONY: tests
tests: $(TEST_OBJ) $(MYLIB_OBJ) $(OBJ)testsMain.o
	$(CXX) $^ -o $(BIN)tests.out $(FLAGS)
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
$(OBJ)%.o : $(LIB)%.cpp
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ)%.o : $(SRC)%.cpp
	$(CXX) $(FLAGS) -c $< -o $@	

$(OBJ)%.o : $(TESTS)$(SRC)%.cpp
	$(CXX) $(FLAGS) -c $< -o $@
#--------------------------------------------------------------------------------------------------
