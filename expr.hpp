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

typedef enum {
  prec_none,      // = 0
  prec_add,       // = 1
  prec_mult       // = 2
} precedence_t;


class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string str_new, Expr *e) = 0;
    
    virtual void print(std::ostream&) = 0;
    std::string to_string();

    void pretty_print(std::ostream&); // driver
    virtual void pretty_print_at(std::ostream&, precedence_t) = 0;
    std::string pretty_print_to_string();
    

};

class Num: public Expr {
public:
    int val;//!< integer value of the Number expression
    Num(int val);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
    
    void print(std::ostream&);
    void pretty_print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t);
    
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

    void print(std::ostream&);
    void pretty_print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t);
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
    
    void print(std::ostream&);
    void pretty_print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t);
};


class Var: public Expr {
public:
    std::string str; //!<  string value of the Var expression
    Var(std::string str);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);

    void print(std::ostream&);
    void pretty_print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t);
};


class Let: public Expr {
public:
    std::string str; //!<  left-hand-side string of the let expression
    Expr *rhs; //!<  right-hand-side expression of the let expression
    Expr *body; // body expression of the let expression
    Let(std::string str, Expr *rhs, Expr *body);
    bool equals(Expr *e);
    int interp();
    bool has_variable();
    Expr* subst(std::string str_new, Expr *e);
    
    void print(std::ostream&);
    void pretty_print(std::ostream&);
    void pretty_print_at(std::ostream&, precedence_t);
};

#endif /* expr_hpp */
