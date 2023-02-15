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



/*
 *** TEST using Catch 2 framework ************************************************
 */


/*
 Kevin & Will's test
 */

TEST_CASE("Pretty Print examples_Kevin"){           //Created from assignment examples
    
    CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(1))))
          ->pretty_print_to_string() == "_let x = 5\n"
                                     "_in  x + 1");

    CHECK((new Add(new Let("x", new Num(5), new Var("x")), new Num(1)))
          ->pretty_print_to_string()== "(_let x = 5\n"
                                    " _in  x) + 1");

    CHECK((new Mult(new Num(5), new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))))
          ->pretty_print_to_string() == "5 * _let x = 5\n"
                                     "    _in  x + 1");

    CHECK((new Add(new Mult(new Num(5), new Let("x", new Num(5), new Var("x"))), new Num(1)))
          ->pretty_print_to_string() == "5 * (_let x = 5\n"
                                     "     _in  x) + 1");

    CHECK((new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
          ->pretty_print_to_string() == "_let x = 5\n"
                                     "_in  (_let y = 3\n"
                                     "      _in  y + 2) + x");

    CHECK((new Let("x", new Let("y", new Num(6), new Mult(new Var("y"), new Num(2))), new Add(new Var("x"), new Num(1))))
          ->pretty_print_to_string() == "_let x = _let y = 6\n"
                                     "         _in  y * 2\n"
                                     "_in  x + 1");
}

TEST_CASE("pretty_print_let_mine_some_reuse_of_kevin_triple_nested_let") {
    Let* tripleNestedLet = new Let("x", new Num(1),
                                   new Let("y", new Num(1),
                                           new Mult(new Add(new Var("x"), new Var("y")), new Var("z"))));
    Let* tripleNestedLet2 = new Let("x", new Num(1),
                                    new Let("y", new Num(1),
                                            new Let("z", new Add(new Var("x"), new Num(1)),
                                                    new Add(new Add(new Var("x"), new Var("y")), new Var("z")))));
    Let* tripleNestedLet3 = new Let("x", new Num(1),
                                    new Let("y", new Num(1),
                                            new Let("z", new Add(new Var("x"), new Num(1)),
                                                    new Mult(new Add(new Var("x"), new Var("y")), new Var("z")))));


    CHECK(tripleNestedLet -> pretty_print_to_string() ==
          "_let x = 1\n"
          "_in  _let y = 1\n"
          "     _in  (x + y) * z"
    );
    CHECK(tripleNestedLet2 -> pretty_print_to_string() ==
          "_let x = 1\n"
          "_in  _let y = 1\n"
          "     _in  _let z = x + 1\n"
          "          _in  (x + y) + z"
    );
    CHECK(tripleNestedLet3 -> pretty_print_to_string() ==
                            "_let x = 1\n"
                            "_in  _let y = 1\n"
                            "     _in  _let z = x + 1\n"
                            "          _in  (x + y) * z"
    );
    Let* tripleNestedLet4 =new Let("x", new Num(5),
                                   new Let("y", new Num(3),
                                           new Add(new Var("y"), new Num(2))));
    Let* tripleNestedLet5 =new Let("x", new Num(5),
                                   new Add(new Let("y", new Num(3),
                                                   new Add(new Var("y"), new Num(2))), new Var("x")));

    CHECK(tripleNestedLet4->pretty_print_to_string() ==
                  "_let x = 5\n"
                  "_in  _let y = 3\n"
                  "     _in  y + 2"
    );

    CHECK(tripleNestedLet5 -> pretty_print_to_string() == "_let x = 5\n"
                                                      "_in  (_let y = 3\n"
                                                      "      _in  y + 2) + x");
    SECTION("assignment_examples") {
        CHECK( (new Add(new Mult(new Num(5), new Let("x", new Num(5), new Var("x"))), new Num(1)))-> pretty_print_to_string()
               == "5 * (_let x = 5\n"
                  "     _in  x) + 1");
        CHECK( (new Mult(new Mult(new Num (2), new Let("x", new Num(5), new Add(new Var("x") ,new  Num(1)) )), new Num(3) )) -> pretty_print_to_string()
               == "(2 * _let x = 5\n"
                  "     _in  x + 1) * 3");
    }
    // A _let needs parentheses when it is nested immediately as the right argument of an unparenthesized *
    // where _let would have needed parentheses in the surrounding context
    // (that is, if the _let used in place of the whole * would need parentheses,
    // then it still needs parentheses within the right-hand size of *).
    SECTION("new_edge") {
        CHECK( (new Mult(new Num (2), new Let("x", new Num(5), new Add(new Var("x") ,new  Num(1)) ) )) -> pretty_print_to_string()
               == "2 * _let x = 5\n"
                  "    _in  x + 1");
        CHECK( (new Add(new Mult(new Num(5), new Let("x", new Num(5), new Mult(new Var("x"), new Num(2)))), new Num(1)))-> pretty_print_to_string()
               == "5 * (_let x = 5\n"
                  "     _in  x * 2) + 1");
        CHECK( (new Mult((new Add(new Mult(new Num(5), new Let("x", new Num(5), new Mult(new Var("x"), new Num(2)))), new Num(1))), new Num(7)))-> pretty_print_to_string()
               == "(5 * (_let x = 5\n"
                  "      _in  x * 2) + 1) * 7");
        CHECK( (new Let("x", new Num(10), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10)))))
                       ->pretty_print_to_string()  == "_let x = 10\n"
                                                      "_in  (x * (10 * 10) * 10) * 10 * 10");
        CHECK( (new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Num(10), new Num(10)))))
                        -> pretty_print_to_string() == "_let x = 1\n"
                                                       "_in  (x * (10 * 10) * x) * 10 * 10" );
        CHECK( (new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Var("y"), new Num(10)))))
                       -> pretty_print_to_string() == "_let x = 1\n"
                                                      "_in  (x * (10 * 10) * x) * y * 10" );
    }
}

TEST_CASE("Let_equals_mine") {
        SECTION("Values_same") {
            REQUIRE( (new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) )));
        }
        SECTION("Values_same_different_rhs") {
            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("x", new Num(5), new Add(new Num(2), new Var("x")) )));
        }
        SECTION("Values_same_different_lhs") {
            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("y", new Num(4), new Add(new Num(2), new Var("x")) )));
        }
        SECTION("Values_same_different_body") {
            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("x", new Num(4), new Mult(new Num(3), new Var("y")) )));
        }
        SECTION("different_types") {
            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals( new Mult(new Num(3), new Var("y")) ));
        }
}

TEST_CASE("Let_has_variable_mine") {
    SECTION("has") {
        REQUIRE( (new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->has_variable());
    }
    SECTION("does_not_has") {
        REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Num(4)) ))->has_variable());
    }
}
TEST_CASE("Let_print_mine") {
    CHECK( (new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x")))) -> to_string()
                                                                                                        == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    CHECK( (new Let("x", new Num(1),
                   new Let("y", new Num(1),
                           new Let("z", new Add(new Var("x"), new Num(1)),
                                   new Mult(new Add(new Var("x"), new Var("y")), new Var("z")))))) -> to_string()
                                                                                                        == "(_let x=1 _in (_let y=1 _in (_let z=(x+1) _in ((x+y)*z))))");
}
TEST_CASE ("Let_interp_mine") {
    SECTION("hw_examples") {
        CHECK((new Add(new Mult(new Num(5), new Let("x", new Num(5), new Var("x"))), new Num(1))) -> interp() == 26);
        CHECK((new Mult(new Num(5), new Let("x", new Num(5), new Add(new Var("x"), new Num(1))))) -> interp() == 30);
    }
    SECTION("from_pretty_print_edge") {
        CHECK( (new Let("x", new Num(1),
                       new Let("y", new Num(1),
                               new Let("z", new Add(new Var("x"), new Num(1)),
                                       new Mult(new Add(new Var("x"), new Var("y")), new Var("z")))))) -> interp() == 4);
        CHECK( (new Mult((new Add(new Mult(new Num(5), new Let("x", new Num(5), new Mult(new Var("x"), new Num(2)))), new Num(1))), new Num(7))) -> interp() == 357); // 51 * 7
        CHECK( (new Let("x", new Num(10), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10)))))
                      ->interp()  == 1000000);
        CHECK( (new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Num(10), new Num(10)))))
                       ->interp()  == 10000);
        CHECK_THROWS_WITH( ((new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Var("y"), new Num(10)))))
                       -> interp() == 10000), "no value for variable");
    }
    SECTION("bypass_middle_let") {
        CHECK ((new Let("x", new Num(2), new Let("z", new Num(4), new Add(new Var("x"), new Num(10)))))
                -> interp() == 12);
    }
}



/*
 Jinny's test
 */

TEST_CASE("Let_pretty_print_to_string (includes pretty_print)"){
    
    SECTION("let in left"){
        CHECK((new Add (new Let("x",new Num(5), new Var("x")) , new Num(1)))
              ->pretty_print_to_string() == "(_let x = 5"    "\n"
                                         " _in  x) + 1");
        CHECK((new Mult( new Num(5), new Let("x",new Num(5), new Add(new Var("x"),new Num(1)))))
              ->pretty_print_to_string() == "5 * _let x = 5"   "\n"
                                         "    _in  x + 1");
    }
    
    SECTION("let in right"){
        CHECK((new Add (new Num(1), new Let("x",new Num(5), new Var("x"))))
              ->pretty_print_to_string() == "1 + _let x = 5"    "\n"
                                         "    _in  x");
    }
    
    SECTION("let in middle"){
        CHECK((new Add (new Mult( new Num(5), new Let("x",new Num(5), new Var("x"))) , new Num(2)))
              ->pretty_print_to_string() == "5 * (_let x = 5"   "\n"
                                         "     _in  x) + 2");
        CHECK((new Mult (new Mult( new Num(5), new Let("x",new Num(5), new Var("x"))) , new Num(2)))
              ->pretty_print_to_string() == "(5 * _let x = 5"   "\n"
                                         "     _in  x) * 2");
        CHECK((new Mult (new Add( new Num(5), new Let("x",new Num(5), new Var("x"))) , new Num(2)))
              ->pretty_print_to_string() == "(5 + _let x = 5"   "\n"
                                         "     _in  x) * 2");
        CHECK((new Mult (new Add(new Let("x",new Num(5), new Var("x")), new Num(5)) , new Num(2)))
              ->pretty_print_to_string() == "((_let x = 5"   "\n"
                                         "  _in  x) + 5) * 2");
    }
    
    SECTION("nested"){
        
        CHECK(( new Let("x", new Num(5), new Let("x", new Num(3), new Add(new Var("x"), new Num(1)))) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  _let x = 3"   "\n"
                                         "     _in  x + 1");
        CHECK(( new Let("x", new Num(5) , new Let("x", new Add( new Var("x"), new Num(5)), new Add(new Var("x"), new Num(1)))) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  _let x = x + 5"   "\n"
                                         "     _in  x + 1");
        
        CHECK(( new Let("x", new Num(5), new Add(new Let("x", new Num(3), new Add(new Var("x"), new Num(1))), new Num(2)) ) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  (_let x = 3"   "\n"
                                         "      _in  x + 1) + 2");
        
        CHECK(( new Let("x", new Num(5), new Add(new Num(2), new Let("x", new Num(3), new Add(new Var("x"), new Num(1))) ) ) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  2 + _let x = 3"   "\n"
                                         "         _in  x + 1");
    }
}


TEST_CASE("Let_all methods"){
    
    SECTION("toString (includes print)"){
        CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(1))) )
              ->to_string() == "(_let x=5 _in (x+1))");
        CHECK((new Let("x", new Add(new Num(5), new Num(2)) , new Add(new Var("x"), new Num(1))) )
              ->to_string() == "(_let x=(5+2) _in (x+1))");
        CHECK( (new Let("x", new Num(5), new Let("x", new Num(6), new Add(new Var("x"), new Num(1)))))
              ->to_string() == "(_let x=5 _in (_let x=6 _in (x+1)))" );
        CHECK( (new Let("x", new Num(5), new Let("y", new Num(6), new Add(new Var("x"), new Num(1)))))
              ->to_string() == "(_let x=5 _in (_let y=6 _in (x+1)))" );
        CHECK( (new Let("x", new Num(5), new Let("x", new Add(new Var("x"),new Num(2)) , new Add(new Var("x"), new Num(1)))))
              ->to_string() == "(_let x=5 _in (_let x=(x+2) _in (x+1)))" );
        CHECK( (new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
              ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))" );
    }
    SECTION("interp"){
        CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(1))) )
              ->interp() == 6);
        CHECK((new Let("x", new Add(new Num(5), new Num(2)) , new Add(new Var("x"), new Num(1))) )
              ->interp() == 8);
        CHECK( (new Let("x", new Num(5), new Let("x", new Num(6), new Add(new Var("x"), new Num(1)))))
              ->interp() == 7 );
        CHECK( (new Let("x", new Num(5), new Let("y", new Num(6), new Add(new Var("x"), new Num(1)))))
              ->interp() == 6 );
        CHECK( (new Let("x", new Num(5), new Let("x", new Add(new Var("x"),new Num(2)) , new Add(new Var("x"), new Num(1)))))
              ->interp() == 8 );
        CHECK( (new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
              ->interp() == 10 );
    }
    SECTION("equal"){
        CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(1))) )
              ->equals(new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))));
        CHECK_FALSE((new Let("x", new Num(5), new Add(new Var("x"), new Num(1))) )
                    ->equals(new Let("y", new Num(5), new Add(new Var("x"), new Num(1)))));
        CHECK_FALSE((new Let("x", new Num(5), new Add(new Var("x"), new Num(1))) )
                    ->equals(new Add(new Num(5), new Add(new Var("x"), new Num(1)))));
    }
    SECTION("has_variable"){
        CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(1))) )
              ->has_variable());
        CHECK((new Let("x", new Add(new Var("x"),new Num(2)) , new Add(new Num(3), new Num(1))))
              ->has_variable());
        CHECK_FALSE((new Let("x", new Num(5) , new Add(new Num(3), new Num(1))))
              ->has_variable());
    }
    SECTION("subst"){
        CHECK( (new Let("x", new Num(5), new Add(new Var("x"), new Num(1))))
              ->subst("x", new Num(10))->interp() == 6 );
        CHECK( (new Let("y", new Num(5), new Add(new Var("x"), new Num(1))))
              ->subst("x", new Num(10))->interp() == 11 );
    }
}



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
        CHECK_FALSE( var -> equals(num) );
    }
    
    SECTION("add_mult"){
        Add* add = new Add( new Num(3), new Num(0) );
        Mult* mult = new Mult ( new Num(3), new Num(0) );
        CHECK_FALSE( add -> equals(mult) );
    }
    
    SECTION("null"){
        CHECK_FALSE( (new Add( new Num(3), new Num(0))) -> equals(NULL) );
        CHECK_FALSE( (new Mult( new Num(3), new Num(0))) -> equals(NULL) );
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

TEST_CASE("Test_toString (includes print)"){
    SECTION("expr_number"){
        CHECK( (new Num(10))->to_string() == "10" );
    }
    SECTION("expr_add"){
        CHECK( (new Add( new Num(2), new Num(3)))->to_string() == "(2+3)" );
        CHECK( (new Add( new Num(-2), new Num(-3)))->to_string() == "(-2+-3)" );
        CHECK( (new Add( (new Add( new Num(1), new Num(2))), new Num(3) ) )->to_string() == "((1+2)+3)" );
        CHECK( (new Add( new Num(1), (new Add( new Num(2), new Num(3))) ) )->to_string() == "(1+(2+3))" );
        CHECK( (new Add( new Var("cat"), (new Add( new Num(1), new Num(2))) ) )->to_string() == "(cat+(1+2))" );
    }
    SECTION("expr_mult"){
        CHECK( (new Mult( new Num(10), new Num(3)))->to_string() == "(10*3)" );
    }
    SECTION("expr_var"){
        CHECK( (new Var("cat"))->to_string() == "cat" );
    }
}


TEST_CASE("Test_pretty_print_to_string (includes pretty_print)"){
        
    SECTION("expr_add_only"){
        
        // test (1 + 2) + 3
        CHECK( ( new Add( new Add(new Num(1),new Num(2)) , new Num(3) ) )
              ->pretty_print_to_string() == "(1 + 2) + 3" );
        // test 1 + (2 + 3)
        CHECK( ( new Add( new Num(1) , new Add(new Num(2),new Num(3)) ) )
              ->pretty_print_to_string() == "1 + 2 + 3" );
        // test 1 + (2 + (3 + (4 + 5)))
        CHECK( ( new Add( new Num(1) , new Add( new Num(2) , new Add( new Num(3) , new Add(new Num(4),new Num(5))))))
              ->pretty_print_to_string() == "1 + 2 + 3 + 4 + 5");
        // test 1 + (((2 + 3) + 4) + 5)
        CHECK( ( new Add( new Num(1) , new Add( new Add( new Add( new Num(2) , new Num(3)) , new Num(4)) , new Num(5))))
              ->pretty_print_to_string() == "1 + ((2 + 3) + 4) + 5");
    }
    
    SECTION("expr_mult_only"){
        
        // test (1 * 2) * 3
        CHECK( ( new Mult( new Mult(new Num(1),new Num(2)) , new Num(3) ) )
              ->pretty_print_to_string() == "(1 * 2) * 3" );
        // test 1 + (2 + 3)
        CHECK( ( new Mult( new Num(1) , new Mult(new Num(2),new Num(3)) ) )
              ->pretty_print_to_string() == "1 * 2 * 3" );
        // test 1 + (2 + (3 + (4 + 5)))
        CHECK( ( new Mult( new Num(1) , new Mult( new Num(2) , new Mult( new Num(3) , new Mult(new Num(4),new Num(5))))))
              ->pretty_print_to_string() == "1 * 2 * 3 * 4 * 5");
        // test 1 + (((2 + 3) + 4) + 5)
        CHECK( ( new Mult( new Num(1) , new Mult( new Mult( new Mult( new Num(2) , new Num(3)) , new Num(4)) , new Num(5))))
              ->pretty_print_to_string() == "1 * ((2 * 3) * 4) * 5");
        // test (10 * ((10 * 10) * 10)) * (10 * 10)
        CHECK((new Mult( new Mult(new Num(10), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10))))
              ->pretty_print_to_string()  == "(10 * (10 * 10) * 10) * 10 * 10");
    }
    
    SECTION("expr_add_mult_mix"){
        
        // test 1 + (2 * 3)
        CHECK( ( new Add( new Num(1) , new Mult(new Num(2),new Num(3)) ) )
              ->pretty_print_to_string() == "1 + 2 * 3" );
        // test (1 * 2) + 3
        CHECK( ( new Add( new Mult(new Num(1),new Num(2)) , new Num(3) ) )
              ->pretty_print_to_string() == "1 * 2 + 3");
        // test 1 * (2 + 3)
        CHECK( ( new Mult( new Num(1) , new Add(new Num(2),new Num(3)) ) )
              ->pretty_print_to_string() == "1 * (2 + 3)");
        // test (9 * (4 + 3)) + ((2 * 4) + 1)
        CHECK((new Add(new Mult(new Num(9), new Add(new Num(4), new Num(3))), new Add(new Mult(new Num(2), new Num(4)), new Num(1))))
              ->pretty_print_to_string() == "9 * (4 + 3) + 2 * 4 + 1");
    }
    
    SECTION("expr_number"){
        CHECK(( new Num(3) )-> pretty_print_to_string() == "3");
        CHECK(( new Num(-3) )-> pretty_print_to_string() == "-3");
    }
    
    SECTION("expr_var"){
        CHECK(( new Var("3") )-> pretty_print_to_string() == "3");
        CHECK(( new Var("variable") )-> pretty_print_to_string() == "variable");
    }
    
}
