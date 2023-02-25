#include <fstream>
#include <iostream>
#include <vector>

#include "header.h"

std::vector<std::pair<Token, std::string>> tokens2;
std::pair<Token, std::string> current;
int i = 0;

struct node {};

struct error_node : node {};

struct null_node : node {};

struct literal_node : node {
    std::string value;
};

struct assign_node : node {
    std::string id;
    node child;
};

struct exspression_node : node {
    node child;
};

struct statement_node : node {
    node child;
};

struct statement_list_node : node {
    node statement;
    node statementList;
};

struct program_node : node {
    node child;
};

node program();
node statement();
node statementList();
node expression();

void match(Token expected);
void nextToken();
void printError();

node expression() {
    exspression_node node;
    switch (current.first) {
        case literal: {
            match(literal);
            literal_node literalNode;
            literalNode.value = current.second;
            node.child = literalNode;
            return node;
        }
        default:
            printError();
            error_node errorNode;
            return errorNode;
    }
}

node assignValue(std::string varName) {
    assign_node node;
    switch (current.first) {
        case assign:
            match(assign);
            node.id = varName;
            node.child = expression();
            return node;
        case endline:
            null_node nullNode;
            return nullNode;
        default:
            printError();
            error_node errorNode;
            return errorNode;
    }
}

node statement() {
    statement_node node;
    switch (current.first) {
        case declare_var: {
            match(declare_var);
            match(id);
            match(declare_type);
            match(type);
            node.child = assignValue(current.second);
            match(endline);
            return node;
        }
        case id:
            match(id);
            node.child = assignValue(current.second);
            match(endline);
            return node;
        case endfile:
            match(endfile);
            null_node nullNode;
            return nullNode;
        default:
            printError();
            error_node errorNode;
            return errorNode;
    }
}

node statementList() {
    statement_list_node node;
    switch (current.first) {
        case endfile:
            null_node nullNode;
            return nullNode;
        default:
            node.statement = statement();
            node.statementList = statementList();
            return node;
    }
}

node program() {
    program_node node;
    node.child = statementList();
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

void parser(std::vector<std::pair<Token, std::string>>& tokens) {
    tokens2 = tokens;
    std::cout << "Number of tokens " << tokens.size() << std::endl;
    nextToken();
    node programNode = program();
}
