//
//  val.cpp
//  6015HW
//
//  Created by Jinny Jeng on 3/7/23.
//

#include "val.hpp"
#include "expr.hpp"


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

Val* NumVal::add_to(Val* other_val){
    NumVal* other_num = dynamic_cast<NumVal*>(other_val);
      if (other_num == NULL)
          throw std::runtime_error("add of non-number");
      return new NumVal(rep + other_num->rep);
}

Val* NumVal::mult_with(Val* other_val){
    NumVal* other_num = dynamic_cast<NumVal*>(other_val);
      if (other_num == NULL)
          throw std::runtime_error("mult of non-number");
      return new NumVal(rep * other_num->rep);
}

std::string NumVal::to_string(){
    return std::to_string(rep);
}


//BoolVal::BoolVal(std::string rep){
//    this->rep = rep;
//}
//
//Val* BoolVal::add_to(Val* other_val){
//    throw std::runtime_error("add of non-number");
//}
//
//Val* BoolVal::mult_with(Val* other_val){
//    throw std::runtime_error("mult of non-number");
//}
