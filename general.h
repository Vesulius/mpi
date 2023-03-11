#pragma once

#include <fstream>
#include <map>
#include <string>
#include <variant>
#include <vector>

extern std::map<std::string, std::pair<Type, std::variant<std::string, int, bool>>> symbolTable;

class Scanner {
    char c;
    std::ifstream sourceFile;
    int row;
    int column;
    std::variant<std::string, int, bool> data;
    Type typeVal;
    Operator opVal;
    Token current;

   public:
    Scanner(std::string);
    Token nextToken();
    std::string getStringLocation();
    std::pair<int, int> getLocation();
    std::variant<std::string, int, bool> getData();
    Type getType();
    Operator getOperator();
};

program_node* parser(Scanner*);

void printVisitor(program_node*);
void typeVisitor(program_node*);
void runnerVisitor(program_node*);
