#include <fstream>
#include <iostream>
#include <variant>
#include <vector>

#include "types.h"
#include "nodes.h"
#include "general.h"

Scanner::Scanner(std::string sourceFilePath, bool* e) {
    error = e;
    sourceFile.open(sourceFilePath);
    if (!sourceFile.is_open()) {
        std::cout << "IO error: unable to open source file " << sourceFilePath << std::endl;
    }
    row = 1;
    column = 1;
}

Scanner::~Scanner() {
    sourceFile.close();
}

Token Scanner::nextToken() {
    while (sourceFile.is_open()) {
        c = sourceFile.get();
        column++;
        switch (c) {
            case EOF:
                return endfile;
            case ';':
                return endline;
            case '+':
                opVal = sum;
                return add;
            case '-':
                opVal = subraction;
                return add;
            case '*':
                opVal = multiplication;
                return multi;
            case '(':
                return lbracet;
            case ')':
                return rbracet;
            case '=':
                opVal = equals;
                return add;
            case '&':
                opVal = conjunction;
                return add;
            case '<':
                opVal = less;
                return add;
            case '>':
                opVal = greater;
                return add;
            case '!':
                return negation;
            case '.':
                if (sourceFile.peek() == '.') {
                    sourceFile.get();
                    return dotdot;
                } else {
                    std::cout << "Syntax error at" << locToStr(getLocation()) << ": keyword .. is missing second dot" << std::endl;
                    *error = true;
                    break;
                }
            case ':':
                c = sourceFile.get();
                if (c == '=') {
                    return assign;
                } else {
                    sourceFile.unget();
                    return declare_type;
                }
            case '\n':
                column = 1;
                row++;
                break;
        }
        if (c == '/') {
            c = sourceFile.get();
            switch (c) {
                case '/':
                    // single comment, skip rest of line
                    while (c != '\n' && c != EOF) {
                        c = sourceFile.get();
                    }
                    if (c == EOF) {
                        return endfile;
                    }
                    sourceFile.unget();
                    column = 0;
                    row++;
                    break;
                case '*': {
                    // multiline comment, skip until "*/"
                    int commentDepth = 1;
                    while (commentDepth > 0) {
                        c = sourceFile.get();
                        if (c == '*' && sourceFile.peek() == '/') {
                            commentDepth--;
                            sourceFile.get();
                        } else if (c == '/' && sourceFile.peek() == '*') {
                            commentDepth++;
                            sourceFile.get();
                        } else if (c == EOF) {
                            std::cout << "Syntax error at" << locToStr(getLocation()) << ": multiline comment missing closing tag" << std::endl;
                            *error = true;
                            break;
                        }
                    }
                    break;
                }
                default:
                    sourceFile.unget();
                    opVal = division;
                    return multi;
            }
        }
        if (c == '\"') {
            std::string stringBuild = "";
            c = sourceFile.get();
            while (c != '\"' && c != EOF) {
                stringBuild += c;
                c = sourceFile.get();
            }
            if (c == EOF) {
                std::cout << "Syntax error at" << locToStr(getLocation()) << ": string missing closing quotation mark" << std::endl;
                *error = true;
            } else {
                data = stringBuild;
                typeVal = type_string;
                return literal;
            }
        }
        if (std::isdigit(c)) {
            std::string numberBuild = "";
            while (std::isdigit(c)) {
                // number, DOES NOT RECOGNIZE FLOATS
                numberBuild += c;
                c = sourceFile.get();
            }
            data = std::stoi(numberBuild);
            typeVal = type_int;
            sourceFile.unget();
            return literal;
        }
        if (std::isalpha(c)) {
            std::string varBuild;
            while (std::isalpha(c)) {
                varBuild += c;
                if (varBuild == "var") {
                    return declare_var;
                }
                if (varBuild == "if") {
                    return if_stmt;
                }
                if (varBuild == "for") {
                    return for_stmt;
                }
                if (varBuild == "in") {
                    if (sourceFile.peek() == 't') {
                        sourceFile.get();
                        typeVal = type_int;
                        return type;
                    } else {
                        return in_statement;
                    }
                }
                if (varBuild == "string") {
                    typeVal = type_string;
                    return type;
                }
                if (varBuild == "bool") {
                    typeVal = type_bool;
                    return type;
                }
                if (varBuild == "read") {
                    return read;
                }
                if (varBuild == "print") {
                    return print;
                }
                if (varBuild == "end") {
                    return end_statement;
                }
                if (varBuild == "true") {
                    data = true;
                    typeVal = type_bool;
                    return literal;
                }
                if (varBuild == "false") {
                    data = false;
                    typeVal = type_bool;
                    return literal;
                }
                if (varBuild == "do") {
                    return do_statement;
                }
                c = sourceFile.get();
            }
            if (varBuild != "") {
                data = varBuild;
                sourceFile.unget();
                return id;
            }
        }
    }
    std::cout << "Syntax error at " << locToStr(getLocation()) << ": bad token match" << std::endl;
    *error = true;
    return endfile;
}

std::pair<int, int> Scanner::getLocation() {
    return {column, row};
}

std::variant<std::string, int, bool> Scanner::getData() {
    return data;
};

Operator Scanner::getOperator() {
    return opVal;
};

Type Scanner::getType() {
    return typeVal;
};
