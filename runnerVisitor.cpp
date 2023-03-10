#include <iostream>

#include "types.h"
#include "nodes.h"
#include "general.h"

#define valuePair std::pair<Type, std::variant<std::string, int, bool>>

std::map<std::string, valuePair> symbolTable;

void runnerVisitor(statement_list_node*);

void killProgram() {
    std::exit(0);
}

valuePair operate(Operator op, valuePair vp1, valuePair vp2) {
    if (vp1.first != vp2.first) {
        std::cout << "Runime error: cannot operate on differend types" << std::endl;
        killProgram();
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
    if (vp1.first == type_string && op == sum) {
        return {type_string, std::get<std::string>(vp1.second) + std::get<std::string>(vp2.second)};
    } else {
        std::cout << "Runtime error: string type cannot be operated with: " << operatorStringMappings[op] << std::endl;
        return vp1;
    }
}

valuePair runnerVisitor(expression_node*);

valuePair runnerVisitor(literal_node* n) {
    return {n->type, n->value};
}

valuePair getVarValue(id_node* n) {
    std::map<std::string, std::pair<Type, std::variant<std::string, int, bool>>>::iterator it = symbolTable.find(n->value);
    if (it == symbolTable.end()) {
        std::cout << "Runtime error: cannot get uninitialized variable: " << n->value << std::endl;
        killProgram();
        return {type_bool, false};
    } else {
        return it->second;
    }
}

void setVarValue(id_node* n, valuePair newPair) {
    std::map<std::string, std::pair<Type, std::variant<std::string, int, bool>>>::iterator it = symbolTable.find(n->value);
    if (it == symbolTable.end()) {
        std::cout << "Runtime error: cannot assing value to uninitialized variable: " << n->value << std::endl;
        killProgram();
    } else if (it->second.first != newPair.first) {
        std::cout << "Runtime error: cannot assing " << typeStringMappings[newPair.first] << " value to " << typeStringMappings[it->second.first] << " variable" << std::endl;
        killProgram();
    } else {
        it->second = newPair;
    }
}

valuePair runnerVisitor(factor_node* n) {
    if (n->id != nullptr) {
        return getVarValue(n->id);
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
    std::string input;
    std::cin >> input;
    if (n->id->type == type_int) {
        std::string::const_iterator it = input.begin();
        while (it != input.end() && std::isdigit(*it)) it++;
        if (it == input.end()) {
            setVarValue(n->id, {type_int, std::stoi(input)});
        } else {
            setVarValue(n->id, {type_string, input});
        }
    } else if (n->id->type == type_string) {
        setVarValue(n->id, {type_string, input});
    } else {
        if (input == "false" || input == "true") {
            setVarValue(n->id, {type_bool, input});
        } else {
            setVarValue(n->id, {type_string, input});
        }
    }
}

void runnerVisitor(assign_node* n) {
    setVarValue(n->id, runnerVisitor(n->expression));
}

void runnerVisitor(declare_node* n) {
    if (symbolTable.count(n->id->value)) {
        std::cout << "Runtime error: var " << n->id->value << " has alredy been declared" << std::endl;
        killProgram();
    } else {
        if (n->id->type == type_string) {
            symbolTable[n->id->value] = {type_string, ""};
        } else if (n->id->type == type_int) {
            symbolTable[n->id->value] = {type_int, 0};
        } else {
            symbolTable[n->id->value] = {type_bool, true};
        }
    }

    if (n->assignement != nullptr) {
        runnerVisitor(n->assignement);
    }
}

void runIf(if_node* n) {
    valuePair vp = runnerVisitor(n->expression);
    if (vp.first != type_bool) {
        std::cout << "Runtime error: if statements can only assess bool types" << std::endl;
        killProgram();
    } else {
        if (std::get<bool>(vp.second)) {
            runnerVisitor(n->statementList);
        }
    }
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
    } else if (n->ifStatement != nullptr) {
        runIf(n->ifStatement);
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