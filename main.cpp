//
//  main.cpp
//  HW2_ExpressionClasses
//
//  Created by Jinny Jeng on 1/17/23.
//

/**
* \mainpage MSDScript
* \author Jinny
*\date 01-17-2023
*/

#include <iostream>
#include "expr.hpp"
#include "cmdline.hpp"
#include <cassert>

#include "catch.h"


int main(int argc, const char * argv[]) {
    
    use_arguments(argc, argv);
    
    (new Let("x", new Num(5), new Add(new Var("x"), new Num(1))) )->print(std::cout);
    
    std::cout<<std::endl;
    
    std::cout<< (new Let("x", new Num(5), new Add(new Var("x"), new Num(1))) )->interp();
    
    std::cout<<std::endl;
    
    return 0;;
    
}
