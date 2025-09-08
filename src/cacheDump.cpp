#include "../headers/cacheDump.hpp"

void consoleDump(const LFU& cache)
{
    std::cout << MANG << "Console dump" << RED << ": " << RESET;
    
    for (auto it = cache.data.begin(); it != cache.data.end(); ++it) 
        std::cout << YELLOW << it->value << RESET << " ";
    std::cout << std::endl;
}

void consoleFullDump(const LFU& cache)
{
    std::cout << MANG << "Console full dump in format " 
              << GREEN "["
              << YELLOW << "value"
              << RED << ":" 
              << YELLOW << "numberOfRequests"
              << RED << ":" 
              << YELLOW << "lastAccessedTime"
              << GREEN "]"
              << RED << ": " << RESET;

    for (auto it = cache.data.begin(); it != cache.data.end(); ++it) 
    {
        std::cout << GREEN "["
                  << YELLOW << it->value 
                  << RED << ":" 
                  << YELLOW << it->numberOfRequests 
                  << RED << ":" 
                  << YELLOW << it->lastAccessedTime 
                  << GREEN << "]"
                  << RESET << " ";
    }
    std::cout << std::endl;
}