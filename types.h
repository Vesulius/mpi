#pragma once

#include <string>

enum Token { endfile,
             endline,
             add,
             multi,
             negation,
             lbracet,
             rbracet,
             declare_type,
             assign,
             literal,
             id,
             declare_var,
             if_stmt,
             for_stmt,
             dotdot,
             in_statement,
             end_statement,
             do_statement,
             type,
             read,
             print };

const std::string tokenStringMappings[21] = {"endfile",
                                             "endline",
                                             "add",
                                             "multi",
                                             "!",
                                             "lbracet",
                                             "rbracet",
                                             "declare type",
                                             "assign",
                                             "literal value",
                                             "id",
                                             "declare var",
                                             "if",
                                             "for",
                                             "..",
                                             "in",
                                             "end",
                                             "do",
                                             "type",
                                             "read",
                                             "print"};

enum Type { type_string,
            type_int,
            type_bool
};

const std::string typeStringMappings[3] = {"string type",
                                           "int type",
                                           "bool type"};

enum Operator { sum,
                subraction,
                multiplication,
                division,
                equals,
                conjunction,
                less,
                greater
};

const std::string operatorStringMappings[9] = {"sum",
                                               "subraction",
                                               "multiplication",
                                               "division",
                                               "=",
                                               "&",
                                               "<",
                                               ">"};
