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
#include "val.hpp"
#include "expr.hpp"
#include "cmdline.hpp"
#include "parse.hpp"
#include <cassert>

#include "catch.h"


int main(int argc, const char ** argv) {
    
    try{
        run_mode_t runmode = use_arguments(argc, argv);
        
        switch(runmode){
                
            case do_nothing:
                break;
                
            case do_interp:
//                std::cout << "Value: " << parse_expr(std::cin)->interp() << "\n";
                std::cout << parse_expr(std::cin)->interp()->to_string() << "\n";
                exit(0);
                
            case do_print:
//                std::cout << "Expression: ";
                parse_expr(std::cin)->print(std::cout);
                std::cout << "\n";
                exit(0);
                
            case do_pretty_print:
//                std::cout << "Expression: \n" << parse_expr(std::cin)->pretty_print_to_string() << "\n";
                std::cout << parse_expr(std::cin)->pretty_print_to_string() << "\n";
                exit(0);
        }
        
        
        return 0;
        
    }catch( std::runtime_error exn){
        
        std::cerr << exn.what() << "\n";
        return 1;
    }
}


