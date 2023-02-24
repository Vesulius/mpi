#include <fstream>
#include <iostream>
#include <vector>

#include "header.h"

std::vector<std::pair<Token, std::string>> tokens2;
Token current;
int i = 0;

void match(Token expected);
void program();
void factor();
void nextToken();
void expression();
void printError();

void program() {
    if (current != endfile) {
        expression();
    }
    match(endfile);
}

void expression() {
    factor();
    switch (current) {
        case subtr:
            match(subtr);
            expression();
            break;
        case sum:
            match(sum);
            expression();
            break;
        case endline:
            match(endline);
            break;
        default:
            printError();
            break;
    }
}

void factor() {
    switch (current) {
        case id:
            match(id);
            break;
        case number_val:
            match(number_val);
            break;
        default:
            printError();
            break;
    }
}

void match2(Token expected1, Token expected2) {
    if (current != expected1 || current != expected2) {
        std::cout << "Parsing error: unexpected token: "
                  << tokenStringMappings[current] << ". Expected: "
                  << tokenStringMappings[expected1] << " or "
                  << tokenStringMappings[expected2] << std::endl;
    }
    nextToken();
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
