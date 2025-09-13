#include "../headers/tests.hpp"
#include <stdint.h>                  
#include <iostream>                 
#include <list>                      
#include "../../common/colors.hpp"                
#include "../../headers/cache.hpp"     
#include "../../headers/idealCache.hpp" 
#include "../../headers/cacheFunc.hpp"  

static TestResult testVerify       (uint64_t testStatus);
static void       printErrorLog    (uint64_t testStatus);
static void       printAlgorithmLog(const LFU<std::size_t, int>& cache, Test test);
static void       printOutputLog   (const LFU<std::size_t, int>& cache, Test test);

namespace CacheLFU {
    static TestResult test         (const Test& test, uint64_t& testStatus);
};

namespace CacheIdeal {
    static TestResult test         (const Test& test, uint64_t& testStatus);
};

static TestResult inputTest        (const Test& test, uint64_t& testStatus);
static TestResult algorithmicTest  (const Test& test, uint64_t& testStatus);

#define IS_ERROR(error) (testStatus & static_cast<uint64_t>(error))
static void printErrorLog(uint64_t testStatus) {
    if (IS_ERROR(TestError::NIL_CACHE_SIZE)) 
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::NIL_CACHE_SIZE) 
                  << " - cache size = 0, it is empty cache\n"                << RESET;
    if (IS_ERROR(TestError::INPUT_VECTOR_OVERFLOW))
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::INPUT_VECTOR_OVERFLOW)
                  << " - too many input values\n"                            << RESET;
    if (IS_ERROR(TestError::INPUT_VECTOR_UNFILLED))
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::INPUT_VECTOR_UNFILLED)
                  << " - you have not entered all the values\n"              << RESET;
    if (IS_ERROR(TestError::INVALID_OUTPUT_VECTOR))
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::INVALID_OUTPUT_VECTOR)
                  << " - incorrect output format\n"                          << RESET;
    if (IS_ERROR(TestError::ALGORITHM_ERROR)) 
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::ALGORITHM_ERROR)
                  << " - the algorithm produced an incorrect result\n"       << RESET;
    if (IS_ERROR(TestError::INCORRECT_NUMBER_OF_HITS))
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::INCORRECT_NUMBER_OF_HITS)
                  << " - the program returned an incorrect number of hits\n" << RESET;
}
#undef IS_ERROR

static void printAlgorithmLog(const LFU<std::size_t, int>& cache, Test test) {
    std::cout << RED << "\t\t\t\texpected [ " << RESET;
    for (std::size_t k = 0; k < test.cacheSize; ++k)
        std::cout << YELLOW << test.outputVec[k] << ' ' << RESET;
    std::cout << RED << ']' << RESET;
    std::cout << RED << ", and got [ " << RESET;
    for (auto it = cache.begin(); it != cache.end(); ++it)
        std::cout << YELLOW << it->value << ' ' << RESET;
    std::cout << RED << "]\n" << RESET;
}

static void printOutputLog(const LFU<std::size_t, int>& cache, Test test) {
    std::cout << RED    << "\t\t\t\texpected number of hits " 
              << YELLOW << test.numberOfHits
              << RED    << " and got " 
              << YELLOW << cache.getNumberOfHits() << std::endl << RESET;
}

static TestResult testVerify(uint64_t testStatus) {
    std::cout << CEAN << "verification... " << RESET;
    if (testStatus == static_cast<uint64_t>(TestError::OK)) {
        std::cout << GREEN << "SUCCESS\n" << RESET;
        return TestResult::TEST_SUCCESS;
    }
    else {
        std::cout << RED << "FAILURE\n" << RESET;
        printErrorLog(testStatus);
        return TestResult::TEST_FAILURE;
    }
}

static TestResult inputTest(const Test& test, uint64_t& testStatus) {
    std::cout << MANG << "\tinput test\t" << RESET;

    if (test.cacheSize == 0)
        testStatus |= static_cast<uint64_t>(TestError::NIL_CACHE_SIZE);
    
    if (test.inputVec.size() != test.nItems) {
        if (test.inputVec.size() > test.nItems)
            testStatus |= static_cast<uint64_t>(TestError::INPUT_VECTOR_OVERFLOW);
        else 
            testStatus |= static_cast<uint64_t>(TestError::INPUT_VECTOR_UNFILLED);
    }

    if (test.outputVec.size() != test.cacheSize) {
        if (test.inputVec.size() >= test.cacheSize)
            testStatus |= static_cast<uint64_t>(TestError::INVALID_OUTPUT_VECTOR);
    }

    TestResult testResult = testVerify(testStatus);
    testStatus = 0;
    return testResult;
}

static TestResult algorithmicTest(const Test& test, uint64_t& testStatus) {
    std::cout << MANG << "\talgor test\t" << RESET;

    LFU<std::size_t, int> cache(test.cacheSize);

    for (std::size_t i = 0; i < test.nItems; ++i)
        cachePut(cache, getKey<std::size_t, int>(test.inputVec[i]), test.inputVec[i]);

    std::size_t i = 0;
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        if (it->value != test.outputVec[i])
            testStatus |= static_cast<uint64_t>(TestError::ALGORITHM_ERROR);
        ++i;
    }

    if (cache.getNumberOfHits() != test.numberOfHits)
        testStatus |= static_cast<uint64_t>(TestError::INCORRECT_NUMBER_OF_HITS);

    TestResult testResult = testVerify(testStatus);
    if (testStatus != static_cast<uint64_t>(TestError::OK)) {
        if (testStatus & static_cast<uint64_t>(TestError::ALGORITHM_ERROR))
            printAlgorithmLog(cache, test);
        if (testStatus & static_cast<uint64_t>(TestError::INCORRECT_NUMBER_OF_HITS))
            printOutputLog(cache, test);
    }

    testStatus = 0;
    return testResult;
}

namespace CacheLFU {
    static TestResult test(const Test& test, uint64_t& testStatus) {
        TestResult resultStatus = inputTest(test, testStatus);
        if (resultStatus == TestResult::TEST_SUCCESS) {
            resultStatus = algorithmicTest(test, testStatus);
            if (resultStatus == TestResult::TEST_SUCCESS)
                std::cout << GREEN << "<Test successfully completed>\n" << RESET;
            else
                std::cout << RED << "<failed test>\n" << RESET;
        } 
        else
            std::cout << RED << "<failed test>\n" << RESET;
        std::cout << std::endl;
        return resultStatus;
    }
};

namespace CacheIdeal {
    static TestResult test(const Test& test, uint64_t& testStatus) {
        IdealCache<int> icache(test.cacheSize, test.inputVec);
        icache.process();

        if (icache.getNumberOfHits() != test.numberOfHits) {
            testStatus |= static_cast<uint64_t>(TestError::INCORRECT_NUMBER_OF_HITS);
        }

        return testVerify(testStatus);
    }
};

namespace CacheLFU {
    void testsRun(const std::vector<Test>& dataBase, std::size_t NUMBER_OF_TESTS) {
        std::cout << CEAN << "___________________________LFU_CACHE_TESTING___________________________\n" <<  RESET;

        uint64_t testStatus   = 0;
        uint64_t resultStatus = 0;

        for (std::size_t i = 0; i < NUMBER_OF_TESTS; ++i) {
            std::cout << BLUE << "test (" << GREEN << i + 1 << BLUE << ')' << RED << ":\n" << RESET;
            resultStatus += static_cast<uint64_t>(CacheLFU::test(dataBase[i], testStatus));
        }

        std::cout << CEAN << "LFU CACHE --- TESTS RESULT: " << RESET;
        if (resultStatus) {
            std::cout << RED << "FAILURE \t[ " << YELLOW << resultStatus 
                      << RED << " test (tests) ended with an error ]" << RESET << std::endl;
        }
        else 
            std::cout << GREEN << "SUCCESS" << RESET << std::endl;
    }
};

namespace CacheIdeal {
    void testsRun(const std::vector<Test>& dataBase, std::size_t NUMBER_OF_TESTS) {
        std::cout << CEAN << "___________________________IDEAL_CACHE_TESTING___________________________\n" <<  RESET;

        uint64_t testStatus   = 0;
        uint64_t resultStatus = 0;

        for (std::size_t i = 0; i < NUMBER_OF_TESTS; ++i) {
            std::cout << BLUE << "test (" << GREEN << i + 1 << BLUE << ')' << RED << ":\t" << RESET;
            resultStatus += static_cast<uint64_t>(CacheIdeal::test(dataBase[i], testStatus));
            testStatus = 0;
        }

        std::cout << CEAN << "IDEAL CACHE --- TESTS RESULT:   " << RESET;
        if (resultStatus) {
            std::cout << RED << "FAILURE \t[ " << YELLOW << resultStatus 
                      << RED << " test (tests) ended with an error ]" << RESET << std::endl;
        }
        else 
            std::cout << GREEN << "SUCCESS" << RESET << std::endl;
    }
};