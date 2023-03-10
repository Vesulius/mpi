#include <iostream>

#include "types.h"
#include "nodes.h"
#include "general.h"

std::string getNTabs(int);

void printVisitor(expression_node*, int);
void printVisitor(statement_list_node*, int);

void printVisitor(literal_node* n, int tablevel) {
    if (n == nullptr) return;
    if (n->type == type_string) {
        std::cout << getNTabs(tablevel) << "literal: " << std::get<std::string>(n->value) << std::endl;
    } else if (n->type == type_bool) {
        std::cout << getNTabs(tablevel) << "literal: " << std::get<bool>(n->value) << std::endl;
    } else {
        std::cout << getNTabs(tablevel) << "literal: " << std::get<int>(n->value) << std::endl;
    }
}

void printVisitor(multi_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "multi: " << operatorStringMappings[n->op] << std::endl;
}

void printVisitor(add_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "add: " << operatorStringMappings[n->op] << std::endl;
}

void printVisitor(id_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "id: " << typeStringMappings[n->type] << " " << n->value << std::endl;
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

void printVisitor(print_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "print" << std::endl;
    printVisitor(n->expression, tablevel + 1);
}

void printVisitor(read_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "read: " << std::endl;
    printVisitor(n->id, tablevel + 1);
}

void printVisitor(assign_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "assign" << std::endl;
    printVisitor(n->id, tablevel + 1);
    printVisitor(n->expression, tablevel + 1);
}

void printVisitor(declare_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "declare" << std::endl;
    printVisitor(n->id, tablevel + 1);
    printVisitor(n->assignement, tablevel + 1);
}

void printVisitor(if_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "if" << std::endl;
    printVisitor(n->expression, tablevel + 1);
    printVisitor(n->statementList, tablevel + 1);
    std::cout << getNTabs(tablevel) << "end if" << std::endl;
}

void printVisitor(statement_node* n, int tablevel) {
    if (n == nullptr) return;
    std::cout << getNTabs(tablevel) << "statement" << std::endl;
    printVisitor(n->assignment, tablevel + 1);
    printVisitor(n->declare, tablevel + 1);
    printVisitor(n->print, tablevel + 1);
    printVisitor(n->read, tablevel + 1);
    printVisitor(n->ifStatement, tablevel + 1);
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
    std::cout << "\nABSTRACT SYNTAX TREE:\n"
              << std::endl;
    printVisitor(n, 0);
}

std::string getNTabs(int n) {
    std::string tabs = "";
    for (int i = 0; i < n; i++) {
        tabs += "     ";
    }
    return tabs;
}