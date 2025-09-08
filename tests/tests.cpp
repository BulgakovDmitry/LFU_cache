#include "tests.hpp"
#include <myLib.hpp>

static void testVerify(const uint64_t testStatus);
static void printErrorLog(const uint64_t testStatus);

#define IS_ERROR(error) (testStatus & static_cast<uint64_t>(error))
static void printErrorLog(const uint64_t testStatus)
{
    if (IS_ERROR(TestError::NIL_CACHE_SIZE)) 
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::NIL_CACHE_SIZE) 
                  << " - cache size = 0, it is empty cache\n" << RESET;
    if (IS_ERROR(TestError::INPUT_VECTOR_OVERFLOW))
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::INPUT_VECTOR_OVERFLOW)
                  << " - too many input values\n" << RESET;
    if (IS_ERROR(TestError::INPUT_VECTOR_UNFILLED))
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::INPUT_VECTOR_UNFILLED)
                  << " - you have not entered all the values\n" << RESET;
    if (IS_ERROR(TestError::INVALID_OUTPUT_VECTOR))
        std::cout << RED << "\t\t\t\tERROR CODE " << static_cast<uint64_t>(TestError::INVALID_OUTPUT_VECTOR)
                  << " - incorrect output format\n" << RESET;
}
#undef IS_ERROR

static void testVerify(const uint64_t testStatus)
{
    std::cout << CEAN << "verification... " << RESET;
    if (testStatus == static_cast<uint64_t>(TestError::OK))
        std::cout << GREEN << "SUCCESS" << RESET;
    else
    {
        std::cout << RED << "FAILURE\n" << RESET;
        printErrorLog(testStatus);
    }
}

void inputTest(const Test& test, uint64_t& testStatus)
{
    if (test.cacheSize == 0)
        testStatus |= static_cast<uint64_t>(TestError::NIL_CACHE_SIZE);
    
    if (test.inputVec.size() != test.nItems)
    {
        if (test.inputVec.size() > test.nItems)
            testStatus |= static_cast<uint64_t>(TestError::INPUT_VECTOR_OVERFLOW);
        else 
            testStatus |= static_cast<uint64_t>(TestError::INPUT_VECTOR_UNFILLED);
    }

    if (test.outputVec.size() != test.cacheSize)
    {
        if (test.inputVec.size() >= test.cacheSize)
            testStatus |= static_cast<uint64_t>(TestError::INVALID_OUTPUT_VECTOR);
    }

    testVerify(testStatus);
    testStatus = 0;
}