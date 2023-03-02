#include <iostream>

#include "header.h"

std::string getNTabs(int);

void printVisitor(expression_node*, int);

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
    printVisitor(n->expression, tablevel + 1);
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

void printVisitor(program_node* n) {
    std::cout << "\nABSTRACT SYNTAX TREE:\n" << std::endl;
    printVisitor(n, 0);
}

std::string getNTabs(int n) {
    std::string tabs = "";
    for (int i = 0; i < n; i++) {
        tabs += "     ";
    }
    return tabs;
}