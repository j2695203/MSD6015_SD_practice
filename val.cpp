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

bool NumVal::equals(Val* e){
    NumVal* n = dynamic_cast<NumVal*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->rep == n->rep );
    }
}

Expr* NumVal::to_expr(){
    return new NumExpr(rep);
}

std::string NumVal::to_string(){
    return std::to_string(rep);
}

Val* NumVal::add_to(Val* other_val){
    NumVal* other_num = dynamic_cast<NumVal*>(other_val);
      if (other_num == NULL)
          throw std::runtime_error("add of non-number");
      return new NumVal((unsigned)rep + (unsigned)other_num->rep);
}

Val* NumVal::mult_with(Val* other_val){
    NumVal* other_num = dynamic_cast<NumVal*>(other_val);
      if (other_num == NULL)
          throw std::runtime_error("mult of non-number");
      return new NumVal((unsigned)rep * (unsigned)other_num->rep);
}

bool NumVal::is_true(){
    throw std::runtime_error("no boolean for NumVal");
}

Val* NumVal::call(Val *actual_arg){
    throw std::runtime_error("no call for NumVal");
}



/*
 *** Class BoolVal ************************************************
 */

BoolVal::BoolVal(bool rep){
    this->rep = rep;
}

bool BoolVal::equals(Val* e){
    BoolVal* n = dynamic_cast<BoolVal*>(e);
    if(n == NULL){
        return false;
    }else{
        return( this->rep == n->rep );
    }
}

Expr* BoolVal::to_expr(){
    return new BoolExpr(rep);
}

std::string BoolVal::to_string(){
    if(rep)
        return "_true";
    else
        return "_false";
}

Val* BoolVal::add_to(Val* other_val){
    throw std::runtime_error("add of non-number");
}

Val* BoolVal::mult_with(Val* other_val){
    throw std::runtime_error("mult of non-number");
}

bool BoolVal::is_true(){
    return rep;
}

Val* BoolVal::call(Val *actual_arg){
    throw std::runtime_error("no call for BoolVal");
}

/*
 *** Class FunVal ************************************************
 */

FunVal::FunVal(std::string formal_arg, Expr *body){
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunVal::equals(Val* e){
    FunVal* n = dynamic_cast<FunVal*>(e);
    if(n == NULL){
        return false;
    }else{
        return( formal_arg == n->formal_arg && body->equals(n->body) );
    }
}

Expr* FunVal::to_expr(){
    return new FunExpr(formal_arg, body);
}

std::string FunVal::to_string(){
    throw std::runtime_error("no to_string for FunVal");
}

Val* FunVal::add_to(Val* other_val){
    throw std::runtime_error("no add_to for FunVal");
}

Val* FunVal::mult_with(Val* other_val){
    throw std::runtime_error("no mult_with for FunVal");
}

bool FunVal::is_true(){
    throw std::runtime_error("no is_true for FunVal");
}

Val* FunVal::call(Val *actual_arg){
    return (body->subst(formal_arg, actual_arg->to_expr()))->interp();
}
