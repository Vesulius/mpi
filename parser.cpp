#include <iostream>
#include <vector>

#include "types.h"
#include "nodes.h"
#include "general.h"

Scanner* scanner;
bool* error;
Token current;

void match(Token expected);
void printError();

program_node* program();
statement_node* statement();
statement_list_node* statementList();
statement_list_node* statementList(bool);
expression_node* expression();

literal_node* literalVal() {
    literal_node* node = new literal_node();
    node->location = scanner->getLocation();
    if (scanner->getType() == type_int) {
        node->value = std::get<int>(scanner->getData());
        node->type = type_int;
    } else if (scanner->getType() == type_string) {
        node->value = std::get<std::string>(scanner->getData());
        node->type = type_string;
    } else {
        node->value = std::get<bool>(scanner->getData());
        node->type = type_bool;
    }
    match(literal);
    return node;
}

id_node* createId() {
    id_node* node = new id_node();
    node->location = scanner->getLocation();
    node->value = std::get<std::string>(scanner->getData());
    match(id);
    return node;
}

multi_node* multiVal() {
    multi_node* node = new multi_node();
    node->location = scanner->getLocation();
    node->op = scanner->getOperator();
    match(multi);
    return node;
}

add_node* addVal() {
    add_node* node = new add_node();
    node->location = scanner->getLocation();
    node->op = scanner->getOperator();
    match(add);
    return node;
}

factor_node* factor() {
    factor_node* node = new factor_node();
    node->location = scanner->getLocation();
    switch (current) {
        case id:
            node->id = createId();
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
            delete node;
            return nullptr;
    }
}

factor_tail_node* factorTail() {
    factor_tail_node* node = new factor_tail_node();
    node->location = scanner->getLocation();
    switch (current) {
        case multi:
            node->multi = multiVal();
            node->factor = factor();
            node->factorTail = factorTail();
            return node;
        case endline:
        case endfile:
        default:
            delete node;
            return nullptr;
    }
}

term_node* term() {
    term_node* node = new term_node();
    node->location = scanner->getLocation();
    switch (current) {
        case endfile:
            delete node;
            return nullptr;
        default:
            node->factor = factor();
            node->factorTail = factorTail();
            return node;
    }
}

term_tail_node* termTail() {
    term_tail_node* node = new term_tail_node();
    node->location = scanner->getLocation();
    switch (current) {
        case add:
            node->add = addVal();
            node->term = term();
            node->termTail = termTail();
            return node;
        case endline:
        case endfile:
        default:
            delete node;
            return nullptr;
    }
}

expression_node* expression() {
    expression_node* node = new expression_node();
    node->negative = false;
    node->location = scanner->getLocation();
    switch (current) {
        case endfile:
            delete node;
            return nullptr;
        case negation:
            match(negation);
            node->negative = true;
        default:
            node->term = term();
            node->termTail = termTail();
            return node;
    }
}

print_node* printVal() {
    print_node* node = new print_node();
    node->location = scanner->getLocation();
    switch (current) {
        case print:
            match(print);
            node->expression = expression();
            return node;
        default:
            delete node;
            return nullptr;
    }
}

read_node* readVal() {
    read_node* node = new read_node();
    node->location = scanner->getLocation();
    switch (current) {
        case read:
            match(read);
            node->id = createId();
            return node;
        default:
            delete node;
            return nullptr;
    }
}

assign_node* assignValue(id_node* idNode) {
    assign_node* node = new assign_node();
    node->location = scanner->getLocation();
    node->id = idNode;
    switch (current) {
        case assign:
            match(assign);
            node->expression = expression();
            return node;
        case endline:
            delete node;
            return nullptr;
        default:
            printError();
            return nullptr;
    }
}

declare_node* declareVar() {
    declare_node* node = new declare_node();
    node->location = scanner->getLocation();
    switch (current) {
        case declare_var: {
            match(declare_var);
            id_node* idNode = createId();
            node->id = idNode;
            match(declare_type);
            node->type = scanner->getType();
            match(type);
            node->assignement = assignValue(idNode);
            return node;
        }
        default:
            printError();
            delete node;
            return nullptr;
    }
}

if_node* createIf() {
    if_node* node = new if_node();
    node->location = scanner->getLocation();
    switch (current) {
        case if_stmt:
            match(if_stmt);
            node->expression = expression();
            match(do_statement);
            node->statementList = statementList();
            match(end_statement);
            match(if_stmt);
            return node;
        default:
            printError();
            delete node;
            return nullptr;
    }
}

for_node* createFor() {
    for_node* node = new for_node();
    node->location = scanner->getLocation();
    switch (current) {
        case for_stmt:
            match(for_stmt);
            node->id = createId();
            match(in_statement);
            node->startExpression = expression();
            match(dotdot);
            node->endExpression = expression();
            match(do_statement);
            node->statementList = statementList();
            match(end_statement);
            match(for_stmt);
            return node;
        default:
            printError();
            delete node;
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
            id_node* idNode = createId();
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
        case if_stmt:
            node->ifStatement = createIf();
            match(endline);
            return node;
        case for_stmt:
            node->forStatement = createFor();
            match(endline);
            return node;
        case endfile:
            match(endfile);
            delete node;
            return nullptr;
        default:
            printError();
            delete node;
            return node;
    }
}

statement_list_node* statementList() {
    statement_list_node* node = new statement_list_node();
    switch (current) {
        case end_statement:
        case endfile:
            delete node;
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
        std::cout << "Parsing error at " << locToStr(scanner->getLocation()) << ": unexpected token: " << tokenStringMappings[current] << ". Expected: " << tokenStringMappings[expected] << std::endl;
        current = endfile;
        *error = true;
        return;
    } else if (expected == endfile) {
        return;
    }
    current = scanner->nextToken();
    std::cout << "Current token: " << tokenStringMappings[current] << std::endl;
}

void printError() {
    std::cout << "Parsing error at " << locToStr(scanner->getLocation())  << ": unexpected token: " << tokenStringMappings[current] << std::endl;
    current = endfile;
    *error = true;
}

program_node* parser(Scanner* s, bool* e) {
    scanner = s;
    error = e;
    current = scanner->nextToken();
    std::cout << "Current token: " << tokenStringMappings[current] << std::endl;
    return program();
}
