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
    
    std::cout<< ( new Let("x", new Num(5), new Add(new Num(2), new Let("x", new Num(3), new Add(new Var("x"), new Num(1))) ) ) )
    ->pretty_print_to_string();
    
    std::cout<<std::endl<<std::endl;
    
    std::cout<< (new Mult (new Add(new Let("x",new Num(5), new Var("x")), new Num(5)) , new Num(2)))
    ->pretty_print_to_string();
    
    std::cout<<std::endl<<std::endl;
    
    std::cout<< ( new Let("x", new Num(5), new Add(new Let("x", new Num(3), new Add(new Var("x"), new Num(1))), new Num(2)) ) )
    ->pretty_print_to_string();
    
    std::cout<<std::endl<<std::endl;
    
    /*
     _let x = 5
     _in _let x = 3
         _in x + 1
     = 4
     */
    std::cout<<( new Let("x", new Num(5), new Let("x", new Num(3), new Add(new Var("x"), new Num(1)))) )
    ->pretty_print_to_string();
    
    std::cout<<std::endl<<std::endl;
    
    std::cout<<( new Let("x", new Num(5), new Let("x", new Num(3), new Add(new Var("x"), new Num(1)))) )
    ->interp();
    
    std::cout<<std::endl<<std::endl;
    
    
    /*
     5 * (_let x = 5
          _in x) + 1
     = 26
     */
    std::cout<<(new Add (new Mult( new Num(5), new Let("x",new Num(5), new Var("x"))) , new Num(1)))
    ->pretty_print_to_string();
    
    std::cout<<std::endl<<std::endl;
    
    std::cout<<(new Add (new Mult( new Num(5), new Let("x",new Num(5), new Var("x"))) , new Num(1)))
    ->interp();
    
    std::cout<<std::endl<<std::endl;
    
    
    /*
     (5 * (_let x = 5
           _in x)) * 1
     = 25
     */
    std::cout<<(new Mult (new Mult( new Num(5), new Let("x",new Num(5), new Var("x"))) , new Num(1)))
    ->pretty_print_to_string();
    
    std::cout<<std::endl<<std::endl;
    
    std::cout<<(new Mult (new Mult( new Num(5), new Let("x",new Num(5), new Var("x"))) , new Num(1)))
    ->interp();
    
    std::cout<<std::endl<<std::endl;
    
    
    /*
     (5 + _let x = 5
          _in x) * 1

     = 10
     */
    std::cout<<(new Mult (new Add( new Num(5), new Let("x",new Num(5), new Var("x"))) , new Num(1)))
    ->pretty_print_to_string();
    
    std::cout<<std::endl<<std::endl;
    
    std::cout<<(new Mult (new Add( new Num(5), new Let("x",new Num(5), new Var("x"))) , new Num(1)))
    ->interp();
    
    std::cout<<std::endl<<std::endl;
    
    
    /*
     (_let x = 5
      _in x) + 1

     = 6
     */
    
    std::cout<<(new Add (new Let("x",new Num(5), new Var("x")) , new Num(1)))
    ->pretty_print_to_string();
    
    std::cout<<std::endl<<std::endl;
    
    std::cout<<(new Add (new Let("x",new Num(5), new Var("x")) , new Num(1)))
    ->interp();
    
    std::cout<<std::endl<<std::endl;
    
    
    /*
     5 * _let x = 5
         _in x + 1

     = 30
     */

    std::cout<<(new Mult( new Num(5), new Let("x",new Num(5), new Add(new Var("x"),new Num(1)))))
    ->pretty_print_to_string();
    
    std::cout<<std::endl<<std::endl;
    
    std::cout<<(new Mult( new Num(5), new Let("x",new Num(5), new Add(new Var("x"),new Num(1)))))
    ->interp();
    
    std::cout<<std::endl<<std::endl;
    
    
    
    return 0;;
    
}
