//
//  run.cpp
//  6015HW_coverage
//
//  Created by Jinny Jeng on 2/13/23.
//

extern "C" {
    #include "run.hpp"
};

#define CATCH_CONFIG_RUNNER
#include "catch.h"

bool run_tests() {
    const char *argv[] = { "test" };
    return (Catch::Session().run(1, argv) == 0);
}
