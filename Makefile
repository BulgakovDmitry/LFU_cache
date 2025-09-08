.SILENT:

#--------------------------------------------------------------------------------------------------
COMPILER = g++
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
SRC  = src/
HPP  = headers/
VEC  = vector/
LIB  = myLib/
OBJ  = obj/
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
INCLUDE_FLAGS = -I$(LIB) -I$(VEC) -I$(HPP) 
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
VECTOR_OBJ = $(OBJ)vector.o 
LFU_OBJ    = $(OBJ)cacheStruct.o $(OBJ)cacheFunc.o
MYLIB_OBJ  = $(OBJ)myLib.o
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
lfu_: lfu
	./lfu.out --interface

run: lfu

run_: lfu_
#--------------------------------------------------------------------------------------------------
     

#--------------------------------------------------------------------------------------------------
clean: 
	rm -f $(OBJ)*
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
lfu: $(LFU_OBJ) $(MYLIB_OBJ) $(VECTOR_OBJ) $(OBJ)main.o
	$(COMPILER) $^ -o lfu.out $(FLAGS)
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
$(OBJ)%.o : $(LIB)%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

$(OBJ)%.o : $(SRC)%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

$(OBJ)%.o : $(VEC)%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@	

$(OBJ)%.o : %.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@
#--------------------------------------------------------------------------------------------------