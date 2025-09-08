#ifndef LFU_TESTS_HPP
#define LFU_TESTS_HPP

#include <iostream>
#include <vector>

struct Test
{
    size_t cacheSize, nItems;
    std::vector<int> inputVec;
    std::vector<int> outputVec;
};

enum class TestError
{
    OK              = 0,
    NIL_CACHE_SIZE  = 1 << 0,
    INPUT_VECTOR_OVERFLOW = 1 << 1,
    INPUT_VECTOR_UNFILLED = 1 << 2,
    INVALID_OUTPUT_VECTOR = 1 << 3,
};

const size_t NUMBER_OF_TESTS = 12;

void inputTest(const Test& test, uint64_t& testStatus);

#endif