#include <string>

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

enum Type { type_string,
            type_int,
            type_bool
};

const std::string typeStringMappings[3] = {"type_string",
                                           "type_int",
                                           "type_bool"};

enum Operator { sum,
                subraction,
                multiplication,
                division
};

const std::string operatorStringMappings[4] = {"sum",
                                               "subraction",
                                               "multiplication",
                                               "division"};
