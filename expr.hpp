/**
* \file expr.h
* \brief expression class and inheritance classes
*
* Longer description goes here.
*/

#ifndef expr_hpp
#define expr_hpp

#include <stdio.h>
#include <string>
#include <stdexcept>
#include "pointer.h"


typedef enum {
  prec_none,      // = 0
  prec_add,       // = 1
  prec_mult       // = 2
} precedence_t;

class Val;

CLASS(Expr) {
public:
    virtual bool equals(PTR(Expr)e) = 0;
    virtual PTR(Val) interp() = 0;
//    virtual bool has_variable() = 0;
    virtual PTR(Expr) subst(std::string str_new, PTR(Expr)e) = 0;
    
    virtual void print(std::ostream&) = 0;
    void pretty_print(std::ostream&); // driver
    virtual void pretty_print_at(std::ostream&, precedence_t, int, long) = 0; // int: , long: stream position
    std::string pretty_print_to_string();
    std::string to_string();
    
    PTR(Expr) parse_expr(std::istream &in);
    PTR(Expr) parse_addend(std::istream &in);
    PTR(Expr) parse_multicand(std::istream &in);
    PTR(Expr) parse_num(std::istream &in);
    PTR(Expr) parse_var(std::istream &in);
    PTR(Expr) parse_let(std::istream &in);

};

class NumExpr: public Expr {
public:
    int val;//!< integer value of the Number expression
    NumExpr(int val);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
//    bool has_variable();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);
    
    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
    
};


class AddExpr: public Expr {
public:
    PTR(Expr)lhs; //!< left-hand-side expression of the Number expression
    PTR(Expr)rhs; //!< right-hand-side expression of the Number expression
    AddExpr(PTR(Expr)lhs, PTR(Expr)rhs);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
//    bool has_variable();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);

    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
};


class MultExpr: public Expr {
public:
    PTR(Expr)lhs; //!<  left-hand-side expression of the Mult expression
    PTR(Expr)rhs; //!<  right-hand-side expression of the Mult expression
    MultExpr(PTR(Expr)lhs, PTR(Expr)rhs);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
//    bool has_variable();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);
    
    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
};


class VarExpr: public Expr {
public:
    std::string str; //!<  string value of the Var expression
    VarExpr(std::string str);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
//    bool has_variable();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);

    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
};


class LetExpr: public Expr {
public:
    std::string str; //!<  left-hand-side string of the let expression
    PTR(Expr)rhs; //!<  right-hand-side expression of the let expression
    PTR(Expr)body; // body expression of the let expression
    LetExpr(std::string str, PTR(Expr)rhs, PTR(Expr)body);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
//    bool has_variable();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);
    
    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
};


class BoolExpr: public Expr{
public:
    bool val;
    BoolExpr(bool val);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
//    bool has_variable();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);
    
    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
};


class IfExpr: public Expr{
public:
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
//    bool has_variable();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);
    
    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
};


class EqExpr: public Expr{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
//    bool has_variable();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);
    
    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
};


class FunExpr: public Expr {
public:
    std::string formal_arg;
    PTR(Expr)body;
    FunExpr(std::string formal_arg, PTR(Expr)body);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);

    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
};

class CallExpr: public Expr {
public:
    PTR(Expr)to_be_called;
    PTR(Expr)actual_arg;
    CallExpr(PTR(Expr)to_be_called, PTR(Expr)actual_arg);
    
    bool equals(PTR(Expr)e);
    PTR(Val) interp();
    PTR(Expr) subst(std::string str_new, PTR(Expr)e);

    void print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t, int, long);
};


#endif /* expr_hpp */
