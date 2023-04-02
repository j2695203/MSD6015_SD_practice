#include "val.hpp"
#include "expr.hpp"
#include "parse.hpp"
#include "cmdline.hpp"


/*
 *** TEST using Catch 2 framework ************************************************
 */
TEST_CASE("quiz function vals") {
    // 1
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                                 "_in f(5) ")->interp()->equals(NEW(NumVal)(6)));

    // 2
    CHECK(parse_str("_let f = _fun (x)\n"
                                 "           7\n"
                                 "_in f(5)")->interp()->equals(NEW(NumVal)(7)));

    // 3
    CHECK(parse_str("_let f = _fun (x)\n"
                                 "           _true\n"
                                 "_in f(5) ")->interp()->equals(NEW(BoolVal)(true)));

    // 4
    CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                             "           x + _true\n"
                                             "_in f(5) ")->interp(), "add of non-number");

    // 5
    CHECK(parse_str("_let f = _fun (x)\n"
                                 "           x + _true\n"
                                 "_in 5 + 1 ")->interp()->equals(NEW(NumVal)(6)));

    // 6
    CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                             "           7\n"
                                             "_in  f(5 + _true)")->interp(), "add of non-number");

    // 7
    CHECK_THROWS_WITH(parse_str("_let f = _fun (x) x+ 1\n"
                                 "_in f + 5")->interp(),"no add_to for FunVal");

    // 8
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _if _false\n"
                                 "    _then f(5)\n"
                                 "    _else f(6)")->interp()->equals(NEW(NumVal)(7)));

    // 9
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (y) y+ 2 \n"
                                 "_in _if _true\n"
                                 "    _then f(5)\n"
                                 "    _else g(5)")->interp()->equals(NEW(NumVal)(6)));

    // 10
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (y) y+ 2 \n"
                                 "_in f(g(5)) ")->interp()->equals(NEW(NumVal)(8)));

    // 11
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (y)\n"
                                 "              f(y + 2)\n"
                                 "_in g(5) ")->interp()->equals(NEW(NumVal)(8)));

    // 12
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (x)\n"
                                 "              f(2) + x\n"
                                 "_in g(5) ")->interp()->equals(NEW(NumVal)(8)));

    // 13
    CHECK_THROWS_WITH(parse_str("_let f = _fun (x) x+ 1 \n"
                                             "_in f 5 ")->interp(), "invalid input");

    // 14
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                                 "_in (f)(5) ")->interp()->equals(NEW(NumVal)(6)));

    // 15
    PTR(AddExpr) add_x_1 = NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1));
    PTR(FunVal) fun_val_x_add_x_1 = NEW(FunVal)("x", add_x_1);
    CHECK(parse_str("_fun (x) x+ 1 ")->interp()->equals(fun_val_x_add_x_1));

    //16
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                                 "_in f ")->interp()->equals(fun_val_x_add_x_1));

    // 17
    CHECK(parse_str("(_fun (x)\n"
                                 "   x + 1)(5)")->interp()->equals(NEW(NumVal)(6)));

    // 18
    CHECK(parse_str("_let f = _if _false\n"
                                 "            _then _fun (x)  \n"
                                 "                        x+ 1 \n"
                                 "           _else _fun (x)\n"
                                 "                       x+ 2\n"
                                 "_in f(5)")->interp()->equals(NEW(NumVal)(7)));

    // 19
    CHECK(parse_str("(_if _false \n"
                                 "  _then _fun (x)\n"
                                 "            x+ 1\n"
                                 "   _else _fun (x)\n"
                                 "                x + 2)(5)")->interp()->equals(NEW(NumVal)(7)));

    // 20
    CHECK(parse_str("_let f = _fun (g)\n"
                                 "           g(5)\n"
                                 "_in _let g = _fun (y)  \n"
                                 "             y + 2\n"
                                 "_in f(g) ")->interp()->equals(NEW(NumVal)(7)));

    // 21
    CHECK(parse_str("_let f = _fun (g)\n"
                                 "           g(5)\n"
                                 "_in f(_fun (y)\n"
                                 "        y + 2)")->interp()->equals(NEW(NumVal)(7)));

    // 22
    CHECK(parse_str("_let f = _fun (x)\n"
                                 "           _fun (y)\n"
                                 "x+ y _in (f(5))(1) ")->interp()->equals(NEW(NumVal)(6)));

    // 23
    CHECK(parse_str("_let f = _fun (x)\n"
                                 "           _fun (y)\n"
                                 "x+ y _in f(5)(1) ")->interp()->equals(NEW(NumVal)(6)));

    // 24
    CHECK(parse_str("_let f = _fun (x)\n"
                                 "           _fun (g)\n"
                                 "             g(x + 1)\n"
                                 "_in _let g = _fun (y)\n"
                                 "              y+ 2 \n"
                                 "_in (f(5))(g) ")->interp()->equals(NEW(NumVal)(8)));

    // 25
    CHECK(parse_str("_let f = _fun (x)\n"
                                 "           _fun (g)\n"
                                 "             g(x + 1)\n"
                                 "_in _let g = _fun (y)\n"
                                 "y+ 2 _in f(5)(g)")->interp()->equals(NEW(NumVal)(8)));

    // 26
    CHECK(parse_str("_let f = _fun (f)\n"
                                 "           _fun (x)\n"
                                 "             _if x == 0\n"
                                 "             _then 0\n"
                                 "             _else x + f(f)(x + -1)\n"
                                 "_in f(f)(3)")->interp()->equals(NEW(NumVal)(6)));
}

TEST_CASE("class example"){
    CHECK( parse_str("_let x = 3"
                     "_in  _if x == 3"
                          "_then 1"
                          "_else 0")->interp()->equals(NEW(NumVal)(1)) );
    CHECK( parse_str("(1 + 2) == 3")->interp()->to_string() == "_true");
    CHECK( (NEW(LetExpr)("x",
                      NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
                      NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))))
          ->interp()->equals(NEW(NumVal)(25)) );
    CHECK_THROWS_WITH(parse_str("_let x = _true + 1"
                                "_in  _if _true"
                                     "_then 5"
                                     "_else x")->interp(), "invalid input");
}

TEST_CASE("BoolExpr"){
    SECTION("BoolExpr::equals()"){
        CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(true)));
        CHECK_FALSE((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(false)));
        CHECK_FALSE((NEW(BoolExpr)(true))->equals(NEW(NumExpr)(3)));
    }
    SECTION("BoolExpr::interp()"){
        CHECK((NEW(BoolExpr)(true))->interp()->equals(NEW(BoolVal)(true)));
    }

    SECTION("BoolExpr::subst()"){
        CHECK((NEW(BoolExpr)(true))->subst("_true", NEW(BoolExpr)(false))->equals(NEW(BoolExpr)(true)));
    }
    SECTION("BoolExpr::print()"){
        CHECK((NEW(BoolExpr)(true))->to_string() == "_true");
    }
    SECTION("BoolExpr::pretty_print_at()"){
        CHECK((NEW(BoolExpr)(true))->pretty_print_to_string() == "_true");
        CHECK((NEW(BoolExpr)(false))->pretty_print_to_string() == "_false");
    }
}

TEST_CASE("IfExpr"){
    SECTION("IfExpr::equals()"){
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true),
                           NEW(NumExpr)(1),
                           NEW(NumExpr)(2)))
              ->equals(NEW(IfExpr)(NEW(BoolExpr)(true),
                                  NEW(NumExpr)(1),
                                  NEW(NumExpr)(2))) );
        CHECK_FALSE( (NEW(IfExpr)(NEW(BoolExpr)(true),
                           NEW(NumExpr)(1),
                           NEW(NumExpr)(2)))
              ->equals(NEW(NumExpr)(1)) );
    }
    SECTION("IfExpr::interp()"){
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true),
                           NEW(NumExpr)(1),
                           NEW(NumExpr)(2)))->interp()
              ->equals(NEW(NumVal)(1)) );
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(false),
                           NEW(NumExpr)(1),
                           NEW(NumExpr)(2)))->interp()
              ->equals(NEW(NumVal)(2)) );
    }
   
    SECTION("IfExpr::subst()"){
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true),
                           NEW(VarExpr)("x"),
                           NEW(NumExpr)(2)))->subst("x", NEW(NumExpr)(30))->interp()
              ->equals(NEW(NumVal)(30)) );
    }
    SECTION("IfExpr::print()"){
        CHECK((NEW(BoolExpr)(true))->to_string() == "_true");
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true),
                           NEW(NumExpr)(1),
                           NEW(NumExpr)(2)))->to_string()==
              "(_if _true _then 1 _else 2)");
    }
    SECTION("IfExpr::pretty_print_at()"){
        CHECK(parse_str("3+2*(_if 1==2 _then 3 _else 0)+2")
              ->pretty_print_to_string()=="3 + 2 * (_if 1 == 2\n"
                                       "         _then 3\n"
                                       "         _else 0) + 2");
        CHECK(parse_str("_let x=(_if 3==3 _then _true _else _false) _in x")
              ->pretty_print_to_string()=="_let x = _if 3 == 3\n"
                                       "         _then _true\n"
                                       "         _else _false\n"
                                       "_in  x");
    }
}

TEST_CASE("EqExpr"){
    SECTION("EqExpr::equals()"){
        CHECK((NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)))
              ->equals(NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3))));
        CHECK_FALSE((NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)))
              ->equals(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3))));
    }
    SECTION("EqExpr::interp()"){
        CHECK((NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)))
              ->interp()->to_string() == "_true");
        CHECK((NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(-3)))
              ->interp()->to_string() == "_false");
    }

    SECTION("EqExpr::subst()"){
        CHECK((NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)))
              ->subst("x", NEW(NumExpr)(3))->interp()->to_string()=="_true");
    }
    SECTION("EqExpr::print()"){
        CHECK((NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)))
              ->to_string() == "(3==3)");
    }
    SECTION("EqExpr::pretty_print_at()"){
        CHECK(parse_str("3+2==6+1")
              ->pretty_print_to_string()=="3 + 2 == 6 + 1" );
        CHECK(parse_str("2*(3+2)==6*1")
              ->pretty_print_to_string()=="2 * (3 + 2) == 6 * 1" );
        CHECK(parse_str("3==2==1==0")
              ->pretty_print_to_string()=="3 == 2 == 1 == 0" );
        CHECK(parse_str("3+3==6+1")
              ->pretty_print_to_string()=="3 + 3 == 6 + 1");
        CHECK(parse_str("3*(2==1)+1")
              ->pretty_print_to_string()=="3 * (2 == 1) + 1");
    }
   
}

TEST_CASE("BoolVal"){
    SECTION("BoolVal::to_expr()"){
        CHECK( ( (NEW(BoolVal)(true))->to_expr() )->equals(NEW(BoolExpr)(true)) );
        CHECK( ( (NEW(BoolVal)(false))->to_expr() )->equals(NEW(BoolExpr)(false)) );
    }
    SECTION("BoolVal::to_string()"){
        CHECK( ( (NEW(BoolVal)(true))->to_string() ) == "_true" );
        CHECK( ( (NEW(BoolVal)(false))->to_string() ) == "_false" );
    }
    SECTION("BoolVal::equals()"){
        CHECK((NEW(BoolVal)(true))->equals(NEW(BoolVal)(true)));
        CHECK_FALSE((NEW(BoolVal)(true))->equals(NEW(BoolVal)(false)));
        CHECK_FALSE((NEW(BoolVal)(true))->equals(NEW(NumVal)(3)));
    }
    SECTION("BoolVal::add_to()"){
        CHECK_THROWS_WITH((NEW(BoolVal)(true))->add_to(NEW(NumVal)(3)), "add of non-number");
    }
    SECTION("BoolVal::mult_with()"){
        CHECK_THROWS_WITH((NEW(BoolVal)(true))->mult_with(NEW(NumVal)(3)), "mult of non-number");
    }
    SECTION("BoolVal::is_true()"){
        CHECK((NEW(BoolVal)(true))->is_true() == true);
        CHECK((NEW(BoolVal)(false))->is_true() == false);
    }
}

TEST_CASE("NumVal"){
    SECTION("NumVal::to_expr()"){
        CHECK( ( (NEW(NumVal)(3))->to_expr() )->equals(NEW(NumExpr)(3)) );
        CHECK( ( (NEW(NumVal)(-3))->to_expr() )->equals(NEW(NumExpr)(-3)) );
        CHECK( ( (NEW(NumVal)(0))->to_expr() )->equals(NEW(NumExpr)(0)) );
    }
    SECTION("NumVal::to_string()"){
        CHECK( ( (NEW(NumVal)(13))->to_string() ) == "13" );
        CHECK( ( (NEW(NumVal)(-3))->to_string() ) == "-3" );
        CHECK( ( (NEW(NumVal)(0))->to_string() ) == "0" );
    }
    SECTION("NumVal::equals()"){
        CHECK_FALSE((NEW(NumVal)(3))->equals(NEW(BoolVal)(true)));
    }
    SECTION("NumVal::add_to()"){
        CHECK_THROWS_WITH((NEW(NumVal)(3))->add_to(NEW(BoolVal)(true)), "add of non-number");
    }
    SECTION("NumVal::mult_with()"){
        CHECK_THROWS_WITH((NEW(NumVal)(3))->mult_with(NEW(BoolVal)(true)), "mult of non-number");
    }
    SECTION("NumVal::is_true()"){
        CHECK_THROWS_WITH((NEW(NumVal)(3))->is_true(), "no boolean for NumVal");
    }
}

TEST_CASE("parse") {
  CHECK_THROWS_WITH( parse_str("()"), "invalid input" );
  
  CHECK( parse_str("(1)")->equals(NEW(NumExpr)(1)) );
  CHECK( parse_str("(((1)))")->equals(NEW(NumExpr)(1)) );
  
  CHECK_THROWS_WITH( parse_str("(1"), "missing close parenthesis" );
  
  CHECK( parse_str("1")->equals(NEW(NumExpr)(1)) );
  CHECK( parse_str("10")->equals(NEW(NumExpr)(10)) );
  CHECK( parse_str("-3")->equals(NEW(NumExpr)(-3)) );
  CHECK( parse_str("  \n 5  ")->equals(NEW(NumExpr)(5)) );
  CHECK_THROWS_WITH( parse_str("-"), "invalid input" );

  // This was some temporary debugging code:
  //  std::istringstream in("-");
  //  parse_num(in)->print(std::cout); std::cout << "\n";
  
  CHECK_THROWS_WITH( parse_str(" -   5  "), "invalid input" );
  
  CHECK( parse_str("x")->equals(NEW(VarExpr)("x")) );
  CHECK( parse_str("xyz")->equals(NEW(VarExpr)("xyz")) );
  CHECK( parse_str("xYz")->equals(NEW(VarExpr)("xYz")) );
  CHECK_THROWS_WITH( parse_str("x_z"), "invalid input" );
//  CHECK_THROWS_WITH( parse_str("x z"), "invalid input" );
  
  CHECK( parse_str("x + y")->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );

  CHECK( parse_str("x * y")->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );

  CHECK( parse_str("z * x + y")
        ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x")),
                         NEW(VarExpr)("y"))) );
  
  CHECK( parse_str("z * (x + y)")
        ->equals(NEW(MultExpr)(NEW(VarExpr)("z"),
                          NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) ));
    
    
//    CHECK_THROWS_WITH( parse_str("x 2"), "invalid input");
    CHECK( parse_str("(_let x = 2 _in x ) + 2")->equals(NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(VarExpr)("x")), NEW(NumExpr)(2))) );
    CHECK( parse_str("(_let x = 2 _in x )")->equals(NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(VarExpr)("x"))) );
    CHECK( parse_str("_let x = 2 _in x")->equals(NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(VarExpr)("x"))) );
    CHECK_THROWS_WITH( parse_str("_let x = 2 y _in x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_2let x = 2 _in x"), "invalid input");
    CHECK_THROWS_WITH( parse_str("_let x y = 2 _in x"), "invalid input");
    CHECK_THROWS_WITH( parse_str("_let x  = 2  in x"), "invalid input");
    CHECK_THROWS_WITH( parse_str("_let 1 = 2  in x"), "invalid input");
    
    CHECK( parse_str("_let same = 1 == 2"
                     "_in  _if 1 == 2"
                          "_then _false + 5"
                          "_else 88") ->interp()->equals(NEW(NumVal)(88)));
    CHECK( parse_str("_if _true==_false\n"
                   "_then 0\n"
                   "_else 1")->interp()->equals(NEW(NumVal)(1)));
    CHECK( parse_str("3 == -3")->interp()->equals(NEW(BoolVal)(false)));
    
}



/*
 Jinny's test
 */

TEST_CASE("Let_pretty_print_to_string (includes pretty_print)"){
    
    SECTION("let in left"){
        CHECK((NEW(AddExpr) (NEW(LetExpr)("x",NEW(NumExpr)(5), NEW(VarExpr)("x")) , NEW(NumExpr)(1)))
              ->pretty_print_to_string() == "(_let x = 5"    "\n"
                                         " _in  x) + 1");
        CHECK((NEW(MultExpr)( NEW(NumExpr)(5), NEW(LetExpr)("x",NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"),NEW(NumExpr)(1)))))
              ->pretty_print_to_string() == "5 * _let x = 5"   "\n"
                                         "    _in  x + 1");
    }
    
    SECTION("let in right"){
        CHECK((NEW(AddExpr) (NEW(NumExpr)(1), NEW(LetExpr)("x",NEW(NumExpr)(5), NEW(VarExpr)("x"))))
              ->pretty_print_to_string() == "1 + _let x = 5"    "\n"
                                         "    _in  x");
    }
    
    SECTION("let in middle"){
        CHECK((NEW(AddExpr) (NEW(MultExpr)( NEW(NumExpr)(5), NEW(LetExpr)("x",NEW(NumExpr)(5), NEW(VarExpr)("x"))) , NEW(NumExpr)(2)))
              ->pretty_print_to_string() == "5 * (_let x = 5"   "\n"
                                         "     _in  x) + 2");
        CHECK((NEW(MultExpr) (NEW(MultExpr)( NEW(NumExpr)(5), NEW(LetExpr)("x",NEW(NumExpr)(5), NEW(VarExpr)("x"))) , NEW(NumExpr)(2)))
              ->pretty_print_to_string() == "(5 * _let x = 5"   "\n"
                                         "     _in  x) * 2");
        CHECK((NEW(MultExpr) (NEW(AddExpr)( NEW(NumExpr)(5), NEW(LetExpr)("x",NEW(NumExpr)(5), NEW(VarExpr)("x"))) , NEW(NumExpr)(2)))
              ->pretty_print_to_string() == "(5 + _let x = 5"   "\n"
                                         "     _in  x) * 2");
        CHECK((NEW(MultExpr) (NEW(AddExpr)(NEW(LetExpr)("x",NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(5)) , NEW(NumExpr)(2)))
              ->pretty_print_to_string() == "((_let x = 5"   "\n"
                                         "  _in  x) + 5) * 2");
    }
    
    SECTION("nested"){
        
        CHECK(( NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  _let x = 3"   "\n"
                                         "     _in  x + 1");
        CHECK(( NEW(LetExpr)("x", NEW(NumExpr)(5) , NEW(LetExpr)("x", NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(5)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  _let x = x + 5"   "\n"
                                         "     _in  x + 1");
        
        CHECK(( NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(2)) ) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  (_let x = 3"   "\n"
                                         "      _in  x + 1) + 2");
        
        CHECK(( NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(NumExpr)(2), NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) ) ) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  2 + _let x = 3"   "\n"
                                         "         _in  x + 1");
    }
}


TEST_CASE("Let_all methods"){
    
    SECTION("toString (includes print)"){
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) )
              ->to_string() == "(_let x=5 _in (x+1))");
        CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2)) , NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) )
              ->to_string() == "(_let x=(5+2) _in (x+1))");
        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
              ->to_string() == "(_let x=5 _in (_let x=6 _in (x+1)))" );
        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("y", NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
              ->to_string() == "(_let x=5 _in (_let y=6 _in (x+1)))" );
        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"),NEW(NumExpr)(2)) , NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
              ->to_string() == "(_let x=5 _in (_let x=(x+2) _in (x+1)))" );
        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))
              ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))" );
    }
    SECTION("interp"){
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) )
              ->interp())->equals(NEW(NumVal)(6)) );
        CHECK( ((NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2)) , NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) )
              ->interp())->equals(NEW(NumVal)(8)) );
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
              ->interp())->equals(NEW(NumVal)(7)) );
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("y", NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
              ->interp())->equals(NEW(NumVal)(6)) );
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"),NEW(NumExpr)(2)) , NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
              ->interp())->equals(NEW(NumVal)(8)) );
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))
              ->interp())->equals(NEW(NumVal)(10)) );
    }
    SECTION("equal"){
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) )
              ->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
        CHECK_FALSE((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) )
                    ->equals(NEW(LetExpr)("y", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
        CHECK_FALSE((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) )
                    ->equals(NEW(AddExpr)(NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
    }

    SECTION("subst"){
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
              ->subst("x", NEW(NumExpr)(10))->interp())->equals(NEW(NumVal)(6)) );
        CHECK( ((NEW(LetExpr)("y", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
              ->subst("x", NEW(NumExpr)(10))->interp())->equals(NEW(NumVal)(11)) );
    }
}



TEST_CASE("Test_Equal_Within_Same_Class"){
    
    SECTION("expr_number"){
        PTR(NumExpr) num_Base = NEW(NumExpr)(3);
        PTR(NumExpr) num_Eq = NEW(NumExpr)(3);
        PTR(NumExpr) num_NoEq = NEW(NumExpr)(5);
        PTR(NumExpr) num_NoEq_neg = NEW(NumExpr)(-3);
        PTR(NumExpr) num_NoEq_zero = NEW(NumExpr)(0);
        
        
        CHECK( num_Base -> equals(num_Eq) );
        CHECK_FALSE( num_Base -> equals(num_NoEq) );
        CHECK_FALSE( num_Base -> equals(num_NoEq_neg) );
        CHECK_FALSE( num_Base -> equals(num_NoEq_zero) );
    }
    
    SECTION("expr_add"){
        PTR(AddExpr) add_Base = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(7));
        PTR(AddExpr) add_Eq = NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(7));
        PTR(AddExpr) add_NoEq = NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(20));
        PTR(AddExpr) add_NoEq_rev = NEW(AddExpr)(NEW(NumExpr)(7), NEW(NumExpr)(5));
        PTR(AddExpr) add_NoEq_neg = NEW(AddExpr)(NEW(NumExpr)(-5), NEW(NumExpr)(-7));
        PTR(AddExpr) add_var = NEW(AddExpr)(NEW(VarExpr)("dog"), NEW(VarExpr)("cat"));
        PTR(AddExpr) add_var_Eq = NEW(AddExpr)(NEW(VarExpr)("dog"), NEW(VarExpr)("cat"));
        PTR(AddExpr) add_var_NoEq = NEW(AddExpr)(NEW(VarExpr)("PIGGG"), NEW(VarExpr)("cat"));
        
        CHECK( add_Base -> equals(add_Eq) );
        CHECK_FALSE( add_Base -> equals(add_NoEq) );
        CHECK_FALSE( add_Base -> equals(add_NoEq_rev) );
        CHECK_FALSE( add_Base -> equals(add_NoEq_neg) );
        CHECK( add_var -> equals(add_var_Eq) );
        CHECK_FALSE( add_var -> equals(add_var_NoEq) );
    }
 
    SECTION("expr_multiple"){
        PTR(MultExpr) mult_Base = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(7));
        PTR(MultExpr) mult_Eq = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(7));
        PTR(MultExpr) mult_NoEq = NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(20));
        PTR(MultExpr) mult_NoEq_rev = NEW(MultExpr)(NEW(NumExpr)(7), NEW(NumExpr)(5));
        
        CHECK( mult_Base -> equals(mult_Eq) );
        CHECK_FALSE( mult_Base -> equals(mult_NoEq) );
        CHECK_FALSE( mult_Base -> equals(mult_NoEq_rev) );
    }
    
    SECTION("expr_variable"){
        PTR(VarExpr) var_Base = NEW(VarExpr)("Dog");
        PTR(VarExpr) var_Eq = NEW(VarExpr)("Dog");
        PTR(VarExpr) var_NoEq = NEW(VarExpr)("Cat");
        PTR(VarExpr) var_NoEq_lower = NEW(VarExpr)("dog");
        
        CHECK( var_Base -> equals(var_Eq) );
        CHECK_FALSE( var_Base -> equals(var_NoEq) );
        CHECK_FALSE( var_Base -> equals(var_NoEq_lower) );
    }
}

TEST_CASE("Test_Equal_Not_Same_Class"){
    
    SECTION("num_var"){
        PTR(NumExpr) num = NEW(NumExpr)(2);
        PTR(VarExpr) var = NEW(VarExpr)("2");
        CHECK_FALSE( num -> equals(var) );
        CHECK_FALSE( var -> equals(num) );
    }
    
    SECTION("add_mult"){
        PTR(AddExpr) add = NEW(AddExpr)( NEW(NumExpr)(3), NEW(NumExpr)(0) );
        PTR(MultExpr) mult = NEW(MultExpr) ( NEW(NumExpr)(3), NEW(NumExpr)(0) );
        CHECK_FALSE( add -> equals(mult) );
    }
    
    SECTION("null"){
        CHECK_FALSE( (NEW(AddExpr)( NEW(NumExpr)(3), NEW(NumExpr)(0))) -> equals(NULL) );
        CHECK_FALSE( (NEW(MultExpr)( NEW(NumExpr)(3), NEW(NumExpr)(0))) -> equals(NULL) );
    }
}

TEST_CASE("Test_interp()"){
    
    SECTION("addExpr"){
        CHECK( ((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(15)),NEW(AddExpr)(NEW(NumExpr)(20),NEW(NumExpr)(20))))->interp())->equals(NEW(NumVal)(65))); // num only
        CHECK_THROWS_WITH( (NEW(AddExpr)(NEW(VarExpr)("dog"), NEW(VarExpr)("cat")))->interp(), "no value for variable" ); // var only
        CHECK_THROWS_WITH( (NEW(AddExpr)(NEW(VarExpr)("dog"), NEW(NumExpr)(2)))->interp(), "no value for variable" ); // num + var
    }

    SECTION("multExpr"){
        CHECK( ((NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))->interp())->equals(NEW(NumVal)(6)) ); // num only
        CHECK_THROWS_WITH( (NEW(MultExpr)(NEW(VarExpr)("dog"), NEW(VarExpr)("cat")))->interp(), "no value for variable" ); // var only
        CHECK_THROWS_WITH( (NEW(MultExpr)(NEW(VarExpr)("dog"), NEW(NumExpr)(2)))->interp(), "no value for variable" ); // num + var
    }
    
    SECTION("numExpr_only"){
        CHECK( ((NEW(NumExpr)(5))->interp())->equals(NEW(NumVal)(5)) );
    }
    
    SECTION("varExpr_only"){
        CHECK_THROWS_WITH( (NEW(VarExpr)("x"))->interp(), "no value for variable" );
    }
   
}


TEST_CASE("Test_subst()"){
    
    SECTION("addExpr"){
        // did substitute
        CHECK( (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
               ->subst("x", NEW(VarExpr)("y"))
               ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );
        // didn't substitute
        CHECK( (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
               ->subst("no_match_str", NEW(VarExpr)("y"))
               ->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7))) );
        
    }

    SECTION("multExpr"){
        // did substitute with nested Expr
        CHECK( ( NEW(MultExpr)( ( NEW(MultExpr)(NEW(VarExpr)("x"),NEW(NumExpr)(7)) )  ,  (NEW(MultExpr)(NEW(VarExpr)("x"),NEW(NumExpr)(7))) ) )
               ->subst("x", NEW(VarExpr)("y"))
               ->equals( NEW(MultExpr)( ( NEW(MultExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7)) )  ,  (NEW(MultExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7))) ) ) );
        // didn't substitute
        CHECK( (NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
               ->subst("no_match_str", NEW(VarExpr)("y"))
               ->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7))) );
    }
    
    SECTION("numExpr_only"){
        // never substitute
        CHECK( (NEW(NumExpr)(5))->subst("5", NEW(NumExpr)(-1))->equals(NEW(NumExpr)(5)) );
    }
    
    SECTION("varExpr_only"){
        // did substitute
        CHECK( (NEW(VarExpr)("original"))->subst("original", NEW(VarExpr)("new"))->equals(NEW(VarExpr)("new")) );
        // didn't substitute
        CHECK( (NEW(VarExpr)("original"))->subst("no_match", NEW(VarExpr)("new"))->equals(NEW(VarExpr)("original")) );
    }
}

TEST_CASE("Test_toString (includes print)"){
    SECTION("expr_number"){
        CHECK( (NEW(NumExpr)(10))->to_string() == "10" );
    }
    SECTION("expr_add"){
        CHECK( (NEW(AddExpr)( NEW(NumExpr)(2), NEW(NumExpr)(3)))->to_string() == "(2+3)" );
        CHECK( (NEW(AddExpr)( NEW(NumExpr)(-2), NEW(NumExpr)(-3)))->to_string() == "(-2+-3)" );
        CHECK( (NEW(AddExpr)( (NEW(AddExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2))), NEW(NumExpr)(3) ) )->to_string() == "((1+2)+3)" );
        CHECK( (NEW(AddExpr)( NEW(NumExpr)(1), (NEW(AddExpr)( NEW(NumExpr)(2), NEW(NumExpr)(3))) ) )->to_string() == "(1+(2+3))" );
        CHECK( (NEW(AddExpr)( NEW(VarExpr)("cat"), (NEW(AddExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2))) ) )->to_string() == "(cat+(1+2))" );
    }
    SECTION("expr_mult"){
        CHECK( (NEW(MultExpr)( NEW(NumExpr)(10), NEW(NumExpr)(3)))->to_string() == "(10*3)" );
    }
    SECTION("expr_var"){
        CHECK( (NEW(VarExpr)("cat"))->to_string() == "cat" );
    }
}


TEST_CASE("Test_pretty_print_to_string (includes pretty_print)"){
        
    SECTION("expr_add_only"){
        
        // test (1 + 2) + 3
        CHECK( ( NEW(AddExpr)( NEW(AddExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)) , NEW(NumExpr)(3) ) )
              ->pretty_print_to_string() == "(1 + 2) + 3" );
        // test 1 + (2 + 3)
        CHECK( ( NEW(AddExpr)( NEW(NumExpr)(1) , NEW(AddExpr)(NEW(NumExpr)(2),NEW(NumExpr)(3)) ) )
              ->pretty_print_to_string() == "1 + 2 + 3" );
        // test 1 + (2 + (3 + (4 + 5)))
        CHECK( ( NEW(AddExpr)( NEW(NumExpr)(1) , NEW(AddExpr)( NEW(NumExpr)(2) , NEW(AddExpr)( NEW(NumExpr)(3) , NEW(AddExpr)(NEW(NumExpr)(4),NEW(NumExpr)(5))))))
              ->pretty_print_to_string() == "1 + 2 + 3 + 4 + 5");
        // test 1 + (((2 + 3) + 4) + 5)
        CHECK( ( NEW(AddExpr)( NEW(NumExpr)(1) , NEW(AddExpr)( NEW(AddExpr)( NEW(AddExpr)( NEW(NumExpr)(2) , NEW(NumExpr)(3)) , NEW(NumExpr)(4)) , NEW(NumExpr)(5))))
              ->pretty_print_to_string() == "1 + ((2 + 3) + 4) + 5");
    }
    
    SECTION("expr_mult_only"){
        
        // test (1 * 2) * 3
        CHECK( ( NEW(MultExpr)( NEW(MultExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)) , NEW(NumExpr)(3) ) )
              ->pretty_print_to_string() == "(1 * 2) * 3" );
        // test 1 + (2 + 3)
        CHECK( ( NEW(MultExpr)( NEW(NumExpr)(1) , NEW(MultExpr)(NEW(NumExpr)(2),NEW(NumExpr)(3)) ) )
              ->pretty_print_to_string() == "1 * 2 * 3" );
        // test 1 + (2 + (3 + (4 + 5)))
        CHECK( ( NEW(MultExpr)( NEW(NumExpr)(1) , NEW(MultExpr)( NEW(NumExpr)(2) , NEW(MultExpr)( NEW(NumExpr)(3) , NEW(MultExpr)(NEW(NumExpr)(4),NEW(NumExpr)(5))))))
              ->pretty_print_to_string() == "1 * 2 * 3 * 4 * 5");
        // test 1 + (((2 + 3) + 4) + 5)
        CHECK( ( NEW(MultExpr)( NEW(NumExpr)(1) , NEW(MultExpr)( NEW(MultExpr)( NEW(MultExpr)( NEW(NumExpr)(2) , NEW(NumExpr)(3)) , NEW(NumExpr)(4)) , NEW(NumExpr)(5))))
              ->pretty_print_to_string() == "1 * ((2 * 3) * 4) * 5");
        // test (10 * ((10 * 10) * 10)) * (10 * 10)
        CHECK((NEW(MultExpr)( NEW(MultExpr)(NEW(NumExpr)(10), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(NumExpr)(10))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10))))
              ->pretty_print_to_string()  == "(10 * (10 * 10) * 10) * 10 * 10");
    }
    
    SECTION("expr_add_mult_mix"){
        
        // test 1 + (2 * 3)
        CHECK( ( NEW(AddExpr)( NEW(NumExpr)(1) , NEW(MultExpr)(NEW(NumExpr)(2),NEW(NumExpr)(3)) ) )
              ->pretty_print_to_string() == "1 + 2 * 3" );
        // test (1 * 2) + 3
        CHECK( ( NEW(AddExpr)( NEW(MultExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)) , NEW(NumExpr)(3) ) )
              ->pretty_print_to_string() == "1 * 2 + 3");
        // test 1 * (2 + 3)
        CHECK( ( NEW(MultExpr)( NEW(NumExpr)(1) , NEW(AddExpr)(NEW(NumExpr)(2),NEW(NumExpr)(3)) ) )
              ->pretty_print_to_string() == "1 * (2 + 3)");
        // test (9 * (4 + 3)) + ((2 * 4) + 1)
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(9), NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(3))), NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(4)), NEW(NumExpr)(1))))
              ->pretty_print_to_string() == "9 * (4 + 3) + 2 * 4 + 1");
    }
    
    SECTION("expr_number"){
        CHECK(( NEW(NumExpr)(3) )-> pretty_print_to_string() == "3");
        CHECK(( NEW(NumExpr)(-3) )-> pretty_print_to_string() == "-3");
    }
    
    SECTION("expr_var"){
        CHECK(( NEW(VarExpr)("3") )-> pretty_print_to_string() == "3");
        CHECK(( NEW(VarExpr)("variable") )-> pretty_print_to_string() == "variable");
    }
    
}


/*
 Kevin & Will's test for let
 */

TEST_CASE("Pretty Print examples_Kevin"){           //Created from assignment examples
    
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
          ->pretty_print_to_string() == "_let x = 5\n"
                                     "_in  x + 1");

    CHECK((NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1)))
          ->pretty_print_to_string()== "(_let x = 5\n"
                                    " _in  x) + 1");

    CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
          ->pretty_print_to_string() == "5 * _let x = 5\n"
                                     "    _in  x + 1");

    CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))
          ->pretty_print_to_string() == "5 * (_let x = 5\n"
                                     "     _in  x) + 1");

    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))
          ->pretty_print_to_string() == "_let x = 5\n"
                                     "_in  (_let y = 3\n"
                                     "      _in  y + 2) + x");

    CHECK((NEW(LetExpr)("x", NEW(LetExpr)("y", NEW(NumExpr)(6), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
          ->pretty_print_to_string() == "_let x = _let y = 6\n"
                                     "         _in  y * 2\n"
                                     "_in  x + 1");
}

TEST_CASE("pretty_print_let_mine_some_reuse_of_kevin_triple_nested_let") {
    PTR(LetExpr) tripleNestedLet = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                   NEW(LetExpr)("y", NEW(NumExpr)(1),
                                           NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z"))));
    PTR(LetExpr) tripleNestedLet2 = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                    NEW(LetExpr)("y", NEW(NumExpr)(1),
                                            NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                    NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z")))));
    PTR(LetExpr) tripleNestedLet3 = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                    NEW(LetExpr)("y", NEW(NumExpr)(1),
                                            NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                    NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z")))));


    CHECK(tripleNestedLet -> pretty_print_to_string() ==
          "_let x = 1\n"
          "_in  _let y = 1\n"
          "     _in  (x + y) * z"
    );
    CHECK(tripleNestedLet2 -> pretty_print_to_string() ==
          "_let x = 1\n"
          "_in  _let y = 1\n"
          "     _in  _let z = x + 1\n"
          "          _in  (x + y) + z"
    );
    CHECK(tripleNestedLet3 -> pretty_print_to_string() ==
                            "_let x = 1\n"
                            "_in  _let y = 1\n"
                            "     _in  _let z = x + 1\n"
                            "          _in  (x + y) * z"
    );
    PTR(LetExpr) tripleNestedLet4 =NEW(LetExpr)("x", NEW(NumExpr)(5),
                                   NEW(LetExpr)("y", NEW(NumExpr)(3),
                                           NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))));
    PTR(LetExpr) tripleNestedLet5 =NEW(LetExpr)("x", NEW(NumExpr)(5),
                                   NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3),
                                                   NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")));

    CHECK(tripleNestedLet4->pretty_print_to_string() ==
                  "_let x = 5\n"
                  "_in  _let y = 3\n"
                  "     _in  y + 2"
    );

    CHECK(tripleNestedLet5 -> pretty_print_to_string() == "_let x = 5\n"
                                                      "_in  (_let y = 3\n"
                                                      "      _in  y + 2) + x");
    SECTION("assignment_examples") {
        CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))-> pretty_print_to_string()
               == "5 * (_let x = 5\n"
                  "     _in  x) + 1");
        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr) (2), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x") ,NEW(NumExpr)(1)) )), NEW(NumExpr)(3) )) -> pretty_print_to_string()
               == "(2 * _let x = 5\n"
                  "     _in  x + 1) * 3");
    }
    // A _let needs parentheses when it is nested immediately as the right argument of an unparenthesized *
    // where _let would have needed parentheses in the surrounding context
    // (that is, if the _let used in place of the whole * would need parentheses,
    // then it still needs parentheses within the right-hand size of *).
    SECTION("new_edge") {
        CHECK( (NEW(MultExpr)(NEW(NumExpr) (2), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x") ,NEW(NumExpr)(1)) ) )) -> pretty_print_to_string()
               == "2 * _let x = 5\n"
                  "    _in  x + 1");
        CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))), NEW(NumExpr)(1)))-> pretty_print_to_string()
               == "5 * (_let x = 5\n"
                  "     _in  x * 2) + 1");
        CHECK( (NEW(MultExpr)((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))), NEW(NumExpr)(1))), NEW(NumExpr)(7)))-> pretty_print_to_string()
               == "(5 * (_let x = 5\n"
                  "      _in  x * 2) + 1) * 7");
        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(10), NEW(MultExpr)( NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(NumExpr)(10))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                       ->pretty_print_to_string()  == "_let x = 10\n"
                                                      "_in  (x * (10 * 10) * 10) * 10 * 10");
        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)( NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                        -> pretty_print_to_string() == "_let x = 1\n"
                                                       "_in  (x * (10 * 10) * x) * 10 * 10" );
        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)( NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(10)))))
                       -> pretty_print_to_string() == "_let x = 1\n"
                                                      "_in  (x * (10 * 10) * x) * y * 10" );
    }
}

TEST_CASE("Let_equals_mine") {
        SECTION("Values_same") {
            REQUIRE( (NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) )));
        }
        SECTION("Values_same_different_rhs") {
            REQUIRE( !(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) )));
        }
        SECTION("Values_same_different_lhs") {
            REQUIRE( !(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals(NEW(LetExpr)("y", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) )));
        }
        SECTION("Values_same_different_body") {
            REQUIRE( !(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(MultExpr)(NEW(NumExpr)(3), NEW(VarExpr)("y")) )));
        }
        SECTION("different_types") {
            REQUIRE( !(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals( NEW(MultExpr)(NEW(NumExpr)(3), NEW(VarExpr)("y")) ));
        }
}


TEST_CASE("Let_print_mine") {
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")))) -> to_string()
                                                                                                        == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(1),
                   NEW(LetExpr)("y", NEW(NumExpr)(1),
                           NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                   NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z")))))) -> to_string()
                                                                                                        == "(_let x=1 _in (_let y=1 _in (_let z=(x+1) _in ((x+y)*z))))");
}
TEST_CASE ("Let_interp_mine") {
    SECTION("hw_examples") {
        CHECK( ((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1))) -> interp())->equals(NEW(NumVal)(26)));
        CHECK( ((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))) -> interp())->equals(NEW(NumVal)(30)));
    }
    SECTION("from_pretty_print_edge") {
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(1),
                       NEW(LetExpr)("y", NEW(NumExpr)(1),
                               NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                       NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z")))))) -> interp())->equals(NEW(NumVal)(4)));
        CHECK( ((NEW(MultExpr)((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))), NEW(NumExpr)(1))), NEW(NumExpr)(7))) -> interp())->equals(NEW(NumVal)(357))); // 51 * 7
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(10), NEW(MultExpr)( NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(NumExpr)(10))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                      ->interp())->equals(NEW(NumVal)(1000000)));
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)( NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                       ->interp())->equals(NEW(NumVal)(10000)));
        CHECK_THROWS_WITH( (((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)( NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(10)))))
                       -> interp())->equals(NEW(NumVal)(10000)) ), "no value for variable");
    }
    SECTION("bypass_middle_let") {
        CHECK( ((NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(LetExpr)("z", NEW(NumExpr)(4), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(10)))))
                -> interp())->equals(NEW(NumVal)(12)));
    }
}
