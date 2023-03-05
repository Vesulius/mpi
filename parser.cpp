#include <iostream>
#include <vector>

#include "header.h"

Scanner* scanner;
Token current;
void match(Token expected);
void printError();

program_node* program();
statement_node* statement();
statement_list_node* statementList();
expression_node* expression();

literal_node* literalVal() {
    literal_node* node = new literal_node();
    if (std::holds_alternative<int>(scanner->getData())) {
        node->value = std::get<int>(scanner->getData());
        node->type = type_int;
    } else {
        node->value = std::get<std::string>(scanner->getData());
        node->type = type_string;
    }
    match(literal);
    return node;
}

id_node* idVal() {
    id_node* node = new id_node();
    node->value = std::get<std::string>(scanner->getData());
    match(id);
    return node;
}

multi_node* multiVal() {
    multi_node* node = new multi_node();
    node->op = std::get<Operator>(scanner->getData());
    match(multi);
    return node;
}

add_node* addVal() {
    add_node* node = new add_node();
    node->op = std::get<Operator>(scanner->getData());
    match(add);
    return node;
}

factor_node* factor() {
    factor_node* node = new factor_node();
    switch (current) {
        case id:
            node->id = idVal();
            return node;
        case literal:
            node->literal = literalVal();
            return node;
        case lbracet:
            match(lbracet);
            node->expression = expression();
            match(rbracet);
            return node;
        default:
            printError();
            return nullptr;
    }
}

factor_tail_node* factorTail() {
    factor_tail_node* node = new factor_tail_node();
    switch (current) {
        case multi:
            node->multi = multiVal();
            node->factor = factor();
            node->factorTail = factorTail();
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
    switch (current) {
        case endfile:
            return nullptr;
        default:
            node->factor = factor();
            node->factorTail = factorTail();
            return node;
    }
}

term_tail_node* termTail() {
    term_tail_node* node = new term_tail_node();
    switch (current) {
        case add:
            node->add = addVal();
            node->term = term();
            node->termTail = termTail();
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
    switch (current) {
        case endfile:
            return nullptr;
        default: {
            node->term = term();
            node->termTail = termTail();
            return node;
        }
    }
}

print_node* printVal() {
    print_node* node = new print_node();
    switch (current) {
        case print:
            match(print);
            node->expression = expression();
            return node;
        default:
            return nullptr;
    }
}

read_node* readVal() {
    read_node* node = new read_node();
    switch (current) {
        case read:
            match(read);
            node->id = idVal();
            return node;
        default:
            return nullptr;
    }
}

assign_node* assignValue(id_node* idNode) {
    assign_node* node = new assign_node();
    node->id = idNode;
    switch (current) {
        case assign:
            match(assign);
            node->expression = expression();
            return node;
        case endline:
            return nullptr;
        default:
            printError();
            return nullptr;
    }
}

declare_node* declareVar() {
    declare_node* node = new declare_node();
    switch (current) {
        case declare_var: {
            match(declare_var);
            id_node* idNode = idVal();
            node->id = idNode;
            match(declare_type);
            node->type = std::get<Type>(scanner->getData());
            match(type);
            node->assignement = assignValue(idNode);
            return node;
        }
        default:
            printError();
            return nullptr;
    }
}

statement_node* statement() {
    statement_node* node = new statement_node();
    switch (current) {
        case declare_var:
            node->declare = declareVar();
            match(endline);
            return node;
        case id: {
            id_node* idNode = idVal();
            node->assignment = assignValue(idNode);
            match(endline);
            return node;
        }
        case print:
            node->print = printVal();
            match(endline);
            return node;
        case read:
            node->read = readVal();
            match(endline);
            return node;
        case endfile:
            match(endfile);
            return nullptr;
        default:
            printError();
            return node;
    }
}

statement_list_node* statementList() {
    statement_list_node* node = new statement_list_node();
    switch (current) {
        case endfile:
            return nullptr;
        default:
            node->statement = statement();
            node->statementList = statementList();
            return node;
    }
}

program_node* program() {
    program_node* node = new program_node();
    node->statementList = statementList();
    match(endfile);
    return node;
}

void match(Token expected) {
    if (current != expected) {
        std::cout << "Parsing error at " << scanner->getLocation() << ": unexpected token: " << tokenStringMappings[current] << ". Expected: " << tokenStringMappings[expected] << std::endl;
        current = endfile;
        return;
    } else if (expected == endfile) {
        return;
    }
    current = scanner->nextToken();
    std::cout << "Current token: " << tokenStringMappings[current] << std::endl;
}

void printError() {
    std::cout << "Parsing error at " << scanner->getLocation() << ": unexpected token: " << tokenStringMappings[current] << std::endl;
    current = endfile;
}

program_node* parser(Scanner* s) {
    scanner = s;
    current = scanner->nextToken();
    std::cout << "Current token: " << tokenStringMappings[current] << std::endl;
    return program();
}
