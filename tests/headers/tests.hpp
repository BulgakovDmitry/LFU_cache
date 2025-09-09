#ifndef LFU_TESTS_HPP
#define LFU_TESTS_HPP

#include <iostream>
#include <vector>
#include <array>

struct Test {
    std::size_t      cacheSize, nItems;
    std::vector<int> inputVec;
    std::vector<int> outputVec;
};

enum class TestError {
    OK                    = 0,
    NIL_CACHE_SIZE        = 1 << 0,
    INPUT_VECTOR_OVERFLOW = 1 << 1,
    INPUT_VECTOR_UNFILLED = 1 << 2,
    INVALID_OUTPUT_VECTOR = 1 << 3,
    ALGORITHM_ERROR       = 1 << 4
};

enum class TestResult {
    TEST_SUCCESS = 0,
    TEST_FAILURE = 1
};

inline constexpr std::size_t NUMBER_OF_TESTS = 12;

void testsRun(const std::array<Test, NUMBER_OF_TESTS>& dataBase);

#endif