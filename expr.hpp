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

class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string str_new, Expr *e) = 0;
//    virtual void print(std::ostream&) = 0;
//    std::string to_string();
//    virtual void pretty_print(std::ostream&) = 0;
};

class Num: public Expr {
public:
    int val;//!< integer value of the Number expression
    Num(int val);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
//    void print(std::ostream&);
//    std::string to_string();
};


class Add: public Expr {
public:
    Expr *lhs; //!< left-hand-side expression of the Number expression
    Expr *rhs; //!< right-hand-side expression of the Number expression
    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
//    void print(std::ostream&);
//    std::string to_string();
};


class Mult: public Expr {
public:
    Expr *lhs; //!<  left-hand-side expression of the Mult expression
    Expr *rhs; //!<  right-hand-side expression of the Mult expression
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
//    void print(std::ostream&);
//    std::string to_string();
};


class Var: public Expr {
public:
    std::string str; //!<  string value of the Var expression
    Var(std::string str);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
//    void print(std::ostream&);
//    std::string to_string();
};




#endif /* expr_hpp */
