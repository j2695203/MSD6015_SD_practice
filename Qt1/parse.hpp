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
#include "pointer.h"

PTR(Expr) parse_expr(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);
PTR(Expr) parse_addend(std::istream &in);
PTR(Expr) parse_multicand(std::istream &in);
PTR(Expr) parse_inner(std::istream &in);

PTR(Expr) parse_num(std::istream &in);
PTR(Expr) parse_var(std::istream &in);
PTR(Expr) parse_let(std::istream &in);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);

PTR(Expr) parse_str(std::string str);// for test

#endif /* parse_hpp */
