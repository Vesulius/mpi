#include <fstream>
#include <iostream>
#include <vector>

#include "header.h"

std::vector<std::pair<Token, std::string>> tokens2;
std::pair<Token, std::string> current;
int i = 0;

struct literal_node {
    std::string value;
};

struct expression_node {
    literal_node* literal;
};

struct assign_node {
    std::string id;
    expression_node* expression;
};

struct statement_node {
    assign_node* assignment;
};

struct statement_list_node {
    statement_node* statement;
    statement_list_node* statementList;
};

struct program_node {
    statement_list_node* statementList;
};

program_node* program();
statement_node* statement();
statement_list_node* statementList();
expression_node* expression();

std::string getNTabs(int);
void match(Token expected);
void nextToken();
void printError();

expression_node* expression() {
    expression_node* node = new expression_node();
    switch (current.first) {
        case literal: {
            match(literal);
            literal_node* literalNode = new literal_node();
            (*literalNode).value = current.second;
            (*node).literal = literalNode;
            return node;
        }
        default:
            printError();
            return node;
    }
}

assign_node* assignValue(std::string varName) {
    assign_node* node = new assign_node();
    switch (current.first) {
        case assign: {
            match(assign);
            (*node).id = varName;
            (*node).expression = expression();
            return node;
        }
        case endline: {
            return nullptr;
        }
        default: {
            printError();
            return node;
        }
    }
}

statement_node* statement() {
    statement_node* node = new statement_node();
    switch (current.first) {
        case declare_var: {
            match(declare_var);
            match(id);
            match(declare_type);
            match(type);
            (*node).assignment = assignValue(current.second);
            match(endline);
            return node;
        }
        case id: {
            match(id);
            (*node).assignment = assignValue(current.second);
            match(endline);
            return node;
        }
        case endfile: {
            match(endfile);
            return nullptr;
        }
        default: {
            printError();
            return node;
        }
    }
}

statement_list_node* statementList() {
    statement_list_node* node = new statement_list_node();
    switch (current.first) {
        case endfile: {
            return nullptr;
        }
        default: {
            (*node).statement = statement();
            (*node).statementList = statementList();
            return node;
        }
    }
}

program_node* program() {
    program_node* node = new program_node();
    (*node).statementList = statementList();
    match(endfile);
    return node;
}

void match(Token expected) {
    if (current.first != expected) {
        std::cout << "Parsing error: unexpected token: " << tokenStringMappings[current.first] << ". Expected: " << tokenStringMappings[expected] << std::endl;
    } else if (expected == endfile) {
        std::cout << "IT WORKED!!" << std::endl;
        return;
    }
    nextToken();
}

void printError() {
    std::cout << "Parsing error: unexpected token: " << tokenStringMappings[current.first] << std::endl;
}

void nextToken() {
    current = tokens2.at(i);
    std::cout << "currenttoken: " << tokenStringMappings[current.first] << std::endl;
    i++;
}

void printVisitor(literal_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "literal node" << std::endl;
}

void printVisitor(expression_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "expression node" << std::endl;
    printVisitor((*n).literal, tablevel + 1);
}

void printVisitor(assign_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "assign node" << std::endl;
    printVisitor((*n).expression, tablevel + 1);
}

void printVisitor(statement_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "statement node" << std::endl;
    printVisitor((*n).assignment, tablevel + 1);
}

void printVisitor(statement_list_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "statement list node" << std::endl;
    printVisitor((*n).statement, tablevel + 1);
    printVisitor((*n).statementList, tablevel + 1);
}

void printVisitor(program_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "program node" << std::endl;
    printVisitor((*n).statementList, tablevel + 1);
}

std::string getNTabs(int n) {
    std::string tabs = "";
    for (int i = 0; i < n; i++) {
        tabs += '\t';
    }
    return tabs;
}

void parser(std::vector<std::pair<Token, std::string>>& tokens) {
    tokens2 = tokens;
    std::cout << "Number of tokens " << tokens.size() << std::endl;
    nextToken();
    program_node* programNode = program();
    printVisitor(programNode, 0);
}
