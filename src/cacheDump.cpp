#include "../headers/cacheDump.hpp"
#include "../headers/cacheFunc.hpp"

static void dumpListNodes   (std::ofstream& gv, const std::vector<int>& vec, std::size_t cacheSize);
static void dumpConnectNodes(std::ofstream& gv, std::size_t vecSize);

void consoleDump(const LFU& cache) {
    std::cout << MANG << "Console dump" << RED << ": " << RESET;
    
    for (auto it = cache.begin(); it != cache.end(); ++it) 
        std::cout << YELLOW << it->value << RESET << " ";
    std::cout << std::endl;
}

void consoleGeneralizedDump(const LFU& cache) {
    std::cout << MANG   << "Console dump in format " 
              << GREEN  << "["
              << YELLOW << "value"
              << RED    << ":" 
              << YELLOW << "numberOfRequests"
              << RED    << ":" 
              << YELLOW << "lastAccessedTime"
              << GREEN  << "]"
              << RED    << ": " << RESET;

    for (auto it = cache.begin(); it != cache.end(); ++it) {
        std::cout << GREEN  << "["
                  << YELLOW << it->value 
                  << RED    << ":" 
                  << YELLOW << it->numberOfRequests 
                  << RED    << ":" 
                  << YELLOW << it->lastAccessedTime 
                  << GREEN  << "]"
                  << RESET  << " ";
    }
    std::cout << std::endl;
}

void consoleFullDump(const std::vector<int>& vec, std::size_t cacheSize) {
    LFU tempCache(cacheSize);

    std::cout << MANG "________________________Console full dump________________________\n" RESET; 

    for (std::size_t i = 0; i < vec.size(); ++i) {
        cachePut(tempCache, getKey(vec[i]), vec[i]);
        consoleGeneralizedDump(tempCache);
    }
}

static void dumpListNodes(std::ofstream& gv, const std::vector<int>& vec, std::size_t cacheSize) {
    ASSERT(gv, "Unable to write in gv file", stderr);
    
    LFU tempCache(cacheSize);
    std::size_t i = 0;
    for (; i < vec.size(); ++i) {
        cachePut(tempCache, getKey(vec[i]), vec[i]);
        gv << "\tnode_" << i << " [shape=Mrecord; style = filled; fillcolor = palegreen;";
        gv << "color = \"#000000\"; fontcolor = \"#000000\";  label=\" {";
        for (auto it = tempCache.begin(); it != tempCache.end(); ++it) 
            gv << it->value << " | ";
        gv << "step " << i + 1 << "} \"];\n";
    }
    gv << "\tnode_" << i << " [shape=Mrecord; style = filled; fillcolor = palegreen;";
    gv << "color = \"#000000\"; fontcolor = \"#000000\";  label=\" { end }\"];\n\n";
}

static void dumpConnectNodes(std::ofstream& gv, std::size_t vecSize) {
    ASSERT(gv, "Unable to write in gv file", stderr);
    for (std::size_t i = 0; i < vecSize; ++i) 
        gv << "\tnode_" << i << " -> node_" << i + 1 << '\n';
}

void graphDump(const std::vector<int>& vec, std::size_t cacheSize) {
    std::ofstream gv(DUMP_FILE_GV);
    ASSERT(gv, "Unable to open gv file", stderr);

    gv << "digraph G {\n";
    gv << "    rankdir=LR;\n";
    gv << "    graph [margin=\"0.8,1.4\"];\n";
    gv << "    node [style=filled, fontcolor=darkblue,";
    gv << "    fillcolor=peachpuff, color=\"#252A34\", penwidth=2.5];\n";
    gv << "    bgcolor=\"lemonchiffon\";\n\n";

    dumpListNodes(gv, vec, cacheSize);
    dumpConnectNodes(gv, vec.size());

    gv << "}\n";
    gv.close();

    std::string dotComand = "dot " + DUMP_FILE_GV + " -Tpng -o " + DUMP_FILE_PNG;
    std::system(dotComand.c_str());
}