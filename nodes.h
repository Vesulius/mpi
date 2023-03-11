#pragma once

#include <variant>

struct expression_node;
struct statement_list_node;

struct id_node {
    std::string value;
    std::pair<int, int> location;
};

struct literal_node {
    Type type;
    std::variant<std::string, int, bool> value;
    std::pair<int, int> location;
};

struct add_node {
    Operator op;
    std::pair<int, int> location;
};

struct multi_node {
    Operator op;
    std::pair<int, int> location;
};

struct factor_node {
    literal_node* literal;
    id_node* id;
    expression_node* expression;
    std::pair<int, int> location;
};

struct factor_tail_node {
    multi_node* multi;
    factor_node* factor;
    factor_tail_node* factorTail;
    std::pair<int, int> location;
};

struct term_node {
    factor_node* factor;
    factor_tail_node* factorTail;
    std::pair<int, int> location;
};

struct term_tail_node {
    add_node* add;
    term_node* term;
    term_tail_node* termTail;
    std::pair<int, int> location;
};

struct expression_node {
    term_node* term;
    term_tail_node* termTail;
    std::pair<int, int> location;
};

struct assign_node {
    expression_node* expression;
    id_node* id;
    std::pair<int, int> location;
};

struct declare_node {
    id_node* id;
    Type type;
    assign_node* assignement;
    std::pair<int, int> location;
};

struct print_node {
    expression_node* expression;
    std::pair<int, int> location;
};

struct read_node {
    id_node* id;
    std::pair<int, int> location;
};

struct if_node {
    expression_node* expression;
    statement_list_node* statementList;
    std::pair<int, int> location;
};

struct for_node {
    id_node* id;
    expression_node* startExpression;
    expression_node* endExpression;
    statement_list_node* statementList;
    std::pair<int, int> location;
};

struct statement_node {
    declare_node* declare;
    assign_node* assignment;
    print_node* print;
    read_node* read;
    if_node* ifStatement;
    for_node* forStatement;
};

struct statement_list_node {
    statement_node* statement;
    statement_list_node* statementList;
};

struct program_node {
    statement_list_node* statementList;
};
