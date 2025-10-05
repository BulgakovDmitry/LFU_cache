# cmake basics

cmake_minimum_required(VERSION 3.14)
project(cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(add_debug_flags TGT) 
    set (DEBUG_FLAGS 
        -D _DEBUG 
        -ggdb3 
        -std=c++17 
        -O0 
        -Wall 
        -Wextra 
        -Weffc++ 
        -Waggressive-loop-optimizations 
        -Wc++14-compat -Wmissing-declarations  
        -Wcast-align 
        -Wcast-qual 
        -Wchar-subscripts 
        -Wconditionally-supported 
        -Wctor-dtor-privacy 
        -Wempty-body 
        -Wfloat-equal
	    -Wformat-nonliteral 
        -Wformat-security 
        -Wformat-signedness 
        -Wformat=2 
        -Winline 
        -Wlogical-op 
        -Wnon-virtual-dtor 
        -Wopenmp-simd
	    -Woverloaded-virtual 
        -Wpacked 
        -Wpointer-arith 
        -Winit-self 
        -Wredundant-decls 
        -Wshadow 
        -Wsign-promo 
        -Wstrict-null-sentinel 
        -Wstrict-overflow=2 
        -Wsuggest-attribute=noreturn 
        -Wsuggest-final-methods 
        -Wsuggest-final-types 
        -Wsuggest-override 	   
        -Wswitch-default 
        -Wswitch-enum 
        -Wsync-nand 
        -Wundef 
        -Wunreachable-code 
        -Wunused 
        -Wuseless-cast 
        -Wvariadic-macros 	     	   
        -Wno-literal-suffix 
        -Wno-missing-field-initializers 
        -Wno-narrowing 
        -Wno-old-style-cast 
        -Wno-varargs 
        -Wstack-protector
    )

	target_compile_options(${TGT} PUBLIC
		$<$<CONFIG:Debug>:
			${DEBUG_FLAGS}
        >
    $<$<CONFIG:Release>:
			${DEBUG_FLAGS}
        >
	)
endfunction(add_debug_flags)

add_executable(idCache src/idCacheMain.cpp)
add_executable(lfuCache src/main.cpp)
add_executable(test 
    tests/src/testsMain.cpp
    tests/src/tests.cpp)

target_include_directories(idCache PUBLIC headers common)
target_include_directories(lfuCache PUBLIC headers common)
target_include_directories(test PUBLIC headers common)

add_debug_flags(idCache)
add_debug_flags(lfuCache)
add_debug_flags(test)

set (RUN_LFU_WITH_INTERFACE run_LFU_with_interface)

add_custom_target(${RUN_LFU_WITH_INTERFACE}
	COMMAND ${CMAKE_BINARY_DIR}/lfuCache --interface
	COMMENT "compile and run {lfuCache} with --interface"
    DEPENDS lfuCache
)

target_compile_definitions(lfuCache PRIVATE PROJECT_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")


