#include <iostream>

#include "types.h"
#include "nodes.h"
#include "general.h"

#define valuePair std::pair<Type, std::variant<std::string, int, bool>>

valuePair operate(Operator op, valuePair vp1, valuePair vp2) {
    if (vp1.first != vp2.first) {
        std::cout << "Runime error: cannot operate on differend types" << std::endl;
        return vp1;
    }
    if (vp1.first == type_int) {
        switch (op) {
            case sum:
                return {type_int, std::get<int>(vp1.second) + std::get<int>(vp2.second)};
            case subraction:
                return {type_int, std::get<int>(vp1.second) - std::get<int>(vp2.second)};
            case multiplication:
                return {type_int, std::get<int>(vp1.second) * std::get<int>(vp2.second)};
            case division:
                return {type_int, std::get<int>(vp1.second) / std::get<int>(vp2.second)};
        }
    }
    if (vp1.first == type_string && op == multiplication) {
        return {type_string, std::get<std::string>(vp1.second) + std::get<std::string>(vp2.second)};
    } else {
        std::cout << "Runime error: string type cannot be operated with: " << operatorStringMappings[op] << std::endl;
        return vp1;
    }
}

valuePair runnerVisitor(expression_node*);

valuePair runnerVisitor(literal_node* n) {
    return {n->type, n->value};
}

valuePair runnerVisitor(factor_node* n) {
    if (n->id != nullptr) {
        return symbolTable[n->id->value];
    } else if (n->literal != nullptr) {
        return runnerVisitor(n->literal);
    } else {
        return runnerVisitor(n->expression);
    }
}

valuePair runnerVisitor(factor_tail_node* n) {
    if (n->factorTail == nullptr) {
        return runnerVisitor(n->factor);
    } else {
        return operate(n->multi->op, runnerVisitor(n->factor), runnerVisitor(n->factorTail));
    }
}

valuePair runnerVisitor(term_node* n) {
    if (n->factorTail == nullptr) {
        return runnerVisitor(n->factor);
    } else {
        return operate(n->factorTail->multi->op, runnerVisitor(n->factor), runnerVisitor(n->factorTail));
    }
}

valuePair runnerVisitor(term_tail_node* n) {
    if (n->termTail == nullptr) {
        return runnerVisitor(n->term);
    } else {
        return operate(n->add->op, runnerVisitor(n->term), runnerVisitor(n->termTail));
    }
}

valuePair runnerVisitor(expression_node* n) {
    if (n->termTail == nullptr) {
        return runnerVisitor(n->term);
    } else {
        return operate(n->termTail->add->op, runnerVisitor(n->term), runnerVisitor(n->termTail));
    }
}

void runnerVisitor(print_node* n) {
    valuePair pair = runnerVisitor(n->expression);
    if (pair.first == type_bool) {
        std::string boolString = std::get<bool>(pair.second) ? "true" : "false";
        std::cout << boolString << std::endl;
    } else if (pair.first == type_string) {
        std::cout << std::get<std::string>(pair.second) << std::endl;
    } else {
        std::cout << std::get<int>(pair.second) << std::endl;
    }
}

void runnerVisitor(read_node* n) {
    // if (n == nullptr) return;
    // runnerVisitor(n->id);
}

void runnerVisitor(assign_node* n) {
    symbolTable[n->id->value] = runnerVisitor(n->expression);
}

void runnerVisitor(declare_node* n) {
    if (n->assignement != nullptr) {
        runnerVisitor(n->assignement);
    }
    // if (n == nullptr) return;
    // runnerVisitor(n->id);
    // runnerVisitor(n->assignement);
}

void runnerVisitor(statement_node* n) {
    if (n->print != nullptr) {
        runnerVisitor(n->print);
    } else if (n->assignment != nullptr) {
        runnerVisitor(n->assignment);
    } else if (n->declare != nullptr) {
        runnerVisitor(n->declare);
    } else if (n->read != nullptr) {
        runnerVisitor(n->read);
    } 
}

void runnerVisitor(statement_list_node* n) {
    runnerVisitor(n->statement);
    if (n->statementList != nullptr) {
        runnerVisitor(n->statementList);
    } 
}

void runnerVisitor(program_node* n) {
    runnerVisitor(n->statementList);
}
