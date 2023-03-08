#include <iostream>

#include "header.h"

std::map<std::string, std::pair<Type, std::variant<std::string, int, bool>>> symbolTable;

enum allTypes {
    check_string,
    check_int,
    check_bool,
    none
};

std::string allTypesStringMappings[4] = {
    "string type",
    "int type",
    "bool type",
    "none"};

allTypes checkType(allTypes t1, allTypes t2) {
    if (t1 == none && t2 != none) {
        return t2;
    } else if (t1 != none && t2 == none) {
        return t1;
    } else if (t1 == none && t2 == none) {
        return none;
    }
    if (t1 != t2) {
        std::cout << "Error: wrong type: expected " << allTypesStringMappings[t1] << " got " << allTypesStringMappings[t2] << std::endl;
        return none;
    }
    return t1;
}

allTypes typeVisitor(expression_node*);
void printVisitor(declare_node*);

allTypes typeVisitor(literal_node* n) {
    return (allTypes)n->type;
}

Operator typeVisitor(multi_node* n) {
    return n->op;
}

Operator typeVisitor(add_node* n) {
    return n->op;
}

std::string typeVisitor(id_node* n) {
    return n->value;
}

allTypes typeVisitor(factor_node* n) {
    if (n == nullptr) return none;
    if (n->expression != nullptr) {
        return typeVisitor(n->expression);
    } else if (n->literal != nullptr) {
        return typeVisitor(n->literal);
    }
    return none;
}

allTypes typeVisitor(factor_tail_node* n) {
    if (n == nullptr) return none;
    if (n->multi != nullptr) {
        checkType(check_int, typeVisitor(n->factor));
        checkType(check_int, typeVisitor(n->factorTail));
        return check_int;
    }
    return none;
}

allTypes typeVisitor(term_node* n) {
    if (n == nullptr) return none;
    allTypes t1 = typeVisitor(n->factor);
    allTypes t2 = typeVisitor(n->factorTail);
    return checkType(t1, t2);
}

allTypes typeVisitor(term_tail_node* n) {
    if (n == nullptr) return none;
    if (n->add != nullptr) {
        checkType(check_int, typeVisitor(n->term));
        checkType(check_int, typeVisitor(n->termTail));
        return check_int;
    }
    return none;
}

allTypes typeVisitor(expression_node* n) {
    if (n == nullptr) return none;
    allTypes t1 = typeVisitor(n->term);
    allTypes t2 = typeVisitor(n->termTail);
    return checkType(t1, t2);
}

void typeVisitor(print_node* n) {
    if (n == nullptr) return;
    typeVisitor(n->expression);
}

void typeVisitor(read_node* n) {
    if (n == nullptr) return;
    typeVisitor(n->id);
}

allTypes typeVisitor(assign_node* n) {
    if (n == nullptr) return none;
    allTypes t = typeVisitor(n->expression);
    std::map<std::string, std::pair<Type, std::variant<std::string, int, bool>>>::iterator it = symbolTable.find(n->id->value);

    if (it == symbolTable.end()) {
        std::cout << "Error: uninitialized variable: " << n->id->value << std::endl;
    }
    return checkType(t, (allTypes)it->second.first);
}

void typeVisitor(declare_node* n) {
    if (n == nullptr) return;
    if (n->type == type_string) {
        symbolTable[n->id->value] = {type_string, ""};
    } else if (n->type == type_int) {
        symbolTable[n->id->value] = {type_int, 0};
    } else {
        symbolTable[n->id->value] = {type_bool, true};
    }
    checkType((allTypes)n->type, typeVisitor(n->assignement));
}

void typeVisitor(statement_node* n) {
    if (n == nullptr) return;
    typeVisitor(n->assignment);
    typeVisitor(n->declare);
    typeVisitor(n->print);
    typeVisitor(n->read);
}

void typeVisitor(statement_list_node* n) {
    if (n == nullptr) return;
    typeVisitor(n->statement);
    typeVisitor(n->statementList);
}

void typeVisitor(program_node* n) {
    if (n == nullptr) return;
    typeVisitor(n->statementList);
}
