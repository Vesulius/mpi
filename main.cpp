#include <fstream>
#include <iostream>
#include <vector>

#include "header.h"

std::vector<std::pair<Token, std::string>> tokens;

int main(int argc, char** argv) {
    std::string sourceFilePath;
    if (argc > 1) {
        sourceFilePath = argv[1];
    } else {
        sourceFilePath = "test.mp";
    }

    scanner(sourceFilePath, tokens);

    for (std::pair i : tokens) {
        std::cout << tokenStringMappings[i.first] << ' ';
    }
    std::cout << std::endl
              << std::endl;

    program_node* programNode = parser(tokens);
    printVisitor(programNode);
    return 0;
}