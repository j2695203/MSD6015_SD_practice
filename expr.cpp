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

int Num::interp(){
    return this->val;
}

bool Num::has_variable(){
    return false;
}

Expr* Num::subst(std::string str_new, Expr *e){
    return ( new Num( this->val ) ); // don't need to substitute a number
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

int Add::interp() {
    return( lhs->interp() + rhs->interp() );
}

bool Add::has_variable(){
    return( lhs->has_variable() || rhs->has_variable() );
}

Expr* Add::subst(std::string str_new, Expr *e){
    return ( new Add( (lhs->subst(str_new, e)) , (rhs->subst(str_new, e))) ) ;
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

int Mult::interp(){
    return( lhs->interp() * rhs->interp() );
}

bool Mult::has_variable(){
    return( lhs->has_variable() || rhs->has_variable() );
}

Expr* Mult::subst(std::string str_new, Expr *e){
    return ( new Mult( (lhs->subst(str_new, e)) , (rhs->subst(str_new, e))) ) ;
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

int Var::interp(){
    throw std::runtime_error("no value for variable");
}

bool Var::has_variable(){
    return true;
}

Expr* Var::subst(std::string str_new, Expr *e){
    // substitute this Var Expr by e if the string of this Var Expr equals str_new
    if(this->str == str_new){
        return e;
    }else{
        return ( new Var(this->str) );
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
        Add* add_var = new Add(new Var("dog"), new Var("cat"));
        Add* add_var_Eq = new Add(new Var("dog"), new Var("cat"));
        Add* add_var_NoEq = new Add(new Var("PIGGG"), new Var("cat"));
        
        CHECK( add_Base -> equals(add_Eq) );
        CHECK_FALSE( add_Base -> equals(add_NoEq) );
        CHECK_FALSE( add_Base -> equals(add_NoEq_rev) );
        CHECK_FALSE( add_Base -> equals(add_NoEq_neg) );
        CHECK( add_var -> equals(add_var_Eq) );
        CHECK_FALSE( add_var -> equals(add_var_NoEq) );
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

TEST_CASE("Test_interp()"){
    
    SECTION("addExpr"){
        CHECK( (new Add(new Add(new Num(10), new Num(15)),new Add(new Num(20),new Num(20))))->interp() == 65); // num only
        CHECK_THROWS_WITH( (new Add(new Var("dog"), new Var("cat")))->interp(), "no value for variable" ); // var only
        CHECK_THROWS_WITH( (new Add(new Var("dog"), new Num(2)))->interp(), "no value for variable" ); // num + var
    }

    SECTION("multExpr"){
        CHECK( (new Mult(new Num(3), new Num(2)))->interp() == 6 ); // num only
        CHECK_THROWS_WITH( (new Mult(new Var("dog"), new Var("cat")))->interp(), "no value for variable" ); // var only
        CHECK_THROWS_WITH( (new Mult(new Var("dog"), new Num(2)))->interp(), "no value for variable" ); // num + var
    }
    
    SECTION("numExpr_only"){
        CHECK( (new Num(5))->interp() == 5 );
    }
    
    SECTION("varExpr_only"){
        CHECK_THROWS_WITH( (new Var("x"))->interp(), "no value for variable" );
    }
   
}

TEST_CASE("Test_hasVariable()"){
    
    SECTION("addExpr"){
        CHECK_FALSE( (new Add(new Add(new Num(10), new Num(15)),new Add(new Num(20),new Num(20))))->has_variable() ); // num only
        CHECK( (new Add(new Var("dog"), new Var("cat")))->has_variable() ); // var only
        CHECK( (new Add(new Var("dog"), new Num(2)))->has_variable() ); // num + var
    }

    SECTION("multExpr"){
        CHECK_FALSE( (new Mult(new Num(3), new Num(2)))->has_variable() ); // num only
        CHECK( (new Mult(new Var("dog"), new Var("cat")))->has_variable() ); // var only
        CHECK( (new Mult(new Var("dog"), new Num(2)))->has_variable() ); // num + var
    }
    
    SECTION("numExpr_only"){
        CHECK_FALSE( (new Num(5))->has_variable() );
    }
    
    SECTION("varExpr_only"){
        CHECK( (new Var("x"))->has_variable() );
    }
}

TEST_CASE("Test_subst()"){
    
    SECTION("addExpr"){
        // did substitute
        CHECK( (new Add(new Var("x"), new Num(7)))
               ->subst("x", new Var("y"))
               ->equals(new Add(new Var("y"), new Num(7))) );
        // didn't substitute
        CHECK( (new Add(new Var("x"), new Num(7)))
               ->subst("no_match_str", new Var("y"))
               ->equals(new Add(new Var("x"), new Num(7))) );
        
    }

    SECTION("multExpr"){
        // did substitute with nested Expr
        CHECK( ( new Mult( ( new Mult(new Var("x"),new Num(7)) )  ,  (new Mult(new Var("x"),new Num(7))) ) )
               ->subst("x", new Var("y"))
               ->equals( new Mult( ( new Mult(new Var("y"),new Num(7)) )  ,  (new Mult(new Var("y"),new Num(7))) ) ) );
        // didn't substitute
        CHECK( (new Mult(new Var("x"), new Num(7)))
               ->subst("no_match_str", new Var("y"))
               ->equals(new Mult(new Var("x"), new Num(7))) );
    }
    
    SECTION("numExpr_only"){
        // never substitute
        CHECK( (new Num(5))->subst("5", new Num(-1))->equals(new Num(5)) );
    }
    
    SECTION("varExpr_only"){
        // did substitute
        CHECK( (new Var("original"))->subst("original", new Var("new"))->equals(new Var("new")) );
        // didn't substitute
        CHECK( (new Var("original"))->subst("no_match", new Var("new"))->equals(new Var("original")) );
    }
    
}
