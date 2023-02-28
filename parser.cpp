#include <fstream>
#include <iostream>
#include <vector>

#include "header.h"

std::vector<std::pair<Token, std::string>> tokens2;
std::pair<Token, std::string> current;
int i = 0;

struct id_node {
    std::string value;
};

struct literal_node {
    std::string value;
};

struct add_node {
    std::string op;
};

struct multi_node {
    std::string op;
};

struct factor_node {
    literal_node* literal;
    id_node* id;
};

struct factor_tail_node {
    multi_node* multi;
    factor_node* factor;
    factor_tail_node* factorTail;
};

struct term_node {
    factor_node* factor;
    factor_tail_node* factorTail;
};

struct term_tail_node {
    add_node* add;
    term_node* term;
    term_tail_node* termTail;
};

struct expression_node {
    // literal_node* literal;
    term_node* term;
    term_tail_node* termTail;
};

struct assign_node {
    std::string id;
    expression_node* expression;
};

struct statement_node {
    id_node* id;
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



literal_node* literalVal() {
    literal_node* node = new literal_node();
    node->value = current.second;
    match(literal);
    return node;
}

id_node* idVal() {
    id_node* node = new id_node();
    node->value = current.second;
    match(id);
    return node;
}

multi_node* multiVal() {
    multi_node* node = new multi_node();
    node->op = current.second;
    match(multi);
    return node;
}

add_node* addVal() {
    add_node* node = new add_node();
    node->op = current.second;
    match(add);
    return node;
}

factor_node* factor() {
    factor_node* node = new factor_node();
    switch (current.first) {
        case id:
            node->id = idVal();
            return node;
        case literal:
            node->literal = literalVal();
            return node;
        default:
            printError();
            return nullptr;
    }
}

factor_tail_node* factorTail() {
    factor_tail_node* node = new factor_tail_node();
    switch (current.first) {
        case multi:
            (*node).multi = multiVal();
            (*node).factor = factor();
            (*node).factorTail = factorTail();
            return node;
        case endline:
        case endfile:
            return nullptr;
        default:
            return nullptr;
    }
}

term_node* term() {
    term_node* node = new term_node();
    switch (current.first) {
        case endfile:
            return nullptr;
        default: {
            (*node).factor = factor();
            (*node).factorTail = factorTail();
            return node;
        }
    }
}

term_tail_node* termTail() {
    term_tail_node* node = new term_tail_node();
    switch (current.first) {
        case add:
            (*node).add = addVal();
            (*node).term = term();
            (*node).termTail = termTail();
            return node;
        case endline:
        case endfile:
            return nullptr;
        default:
            return nullptr;
    }
}

expression_node* expression() {
    expression_node* node = new expression_node();
    switch (current.first) {
        case endfile:
            return nullptr;
        default: {
            (*node).term = term();
            (*node).termTail = termTail();
            return node;
        }
    }
}

assign_node* assignValue() {
    assign_node* node = new assign_node();
    switch (current.first) {
        case assign:
            match(assign);
            (*node).expression = expression();
            return node;
        case endline:
            return nullptr;
        default:
            printError();
            return nullptr;
    }
}

statement_node* statement() {
    statement_node* node = new statement_node();
    switch (current.first) {
        case declare_var: {
            match(declare_var);
            node->id = idVal();
            match(declare_type);
            match(type);
            (*node).assignment = assignValue();
            match(endline);
            return node;
        }
        case id: {
            node->id = idVal();
            (*node).assignment = assignValue();
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
    current = {endfile, ""};
    std::cout << "Parsing error: unexpected token: " << tokenStringMappings[current.first] << std::endl;
}

void nextToken() {
    current = tokens2.at(i);
    std::cout << "currenttoken: " << tokenStringMappings[current.first] << std::endl;
    i++;
}

void printVisitor(literal_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "literal: " << n->value << std::endl;
}

void printVisitor(multi_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "multi: " << n->op << std::endl;
}

void printVisitor(add_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "add: " << n->op << std::endl;
}

void printVisitor(id_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "id" << std::endl;
}

void printVisitor(factor_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "factor" << std::endl;
    printVisitor(n->id, tablevel + 1);
    printVisitor(n->literal, tablevel + 1);
}

void printVisitor(factor_tail_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "factor tail" << std::endl;
    printVisitor(n->multi, tablevel + 1);
    printVisitor(n->factor, tablevel + 1);
    printVisitor(n->factorTail, tablevel + 1);
}

void printVisitor(term_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "term" << std::endl;
    printVisitor(n->factor, tablevel + 1);
    printVisitor(n->factorTail, tablevel + 1);
}

void printVisitor(term_tail_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "term tail" << std::endl;
    printVisitor(n->add, tablevel + 1);
    printVisitor(n->term, tablevel + 1);
    printVisitor(n->termTail, tablevel + 1);
}

void printVisitor(expression_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "expression" << std::endl;
    printVisitor(n->term, tablevel + 1);
    printVisitor(n->termTail, tablevel + 1);
}

void printVisitor(assign_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "assign" << std::endl;
    printVisitor(n->expression, tablevel + 1);
}

void printVisitor(statement_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "statement" << std::endl;
    printVisitor(n->assignment, tablevel + 1);
}

void printVisitor(statement_list_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "statement list" << std::endl;
    printVisitor(n->statement, tablevel + 1);
    printVisitor(n->statementList, tablevel + 1);
}

void printVisitor(program_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "program" << std::endl;
    printVisitor(n->statementList, tablevel + 1);
}

std::string getNTabs(int n) {
    std::string tabs = "";
    for (int i = 0; i < n; i++) {
        tabs += "     ";
    }
    return tabs;
}

void parser(std::vector<std::pair<Token, std::string>>& tokens) {
    tokens2 = tokens;
    std::cout << "Number of tokens " << tokens.size() << "\n"
              << std::endl;
    nextToken();
    program_node* programNode = program();
    printVisitor(programNode, 0);
}
