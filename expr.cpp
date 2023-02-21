/**
* \file expr.cpp
* \brief contains expression class definition
*
* No more verbose description here lol
* \author Jinny
*/

#include <iostream>
#include <sstream>
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


Num::Num(int val) {
    this->val = val;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Number Expression
*/
bool Num::equals(Expr* e){
    Num* n = dynamic_cast<Num*>(e);
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
int Num::interp(){
    return this->val;
}
/**
* \brief Check if the expression is a variable or contains a variable. Num expression always return false.
* \return bool if the num expression is a variable. It should always return false.
*/
bool Num::has_variable(){
    return false;
}
/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* Num::subst(std::string str_new, Expr *e){
    return ( new Num( this->val ) ); // don't need to substitute a number
}
/**
 *\brief print expression with infix and parentheses
 */
void Num::print(std::ostream& ot){
    ot<<std::to_string(val);
}

/**
* \brief print the expression based on precedence rules
* \param ost first argument
* \param p second argument, the precedence of previous expression
*/
void Num::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){
    ost<<std::to_string(val);
}




/*
 *** Class Add ************************************************
 */


Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Add Expression with same arguments
*/
bool Add::equals(Expr* e){
    Add* n = dynamic_cast<Add*>(e);
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
int Add::interp() {
    return( lhs->interp() + rhs->interp() );
}
/**
* \brief Check if the Add expression is a variable or contains a variable.
* \return bool if the Add expression is a variable or contains a variable.
*/
bool Add::has_variable(){
    return( lhs->has_variable() || rhs->has_variable() );
}
/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* Add::subst(std::string str_new, Expr *e){
    return ( new Add( (lhs->subst(str_new, e)) , (rhs->subst(str_new, e))) ) ;
}
/**
 *\brief print expression with infix and parentheses
 */
void Add::print(std::ostream& ot){
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
void Add::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){

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

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Mult Expression with same arguments
*/
bool Mult::equals(Expr* e){
    Mult* n = dynamic_cast<Mult*>(e);
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
int Mult::interp(){
    return( lhs->interp() * rhs->interp() );
}
/**
* \brief Check if the Mult expression is a variable or contains a variable.
* \return bool if the Mult expression is a variable or contains a variable.
*/
bool Mult::has_variable(){
    return( lhs->has_variable() || rhs->has_variable() );
}
/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* Mult::subst(std::string str_new, Expr *e){
    return ( new Mult( (lhs->subst(str_new, e)) , (rhs->subst(str_new, e))) ) ;
}
/**
 *\brief print expression with infix and parentheses
 */
void Mult::print(std::ostream& ot){
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
void Mult::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){

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

Var::Var(std::string str) {
    this->str = str;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Var Expression with same arguments
*/
bool Var::equals(Expr* e){
    Var* n = dynamic_cast<Var*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->str == n->str );
    }
}

/**
* \brief A variable has no value, so interp for a variable should throw a std::runtime_error exception
*/
int Var::interp(){
    throw std::runtime_error("no value for variable");
}
/**
* \brief Check if the Var expression is a variable.
* \return bool if the Var expression is a variable. It should always return true.
*/
bool Var::has_variable(){
    return true;
}
/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* Var::subst(std::string str_new, Expr *e){
    // substitute this Var Expr by e if the string of this Var Expr equals str_new
    if(this->str == str_new){
        return e;
    }else{
        return ( new Var(this->str) );
    }
}

void Var::print(std::ostream& ot){
    ot<<str;
}


void Var::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){
    ost<<str;
}


/*
 *** Class Let ************************************************
 */

Let::Let(std::string str, Expr *rhs, Expr *body) {
    this->str = str;
    this->rhs = rhs;
    this->body = body;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Let Expression with same arguments
*/
bool Let::equals(Expr* e){
    Let* n = dynamic_cast<Let*>(e);
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
int Let::interp(){
    
    // rhs calculate first if they're all numbers                     // how about x + 7 + 5 ?????
    if(!rhs->has_variable()){
        rhs = new Num(rhs->interp());
    }
    
    return body->subst(str,rhs)->interp();
}

/**
* \brief Check if the Let expression is a variable or contains a variable.
* \return bool if the Let expression is a variable or contains a variable.
*/
bool Let::has_variable(){
    return( rhs->has_variable() || body->has_variable() );
}

/**
* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result Expr* should have the given replacement, instead.
* \param str_new first argument, the string to be replaced
* \param e second argument, the Expression replace str_new
* \return Expr* that after replacement
*/
Expr* Let::subst(std::string str_new, Expr *e){
    // always substitute in the right-hand side
    // bind different〈variable〉: substitute in the body
    // bind same〈variable〉: don’t substitute in the body
    
    if( str != str_new ){
        return ( new Let(str, rhs->subst(str_new, e), body->subst(str_new, e)) );
    }else{
        return ( new Let(str, rhs->subst(str_new, e), body->subst(str, rhs->subst(str_new, e))) );
    }
}

/**
 *\brief print expression with infix and parentheses
 */
void Let::print(std::ostream& ot){
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
void Let::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long lastLinePos){

    
    if(p > 1 && acc > 0){ // mult->let or add->let  , and not the rightest expression
        ost<<"(";
    }
    long letPos = ost.tellp();
    
    // _let
    ost<<"_let "<<str<<" = ";
    rhs->pretty_print_at(ost, prec_none, acc, 0);
    
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
    
    
    if(p > 1 && acc > 0){ // mult->let or add->let
        ost<<")";
    }

}
