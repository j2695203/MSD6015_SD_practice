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
    return new Num(n);
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
    return new Var(s);
}

Expr* parse_expr(std::istream &in){
    Expr* e = parse_addend(in);
    
    skip_whitespace(in);
    
    int c = in.peek();
    if( c == '+'){
        consume(in, c);
        Expr* rhs = parse_expr(in);
        return new Add(e, rhs);
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
        return new Mult(e, rhs);
    }else if( (c >= 32 && c <= 126) && c != '+' && c != ')' && c != '_' ){ // no other character
        throw std::runtime_error("invalid input");
    }else{
        return e;
    }
}

/**
 num, (expr), var, _let var = expr _in expr
 */
Expr* parse_multicand(std::istream &in){  // = expr
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
        return parse_let(in);
    }else{
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

Expr* parse_let(std::istream &in){
    
    skip_whitespace(in);
    
    // _let
    int c = in.peek();
    if ( c == '_' ){
        consume(in, c);
        consume(in,'l');
        consume(in,'e');
        consume(in,'t');
        skip_whitespace(in);
        
        // variable (string)
        std::string str;
        c = in.peek();
        if( !isalpha(c) ){
            throw std::runtime_error("invalid input");
        }
        while(1){
            c = in.peek();
            if( isalpha(c) ){
                consume(in, c);
                str += c;
            }else{
                break;
            }
        }
        skip_whitespace(in);
        
        // "="
        c = in.get();
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
        return new Let(str, rhs, body);
        
    }else{ // (multicand will check if it's _let first, so never enter this)
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

// for test
Expr* parse_str(std::string str){
    // put the test string into stream
    std::istringstream in(str) ;
    Expr* e = parse_addend(in);
    
    skip_whitespace(in);
    
    int c = in.peek();
    if( c == '+'){
        consume(in, c);
        Expr* rhs = parse_expr(in);
        return new Add(e, rhs);
    }else{
        return e;
    }
}
