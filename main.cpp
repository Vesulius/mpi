#include <fstream>
#include <iostream>
#include <vector>

#include "types.h"
#include "nodes.h"
#include "general.h"

int main(int argc, char** argv) {
    DebugLog("\nRUNNING IN DEBUG MODE:");

    std::string sourceFilePath;
    if (argc > 1) {
        sourceFilePath = argv[1];
    } else {
        std::cout << "Error: no sourcefile specified" << std::endl;
        return 0;
    }

    bool error = false;
    Scanner* scanner = new Scanner(sourceFilePath, &error);

    DebugLog("\nPARSING:");
    program_node* programNode = parser(scanner, &error);
    delete scanner;

    if (!error) {
        #ifdef DEBUG
        printVisitor(programNode);
        #endif
        DebugLog("\nPROGRAM OUTPUT:");
        runnerVisitor(programNode);
    } else {
        std::cout << "At least one parsing error was fond. Halting execution" << std::endl;
    }
    return 0;
}
