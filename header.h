#include <fstream>
#include <string>
#include <variant>
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

enum Type {
    type_string,
    type_int,
    type_bool
};

const std::string typeStringMappings[3] = {
    "type_string",
    "type_int",
    "type_bool"
};

enum Operator {
    sum,
    subraction,
    multiplication,
    division
};

const std::string operatorStringMappings[4] = {
    "sum",
    "subraction",
    "multiplication",
    "division"
};

struct expression_node;

struct id_node {
    std::string value;
};

struct literal_node {
    Type type;
    std::variant<std::string, int, bool> value;
};

struct add_node {
    Operator op;
};

struct multi_node {
    Operator op;
};

struct factor_node {
    literal_node* literal;
    id_node* id;
    expression_node* expression;
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

struct print_node {
    expression_node* expression;
};

struct read_node {
    id_node* id;
};

struct statement_node {
    id_node* id;
    assign_node* assignment;
    print_node* print;
    read_node* read;
};

struct statement_list_node {
    statement_node* statement;
    statement_list_node* statementList;
};

struct program_node {
    statement_list_node* statementList;
};

class Scanner {
    char c;
    std::ifstream sourceFile;
    int row;
    int column;
    std::variant<std::string, int, Type, Operator> data;
    Token current;

   public:
    Scanner(std::string);
    Token nextToken();
    std::string getLocation();
    std::variant<std::string, int, Type, Operator> getData();
};

void printVisitor(program_node*);
program_node* parser(Scanner*);
