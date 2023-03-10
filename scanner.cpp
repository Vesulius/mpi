#include <fstream>
#include <iostream>
#include <variant>
#include <vector>

#include "types.h"
#include "nodes.h"
#include "general.h"

Scanner::Scanner(std::string sourceFilePath) {
    sourceFile.open(sourceFilePath);
    if (!sourceFile.is_open()) {
        std::cout << "IO error: unable to open source file " << sourceFilePath << std::endl;
    }
    row = 0;
    column = 0;
}

Token Scanner::nextToken() {
    while (sourceFile.is_open()) {
        c = sourceFile.get();
        column++;
        switch (c) {
            case EOF:
                sourceFile.close();
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
            case ':':
                c = sourceFile.get();
                if (c == '=') {
                    return assign;
                } else {
                    sourceFile.unget();
                    return declare_type;
                }
            case '\n':
                column = 0;
                row++;
                break;
        }
        if (c == '/') {
            c = sourceFile.get();
            switch (c) {
                case '/':
                    // single comment, skip rest of line
                    do {
                        c = sourceFile.get();
                    } while (c != '\n' || c != EOF);
                    column = 0;
                    row++;
                    break;
                case '*':
                    // multiline comment, skip until "*/"
                    // DOES NOT WORK WITH NESTED MULTILINE COMMENTS
                    while (true) {
                        c = sourceFile.get();
                        if (c == '*') {
                            c = sourceFile.get();
                            if (c == '/') {
                                break;
                            } else {
                                sourceFile.unget();
                            }
                        }
                    }
                    break;
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
                std::cout << "Syntax error at" << getLocation() << ": string missing closing quotation mark" << std::endl;
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
            std::string varBuild = "";
            while (std::isalpha(c)) {
                varBuild += c;
                c = sourceFile.get();
                if (varBuild == "var") {
                    sourceFile.unget();
                    return declare_var;
                }
                if (varBuild == "if") {
                    sourceFile.unget();
                    return if_stmt;
                }
                if (varBuild == "int") {
                    sourceFile.unget();
                    typeVal = type_int;
                    return type;
                }
                if (varBuild == "string") {
                    sourceFile.unget();
                    typeVal = type_string;
                    return type;
                }
                if (varBuild == "bool") {
                    sourceFile.unget();
                    typeVal = type_bool;
                    return type;
                }
                if (varBuild == "read") {
                    sourceFile.unget();
                    return read;
                }
                if (varBuild == "print") {
                    sourceFile.unget();
                    return print;
                }
                if (varBuild == "end") {
                    sourceFile.unget();
                    return end_statement;
                }
                if (varBuild == "true") {
                    sourceFile.unget();
                    data = true;
                    typeVal = type_bool;
                    return literal;
                }
                if (varBuild == "false") {
                    sourceFile.unget();
                    data = false;
                    typeVal = type_bool;
                    return literal;
                }
                if (varBuild == "do") {
                    sourceFile.unget();
                    return do_statement;
                }
            }
            if (varBuild != "") {
                data = varBuild;
                sourceFile.unget();
                return id;
            }
        }
    }
    std::cout << "Syntax error at" << getLocation() << ": bad token match" << std::endl;
    return endfile;
}

std::string Scanner::getLocation() {
    std::string s = "column: ";
    s += std::to_string(column);
    s += " row: ";
    s += std::to_string(row);
    return s;
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
