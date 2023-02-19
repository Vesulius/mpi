#include <fstream>
#include <iostream>
#include <vector>

enum Token { endfile,
             sum,
             subtr,
             multi,
             division,
             lbracet,
             rbracet,
             declare_type,
             assign,
             number_val,
             string_val,
             var_val,
             declare_var,
             if_stmt,
             while_stmt,
             int_type,
             string_type,
             read,
             print };

std::string tokenStringsMappings[19] = {"endfile",
                                        "sum",
                                        "subtr",
                                        "multi",
                                        "division",
                                        "lbracet",
                                        "rbracet",
                                        "declare_type",
                                        "assign",
                                        "number_val",
                                        "string_val",
                                        "var_val",
                                        "declare_var",
                                        "if_stmt",
                                        "while_stmt",
                                        "int_type",
                                        "string_type",
                                        "read",
                                        "print"};

std::vector<std::pair<Token, std::string>> tokens;
std::string sourceFilePath;

void scanner() {
    std::ifstream sourceFile(sourceFilePath);
    char c;
    if (sourceFile.is_open()) {
        while (sourceFile.good()) {
            c = sourceFile.get();
            switch (c) {
                case EOF:
                    tokens.push_back({endfile, ""});
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
                break;
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
                        tokens.push_back({int_type, ""});
                        varBuild = "";
                    }
                    if (varBuild == "string") {
                        tokens.push_back({string_type, ""});
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
                    tokens.push_back({var_val, varBuild});
                }
                sourceFile.unget();
            }
        }
        sourceFile.close();
    } else {
        std::cout << "Unable to open file";
    }
    for (std::pair i : tokens)
        std::cout << tokenStringsMappings[i.first] << ' ';
}

int main(int argc, char** argv) {
    if (argc > 1) {
        sourceFilePath = argv[1];
    } else {
        sourceFilePath = "test.mp";
    }

    scanner();

    return 0;
}