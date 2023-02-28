//
//  testDriver.cpp
//  6015HW
//
//  Created by Jinny Jeng on 2/25/23.
//
#include <iostream>
#include "testDriver.hpp"

std::string random_expr_var() {
    if ( (rand() % 2) < 1 ) return "x";
    else return "y";
}

std::string random_op() {
    if ( (rand() % 2) < 1 ) return "+";
    else return "*";
}

// generate random expressions
std::string random_expr_string() {
    
    int r = (rand() % 100);
    
    if ( r < 45 ) // numExpr
        return std::to_string(rand()%100);
    else if ( r < 50 ) // negative numExpr
        return "-" + std::to_string(rand()%100);
    else if ( r < 52 ) // varExpr
        return random_expr_var();
    else if ( r < 55 )
        return "(" + random_expr_string() + ")";
    else if ( r < 98 ) // addExpr, multExpr
        return random_expr_string() + random_op() + random_expr_string();
    else
        return "_let " + random_expr_var() + " = " + random_expr_string() + " _in " + random_expr_string();

}

int main(int argc, char **argv){
    
    int iter = 100;
    srand(clock());
    
    try{
        // test the program itself
        if( argc == 2 ){

            const char * const interp_argv[] = { argv[1] , "--interp" };
            const char * const print_argv[] = { argv[1] , "--print" };

            for (int i = 0; i < iter; i++){
                std::string in = random_expr_string();
                std::cout << "Trying " << in << "\n";
                
                ExecResult interp_result = exec_program(2, interp_argv, in);
                ExecResult print_result = exec_program(2, print_argv, in);
                
                ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
                
                if (interp_again_result.out != interp_result.out){
                    std::cout << "\n";
                    std::cout << "Output1 (interp): " << interp_result.out;
                    std::cout << "Output2-1(print): " << print_result.out;
                    std::cout << "Output2-2(interpagain): " << interp_again_result.out;
                    std::cout << "\n";
                    
                    throw std::runtime_error("different results");
                }
            }
            
        // test two programs
        }else if( argc == 3 ){
            
            const char * const interp1_argv[] = { argv[1], "--interp" };
            const char * const interp2_argv[] = { argv[2], "--interp" };
            
            const char * const print1_argv[] = { argv[1], "--print" };
            const char * const print2_argv[] = { argv[2], "--print" };
            
            const char * const pretty_print1_argv[] = { argv[1], "--pretty-print" };
            const char * const pretty_print2_argv[] = { argv[2], "--pretty-print" };

            for (int i = 0; i < iter; i++){
                
                std::string in = random_expr_string();
                std::cout << "\n\nTrying " << in << "\n";
                
                ExecResult interp1_result = exec_program(2, interp1_argv, in);
                ExecResult interp2_result = exec_program(2, interp2_argv, in);
                
                ExecResult print1_result = exec_program(2, print1_argv, in);
                ExecResult print2_result = exec_program(2, print2_argv, in);
                
                ExecResult pretty_print1_result = exec_program(2, pretty_print1_argv, in);
                ExecResult pretty_print2_result = exec_program(2, pretty_print2_argv, in);
                
                if (interp1_result.out != interp2_result.out){
                    std::cout << "\n";
                    std::cout << "Output1(interp): " << interp1_result.out;
                    std::cout << "Output2(interp): " << interp2_result.out;
                    throw std::runtime_error("different results: --interp");
                }
                if (print1_result.out != print2_result.out){
                    std::cout << "\n";
                    std::cout << "Output1(print): " << print1_result.out;
                    std::cout << "Output2(print): " << print2_result.out;
                    throw std::runtime_error("different results: --print");
                }
                if (pretty_print1_result.out != pretty_print2_result.out){
                    std::cout << "\n";
                    std::cout << "Output1(prt_print): " << pretty_print1_result.out;
                    std::cout << "Output2(prt_print): " << pretty_print2_result.out;
                    throw std::runtime_error("different results: --pretty-print");
                }
            }
        }
        else{
            throw std::runtime_error("invalid test input");
        }
        
    }catch( std::runtime_error exn){
        
        std::cerr << exn.what() << "\n";
        return 1;
    }
    
    return 0;
}

