#include <fstream>
#include <iostream>
#include <variant>
#include <vector>

#include "header.h"

Scanner::Scanner(std::string sourceFilePath) {
    sourceFile.open(sourceFilePath);
    if (!sourceFile.is_open()) {
        std::cout << "Error: Unable to open source file " << sourceFilePath << std::endl;
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
                data = sum;
                return add;
            case '-':
                data = subraction;
                return add;
            case '*':
                data = multiplication;
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
                    data = division;
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
            sourceFile.unget();
            return literal;
        }
        if (std::isalpha(c)) {
            std::string varBuild = "";
            while (std::isalpha(c)) {
                varBuild += c;
                c = sourceFile.get();
                if (varBuild == "var") {
                    return declare_var;
                }
                if (varBuild == "if") {
                    return if_stmt;
                }
                if (varBuild == "int") {
                    data = type_int;
                    return type;
                }
                if (varBuild == "string") {
                    data = type_string;
                    return type;
                }
                if (varBuild == "read") {
                    return read;
                }
                if (varBuild == "print") {
                    return print;
                }
            }
            if (varBuild != "") {
                data = varBuild;
                return id;
            }
            sourceFile.unget();
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

std::variant<std::string, int, Type, Operator> Scanner::getData() {
    return data;
};
