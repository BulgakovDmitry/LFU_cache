#ifndef LFU_TESTS_HPP
#define LFU_TESTS_HPP

#include <cstddef>  
#include <vector>

namespace caches {

struct Test {
    std::size_t      cacheSize, nItems, numberOfHits;
    std::vector<int> inputVec;
    std::vector<int> outputVec;
};

const std::size_t DASH = 0; 

enum class TestError {
    OK                       = 0,
    NIL_CACHE_SIZE           = 1 << 0,
    INPUT_VECTOR_OVERFLOW    = 1 << 1,
    INPUT_VECTOR_UNFILLED    = 1 << 2,
    INVALID_OUTPUT_VECTOR    = 1 << 3,
    ALGORITHM_ERROR          = 1 << 4,
    INCORRECT_NUMBER_OF_HITS = 1 << 5
};

enum class TestResult {
    TEST_SUCCESS = 0,
    TEST_FAILURE = 1
};

namespace CacheLFU {
    void testsRun(const std::vector<Test>& dataBase, 
                        std::size_t NUMBER_OF_TESTS);
};
namespace CacheIdeal {
    void testsRun(const std::vector<Test>& dataBase, 
                        std::size_t NUMBER_OF_TESTS);
};

} // namespace caches

#endif // LFU_TESTS_HPP