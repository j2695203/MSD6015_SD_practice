//
//  cmdline.cpp
//  HW1_MSDscriptCommandLine
//
//  Created by Jinny Jeng on 1/16/23.
//
#define CATCH_CONFIG_RUNNER

#include "cmdline.hpp"


void use_arguments(int argc, const char * argv[]){
    
    bool isTested = false;
    
    for(int i = 1; i < argc; i++ ){
        
        if( strcmp(argv[i], "--help") == 0 ){
            std::cout << "Accepted arguments: --test --help \n";
            exit(0);
        }else if( strcmp(argv[i], "--test") == 0 && !isTested ){
            isTested = true;
            if( Catch::Session().run(1, argv) != 0){
                exit(1);
            }
        }else if ( strcmp(argv[i], "--test") == 0 && isTested ){
            std::cerr << "Error: Already tested. \n";
            exit(1);
        }else{
            std::cerr << "Error: Not an accepted argument. \n";
            exit(1);
        }
    }
    
    return;
}
