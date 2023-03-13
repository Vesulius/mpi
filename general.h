#pragma once

#include <fstream>
#include <map>
#include <string>
#include <variant>
#include <vector>

#ifdef DEBUG
#define DebugLog(x) std::cout << x << std::endl
#else
#define DebugLog(x)
#endif

class Scanner {
    std::ifstream sourceFile;
    bool* error;
    char c;
    int row;
    int column;
    std::variant<std::string, int, bool> data;
    Type typeVal;
    Operator opVal;
    Token current;

   public:
    Scanner(std::string, bool* error);
    ~Scanner();
    Token nextToken();
    std::pair<int, int> getLocation();
    std::variant<std::string, int, bool> getData();
    Type getType();
    Operator getOperator();
};

program_node* parser(Scanner*, bool*);
extern bool* error;

static std::string locToStr(std::pair<int, int> pair) {
    std::string s = std::to_string(pair.first) + "," + std::to_string(pair.second);
    return s;
}

void printVisitor(program_node*);
void typeVisitor(program_node*);
void runnerVisitor(program_node*);
