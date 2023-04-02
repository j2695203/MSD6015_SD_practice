/**
* \file cmdline.h
* \brief cmdline class
*
* Longer description goes here.
*/

#ifndef cmdline_hpp
#define cmdline_hpp

#include <stdio.h>
#include <iostream>

#include "catch.h"
#include "pointer.h"


typedef enum {
    do_nothing,
    do_interp,
    do_print,
    do_pretty_print
} run_mode_t;

run_mode_t use_arguments(int argc, const char **argv);

#endif /* cmdline_hpp */
