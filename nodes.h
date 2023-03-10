#pragma once

#include <variant>

struct expression_node;

struct id_node {
    Type type;
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
    expression_node* expression;
    id_node* id;
};

struct declare_node {
    id_node* id;
    assign_node* assignement;
};

struct print_node {
    expression_node* expression;
};

struct read_node {
    id_node* id;
};

struct statement_node {
    declare_node* declare;
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
