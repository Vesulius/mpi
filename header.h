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

struct id_node {
    std::string value;
};

struct literal_node {
    std::string value;
};

struct add_node {
    std::string op;
};

struct multi_node {
    std::string op;
};

struct factor_node {
    literal_node* literal;
    id_node* id;
};

struct factor_tail_node {
    multi_node* multi;
    factor_node* factor;
    factor_tail_node* factorTail;
};

struct term_node {
    factor_node* factor;
    factor_tail_node* factorTail;
};

struct term_tail_node {
    add_node* add;
    term_node* term;
    term_tail_node* termTail;
};

struct expression_node {
    term_node* term;
    term_tail_node* termTail;
};

struct assign_node {
    std::string id;
    expression_node* expression;
};

struct statement_node {
    id_node* id;
    assign_node* assignment;
};

struct statement_list_node {
    statement_node* statement;
    statement_list_node* statementList;
};

struct program_node {
    statement_list_node* statementList;
};

void printVisitor(program_node*);
void scanner(std::string, std::vector<std::pair<Token, std::string>>&);
program_node* parser(std::vector<std::pair<Token, std::string>>&);
