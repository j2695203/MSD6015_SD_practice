/**
* \file expr.cpp
* \brief contains expression class definition
*
* No more verbose description here lol
* \author Jinny
*/

#include <iostream>
#include <sstream>
#include "val.hpp"
#include "expr.hpp"
#include "catch.h"


/*
 *** Class Expr ************************************************
 */

/**
* \brief returning the str value of the output stream
 */
std::string Expr::to_string() {
    std::stringstream st("");
    this->print(st);
    return st.str();
}

/**
* \brief returning the str value of the pretty print function
 */
std::string Expr::pretty_print_to_string() {
    std::stringstream st("");
    this->pretty_print(st);
    return st.str();
}

/**
 *\brief call function to print the expression by passing ostream and precedence rules
 */
void Expr::pretty_print(std::ostream& ost){
    precedence_t prec = prec_none; // for first time (driver)
    pretty_print_at(ost, prec, 0, ost.tellp());
}




/*
 *** Class Num ************************************************
 */


NumExpr::NumExpr(int val) {
    this->val = val;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Number Expression
*/
bool NumExpr::equals(Expr* e){
    NumExpr* n = dynamic_cast<NumExpr*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->val == n->val );
    }
}

/**
* \brief return an int for the value of the number.
* \return int for this Number Expression
*/
Val* NumExpr::interp(){
    return new NumVal(this->val);
}
/**
* \brief Check if the expression is a variable or contains a variable. Num expression always return false.
* \return bool if the num expression is a variable. It should always return false.
*/
bool NumExpr::has_variable(){
    return false;
}
/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* NumExpr::subst(std::string str_new, Expr *e){
    return ( new NumExpr( this->val ) ); // don't need to substitute a number
}
/**
 *\brief print expression with infix and parentheses
 */
void NumExpr::print(std::ostream& ot){
    ot<<std::to_string(val);
}

/**
* \brief print the expression based on precedence rules
* \param ost first argument
* \param p second argument, the precedence of previous expression
*/
void NumExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){
    ost<<std::to_string(val);
}




/*
 *** Class Add ************************************************
 */


AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Add Expression with same arguments
*/
bool AddExpr::equals(Expr* e){
    AddExpr* n = dynamic_cast<AddExpr*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs) );
    }
}

/**
* \brief Calculate the sum of the subexpression values
* \return int for this Add Expression
*/
Val* AddExpr::interp() {
    return( lhs->interp()->add_to(rhs->interp()) );
}
/**
* \brief Check if the Add expression is a variable or contains a variable.
* \return bool if the Add expression is a variable or contains a variable.
*/
bool AddExpr::has_variable(){
    return( lhs->has_variable() || rhs->has_variable() );
}
/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* AddExpr::subst(std::string str_new, Expr *e){
    return ( new AddExpr( (lhs->subst(str_new, e)) , (rhs->subst(str_new, e))) ) ;
}
/**
 *\brief print expression with infix and parentheses
 */
void AddExpr::print(std::ostream& ot){
    ot<<"(";
    lhs->print(ot);
    ot<<"+";
    rhs->print(ot);
    ot<<")";
}

/**
* \brief print the expression based on precedence rules
* \param ost first argument
* \param p second argument, the precedence of previous expression
*/
void AddExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){

    // left parentheses
    if(p > 1 ){ // if mul->add
        ost<<"(";
        lhs->pretty_print_at(ost, static_cast<precedence_t>(prec_add+1), acc+1, pos);
        ost<<" + ";
        rhs->pretty_print_at(ost, prec_add, 0, pos);
        ost<<")";
    }else{
        lhs->pretty_print_at(ost, static_cast<precedence_t>(prec_add+1), acc+1, pos);
        ost<<" + ";
        rhs->pretty_print_at(ost, prec_add, acc, pos);
    }
    

}


/*
 *** Class Mult ************************************************
 */

MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Mult Expression with same arguments
*/
bool MultExpr::equals(Expr* e){
    MultExpr* n = dynamic_cast<MultExpr*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs) );
    }
}

/**
* \brief Calculate the product of the subexpression values
* \return int for this Mult Expression
*/
Val* MultExpr::interp(){
    return( lhs->interp()->mult_with(rhs->interp()) );
}
/**
* \brief Check if the Mult expression is a variable or contains a variable.
* \return bool if the Mult expression is a variable or contains a variable.
*/
bool MultExpr::has_variable(){
    return( lhs->has_variable() || rhs->has_variable() );
}
/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* MultExpr::subst(std::string str_new, Expr *e){
    return ( new MultExpr( (lhs->subst(str_new, e)) , (rhs->subst(str_new, e))) ) ;
}
/**
 *\brief print expression with infix and parentheses
 */
void MultExpr::print(std::ostream& ot){
    ot<<"(";
    lhs->print(ot);
    ot<<"*";
    rhs->print(ot);
    ot<<")";
}

/**
* \brief print the expression based on precedence rules
* \param ost first argument
* \param p second argument, the precedence of previous expression
*/
void MultExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){

    // left parentheses
    if( p > 2 ){
        ost<<"(";
        lhs->pretty_print_at(ost, static_cast<precedence_t>(prec_mult+1), acc+1, pos);
        ost<<" * ";
        rhs->pretty_print_at(ost, prec_mult, 0, pos);
        ost<<")";
    }else{
        lhs->pretty_print_at(ost, static_cast<precedence_t>(prec_mult+1), acc+1, pos);
        ost<<" * ";
        rhs->pretty_print_at(ost, prec_mult, acc, pos);
    }
    
}

/*
 *** Class Var (variable) ************************************************
 */

VarExpr::VarExpr(std::string str) {
    this->str = str;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Var Expression with same arguments
*/
bool VarExpr::equals(Expr* e){
    VarExpr* n = dynamic_cast<VarExpr*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->str == n->str );
    }
}

/**
* \brief A variable has no value, so interp for a variable should throw a std::runtime_error exception
*/
Val* VarExpr::interp(){
    throw std::runtime_error("no value for variable");
}
/**
* \brief Check if the Var expression is a variable.
* \return bool if the Var expression is a variable. It should always return true.
*/
bool VarExpr::has_variable(){
    return true;
}
/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* VarExpr::subst(std::string str_new, Expr *e){
    // substitute this Var Expr by e if the string of this Var Expr equals str_new
    if(this->str == str_new){
        return e;
    }else{
        return ( new VarExpr(this->str) );
    }
}

void VarExpr::print(std::ostream& ot){
    ot<<str;
}


void VarExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){
    ost<<str;
}


/*
 *** Class Let ************************************************
 */

LetExpr::LetExpr(std::string str, Expr *rhs, Expr *body) {
    this->str = str;
    this->rhs = rhs;
    this->body = body;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Let Expression with same arguments
*/
bool LetExpr::equals(Expr* e){
    LetExpr* n = dynamic_cast<LetExpr*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->str == (n->str) && this->rhs->equals(n->rhs) && this->body->equals(n->body) );
    }
}

/**
* \brief Calculate the product of the subexpression values
* \return int for this Let Expression
*/
Val* LetExpr::interp(){

//    // rhs calculate first if they're all numbers                     // how about x + 7 + 5 ?????
//    if(!rhs->has_variable()){
//        rhs = new NumExpr(rhs->interp()->rep);
//    }

//    Val* rhs_val = rhs->interp();
    return body->subst(str,rhs)->interp();
}

/**
* \brief Check if the Let expression is a variable or contains a variable.
* \return bool if the Let expression is a variable or contains a variable.
*/
bool LetExpr::has_variable(){
    return( rhs->has_variable() || body->has_variable() );
}

/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* LetExpr::subst(std::string str_new, Expr *e){
    // always substitute in the right-hand side
    // bind different〈variable〉: substitute in the body
    // bind same〈variable〉: don’t substitute in the body
    
    if( str != str_new ){
        return ( new LetExpr(str, rhs->subst(str_new, e), body->subst(str_new, e)) );
    }else{
        return ( new LetExpr(str, rhs->subst(str_new, e), body->subst(str, rhs->subst(str_new, e))) );
    }
}

/**
 *\brief print expression with infix and parentheses
 */
void LetExpr::print(std::ostream& ot){
    ot<<"(_let "<<str<<"=";
    rhs->print(ot);
    ot<<" _in ";
    body->print(ot);
    ot<<")";
}

/**
* \brief print the expression based on precedence rules
* \param ost first argument
* \param p second argument, the precedence of previous expression
*/
void LetExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long lastLinePos){

    
    if(p > 1 && acc > 0){ // mult->let or add->let  , and not the rightest expression
        ost<<"(";
    }
    long letPos = ost.tellp();
    
    // _let
    ost<<"_let "<<str<<" = ";
    rhs->pretty_print_at(ost, prec_none, acc, lastLinePos);
    
    // new line
    ost<< "\n";
    long newLinePos = ost.tellp();
    
    // blank
    // long space = letPos-lastLinePos;
    // os<<std::string(space, ' ');
    for(int i = 0; i < letPos - (lastLinePos) ; i++){
        ost<< " ";
    }
    
    
    // _in
    ost<< "_in  ";
    body->pretty_print_at(ost, prec_none, acc, newLinePos );
    
    
    if(p > 1 && acc > 0){ // mult->let or add->let         ???????????? >1
        ost<<")";
    }

}
