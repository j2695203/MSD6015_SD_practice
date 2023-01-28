//
//  expr.cpp
//  HW2_ExpressionClasses
//
//  Created by Jinny Jeng on 1/17/23.
//

#include "expr.hpp"
#include "catch.h"

/*
 Class Num
 */

Num::Num(int val) {
    this->val = val;
}

bool Num::equals(Expr* e){
    Num* n = dynamic_cast<Num*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->val == n->val );
    }
}

/*
 Class Add
 */

Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Add::equals(Expr* e){
    Add* n = dynamic_cast<Add*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs) );
    }
}

/*
Class Mult
 */

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Mult::equals(Expr* e){
    Mult* n = dynamic_cast<Mult*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs) );
    }
}

/*
 Class Var (variable)
 */

Var::Var(std::string str) {
    this->str = str;
}

bool Var::equals(Expr* e){
    Var* n = dynamic_cast<Var*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->str == n->str );
    }
}

/*
 TEST using Catch 2 framework
 */

TEST_CASE("Test_Equal_Within_Same_Class"){
    
    SECTION("expr_number"){
        Num* num_Base = new Num(3);
        Num* num_Eq = new Num(3);
        Num* num_NoEq = new Num(5);
        Num* num_NoEq_neg = new Num(-3);
        Num* num_NoEq_zero = new Num(0);
        
        
        CHECK( num_Base -> equals(num_Eq) );
        CHECK_FALSE( num_Base -> equals(num_NoEq) );
        CHECK_FALSE( num_Base -> equals(num_NoEq_neg) );
        CHECK_FALSE( num_Base -> equals(num_NoEq_zero) );
    }
    
    SECTION("expr_add"){
        Add* add_Base = new Add(new Num(5), new Num(7));
        Add* add_Eq = new Add(new Num(5), new Num(7));
        Add* add_NoEq = new Add(new Num(10), new Num(20));
        Add* add_NoEq_rev = new Add(new Num(7), new Num(5));
        Add* add_NoEq_neg = new Add(new Num(-5), new Num(-7));
        
        CHECK( add_Base -> equals(add_Eq) );
        CHECK_FALSE( add_Base -> equals(add_NoEq) );
        CHECK_FALSE( add_Base -> equals(add_NoEq_rev) );
        CHECK_FALSE( add_Base -> equals(add_NoEq_neg) );
    }
 
    SECTION("expr_multiple"){
        Mult* mult_Base = new Mult(new Num(5), new Num(7));
        Mult* mult_Eq = new Mult(new Num(5), new Num(7));
        Mult* mult_NoEq = new Mult(new Num(10), new Num(20));
        Mult* mult_NoEq_rev = new Mult(new Num(7), new Num(5));
        
        CHECK( mult_Base -> equals(mult_Eq) );
        CHECK_FALSE( mult_Base -> equals(mult_NoEq) );
        CHECK_FALSE( mult_Base -> equals(mult_NoEq_rev) );
    }
    
    SECTION("expr_variable"){
        Var* var_Base = new Var("Dog");
        Var* var_Eq = new Var("Dog");
        Var* var_NoEq = new Var("Cat");
        Var* var_NoEq_lower = new Var("dog");
        
        CHECK( var_Base -> equals(var_Eq) );
        CHECK_FALSE( var_Base -> equals(var_NoEq) );
        CHECK_FALSE( var_Base -> equals(var_NoEq_lower) );
    }
}

TEST_CASE("Test_Equal_Not_Same_Class"){
    
    SECTION("num_var"){
        Num* num = new Num(2);
        Var* var = new Var("2");
        CHECK_FALSE( num -> equals(var) );
    }
    
    SECTION("add_mult"){
        Add* add = new Add( new Num(3), new Num(0) );
        Mult* mult = new Mult ( new Num(3), new Num(0) );
        CHECK_FALSE( add -> equals(mult) );
    }
}
