#include <fstream>
#include <iostream>
#include <vector>

#include "types.h"
#include "nodes.h"
#include "general.h"

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
    //     std::cout << tokenStringMappings[t] << std::endl;
    //     t = scann->nextToken();
    // }

    bool parserError = false;
    program_node* programNode = parser(scann, &parserError);

    if (!parserError) {
        printVisitor(programNode);
        std::cout << "\nPROGRAM OUTPUT:\n"
                  << std::endl;
        runnerVisitor(programNode);
    }
    return 0;
}
