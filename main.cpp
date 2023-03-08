#include <fstream>
#include <iostream>
#include <vector>

#include "header.h"

int main(int argc, char** argv) {
    std::string sourceFilePath;
    if (argc > 1) {
        sourceFilePath = argv[1];
    } else {
        sourceFilePath = "test.mp";
    }

    Scanner* scann = new Scanner(sourceFilePath);

    // Token t = scann->nextToken();

    // while (t != endfile) {
    //     std::cout << tokenStringMappings[t] << " " << std::endl;
    //     t = scann->nextToken();
    // }

    program_node* programNode = parser(scann);
    printVisitor(programNode);
    typeVisitor(programNode);

    return 0;
}
