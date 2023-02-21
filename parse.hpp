//
//  parse.hpp
//  6015HW
//
//  Created by Jinny Jeng on 2/20/23.
//

#ifndef parse_hpp
#define parse_hpp

#include <stdio.h>
#include <sstream>
#include "expr.hpp"

Expr* parse_expr(std::istream &in);
Expr* parse_addend(std::istream &in);
Expr* parse_multicand(std::istream &in);
Expr* parse_num(std::istream &in);
Expr* parse_var(std::istream &in);
Expr* parse_let(std::istream &in);
Expr* parse_str(std::string str);

#endif /* parse_hpp */
