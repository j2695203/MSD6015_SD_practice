//
//  val.cpp
//  6015HW
//
//  Created by Jinny Jeng on 3/7/23.
//

#include "val.hpp"
#include "expr.hpp"

/*
 *** Class NumVal ************************************************
 */

NumVal::NumVal(int rep){
    this->rep = rep;
}

bool NumVal::equals(PTR(Val) e){
    PTR(NumVal) n = CAST(NumVal)(e);
    if(n == NULL){
        return false;
    }else{
        return( this->rep == n->rep );
    }
}

PTR(Expr) NumVal::to_expr(){
    return NEW(NumExpr)(rep);
}

std::string NumVal::to_string(){
    return std::to_string(rep);
}

PTR(Val) NumVal::add_to(PTR(Val) other_val){
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
      if (other_num == NULL)
          throw std::runtime_error("add of non-number");
      return NEW(NumVal)((unsigned)rep + (unsigned)other_num->rep);
}

PTR(Val) NumVal::mult_with(PTR(Val) other_val){
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
      if (other_num == NULL)
          throw std::runtime_error("mult of non-number");
      return NEW(NumVal)((unsigned)rep * (unsigned)other_num->rep);
}

bool NumVal::is_true(){
    throw std::runtime_error("no boolean for NumVal");
}

PTR(Val) NumVal::call(PTR(Val)actual_arg){
    throw std::runtime_error("no call for NumVal");
}



/*
 *** Class BoolVal ************************************************
 */

BoolVal::BoolVal(bool rep){
    this->rep = rep;
}

bool BoolVal::equals(PTR(Val) e){
    PTR(BoolVal) n = CAST(BoolVal)(e);
    if(n == NULL){
        return false;
    }else{
        return( this->rep == n->rep );
    }
}

PTR(Expr) BoolVal::to_expr(){
    return NEW(BoolExpr)(rep);
}

std::string BoolVal::to_string(){
    if(rep)
        return "_true";
    else
        return "_false";
}

PTR(Val) BoolVal::add_to(PTR(Val) other_val){
    throw std::runtime_error("add of non-number");
}

PTR(Val) BoolVal::mult_with(PTR(Val) other_val){
    throw std::runtime_error("mult of non-number");
}

bool BoolVal::is_true(){
    return rep;
}

PTR(Val) BoolVal::call(PTR(Val)actual_arg){
    throw std::runtime_error("no call for BoolVal");
}

/*
 *** Class FunVal ************************************************
 */

FunVal::FunVal(std::string formal_arg, PTR(Expr)body){
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunVal::equals(PTR(Val) e){
    PTR(FunVal) n = CAST(FunVal)(e);
    if(n == NULL){
        return false;
    }else{
        return( formal_arg == n->formal_arg && body->equals(n->body) );
    }
}

PTR(Expr) FunVal::to_expr(){
    return NEW(FunExpr)(formal_arg, body);
}

std::string FunVal::to_string(){
    throw std::runtime_error("no to_string for FunVal");
}

PTR(Val) FunVal::add_to(PTR(Val) other_val){
    throw std::runtime_error("no add_to for FunVal");
}

PTR(Val) FunVal::mult_with(PTR(Val) other_val){
    throw std::runtime_error("no mult_with for FunVal");
}

bool FunVal::is_true(){
    throw std::runtime_error("no is_true for FunVal");
}

PTR(Val) FunVal::call(PTR(Val)actual_arg){
    return (body->subst(formal_arg, actual_arg->to_expr()))->interp();
}
