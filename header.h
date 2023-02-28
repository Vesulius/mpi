#include <string>
#include <vector>

enum Token { endfile,
             endline,
             add,
             multi,
             lbracet,
             rbracet,
             declare_type,
             assign,
             literal,
             id,
             declare_var,
             if_stmt,
             while_stmt,
             type,
             read,
             print };

const std::string tokenStringMappings[19] = {"endfile",
                                             "endline",
                                             "add",
                                             "multi",
                                             "lbracet",
                                             "rbracet",
                                             "declare_type",
                                             "assign",
                                             "literal",
                                             "id",
                                             "declare_var",
                                             "if_stmt",
                                             "while_stmt",
                                             "type",
                                             "read",
                                             "print"};

void scanner(std::string, std::vector<std::pair<Token, std::string>>&);
void parser(std::vector<std::pair<Token, std::string>>&);
