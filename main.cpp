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
    bool error = false;

    Scanner* scann = new Scanner(sourceFilePath, &error);

    // Token t = scann->nextToken();
    // while (t != endfile) {
    //     std::cout << tokenStringMappings[t] << std::endl;
    //     t = scann->nextToken();
    // }

    program_node* programNode = parser(scann, &error);

    if (!error) {
        printVisitor(programNode);
        std::cout << "\nPROGRAM OUTPUT:" << std::endl;
        runnerVisitor(programNode);
    }
    return 0;
}
