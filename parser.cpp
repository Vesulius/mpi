#include <fstream>
#include <iostream>
#include <vector>

#include "header.h"

std::vector<std::pair<Token, std::string>> tokens2;
Token current;
int i = 0;

void match(Token expected);
void program();
void statementList();
void factor();
void nextToken();
void expression();
void printError();

// void statementList() {
//     switch (current) {
//         case id:
//         case number_val:

//             break;
//         case endfile:
//             return;
//         default:
//             printError();
//     }
// }

void assignValue() {
    switch (current) {
        case assign:
            match(assign);
            expression();
            break;
        case endline:
            break;
        default:
            printError();
            break;
    }
}

void statement() {
    switch (current) {
        case declare_var:
            match(declare_var);
            match(id);
            match(declare_type);
            match(type);
            assignValue();
            match(endline);
            break;
        case id:
            match(id);
            assignValue();
            match(endline);
            break;
        case endfile:
            match(endfile);
            break;
        default:
            printError();
            break;
    }
}

void program() {
    statement();
    match(endfile);
}

void expression() {
    switch (current) {
        case number_val:
            match(number_val);
            break;
        case string_val:
            match(string_val);
            break;
        default:
            printError();
            break;
    }
}

void match(Token expected) {
    if (current != expected) {
        std::cout << "Parsing error: unexpected token: " << tokenStringMappings[current] << ". Expected: " << tokenStringMappings[expected] << std::endl;
    } else if (expected == endfile) {
        std::cout << "IT WORKED!!" << std::endl;
        return;
    }
    nextToken();
}

void printError() {
    std::cout << "Parsing error: unexpected token: " << tokenStringMappings[current] << std::endl;
}

void nextToken() {
    current = tokens2.at(i).first;
    std::cout << "currenttoken: " << tokenStringMappings[current] << std::endl;
    i++;
}

void parser(std::vector<std::pair<Token, std::string>> &tokens) {
    tokens2 = tokens;
    std::cout << "Number of tokens " << tokens.size() << std::endl;
    nextToken();
    program();
}
