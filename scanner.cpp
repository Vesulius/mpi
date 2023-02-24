#include <fstream>
#include <iostream>
#include <vector>

#include "header.h"

void scanner(std::string sourceFilePath, std::vector<std::pair<Token, std::string>> &tokens) {
    std::ifstream sourceFile(sourceFilePath);
    char c;
    bool charsLeft = true;
    if (sourceFile.is_open()) {
        while (charsLeft) {
            c = sourceFile.get();
            switch (c) {
                case EOF:
                    tokens.push_back({endfile, ""});
                    charsLeft = false;
                    break;
                case ';':
                    tokens.push_back({endline, ""});
                    break;
                case '+':
                    tokens.push_back({sum, ""});
                    break;
                case '-':
                    tokens.push_back({subtr, ""});
                    break;
                case '*':
                    tokens.push_back({multi, ""});
                    break;
                case '(':
                    tokens.push_back({lbracet, ""});
                    break;
                case ')':
                    tokens.push_back({rbracet, ""});
                    break;
                case ':':
                    c = sourceFile.get();
                    if (c == '=') {
                        tokens.push_back({assign, ""});
                    } else {
                        tokens.push_back({declare_type, ""});
                        sourceFile.unget();
                    }
                    break;
            }
            if (c == '/') {
                c = sourceFile.get();
                switch (c) {
                    case '/':
                        // single comment, skip rest of line
                        do {
                            c = sourceFile.get();
                            break;
                        } while (c != '\n' || c != EOF);
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
                        tokens.push_back({division, ""});
                        break;
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
                    std::cout << "Syntax error: string missing closing quotation mark" << std::endl;
                } else {
                    tokens.push_back({string_val, stringBuild});
                }
            }
            if (std::isdigit(c)) {
                std::string numberBuild = "";
                while (std::isdigit(c)) {
                    // number, DOES NOT RECOGNIZE FLOATS
                    numberBuild += c;
                    c = sourceFile.get();
                }
                tokens.push_back({number_val, numberBuild});
                sourceFile.unget();
            }
            if (std::isalpha(c)) {
                std::string varBuild = "";
                while (std::isalpha(c)) {
                    varBuild += c;
                    c = sourceFile.get();
                    if (varBuild == "var") {
                        tokens.push_back({declare_var, ""});
                        varBuild = "";
                    }
                    if (varBuild == "if") {
                        tokens.push_back({if_stmt, ""});
                        varBuild = "";
                    }
                    if (varBuild == "int") {
                        tokens.push_back({type, "integer"});
                        varBuild = "";
                    }
                    if (varBuild == "string") {
                        tokens.push_back({type, "string"});
                        varBuild = "";
                    }
                    if (varBuild == "read") {
                        tokens.push_back({read, ""});
                        varBuild = "";
                    }
                    if (varBuild == "print") {
                        tokens.push_back({print, ""});
                        varBuild = "";
                    }
                }
                if (varBuild != "") {
                    tokens.push_back({id, varBuild});
                }
                sourceFile.unget();
            }
        }
        sourceFile.close();
    } else {
        std::cout << "Error: Unable to open source file";
    }
}
