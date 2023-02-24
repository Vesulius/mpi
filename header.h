#include <string>
#include <vector>

enum Token { endfile,
             endline,
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
             id,
             declare_var,
             if_stmt,
             while_stmt,
             int_type,
             string_type,
             read,
             print };

const std::string tokenStringMappings[20] = {"endfile",
                                             "endline",
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
                                             "id",
                                             "declare_var",
                                             "if_stmt",
                                             "while_stmt",
                                             "int_type",
                                             "string_type",
                                             "read",
                                             "print"};

void scanner(std::string, std::vector<std::pair<Token, std::string>>&);
void parser(std::vector<std::pair<Token, std::string>>&);
