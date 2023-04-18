#ifndef env_hpp
#define env_hpp

#include <stdio.h>
#include <string>
#include "pointer.h"

class Val;

CLASS(Env){
public:
    virtual PTR(Val) lookup(std::string find_name) = 0;
    static PTR(Env) empty;
};


class EmptyEnv : public Env {
public:
    EmptyEnv();
    
    PTR(Val) lookup(std::string find_name);
};


class ExtendedEnv : public Env {
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;
    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    
    PTR(Val) lookup(std::string find_name);
};

#endif /* env_hpp */
