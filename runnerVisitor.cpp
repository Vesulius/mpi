#include <iostream>

#include "types.h"
#include "nodes.h"
#include "general.h"

#define ValuePair std::pair<Type, std::variant<std::string, int, bool>>
#define MaxScope 5

std::map<std::string, ValuePair> tables[MaxScope];
int scope = 0;

void runnerVisitor(statement_list_node*);

void killProgram() {
    std::exit(0);
}

ValuePair operate(Operator op, ValuePair vp1, ValuePair vp2, std::pair<int, int> location) {
    if (vp1.first != vp2.first) {
        std::cout << "Runime error at " << locToStr(location) << ": cannot operate on differend types" << std::endl;
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
            case equals:
                return {type_bool, std::get<int>(vp1.second) == std::get<int>(vp2.second)};
            case less:
                return {type_bool, std::get<int>(vp1.second) < std::get<int>(vp2.second)};
            case greater:
                return {type_bool, std::get<int>(vp1.second) > std::get<int>(vp2.second)};
            default:
                std::cout << "Runtime error at " << locToStr(location) << ": int type cannot be operated with: " << operatorStringMappings[op] << std::endl;
                killProgram();
        }
    } else if (vp1.first == type_string) {
        switch (op) {
            case sum:
                return {type_string, std::get<std::string>(vp1.second) + std::get<std::string>(vp2.second)};
            case equals:
                return {type_bool, std::get<std::string>(vp1.second) == std::get<std::string>(vp2.second)};
            default:
                std::cout << "Runtime error at " << locToStr(location) << ": string type cannot be operated with: " << operatorStringMappings[op] << std::endl;
                killProgram();
        }
    } else {
        switch (op) {
            case equals:
                return {type_bool, std::get<bool>(vp1.second) == std::get<bool>(vp2.second)};
            case conjunction:
                return {type_bool, std::get<bool>(vp1.second) && std::get<bool>(vp2.second)};
            default:
                std::cout << "Runtime error at " << locToStr(location) << ": bool type cannot be operated with: " << operatorStringMappings[op] << std::endl;
                killProgram();
        }
    }
    std::cout << "Runtime error at " << locToStr(location) << ": oparating error with: " << operatorStringMappings[op] << std::endl;
    killProgram();
    return {type_bool, false};
}

ValuePair runnerVisitor(expression_node*);

ValuePair runnerVisitor(literal_node* n) {
    return {n->type, n->value};
}

ValuePair getVarValue(id_node* n) {
    std::map<std::string, std::pair<Type, std::variant<std::string, int, bool>>>::iterator it;
    for (int i = scope; i >= 0; i--) {
        it = tables[i].find(n->value);
        if (it != tables[i].end()) {
            break;
        }
    }
    if (it == tables[0].end()) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": cannot get uninitialized variable: " << n->value << std::endl;
        killProgram();
        return {type_bool, false};
    } else {
        return it->second;
    }
}

void setVarValue(id_node* n, ValuePair newPair) {
    std::map<std::string, std::pair<Type, std::variant<std::string, int, bool>>>::iterator it;
    for (int i = scope; i >= 0; i--) {
        it = tables[i].find(n->value);
        if (it != tables[i].end()) {
            break;
        }
    }
    if (it == tables[0].end()) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": cannot assing value to uninitialized variable: " << n->value << std::endl;
        killProgram();
    } else if (it->second.first != newPair.first) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": cannot assing " << typeStringMappings[newPair.first] << " value to " << typeStringMappings[it->second.first] << " variable" << std::endl;
        killProgram();
    } else {
        it->second = newPair;
    }
}

ValuePair runnerVisitor(factor_node* n) {
    if (n->id != nullptr) {
        return getVarValue(n->id);
    } else if (n->literal != nullptr) {
        return runnerVisitor(n->literal);
    } else {
        return runnerVisitor(n->expression);
    }
}

ValuePair runnerVisitor(factor_tail_node* n) {
    if (n->factorTail == nullptr) {
        return runnerVisitor(n->factor);
    } else {
        return operate(n->multi->op, runnerVisitor(n->factor), runnerVisitor(n->factorTail), n->location);
    }
}

ValuePair runnerVisitor(term_node* n) {
    if (n->factorTail == nullptr) {
        return runnerVisitor(n->factor);
    } else {
        return operate(n->factorTail->multi->op, runnerVisitor(n->factor), runnerVisitor(n->factorTail), n->location);
    }
}

ValuePair runnerVisitor(term_tail_node* n) {
    if (n->termTail == nullptr) {
        return runnerVisitor(n->term);
    } else {
        return operate(n->add->op, runnerVisitor(n->term), runnerVisitor(n->termTail), n->location);
    }
}

ValuePair runnerVisitor(expression_node* n) {
    ValuePair vp;
    if (n->termTail == nullptr) {
        vp = runnerVisitor(n->term);
    } else {
        vp = operate(n->termTail->add->op, runnerVisitor(n->term), runnerVisitor(n->termTail), n->location);
    }
    if (n->negative) {
        if (vp.first == type_bool) {
            return {vp.first, !std::get<bool>(vp.second)};
        } else {
            std::cout << "Runtime error at " << locToStr(n->location) << ":  ! operator is only applicable to bool type values" << std::endl;
            killProgram();
        }
    }
    return vp;
}

void runPrint(print_node* n) {
    ValuePair pair = runnerVisitor(n->expression);
    if (pair.first == type_bool) {
        std::string boolString = std::get<bool>(pair.second) ? "true" : "false";
        std::cout << boolString << std::endl;
    } else if (pair.first == type_string) {
        std::cout << std::get<std::string>(pair.second) << std::endl;
    } else {
        std::cout << std::get<int>(pair.second) << std::endl;
    }
}

void runRead(read_node* n) {
    std::string input;
    std::cin >> input;
    if (getVarValue(n->id).first == type_int) {
        std::string::const_iterator it = input.begin();
        while (it != input.end() && std::isdigit(*it)) it++;
        if (it == input.end()) {
            setVarValue(n->id, {type_int, std::stoi(input)});
        } else {
            setVarValue(n->id, {type_string, input});
        }
    } else if (getVarValue(n->id).first == type_string) {
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
    if (tables[scope].count(n->id->value)) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": var " << n->id->value << " has alredy been declared" << std::endl;
        killProgram();
    } else {
        if (n->type == type_string) {
            tables[scope][n->id->value] = {type_string, ""};
        } else if (n->type == type_int) {
            tables[scope][n->id->value] = {type_int, 0};
        } else {
            tables[scope][n->id->value] = {type_bool, true};
        }
    }

    if (n->assignement != nullptr) {
        runnerVisitor(n->assignement);
    }
}

void runIf(if_node* n) {
    scope++;
    if (scope >= MaxScope) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": scope depth of " << MaxScope << " exceeded" << std::endl;
        killProgram();
    }
    ValuePair vp = runnerVisitor(n->expression);
    if (vp.first != type_bool) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": if statements can only assess bool types" << std::endl;
        killProgram();
    } else {
        if (std::get<bool>(vp.second) && n->statementList != nullptr) {
            runnerVisitor(n->statementList);
        }
        tables[scope].clear();
        scope--;
    }
}

void runFor(for_node* n) {
    if (getVarValue(n->id).first != type_int) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": for statement variable " << n->id->value << " must be int type" << std::endl;
        killProgram();
    }
    ValuePair start = runnerVisitor(n->startExpression);
    ValuePair end = runnerVisitor(n->endExpression);
    if (start.first != type_int) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": for statement start value must be int type" << std::endl;
        killProgram();
    }
    if (end.first != type_int) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": for statement end value must be int type" << std::endl;
        killProgram();
    }
    if (start.second > end.second) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": for statement start value must be greater than end value" << std::endl;
        killProgram();
    }
    setVarValue(n->id, start);
    int index = std::get<int>(start.second);
    scope++;
    if (scope >= MaxScope) {
        std::cout << "Runtime error at " << locToStr(n->location) << ": scope depth of " << MaxScope << " exceeded" << std::endl;
        killProgram();
    }
    if (n->statementList != nullptr) {
        while (index <= std::get<int>(end.second)) {
            runnerVisitor(n->statementList);
            setVarValue(n->id, {type_int, ++index});
            tables[scope].clear();
        }
    }
    tables[scope].clear();
    scope--;
}

void runnerVisitor(statement_node* n) {
    if (n->print != nullptr) {
        runPrint(n->print);
    } else if (n->assignment != nullptr) {
        runnerVisitor(n->assignment);
    } else if (n->declare != nullptr) {
        runnerVisitor(n->declare);
    } else if (n->read != nullptr) {
        runRead(n->read);
    } else if (n->ifStatement != nullptr) {
        runIf(n->ifStatement);
    } else if (n->forStatement != nullptr) {
        runFor(n->forStatement);
    }
}

void runnerVisitor(statement_list_node* n) {
    runnerVisitor(n->statement);
    if (n->statementList != nullptr) {
        runnerVisitor(n->statementList);
    }
}

void runnerVisitor(program_node* n) {
    if (n->statementList != nullptr) {
        runnerVisitor(n->statementList);
    }
}
