//
//  val.hpp
//  6015HW
//
//  Created by Jinny Jeng on 3/7/23.
//

#ifndef val_hpp
#define val_hpp

#include <stdio.h>
#include <string>
#include "pointer.h"


class Expr;

class Env;

CLASS(Val){
public:
    virtual bool equals(PTR(Val) e) = 0;
//    virtual PTR(Expr) to_expr() = 0;
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) mult_with(PTR(Val) other_val) = 0;
    virtual std::string to_string() = 0;
    virtual bool is_true() = 0;
    virtual PTR(Val) call(PTR(Val)actual_arg) = 0;
};


class NumVal: public Val{
public:
    int rep;//!< integer value of the Number expression
    NumVal(int rep);
    
    bool equals(PTR(Val) e);
//    PTR(Expr) to_expr();
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_with(PTR(Val) other_val);
    std::string to_string();
    bool is_true();
    virtual PTR(Val) call(PTR(Val)actual_arg);
};


class BoolVal: public Val{
public:
    bool rep;
    BoolVal(bool rep);
    
    bool equals(PTR(Val) e);
//    PTR(Expr) to_expr();
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_with(PTR(Val) other_val);
    std::string to_string();
    bool is_true();
    virtual PTR(Val) call(PTR(Val)actual_arg);
};


class FunVal: public Val{
public:
    std::string formal_arg;
    PTR(Expr)body;
    PTR(Env)env;
    FunVal(std::string formal_arg, PTR(Expr)body, PTR(Env)env);
    
    bool equals(PTR(Val) e);
//    PTR(Expr) to_expr();
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_with(PTR(Val) other_val);
    std::string to_string();
    bool is_true();
    virtual PTR(Val) call(PTR(Val)actual_arg);
};


#endif /* val_hpp */
