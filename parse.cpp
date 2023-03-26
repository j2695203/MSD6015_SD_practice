//
//  parse.cpp
//  6015HW
//
//  Created by Jinny Jeng on 2/20/23.
//
#include <iostream>
#include "parse.hpp"


static void consume(std::istream &in, int expect){
    int c = in.get();
    if( c != expect ){
        throw std::runtime_error("consume mismatch");
    }
}

static void skip_whitespace(std::istream &in){
    while(1){
        int c = in.peek();
        if( !isspace(c) ){
            break;
        }
        consume(in, c);
    }
}

Expr* parse_expr(std::istream &in){
    Expr* e = parse_comparg(in);
    skip_whitespace(in);
    
    int c = in.peek();
    
    if( isdigit(c) || isalpha(c) ){
        throw std::runtime_error("invalid input");
    }else if( c == '='){
        consume(in, c);
        consume(in, '=');
        Expr* rhs = parse_expr(in);
        return new EqExpr(e, rhs);
    }else{
        return e;
    }
}

Expr* parse_comparg(std::istream &in){
    Expr* e = parse_addend(in);
    
    skip_whitespace(in);
    
    int c = in.peek();
    if( c == '+'){
        consume(in, c);
        Expr* rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
    }else{
        return e;
    }
}

Expr* parse_addend(std::istream &in){
    Expr* e = parse_multicand(in);
    
    skip_whitespace(in);
    
    int c = in.peek();
    if( c == '*'){
        consume(in, c);
        Expr* rhs = parse_addend(in);
        return new MultExpr(e, rhs);
//    }else if( (c >= 32 && c <= 126) && c != '+' && c != ')' && c != '_' ){ // no other character
//        throw std::runtime_error("invalid input");
    }else{
        return e;
    }
}

Expr* parse_multicand(std::istream &in){
    Expr* e = parse_inner(in);
    
    skip_whitespace(in);
    
    while( in.peek() == '('){
        consume(in, '(');
        Expr* actual_arg = parse_expr(in);
        consume(in, ')');
        e = new CallExpr(e, actual_arg);
    }
    return e;
}

/**
 num, (expr), var, _let var = expr _in expr
 */
Expr* parse_inner(std::istream &in){
    // skip whitespace
    skip_whitespace(in);
    
    int c = in.peek();
    
    if( (c == '-') || isdigit(c) ){
        return parse_num(in);
        
    }else if ( c == '(' ){
        consume(in, c);
        Expr* e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if( c != ')' ){
            throw std::runtime_error("missing close parenthesis");
        }
        return e;
        
    }else if( isalpha(c) ){
        return parse_var(in);
        
    }else if( c == '_' ){
        consume(in, c);
        
        std::string keyword = parse_var(in)->to_string();
        
        if( keyword == "let" ) // let
            return parse_let(in);
        else if( keyword == "true" ) // true
            return new BoolExpr(true);
        else if( keyword == "false" ) // false
            return new BoolExpr(false);
        else if( keyword == "if" ) // if
            return parse_if(in);
        else if( keyword == "fun" ) // fun
            return parse_fun(in);
        else
            throw std::runtime_error("invalid input");
        
    }else{
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

Expr* parse_num(std::istream &in){
    // skip whitespace
    skip_whitespace(in);
    
    // check if negative
    bool negative = false;
    if( in.peek() == '-' ){
        consume(in, '-');
        negative = true;
    }
    
    // check is a digit after -
    int c = in.peek();
    if( !isdigit(c) ){
        throw std::runtime_error("invalid input");
    }
    
    // read digit number
    int n = 0;
    while(1){
        int c = in.peek();
        if( isdigit(c) ){
            consume(in, c);
            n = n*10 + (c-'0');
        }else{
            break;
        }
    }
    // add negative sign
    if( negative ){
        n = -n;
    }
    return new NumExpr(n);
}

Expr* parse_var(std::istream &in){
    // skip whitespace
    skip_whitespace(in);
    
    // read characters
    std::string s;
    while(1){
        int c = in.peek();
        if( isalpha(c) ){
            consume(in, c);
            s += c;
        }else if( c == '_' ){ // should not allow immediate _let
            throw std::runtime_error("invalid input");
        }else{
            break;
        }
    }
    return new VarExpr(s);
}


Expr* parse_let(std::istream &in){
    
    skip_whitespace(in);
    
    // variable (string)
    std::string str;
    str = parse_var(in)->to_string();
    skip_whitespace(in);
    
    // "="
    int c = in.get();
    if( c != '=' ){
        throw std::runtime_error("invalid input");
    }
    skip_whitespace(in);
    
    // rhs expression
    Expr* rhs = parse_expr(in);
    skip_whitespace(in);
    
    // "_in"
    c = in.get();
    if( c != '_' ){ // (rhs will deals with it first, so never enter this)
        throw std::runtime_error("invalid input");
    }
    consume(in, 'i');
    consume(in, 'n');
    skip_whitespace(in);
    
    // body expression
    Expr* body = parse_expr(in);
    
    // return let expression
    return new LetExpr(str, rhs, body);
}


Expr* parse_if(std::istream &in){
    
    skip_whitespace(in);
    
    // test expression
    Expr* test_part = parse_expr(in);
    skip_whitespace(in);
    
    // _then
    consume(in,'_');
    consume(in,'t');
    consume(in,'h');
    consume(in,'e');
    consume(in,'n');
    skip_whitespace(in);
    
    // then expression
    Expr* then_part = parse_expr(in);
    skip_whitespace(in);
    
    // _else
    consume(in,'_');
    consume(in,'e');
    consume(in,'l');
    consume(in,'s');
    consume(in,'e');
    skip_whitespace(in);
    
    // else expression
    Expr* else_part = parse_expr(in);
    skip_whitespace(in);
    
    // return if expression
    return new IfExpr(test_part, then_part, else_part);
    
}

Expr* parse_fun(std::istream &in){
    
    skip_whitespace(in);
    
    // "("
    if( in.get() != '(' ){
        throw std::runtime_error("invalid input");
    }
    skip_whitespace(in);
    
    // formal_arg expression
    std::string formal_arg = parse_var(in)->to_string();
    skip_whitespace(in);
    
    // ")"
    if( in.get() != ')' ){
        throw std::runtime_error("invalid input");
    }
    skip_whitespace(in);
    
    // body expression
    Expr* body = parse_expr(in);
    
    // return fun expression
    return new FunExpr(formal_arg, body);
}



// for test
Expr* parse_str(std::string str){
    // put the test string into stream
    std::istringstream in(str) ;
    Expr* e = parse_comparg(in);
    
    skip_whitespace(in);
    
    int c = in.peek();
    if( c == '='){
        consume(in, c);
        consume(in, '=');
        Expr* rhs = parse_expr(in);
        return new EqExpr(e, rhs);
    }else{
        return e;
    }
}
