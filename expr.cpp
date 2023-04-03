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
#include "env.hpp"


/*
 *** Class PTR(Expr)***********************************************
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
bool NumExpr::equals(PTR(Expr) e){
    PTR(NumExpr) n = CAST(NumExpr)(e);
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
PTR(Val) NumExpr::interp(PTR(Env)env){
    return NEW(NumVal)(val);
}
///**
//* \brief Check if the expression is a variable or contains a variable. Num expression always return false.
//* \return bool if the num expression is a variable. It should always return false.
//*/
//bool NumExpr::has_variable(){
//    return false;
//}

///**
//* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result PTR(Expr) should have the given replacement, instead.
//* \param str_new first argument, the string to be replaced
//* \param e second argument, the Expression replace str_new
//* \return PTR(Expr) that after replacement
//*/
//PTR(Expr) NumExpr::subst(std::string str_new, PTR(Expr)e){
//    return ( NEW(NumExpr)( this->val ) ); // don't need to substitute a number
//}

/**
 *\brief print expression with infix and parentheses
 */
void NumExpr::print(std::ostream& ost){
    ost<<std::to_string(val);
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


AddExpr::AddExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Add Expression with same arguments
*/
bool AddExpr::equals(PTR(Expr) e){
    PTR(AddExpr) n = CAST(AddExpr)(e);
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
PTR(Val) AddExpr::interp(PTR(Env)env) {
    return( lhs->interp(env)->add_to(rhs->interp(env)) );
}

///**
//* \brief Check if the Add expression is a variable or contains a variable.
//* \return bool if the Add expression is a variable or contains a variable.
//*/
//bool AddExpr::has_variable(){
//    return( lhs->has_variable() || rhs->has_variable() );
//}

///**
//* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result PTR(Expr) should have the given replacement, instead.
//* \param str_new first argument, the string to be replaced
//* \param e second argument, the Expression replace str_new
//* \return PTR(Expr) that after replacement
//*/
//PTR(Expr) AddExpr::subst(std::string str_new, PTR(Expr)e){
//    return ( NEW(AddExpr)( (lhs->subst(str_new, e)) , (rhs->subst(str_new, e))) ) ;
//}

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
        rhs->pretty_print_at(ost, prec_add, 0, pos);
    }
    

}


/*
 *** Class Mult ************************************************
 */

MultExpr::MultExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Mult Expression with same arguments
*/
bool MultExpr::equals(PTR(Expr) e){
    PTR(MultExpr) n = CAST(MultExpr)(e);
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
PTR(Val) MultExpr::interp(PTR(Env)env){
    return( lhs->interp(env)->mult_with(rhs->interp(env)) );
}
///**
//* \brief Check if the Mult expression is a variable or contains a variable.
//* \return bool if the Mult expression is a variable or contains a variable.
//*/
//bool MultExpr::has_variable(){
//    return( lhs->has_variable() || rhs->has_variable() );
//}

///**
//* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result PTR(Expr) should have the given replacement, instead.
//* \param str_new first argument, the string to be replaced
//* \param e second argument, the Expression replace str_new
//* \return PTR(Expr) that after replacement
//*/
//PTR(Expr) MultExpr::subst(std::string str_new, PTR(Expr)e){
//    return ( NEW(MultExpr)( (lhs->subst(str_new, e)) , (rhs->subst(str_new, e))) ) ;
//}

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
    
    // check 1. rhs of * is let/if  2. * is other's lhs 3. !(p > 2) no () outside  4. p>0 means not let's body -> acc of rhs not reset to 0 -> to check add () or not
    bool accReset = true;
    PTR(LetExpr) n = CAST(LetExpr)(rhs);
    PTR(IfExpr) m = CAST(IfExpr)(rhs);
    if( (n != NULL || m != NULL) && acc > 0 && !(p > 2) && p > 0){
        accReset = false;
    }
    
    if( p > 2 ){
        ost<<"(";
        lhs->pretty_print_at(ost, static_cast<precedence_t>(prec_mult+1), acc+1, pos);
        ost<<" * ";
        rhs->pretty_print_at(ost, prec_mult, (accReset)? 0:acc , pos);
        ost<<")";
    }else{
        lhs->pretty_print_at(ost, static_cast<precedence_t>(prec_mult+1), acc+1, pos);
        ost<<" * ";
        rhs->pretty_print_at(ost, prec_mult, (accReset)? 0:acc , pos);
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
bool VarExpr::equals(PTR(Expr) e){
    PTR(VarExpr) n = CAST(VarExpr)(e);
    if(n == NULL){
        return false;
    }else{
        return( this->str == n->str );
    }
}

/**
* \brief A variable has no value, so interp for a variable should throw a std::runtime_error exception
*/
PTR(Val) VarExpr::interp(PTR(Env)env){
    return env->lookup(this->str);
}
///**
//* \brief Check if the Var expression is a variable.
//* \return bool if the Var expression is a variable. It should always return true.
//*/
//bool VarExpr::has_variable(){
//    return true;
//}

///**
//* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result PTR(Expr) should have the given replacement, instead.
//* \param str_new first argument, the string to be replaced
//* \param e second argument, the Expression replace str_new
//* \return PTR(Expr) that after replacement
//*/
//PTR(Expr) VarExpr::subst(std::string str_new, PTR(Expr)e){
//    // substitute this Var Expr by e if the string of this Var Expr equals str_new
//    if(this->str == str_new){
//        return e;
//    }else{
//        return ( NEW(VarExpr)(this->str) );
//    }
//}

void VarExpr::print(std::ostream& ot){
    ot<<str;
}


void VarExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){
    ost<<str;
}


/*
 *** Class Let ************************************************
 */

LetExpr::LetExpr(std::string str, PTR(Expr)rhs, PTR(Expr)body) {
    this->str = str;
    this->rhs = rhs;
    this->body = body;
}

/**
* \brief Compare if two Expressions are the same type with same arguments
* \param e first argument,the Expression to be compared
* \return bool if the Expression to be compared is a Let Expression with same arguments
*/
bool LetExpr::equals(PTR(Expr) e){
    PTR(LetExpr) n = CAST(LetExpr)(e);
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
PTR(Val) LetExpr::interp(PTR(Env)env){

//    // rhs calculate first if they're all numbers                     // how about x + 7 + 5 ?????
//    if(!rhs->has_variable()){
//        rhs = NEW(NumExpr)(rhs->interp()->rep);
//    }

    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(str,rhs_val,env);
    return body->interp(new_env);
}

///**
//* \brief Check if the Let expression is a variable or contains a variable.
//* \return bool if the Let expression is a variable or contains a variable.
//*/
//bool LetExpr::has_variable(){
//    return( rhs->has_variable() || body->has_variable() );
//}

///**
//* \brief Everywhere that the expression (whose subst method is called) contains a variable matching the str_new, the result PTR(Expr) should have the given replacement, instead.
//* \param str_new first argument, the string to be replaced
//* \param e second argument, the Expression replace str_new
//* \return PTR(Expr) that after replacement
//*/
//PTR(Expr) LetExpr::subst(std::string str_new, PTR(Expr)e){
//    // always substitute in the right-hand side
//    // bind different〈variable〉: substitute in the body
//    // bind same〈variable〉: don’t substitute in the body
//    if( str != str_new ){
//        return ( NEW(LetExpr)(str, rhs->subst(str_new, e), body->subst(str_new, e)) );
//    }else{
//        return ( NEW(LetExpr)(str, rhs->subst(str_new, e), body->subst(str, rhs->subst(str_new, e))) );
//    }
//}

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

    
    if(p > 0 && acc > 0){ // mult->let or add->let  , and not the rightest expression(acc=0 means rightest)
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
    
    
    if(p > 0 && acc > 0){
        ost<<")";
    }

}


/*
 *** Class BoolExpr ************************************************
 */
BoolExpr::BoolExpr(bool val){
    this->val = val;
}

bool BoolExpr::equals(PTR(Expr)e){
    PTR(BoolExpr) n = CAST(BoolExpr)(e);
    if(n == NULL){
        return false;
    }else{
        return( this->val == n->val );
    }
}

PTR(Val) BoolExpr::interp(PTR(Env)env){
    return NEW(BoolVal)(val);
}

//bool BoolExpr::has_variable(){
//    return false;
//}

//PTR(Expr) BoolExpr::subst(std::string str_new, PTR(Expr)e){
//    return ( NEW(BoolExpr)( this->val ) ); // don't need to substitute a boolean
//}

void BoolExpr::print(std::ostream& ost){
    ost<< NEW(BoolVal)(val)->to_string();
}

void BoolExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){
    ost<< NEW(BoolVal)(val)->to_string();
}

/*
 *** Class IfExpr ************************************************
 */
IfExpr::IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part){
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}

bool IfExpr::equals(PTR(Expr)e){
    PTR(IfExpr) n = CAST(IfExpr)(e);
    if(n == NULL){
        return false;
    }else{
        return( this->test_part->equals(n->test_part) &&
               this->then_part->equals(n->then_part) &&
               this->else_part->equals(n->else_part) );
    }
}

PTR(Val) IfExpr::interp(PTR(Env)env){
    if (test_part->interp(env)->is_true())
        return then_part->interp(env);
    else
        return else_part->interp(env);
}

//bool IfExpr::has_variable(){
//    return ( test_part->has_variable() ||
//            then_part->has_variable() ||
//            else_part->has_variable() );
//}

//PTR(Expr) IfExpr::subst(std::string str_new, PTR(Expr)e){
//    return ( NEW(IfExpr)( test_part->subst(str_new,e),
//                        then_part->subst(str_new,e),
//                        else_part->subst(str_new,e) ) );
//}

void IfExpr::print(std::ostream& ost){
    ost<<"(_if ";
    test_part->print(ost);
    ost<<" _then ";
    then_part->print(ost);
    ost<<" _else ";
    else_part->print(ost);
    ost<<")";
}

void IfExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long lastLinePos){

    if(p > 0 && acc > 0){ // mult->let or add->let  , and not the rightest expression(acc=0 means rightest)
        ost<<"(";
    }
    long ifPos = ost.tellp();
    
    // _if
    ost<<"_if ";
    test_part->pretty_print_at(ost, prec_none, acc, lastLinePos);
    
    // new line blank
    ost<< "\n";
    long newLinePos = ost.tellp();
    for(int i = 0; i < ifPos - (lastLinePos) ; i++){
        ost<< " ";
    }
    
    // _then
    ost<< "_then ";
    then_part->pretty_print_at(ost, prec_none, acc, newLinePos);
    
    // new line blank
    ost<< "\n";
    newLinePos = ost.tellp();
    for(int i = 0; i < ifPos - (lastLinePos) ; i++){
        ost<< " ";
    }
    
    // _else
    ost<< "_else ";
    else_part->pretty_print_at(ost, prec_none, acc, newLinePos);

    if(p > 0 && acc > 0){
        ost<<")";
    }

}



/*
 *** Class EqExpr ************************************************
 */

EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals(PTR(Expr)e){
    PTR(EqExpr) n = CAST(EqExpr)(e);
    if(n == NULL){
        return false;
    }else{
        return( this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs) );
    }
}

PTR(Val) EqExpr::interp(PTR(Env)env){
    return NEW(BoolVal)(lhs->interp(env)->equals(rhs->interp(env)));
}

//bool EqExpr::has_variable(){
//    return ( lhs->has_variable() || rhs->has_variable() );
//}

//PTR(Expr) EqExpr::subst(std::string str_new, PTR(Expr)e){
//    return NEW(EqExpr)( lhs->subst(str_new, e), rhs->subst(str_new, e) );
//}

void EqExpr::print(std::ostream& ost){
    ost<<"(";
    lhs->print(ost);
    ost<<"==";
    rhs->print(ost);
    ost<<")";
}

void EqExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){
    
    // left parentheses
    if(p > 0 ){
        ost<<"(";
        lhs->pretty_print_at(ost, static_cast<precedence_t>(prec_none+1), acc+1, pos);
        ost<<" == ";
        rhs->pretty_print_at(ost, prec_none, 0, pos);
        ost<<")";
    }else{
        lhs->pretty_print_at(ost, static_cast<precedence_t>(prec_none+1), acc+1, pos);
        ost<<" == ";
        rhs->pretty_print_at(ost, prec_none, acc, pos);
    }
    
}


/*
 *** Class FunExpr ************************************************
 */
FunExpr::FunExpr(std::string formal_arg, PTR(Expr)body){
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr)e){
    PTR(FunExpr) n = CAST(FunExpr)(e);
    if(n == NULL){
        return false;
    }else{
        return( this->formal_arg == (n->formal_arg) && this->body->equals(n->body) );
    }
}

PTR(Val) FunExpr::interp(PTR(Env)env){
    return NEW(FunVal)(formal_arg, body, env);
}


//PTR(Expr) FunExpr::subst(std::string str_new, PTR(Expr)e){
//    if( str_new == formal_arg ){
//        return NEW(FunExpr)( formal_arg ,body );
//    }
//    return NEW(FunExpr)( formal_arg ,body->subst(str_new, e) );
//}

void FunExpr::print(std::ostream& ost){
    ost<<"(_fun (" << formal_arg << ") ";
    body->print(ost);
    ost<<")";
}

void FunExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){
}


/*
 *** Class CallExpr ************************************************
 */
CallExpr::CallExpr(PTR(Expr)to_be_called, PTR(Expr)actual_arg){
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr)e){
    PTR(CallExpr) n = CAST(CallExpr)(e);
    if(n == NULL){
        return false;
    }else{
        return( to_be_called->equals(n->to_be_called) && actual_arg->equals(n->actual_arg) );
    }
}

PTR(Val) CallExpr::interp(PTR(Env)env){
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}


//PTR(Expr) CallExpr::subst(std::string str_new, PTR(Expr)e){
//    return NEW(CallExpr)( to_be_called->subst(str_new, e), actual_arg->subst(str_new, e) );
//}

void CallExpr::print(std::ostream& ost){
    to_be_called->print(ost);
    ost<<"(";
    actual_arg->print(ost);
    ost<<")";
}

void CallExpr::pretty_print_at(std::ostream& ost, precedence_t p, int acc, long pos){
}
