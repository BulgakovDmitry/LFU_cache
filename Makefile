.SILENT:

#--------------------------------------------------------------------------------------------------
COMPILER = g++
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
SRC   = src/
HPP   = headers/
LIB   = myLib/
OBJ   = obj/
TESTS = tests/
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
INCLUDE_FLAGS = -I$(LIB) -I$(HPP) 
SANITAZER     = -fsanitize=address
SFML_FLAGS    = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
GDB           = -g
OPTIMIZ_OF    = -O0

FLAGS		  = -D _DEBUG -ggdb3 -std=c++17 $(OPTIMIZ_OF) -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations \
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
LFU_OBJ    = $(OBJ)cacheStruct.o $(OBJ)cacheFunc.o $(OBJ)cacheDump.o
MYLIB_OBJ  = $(OBJ)myLib.o
TEST_OBJ   = $(OBJ)tests.o $(LFU_OBJ) $(MYLIB_OBJ)
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
.PHONY: lfu_
lfu_: lfu
	./lfu.out --interface

.PHONY: test_
test_: test
	./tests.out

.PHONY: run
run: lfu

.PHONY: run_
run_: lfu_
#--------------------------------------------------------------------------------------------------
     

#--------------------------------------------------------------------------------------------------
.PHONY: clean
clean: 
	rm -f $(OBJ)*
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
.PHONY: lfu 
lfu: $(LFU_OBJ) $(MYLIB_OBJ) $(OBJ)main.o
	$(COMPILER) $^ -o lfu.out $(FLAGS)

.PHONY: test
test: $(TEST_OBJ) $(MYLIB_OBJ) $(OBJ)testsMain.o
	$(COMPILER) $^ -o tests.out $(FLAGS)
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
$(OBJ)%.o : $(LIB)%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

$(OBJ)%.o : $(SRC)%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@	

$(OBJ)%.o : %.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

$(OBJ)%.o : $(TESTS)$(SRC)%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@
#--------------------------------------------------------------------------------------------------