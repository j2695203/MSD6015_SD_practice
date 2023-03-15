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


class Expr;


class Val{
public:
    virtual bool equals(Val* e) = 0;
    virtual Expr* to_expr() = 0;
    virtual Val* add_to(Val* other_val) = 0;
    virtual Val* mult_with(Val* other_val) = 0;
    virtual std::string to_string() = 0;
    virtual bool is_true() = 0;
};


class NumVal: public Val{
public:
    int rep;//!< integer value of the Number expression
    NumVal(int rep);
    
    bool equals(Val* e);
    Expr* to_expr();
    Val* add_to(Val* other_val);
    Val* mult_with(Val* other_val);
    std::string to_string();
    bool is_true();
};


class BoolVal: public Val{
public:
    bool rep;
    BoolVal(bool rep);
    
    bool equals(Val* e);
    Expr* to_expr();
    Val* add_to(Val* other_val);
    Val* mult_with(Val* other_val);
    std::string to_string();
    bool is_true();
};


#endif /* val_hpp */
