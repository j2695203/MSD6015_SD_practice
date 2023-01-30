//
//  expr.hpp
//  HW2_ExpressionClasses
//
//  Created by Jinny Jeng on 1/17/23.
//

#ifndef expr_hpp
#define expr_hpp

#include <stdio.h>
#include <string>
#include <stdexcept>

class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string str_new, Expr *e) = 0;
};

class Num: public Expr {
public:
    int val;
    Num(int val);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
};


class Add: public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
};


class Mult: public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
};


class Var: public Expr {
public:
    std::string str;
    Var(std::string str);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
};




#endif /* expr_hpp */
