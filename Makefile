.SILENT:

#--------------------------------------------------------------------------------------------------
CXX = g++
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
SRC   := src/
HPP   := headers/
BUILD := build/
TESTS := tests/
OBJ   := $(BUILD)obj/
BIN   := $(BUILD)bin/
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
.PHONY: dirs
dirs:
	@mkdir -p $(BIN) $(OBJ)
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
SANITAZER := -fsanitize=address

FLAGS_DEBUG := -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations \
	           -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wctor-dtor-privacy -Wempty-body -Wfloat-equal          \
	           -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd  \
	     	   -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel     \
	     	   -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override            \
	     	   -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros              \
	     	   -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector        \
	     $(SANITAZER)

FLAGS_RELEASE := -O2 -DNDEBUG
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
.PHONY: all
all: lfu

.PHONY: run
run: lfu
	./$(BIN)lfu.out 

.PHONY: run_with_interface
run_with_interface: lfu
	./$(BIN)lfu.out  --interface


.PHONY: tests
test: tests
	./$(BIN)tests.out
#--------------------------------------------------------------------------------------------------
     

#--------------------------------------------------------------------------------------------------
.PHONY: clean
clean:
	rm -rf -- $(OBJ)/* $(BIN)/*
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
.PHONY: help
help:
	printf '%s\n'                                                            \
	"Main goals:" 									                         \
	"  make                    - compile all"                                \
	"  make run_with_interface - compile and run {lfu.out} with --interface" \
	"  make run                - compile and run {lfu.out}"                  \
	"  make test               - compile and run tests {tests.out}"          \
	"  make lfu                - compile LFU algorithm"                      \
	"  make clean              - clean build/obj/ and build/bin/" 
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
.PHONY: lfu 
lfu: $(OBJ)main.o | dirs
	$(CXX) $^ -o $(BIN)lfu.out $(FLAGS_RELEASE)

.PHONY: tests
tests: $(OBJ)tests.o $(OBJ)testsMain.o | dirs
	$(CXX) $^ -o $(BIN)tests.out $(FLAGS_RELEASE)
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
$(OBJ)%.o : $(SRC)%.cpp | dirs
	$(CXX) $(FLAGS_RELEASE) -c $< -o $@	

$(OBJ)%.o : $(TESTS)$(SRC)%.cpp | dirs
	$(CXX) $(FLAGS_RELEASE) -c $< -o $@
#--------------------------------------------------------------------------------------------------
