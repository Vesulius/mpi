// #include <iostream>

// #include "header.h"

// std::string getNTabs(int);

// void typeVisitor(expression_node*, int);

// void typeVisitor(literal_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "literal: " << n->value << std::endl;
// }

// void typeVisitor(multi_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "multi: " << n->op << std::endl;
// }

// void typeVisitor(add_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "add: " << n->op << std::endl;
// }

// void typeVisitor(id_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "id" << std::endl;
// }

// void typeVisitor(factor_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "factor" << std::endl;
//     typeVisitor(n->id, tablevel + 1);
//     typeVisitor(n->literal, tablevel + 1);
//     typeVisitor(n->expression, tablevel + 1);
// }

// void typeVisitor(factor_tail_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "factor tail" << std::endl;
//     typeVisitor(n->multi, tablevel + 1);
//     typeVisitor(n->factor, tablevel + 1);
//     typeVisitor(n->factorTail, tablevel + 1);
// }

// void typeVisitor(term_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "term" << std::endl;
//     typeVisitor(n->factor, tablevel + 1);
//     typeVisitor(n->factorTail, tablevel + 1);
// }

// void typeVisitor(term_tail_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "term tail" << std::endl;
//     typeVisitor(n->add, tablevel + 1);
//     typeVisitor(n->term, tablevel + 1);
//     typeVisitor(n->termTail, tablevel + 1);
// }

// void typeVisitor(expression_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "expression" << std::endl;
//     typeVisitor(n->term, tablevel + 1);
//     typeVisitor(n->termTail, tablevel + 1);
// }

// void typeVisitor(assign_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "assign" << std::endl;
//     typeVisitor(n->expression, tablevel + 1);
// }

// void typeVisitor(statement_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "statement" << std::endl;
//     typeVisitor(n->assignment, tablevel + 1);
// }

// void typeVisitor(statement_list_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "statement list" << std::endl;
//     typeVisitor(n->statement, tablevel + 1);
//     typeVisitor(n->statementList, tablevel + 1);
// }

// void typeVisitor(program_node* n, int tablevel) {
//     if (n == nullptr) return;
//     std::cout << getNTabs(tablevel) << "program" << std::endl;
//     typeVisitor(n->statementList, tablevel + 1);
// }

// void typeVisitor(program_node* n) {
//     std::cout << "\nCHECK TYPE:\n" << std::endl;
//     typeVisitor(n, 0);
// }

// std::string getNTabs(int n) {
//     std::string tabs = "";
//     for (int i = 0; i < n; i++) {
//         tabs += "    ";
//     }
//     return tabs;
// }
