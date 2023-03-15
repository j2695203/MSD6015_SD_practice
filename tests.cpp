#include "val.hpp"
#include "expr.hpp"
#include "parse.hpp"
#include "cmdline.hpp"


/*
 *** TEST using Catch 2 framework ************************************************
 */
TEST_CASE("class example"){
    CHECK( parse_str("_let x = 3"
                     "_in  _if x == 3"
                          "_then 1"
                          "_else 0")->interp()->equals(new NumVal(1)) );
    CHECK( parse_str("(1 + 2) == 3")->interp()->to_string() == "_true");
    CHECK( (new LetExpr("x",
                      new AddExpr(new NumExpr(2), new NumExpr(3)),
                      new MultExpr(new VarExpr("x"), new VarExpr("x"))))
          ->interp()->equals(new NumVal(25)) );
    CHECK_THROWS_WITH(parse_str("_let x = _true + 1"
                                "_in  _if _true"
                                     "_then 5"
                                     "_else x")->interp(), "add of non-number");
}

TEST_CASE("BoolExpr"){
    SECTION("BoolExpr::equals()"){
        CHECK((new BoolExpr(true))->equals(new BoolExpr(true)));
        CHECK_FALSE((new BoolExpr(true))->equals(new BoolExpr(false)));
        CHECK_FALSE((new BoolExpr(true))->equals(new NumExpr(3)));
    }
    SECTION("BoolExpr::interp()"){
        CHECK((new BoolExpr(true))->interp()->equals(new BoolVal(true)));
    }
    SECTION("BoolExpr::has_variable()"){
        CHECK_FALSE((new BoolExpr(true))->has_variable());
    }
    SECTION("BoolExpr::subst()"){
        CHECK((new BoolExpr(true))->subst("_true", new BoolExpr(false))->equals(new BoolExpr(true)));
    }
    SECTION("BoolExpr::print()"){
        CHECK((new BoolExpr(true))->to_string() == "_true");
    }
    SECTION("BoolExpr::pretty_print_at()"){
        CHECK((new BoolExpr(true))->pretty_print_to_string() == "_true");
        CHECK((new BoolExpr(false))->pretty_print_to_string() == "_false");
    }
}

TEST_CASE("IfExpr"){
    SECTION("IfExpr::equals()"){
        CHECK( (new IfExpr(new BoolExpr(true),
                           new NumExpr(1),
                           new NumExpr(2)))
              ->equals(new IfExpr(new BoolExpr(true),
                                  new NumExpr(1),
                                  new NumExpr(2))) );
        CHECK_FALSE( (new IfExpr(new BoolExpr(true),
                           new NumExpr(1),
                           new NumExpr(2)))
              ->equals(new NumExpr(1)) );
    }
    SECTION("IfExpr::interp()"){
        CHECK( (new IfExpr(new BoolExpr(true),
                           new NumExpr(1),
                           new NumExpr(2)))->interp()
              ->equals(new NumVal(1)) );
        CHECK( (new IfExpr(new BoolExpr(false),
                           new NumExpr(1),
                           new NumExpr(2)))->interp()
              ->equals(new NumVal(2)) );
    }
    SECTION("IfExpr::has_variable()"){
        CHECK( (new IfExpr(new BoolExpr(true),
                           new NumExpr(1),
                           new NumExpr(2)))->has_variable() == false);
        CHECK( (new IfExpr(new BoolExpr(true),
                           new VarExpr("x"),
                           new NumExpr(2)))->has_variable() == true);
    }
    SECTION("IfExpr::subst()"){
        CHECK( (new IfExpr(new BoolExpr(true),
                           new VarExpr("x"),
                           new NumExpr(2)))->subst("x", new NumExpr(30))->interp()
              ->equals(new NumVal(30)) );
    }
    SECTION("IfExpr::print()"){
        CHECK((new BoolExpr(true))->to_string() == "_true");
        CHECK( (new IfExpr(new BoolExpr(true),
                           new NumExpr(1),
                           new NumExpr(2)))->to_string()==
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
        CHECK((new EqExpr(new NumExpr(3), new NumExpr(3)))
              ->equals(new EqExpr(new NumExpr(3), new NumExpr(3))));
        CHECK_FALSE((new EqExpr(new NumExpr(3), new NumExpr(3)))
              ->equals(new AddExpr(new NumExpr(3), new NumExpr(3))));
    }
    SECTION("EqExpr::interp()"){
        CHECK((new EqExpr(new NumExpr(3), new NumExpr(3)))
              ->interp()->to_string() == "_true");
        CHECK((new EqExpr(new NumExpr(3), new NumExpr(-3)))
              ->interp()->to_string() == "_false");
    }
    SECTION("EqExpr::has_variable()"){
        CHECK((new EqExpr(new VarExpr("x"), new NumExpr(3)))->has_variable());
        CHECK_FALSE((new EqExpr(new NumExpr(3), new NumExpr(3)))->has_variable());
    }
    SECTION("EqExpr::subst()"){
        CHECK((new EqExpr(new VarExpr("x"), new NumExpr(3)))
              ->subst("x", new NumExpr(3))->interp()->to_string()=="_true");
    }
    SECTION("EqExpr::print()"){
        CHECK((new EqExpr(new NumExpr(3), new NumExpr(3)))
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
        CHECK( ( (new BoolVal(true))->to_expr() )->equals(new BoolExpr(true)) );
        CHECK( ( (new BoolVal(false))->to_expr() )->equals(new BoolExpr(false)) );
    }
    SECTION("BoolVal::to_string()"){
        CHECK( ( (new BoolVal(true))->to_string() ) == "_true" );
        CHECK( ( (new BoolVal(false))->to_string() ) == "_false" );
    }
    SECTION("BoolVal::equals()"){
        CHECK((new BoolVal(true))->equals(new BoolVal(true)));
        CHECK_FALSE((new BoolVal(true))->equals(new BoolVal(false)));
        CHECK_FALSE((new BoolVal(true))->equals(new NumVal(3)));
    }
    SECTION("BoolVal::add_to()"){
        CHECK_THROWS_WITH((new BoolVal(true))->add_to(new NumVal(3)), "add of non-number");
    }
    SECTION("BoolVal::mult_with()"){
        CHECK_THROWS_WITH((new BoolVal(true))->mult_with(new NumVal(3)), "mult of non-number");
    }
    SECTION("BoolVal::is_true()"){
        CHECK((new BoolVal(true))->is_true() == true);
        CHECK((new BoolVal(false))->is_true() == false);
    }
}

TEST_CASE("NumVal"){
    SECTION("NumVal::to_expr()"){
        CHECK( ( (new NumVal(3))->to_expr() )->equals(new NumExpr(3)) );
        CHECK( ( (new NumVal(-3))->to_expr() )->equals(new NumExpr(-3)) );
        CHECK( ( (new NumVal(0))->to_expr() )->equals(new NumExpr(0)) );
    }
    SECTION("NumVal::to_string()"){
        CHECK( ( (new NumVal(13))->to_string() ) == "13" );
        CHECK( ( (new NumVal(-3))->to_string() ) == "-3" );
        CHECK( ( (new NumVal(0))->to_string() ) == "0" );
    }
    SECTION("NumVal::equals()"){
        CHECK_FALSE((new NumVal(3))->equals(new BoolVal(true)));
    }
    SECTION("NumVal::add_to()"){
        CHECK_THROWS_WITH((new NumVal(3))->add_to(new BoolVal(true)), "add of non-number");
    }
    SECTION("NumVal::mult_with()"){
        CHECK_THROWS_WITH((new NumVal(3))->mult_with(new BoolVal(true)), "mult of non-number");
    }
    SECTION("NumVal::is_true()"){
        CHECK_THROWS_WITH((new NumVal(3))->is_true(), "no boolean for NumVal");
    }
}

TEST_CASE("parse") {
  CHECK_THROWS_WITH( parse_str("()"), "invalid input" );
  
  CHECK( parse_str("(1)")->equals(new NumExpr(1)) );
  CHECK( parse_str("(((1)))")->equals(new NumExpr(1)) );
  
  CHECK_THROWS_WITH( parse_str("(1"), "missing close parenthesis" );
  
  CHECK( parse_str("1")->equals(new NumExpr(1)) );
  CHECK( parse_str("10")->equals(new NumExpr(10)) );
  CHECK( parse_str("-3")->equals(new NumExpr(-3)) );
  CHECK( parse_str("  \n 5  ")->equals(new NumExpr(5)) );
  CHECK_THROWS_WITH( parse_str("-"), "invalid input" );

  // This was some temporary debugging code:
  //  std::istringstream in("-");
  //  parse_num(in)->print(std::cout); std::cout << "\n";
  
  CHECK_THROWS_WITH( parse_str(" -   5  "), "invalid input" );
  
  CHECK( parse_str("x")->equals(new VarExpr("x")) );
  CHECK( parse_str("xyz")->equals(new VarExpr("xyz")) );
  CHECK( parse_str("xYz")->equals(new VarExpr("xYz")) );
  CHECK_THROWS_WITH( parse_str("x_z"), "invalid input" );
//  CHECK_THROWS_WITH( parse_str("x z"), "invalid input" );
  
  CHECK( parse_str("x + y")->equals(new AddExpr(new VarExpr("x"), new VarExpr("y"))) );

  CHECK( parse_str("x * y")->equals(new MultExpr(new VarExpr("x"), new VarExpr("y"))) );

  CHECK( parse_str("z * x + y")
        ->equals(new AddExpr(new MultExpr(new VarExpr("z"), new VarExpr("x")),
                         new VarExpr("y"))) );
  
  CHECK( parse_str("z * (x + y)")
        ->equals(new MultExpr(new VarExpr("z"),
                          new AddExpr(new VarExpr("x"), new VarExpr("y"))) ));
    
    
//    CHECK_THROWS_WITH( parse_str("x 2"), "invalid input");
    CHECK( parse_str("(_let x = 2 _in x ) + 2")->equals(new AddExpr(new LetExpr("x", new NumExpr(2), new VarExpr("x")), new NumExpr(2))) );
    CHECK( parse_str("(_let x = 2 _in x )")->equals(new LetExpr("x", new NumExpr(2), new VarExpr("x"))) );
    CHECK( parse_str("_let x = 2 _in x")->equals(new LetExpr("x", new NumExpr(2), new VarExpr("x"))) );
    CHECK_THROWS_WITH( parse_str("_let x = 2 y _in x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_2let x = 2 _in x"), "invalid input");
    CHECK_THROWS_WITH( parse_str("_let x y = 2 _in x"), "invalid input");
    CHECK_THROWS_WITH( parse_str("_let x  = 2  in x"), "invalid input");
    CHECK_THROWS_WITH( parse_str("_let 1 = 2  in x"), "invalid input");
    
    CHECK( parse_str("_let same = 1 == 2"
                     "_in  _if 1 == 2"
                          "_then _false + 5"
                          "_else 88") ->interp()->equals(new NumVal(88)));
    CHECK( parse_str("_if _true==_false"
                   "_then 0"
                   "_else 1")->interp()->equals(new NumVal(1)));
    CHECK( parse_str("3 == -3")->interp()->equals(new BoolVal(false)));
    
}



/*
 Jinny's test
 */

TEST_CASE("Let_pretty_print_to_string (includes pretty_print)"){
    
    SECTION("let in left"){
        CHECK((new AddExpr (new LetExpr("x",new NumExpr(5), new VarExpr("x")) , new NumExpr(1)))
              ->pretty_print_to_string() == "(_let x = 5"    "\n"
                                         " _in  x) + 1");
        CHECK((new MultExpr( new NumExpr(5), new LetExpr("x",new NumExpr(5), new AddExpr(new VarExpr("x"),new NumExpr(1)))))
              ->pretty_print_to_string() == "5 * _let x = 5"   "\n"
                                         "    _in  x + 1");
    }
    
    SECTION("let in right"){
        CHECK((new AddExpr (new NumExpr(1), new LetExpr("x",new NumExpr(5), new VarExpr("x"))))
              ->pretty_print_to_string() == "1 + _let x = 5"    "\n"
                                         "    _in  x");
    }
    
    SECTION("let in middle"){
        CHECK((new AddExpr (new MultExpr( new NumExpr(5), new LetExpr("x",new NumExpr(5), new VarExpr("x"))) , new NumExpr(2)))
              ->pretty_print_to_string() == "5 * (_let x = 5"   "\n"
                                         "     _in  x) + 2");
        CHECK((new MultExpr (new MultExpr( new NumExpr(5), new LetExpr("x",new NumExpr(5), new VarExpr("x"))) , new NumExpr(2)))
              ->pretty_print_to_string() == "(5 * _let x = 5"   "\n"
                                         "     _in  x) * 2");
        CHECK((new MultExpr (new AddExpr( new NumExpr(5), new LetExpr("x",new NumExpr(5), new VarExpr("x"))) , new NumExpr(2)))
              ->pretty_print_to_string() == "(5 + _let x = 5"   "\n"
                                         "     _in  x) * 2");
        CHECK((new MultExpr (new AddExpr(new LetExpr("x",new NumExpr(5), new VarExpr("x")), new NumExpr(5)) , new NumExpr(2)))
              ->pretty_print_to_string() == "((_let x = 5"   "\n"
                                         "  _in  x) + 5) * 2");
    }
    
    SECTION("nested"){
        
        CHECK(( new LetExpr("x", new NumExpr(5), new LetExpr("x", new NumExpr(3), new AddExpr(new VarExpr("x"), new NumExpr(1)))) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  _let x = 3"   "\n"
                                         "     _in  x + 1");
        CHECK(( new LetExpr("x", new NumExpr(5) , new LetExpr("x", new AddExpr( new VarExpr("x"), new NumExpr(5)), new AddExpr(new VarExpr("x"), new NumExpr(1)))) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  _let x = x + 5"   "\n"
                                         "     _in  x + 1");
        
        CHECK(( new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("x", new NumExpr(3), new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(2)) ) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  (_let x = 3"   "\n"
                                         "      _in  x + 1) + 2");
        
        CHECK(( new LetExpr("x", new NumExpr(5), new AddExpr(new NumExpr(2), new LetExpr("x", new NumExpr(3), new AddExpr(new VarExpr("x"), new NumExpr(1))) ) ) )
              ->pretty_print_to_string() == "_let x = 5"      "\n"
                                         "_in  2 + _let x = 3"   "\n"
                                         "         _in  x + 1");
    }
}


TEST_CASE("Let_all methods"){
    
    SECTION("toString (includes print)"){
        CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))) )
              ->to_string() == "(_let x=5 _in (x+1))");
        CHECK((new LetExpr("x", new AddExpr(new NumExpr(5), new NumExpr(2)) , new AddExpr(new VarExpr("x"), new NumExpr(1))) )
              ->to_string() == "(_let x=(5+2) _in (x+1))");
        CHECK( (new LetExpr("x", new NumExpr(5), new LetExpr("x", new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(1)))))
              ->to_string() == "(_let x=5 _in (_let x=6 _in (x+1)))" );
        CHECK( (new LetExpr("x", new NumExpr(5), new LetExpr("y", new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(1)))))
              ->to_string() == "(_let x=5 _in (_let y=6 _in (x+1)))" );
        CHECK( (new LetExpr("x", new NumExpr(5), new LetExpr("x", new AddExpr(new VarExpr("x"),new NumExpr(2)) , new AddExpr(new VarExpr("x"), new NumExpr(1)))))
              ->to_string() == "(_let x=5 _in (_let x=(x+2) _in (x+1)))" );
        CHECK( (new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x"))))
              ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))" );
    }
    SECTION("interp"){
        CHECK( ((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))) )
              ->interp())->equals(new NumVal(6)) );
        CHECK( ((new LetExpr("x", new AddExpr(new NumExpr(5), new NumExpr(2)) , new AddExpr(new VarExpr("x"), new NumExpr(1))) )
              ->interp())->equals(new NumVal(8)) );
        CHECK( ((new LetExpr("x", new NumExpr(5), new LetExpr("x", new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(1)))))
              ->interp())->equals(new NumVal(7)) );
        CHECK( ((new LetExpr("x", new NumExpr(5), new LetExpr("y", new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(1)))))
              ->interp())->equals(new NumVal(6)) );
        CHECK( ((new LetExpr("x", new NumExpr(5), new LetExpr("x", new AddExpr(new VarExpr("x"),new NumExpr(2)) , new AddExpr(new VarExpr("x"), new NumExpr(1)))))
              ->interp())->equals(new NumVal(8)) );
        CHECK( ((new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x"))))
              ->interp())->equals(new NumVal(10)) );
    }
    SECTION("equal"){
        CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))) )
              ->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))));
        CHECK_FALSE((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))) )
                    ->equals(new LetExpr("y", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))));
        CHECK_FALSE((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))) )
                    ->equals(new AddExpr(new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))));
    }
    SECTION("has_variable"){
        CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))) )
              ->has_variable());
        CHECK((new LetExpr("x", new AddExpr(new VarExpr("x"),new NumExpr(2)) , new AddExpr(new NumExpr(3), new NumExpr(1))))
              ->has_variable());
        CHECK_FALSE((new LetExpr("x", new NumExpr(5) , new AddExpr(new NumExpr(3), new NumExpr(1))))
              ->has_variable());
    }
    SECTION("subst"){
        CHECK( ((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))
              ->subst("x", new NumExpr(10))->interp())->equals(new NumVal(6)) );
        CHECK( ((new LetExpr("y", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))
              ->subst("x", new NumExpr(10))->interp())->equals(new NumVal(11)) );
    }
}



TEST_CASE("Test_Equal_Within_Same_Class"){
    
    SECTION("expr_number"){
        NumExpr* num_Base = new NumExpr(3);
        NumExpr* num_Eq = new NumExpr(3);
        NumExpr* num_NoEq = new NumExpr(5);
        NumExpr* num_NoEq_neg = new NumExpr(-3);
        NumExpr* num_NoEq_zero = new NumExpr(0);
        
        
        CHECK( num_Base -> equals(num_Eq) );
        CHECK_FALSE( num_Base -> equals(num_NoEq) );
        CHECK_FALSE( num_Base -> equals(num_NoEq_neg) );
        CHECK_FALSE( num_Base -> equals(num_NoEq_zero) );
    }
    
    SECTION("expr_add"){
        AddExpr* add_Base = new AddExpr(new NumExpr(5), new NumExpr(7));
        AddExpr* add_Eq = new AddExpr(new NumExpr(5), new NumExpr(7));
        AddExpr* add_NoEq = new AddExpr(new NumExpr(10), new NumExpr(20));
        AddExpr* add_NoEq_rev = new AddExpr(new NumExpr(7), new NumExpr(5));
        AddExpr* add_NoEq_neg = new AddExpr(new NumExpr(-5), new NumExpr(-7));
        AddExpr* add_var = new AddExpr(new VarExpr("dog"), new VarExpr("cat"));
        AddExpr* add_var_Eq = new AddExpr(new VarExpr("dog"), new VarExpr("cat"));
        AddExpr* add_var_NoEq = new AddExpr(new VarExpr("PIGGG"), new VarExpr("cat"));
        
        CHECK( add_Base -> equals(add_Eq) );
        CHECK_FALSE( add_Base -> equals(add_NoEq) );
        CHECK_FALSE( add_Base -> equals(add_NoEq_rev) );
        CHECK_FALSE( add_Base -> equals(add_NoEq_neg) );
        CHECK( add_var -> equals(add_var_Eq) );
        CHECK_FALSE( add_var -> equals(add_var_NoEq) );
    }
 
    SECTION("expr_multiple"){
        MultExpr* mult_Base = new MultExpr(new NumExpr(5), new NumExpr(7));
        MultExpr* mult_Eq = new MultExpr(new NumExpr(5), new NumExpr(7));
        MultExpr* mult_NoEq = new MultExpr(new NumExpr(10), new NumExpr(20));
        MultExpr* mult_NoEq_rev = new MultExpr(new NumExpr(7), new NumExpr(5));
        
        CHECK( mult_Base -> equals(mult_Eq) );
        CHECK_FALSE( mult_Base -> equals(mult_NoEq) );
        CHECK_FALSE( mult_Base -> equals(mult_NoEq_rev) );
    }
    
    SECTION("expr_variable"){
        VarExpr* var_Base = new VarExpr("Dog");
        VarExpr* var_Eq = new VarExpr("Dog");
        VarExpr* var_NoEq = new VarExpr("Cat");
        VarExpr* var_NoEq_lower = new VarExpr("dog");
        
        CHECK( var_Base -> equals(var_Eq) );
        CHECK_FALSE( var_Base -> equals(var_NoEq) );
        CHECK_FALSE( var_Base -> equals(var_NoEq_lower) );
    }
}

TEST_CASE("Test_Equal_Not_Same_Class"){
    
    SECTION("num_var"){
        NumExpr* num = new NumExpr(2);
        VarExpr* var = new VarExpr("2");
        CHECK_FALSE( num -> equals(var) );
        CHECK_FALSE( var -> equals(num) );
    }
    
    SECTION("add_mult"){
        AddExpr* add = new AddExpr( new NumExpr(3), new NumExpr(0) );
        MultExpr* mult = new MultExpr ( new NumExpr(3), new NumExpr(0) );
        CHECK_FALSE( add -> equals(mult) );
    }
    
    SECTION("null"){
        CHECK_FALSE( (new AddExpr( new NumExpr(3), new NumExpr(0))) -> equals(NULL) );
        CHECK_FALSE( (new MultExpr( new NumExpr(3), new NumExpr(0))) -> equals(NULL) );
    }
}

TEST_CASE("Test_interp()"){
    
    SECTION("addExpr"){
        CHECK( ((new AddExpr(new AddExpr(new NumExpr(10), new NumExpr(15)),new AddExpr(new NumExpr(20),new NumExpr(20))))->interp())->equals(new NumVal(65))); // num only
        CHECK_THROWS_WITH( (new AddExpr(new VarExpr("dog"), new VarExpr("cat")))->interp(), "no value for variable" ); // var only
        CHECK_THROWS_WITH( (new AddExpr(new VarExpr("dog"), new NumExpr(2)))->interp(), "no value for variable" ); // num + var
    }

    SECTION("multExpr"){
        CHECK( ((new MultExpr(new NumExpr(3), new NumExpr(2)))->interp())->equals(new NumVal(6)) ); // num only
        CHECK_THROWS_WITH( (new MultExpr(new VarExpr("dog"), new VarExpr("cat")))->interp(), "no value for variable" ); // var only
        CHECK_THROWS_WITH( (new MultExpr(new VarExpr("dog"), new NumExpr(2)))->interp(), "no value for variable" ); // num + var
    }
    
    SECTION("numExpr_only"){
        CHECK( ((new NumExpr(5))->interp())->equals(new NumVal(5)) );
    }
    
    SECTION("varExpr_only"){
        CHECK_THROWS_WITH( (new VarExpr("x"))->interp(), "no value for variable" );
    }
   
}

TEST_CASE("Test_hasVariable()"){
    
    SECTION("addExpr"){
        CHECK_FALSE( (new AddExpr(new AddExpr(new NumExpr(10), new NumExpr(15)),new AddExpr(new NumExpr(20),new NumExpr(20))))->has_variable() ); // num only
        CHECK( (new AddExpr(new VarExpr("dog"), new VarExpr("cat")))->has_variable() ); // var only
        CHECK( (new AddExpr(new VarExpr("dog"), new NumExpr(2)))->has_variable() ); // num + var
    }

    SECTION("multExpr"){
        CHECK_FALSE( (new MultExpr(new NumExpr(3), new NumExpr(2)))->has_variable() ); // num only
        CHECK( (new MultExpr(new VarExpr("dog"), new VarExpr("cat")))->has_variable() ); // var only
        CHECK( (new MultExpr(new VarExpr("dog"), new NumExpr(2)))->has_variable() ); // num + var
    }
    
    SECTION("numExpr_only"){
        CHECK_FALSE( (new NumExpr(5))->has_variable() );
    }
    
    SECTION("varExpr_only"){
        CHECK( (new VarExpr("x"))->has_variable() );
    }
}

TEST_CASE("Test_subst()"){
    
    SECTION("addExpr"){
        // did substitute
        CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
               ->subst("x", new VarExpr("y"))
               ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
        // didn't substitute
        CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
               ->subst("no_match_str", new VarExpr("y"))
               ->equals(new AddExpr(new VarExpr("x"), new NumExpr(7))) );
        
    }

    SECTION("multExpr"){
        // did substitute with nested Expr
        CHECK( ( new MultExpr( ( new MultExpr(new VarExpr("x"),new NumExpr(7)) )  ,  (new MultExpr(new VarExpr("x"),new NumExpr(7))) ) )
               ->subst("x", new VarExpr("y"))
               ->equals( new MultExpr( ( new MultExpr(new VarExpr("y"),new NumExpr(7)) )  ,  (new MultExpr(new VarExpr("y"),new NumExpr(7))) ) ) );
        // didn't substitute
        CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))
               ->subst("no_match_str", new VarExpr("y"))
               ->equals(new MultExpr(new VarExpr("x"), new NumExpr(7))) );
    }
    
    SECTION("numExpr_only"){
        // never substitute
        CHECK( (new NumExpr(5))->subst("5", new NumExpr(-1))->equals(new NumExpr(5)) );
    }
    
    SECTION("varExpr_only"){
        // did substitute
        CHECK( (new VarExpr("original"))->subst("original", new VarExpr("new"))->equals(new VarExpr("new")) );
        // didn't substitute
        CHECK( (new VarExpr("original"))->subst("no_match", new VarExpr("new"))->equals(new VarExpr("original")) );
    }
}

TEST_CASE("Test_toString (includes print)"){
    SECTION("expr_number"){
        CHECK( (new NumExpr(10))->to_string() == "10" );
    }
    SECTION("expr_add"){
        CHECK( (new AddExpr( new NumExpr(2), new NumExpr(3)))->to_string() == "(2+3)" );
        CHECK( (new AddExpr( new NumExpr(-2), new NumExpr(-3)))->to_string() == "(-2+-3)" );
        CHECK( (new AddExpr( (new AddExpr( new NumExpr(1), new NumExpr(2))), new NumExpr(3) ) )->to_string() == "((1+2)+3)" );
        CHECK( (new AddExpr( new NumExpr(1), (new AddExpr( new NumExpr(2), new NumExpr(3))) ) )->to_string() == "(1+(2+3))" );
        CHECK( (new AddExpr( new VarExpr("cat"), (new AddExpr( new NumExpr(1), new NumExpr(2))) ) )->to_string() == "(cat+(1+2))" );
    }
    SECTION("expr_mult"){
        CHECK( (new MultExpr( new NumExpr(10), new NumExpr(3)))->to_string() == "(10*3)" );
    }
    SECTION("expr_var"){
        CHECK( (new VarExpr("cat"))->to_string() == "cat" );
    }
}


TEST_CASE("Test_pretty_print_to_string (includes pretty_print)"){
        
    SECTION("expr_add_only"){
        
        // test (1 + 2) + 3
        CHECK( ( new AddExpr( new AddExpr(new NumExpr(1),new NumExpr(2)) , new NumExpr(3) ) )
              ->pretty_print_to_string() == "(1 + 2) + 3" );
        // test 1 + (2 + 3)
        CHECK( ( new AddExpr( new NumExpr(1) , new AddExpr(new NumExpr(2),new NumExpr(3)) ) )
              ->pretty_print_to_string() == "1 + 2 + 3" );
        // test 1 + (2 + (3 + (4 + 5)))
        CHECK( ( new AddExpr( new NumExpr(1) , new AddExpr( new NumExpr(2) , new AddExpr( new NumExpr(3) , new AddExpr(new NumExpr(4),new NumExpr(5))))))
              ->pretty_print_to_string() == "1 + 2 + 3 + 4 + 5");
        // test 1 + (((2 + 3) + 4) + 5)
        CHECK( ( new AddExpr( new NumExpr(1) , new AddExpr( new AddExpr( new AddExpr( new NumExpr(2) , new NumExpr(3)) , new NumExpr(4)) , new NumExpr(5))))
              ->pretty_print_to_string() == "1 + ((2 + 3) + 4) + 5");
    }
    
    SECTION("expr_mult_only"){
        
        // test (1 * 2) * 3
        CHECK( ( new MultExpr( new MultExpr(new NumExpr(1),new NumExpr(2)) , new NumExpr(3) ) )
              ->pretty_print_to_string() == "(1 * 2) * 3" );
        // test 1 + (2 + 3)
        CHECK( ( new MultExpr( new NumExpr(1) , new MultExpr(new NumExpr(2),new NumExpr(3)) ) )
              ->pretty_print_to_string() == "1 * 2 * 3" );
        // test 1 + (2 + (3 + (4 + 5)))
        CHECK( ( new MultExpr( new NumExpr(1) , new MultExpr( new NumExpr(2) , new MultExpr( new NumExpr(3) , new MultExpr(new NumExpr(4),new NumExpr(5))))))
              ->pretty_print_to_string() == "1 * 2 * 3 * 4 * 5");
        // test 1 + (((2 + 3) + 4) + 5)
        CHECK( ( new MultExpr( new NumExpr(1) , new MultExpr( new MultExpr( new MultExpr( new NumExpr(2) , new NumExpr(3)) , new NumExpr(4)) , new NumExpr(5))))
              ->pretty_print_to_string() == "1 * ((2 * 3) * 4) * 5");
        // test (10 * ((10 * 10) * 10)) * (10 * 10)
        CHECK((new MultExpr( new MultExpr(new NumExpr(10), new MultExpr(new MultExpr(new NumExpr(10), new NumExpr(10)), new NumExpr(10))), new MultExpr(new NumExpr(10), new NumExpr(10))))
              ->pretty_print_to_string()  == "(10 * (10 * 10) * 10) * 10 * 10");
    }
    
    SECTION("expr_add_mult_mix"){
        
        // test 1 + (2 * 3)
        CHECK( ( new AddExpr( new NumExpr(1) , new MultExpr(new NumExpr(2),new NumExpr(3)) ) )
              ->pretty_print_to_string() == "1 + 2 * 3" );
        // test (1 * 2) + 3
        CHECK( ( new AddExpr( new MultExpr(new NumExpr(1),new NumExpr(2)) , new NumExpr(3) ) )
              ->pretty_print_to_string() == "1 * 2 + 3");
        // test 1 * (2 + 3)
        CHECK( ( new MultExpr( new NumExpr(1) , new AddExpr(new NumExpr(2),new NumExpr(3)) ) )
              ->pretty_print_to_string() == "1 * (2 + 3)");
        // test (9 * (4 + 3)) + ((2 * 4) + 1)
        CHECK((new AddExpr(new MultExpr(new NumExpr(9), new AddExpr(new NumExpr(4), new NumExpr(3))), new AddExpr(new MultExpr(new NumExpr(2), new NumExpr(4)), new NumExpr(1))))
              ->pretty_print_to_string() == "9 * (4 + 3) + 2 * 4 + 1");
    }
    
    SECTION("expr_number"){
        CHECK(( new NumExpr(3) )-> pretty_print_to_string() == "3");
        CHECK(( new NumExpr(-3) )-> pretty_print_to_string() == "-3");
    }
    
    SECTION("expr_var"){
        CHECK(( new VarExpr("3") )-> pretty_print_to_string() == "3");
        CHECK(( new VarExpr("variable") )-> pretty_print_to_string() == "variable");
    }
    
}


/*
 Kevin & Will's test for let
 */

TEST_CASE("Pretty Print examples_Kevin"){           //Created from assignment examples
    
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))
          ->pretty_print_to_string() == "_let x = 5\n"
                                     "_in  x + 1");

    CHECK((new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1)))
          ->pretty_print_to_string()== "(_let x = 5\n"
                                    " _in  x) + 1");

    CHECK((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))
          ->pretty_print_to_string() == "5 * _let x = 5\n"
                                     "    _in  x + 1");

    CHECK((new AddExpr(new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x"))), new NumExpr(1)))
          ->pretty_print_to_string() == "5 * (_let x = 5\n"
                                     "     _in  x) + 1");

    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x"))))
          ->pretty_print_to_string() == "_let x = 5\n"
                                     "_in  (_let y = 3\n"
                                     "      _in  y + 2) + x");

    CHECK((new LetExpr("x", new LetExpr("y", new NumExpr(6), new MultExpr(new VarExpr("y"), new NumExpr(2))), new AddExpr(new VarExpr("x"), new NumExpr(1))))
          ->pretty_print_to_string() == "_let x = _let y = 6\n"
                                     "         _in  y * 2\n"
                                     "_in  x + 1");
}

TEST_CASE("pretty_print_let_mine_some_reuse_of_kevin_triple_nested_let") {
    LetExpr* tripleNestedLet = new LetExpr("x", new NumExpr(1),
                                   new LetExpr("y", new NumExpr(1),
                                           new MultExpr(new AddExpr(new VarExpr("x"), new VarExpr("y")), new VarExpr("z"))));
    LetExpr* tripleNestedLet2 = new LetExpr("x", new NumExpr(1),
                                    new LetExpr("y", new NumExpr(1),
                                            new LetExpr("z", new AddExpr(new VarExpr("x"), new NumExpr(1)),
                                                    new AddExpr(new AddExpr(new VarExpr("x"), new VarExpr("y")), new VarExpr("z")))));
    LetExpr* tripleNestedLet3 = new LetExpr("x", new NumExpr(1),
                                    new LetExpr("y", new NumExpr(1),
                                            new LetExpr("z", new AddExpr(new VarExpr("x"), new NumExpr(1)),
                                                    new MultExpr(new AddExpr(new VarExpr("x"), new VarExpr("y")), new VarExpr("z")))));


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
    LetExpr* tripleNestedLet4 =new LetExpr("x", new NumExpr(5),
                                   new LetExpr("y", new NumExpr(3),
                                           new AddExpr(new VarExpr("y"), new NumExpr(2))));
    LetExpr* tripleNestedLet5 =new LetExpr("x", new NumExpr(5),
                                   new AddExpr(new LetExpr("y", new NumExpr(3),
                                                   new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x")));

    CHECK(tripleNestedLet4->pretty_print_to_string() ==
                  "_let x = 5\n"
                  "_in  _let y = 3\n"
                  "     _in  y + 2"
    );

    CHECK(tripleNestedLet5 -> pretty_print_to_string() == "_let x = 5\n"
                                                      "_in  (_let y = 3\n"
                                                      "      _in  y + 2) + x");
    SECTION("assignment_examples") {
        CHECK( (new AddExpr(new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x"))), new NumExpr(1)))-> pretty_print_to_string()
               == "5 * (_let x = 5\n"
                  "     _in  x) + 1");
        CHECK( (new MultExpr(new MultExpr(new NumExpr (2), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x") ,new  NumExpr(1)) )), new NumExpr(3) )) -> pretty_print_to_string()
               == "(2 * _let x = 5\n"
                  "     _in  x + 1) * 3");
    }
    // A _let needs parentheses when it is nested immediately as the right argument of an unparenthesized *
    // where _let would have needed parentheses in the surrounding context
    // (that is, if the _let used in place of the whole * would need parentheses,
    // then it still needs parentheses within the right-hand size of *).
    SECTION("new_edge") {
        CHECK( (new MultExpr(new NumExpr (2), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x") ,new  NumExpr(1)) ) )) -> pretty_print_to_string()
               == "2 * _let x = 5\n"
                  "    _in  x + 1");
        CHECK( (new AddExpr(new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(2)))), new NumExpr(1)))-> pretty_print_to_string()
               == "5 * (_let x = 5\n"
                  "     _in  x * 2) + 1");
        CHECK( (new MultExpr((new AddExpr(new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(2)))), new NumExpr(1))), new NumExpr(7)))-> pretty_print_to_string()
               == "(5 * (_let x = 5\n"
                  "      _in  x * 2) + 1) * 7");
        CHECK( (new LetExpr("x", new NumExpr(10), new MultExpr( new MultExpr(new VarExpr("x"), new MultExpr(new MultExpr(new NumExpr(10), new NumExpr(10)), new NumExpr(10))), new MultExpr(new NumExpr(10), new NumExpr(10)))))
                       ->pretty_print_to_string()  == "_let x = 10\n"
                                                      "_in  (x * (10 * 10) * 10) * 10 * 10");
        CHECK( (new LetExpr("x", new NumExpr(1), new MultExpr( new MultExpr(new VarExpr("x"), new MultExpr(new MultExpr(new NumExpr(10), new NumExpr(10)), new VarExpr("x"))), new MultExpr(new NumExpr(10), new NumExpr(10)))))
                        -> pretty_print_to_string() == "_let x = 1\n"
                                                       "_in  (x * (10 * 10) * x) * 10 * 10" );
        CHECK( (new LetExpr("x", new NumExpr(1), new MultExpr( new MultExpr(new VarExpr("x"), new MultExpr(new MultExpr(new NumExpr(10), new NumExpr(10)), new VarExpr("x"))), new MultExpr(new VarExpr("y"), new NumExpr(10)))))
                       -> pretty_print_to_string() == "_let x = 1\n"
                                                      "_in  (x * (10 * 10) * x) * y * 10" );
    }
}

TEST_CASE("Let_equals_mine") {
        SECTION("Values_same") {
            REQUIRE( (new LetExpr("x", new NumExpr(4), new AddExpr(new NumExpr(2), new VarExpr("x")) ))->equals(new LetExpr("x", new NumExpr(4), new AddExpr(new NumExpr(2), new VarExpr("x")) )));
        }
        SECTION("Values_same_different_rhs") {
            REQUIRE( !(new LetExpr("x", new NumExpr(4), new AddExpr(new NumExpr(2), new VarExpr("x")) ))->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new NumExpr(2), new VarExpr("x")) )));
        }
        SECTION("Values_same_different_lhs") {
            REQUIRE( !(new LetExpr("x", new NumExpr(4), new AddExpr(new NumExpr(2), new VarExpr("x")) ))->equals(new LetExpr("y", new NumExpr(4), new AddExpr(new NumExpr(2), new VarExpr("x")) )));
        }
        SECTION("Values_same_different_body") {
            REQUIRE( !(new LetExpr("x", new NumExpr(4), new AddExpr(new NumExpr(2), new VarExpr("x")) ))->equals(new LetExpr("x", new NumExpr(4), new MultExpr(new NumExpr(3), new VarExpr("y")) )));
        }
        SECTION("different_types") {
            REQUIRE( !(new LetExpr("x", new NumExpr(4), new AddExpr(new NumExpr(2), new VarExpr("x")) ))->equals( new MultExpr(new NumExpr(3), new VarExpr("y")) ));
        }
}

TEST_CASE("Let_has_variable_mine") {
    SECTION("has") {
        REQUIRE( (new LetExpr("x", new NumExpr(4), new AddExpr(new NumExpr(2), new VarExpr("x")) ))->has_variable());
    }
    SECTION("does_not_has") {
        REQUIRE( !(new LetExpr("x", new NumExpr(4), new AddExpr(new NumExpr(2), new NumExpr(4)) ))->has_variable());
    }
}
TEST_CASE("Let_print_mine") {
    CHECK( (new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x")))) -> to_string()
                                                                                                        == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    CHECK( (new LetExpr("x", new NumExpr(1),
                   new LetExpr("y", new NumExpr(1),
                           new LetExpr("z", new AddExpr(new VarExpr("x"), new NumExpr(1)),
                                   new MultExpr(new AddExpr(new VarExpr("x"), new VarExpr("y")), new VarExpr("z")))))) -> to_string()
                                                                                                        == "(_let x=1 _in (_let y=1 _in (_let z=(x+1) _in ((x+y)*z))))");
}
TEST_CASE ("Let_interp_mine") {
    SECTION("hw_examples") {
        CHECK( ((new AddExpr(new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x"))), new NumExpr(1))) -> interp())->equals(new NumVal(26)));
        CHECK( ((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))) -> interp())->equals(new NumVal(30)));
    }
    SECTION("from_pretty_print_edge") {
        CHECK( ((new LetExpr("x", new NumExpr(1),
                       new LetExpr("y", new NumExpr(1),
                               new LetExpr("z", new AddExpr(new VarExpr("x"), new NumExpr(1)),
                                       new MultExpr(new AddExpr(new VarExpr("x"), new VarExpr("y")), new VarExpr("z")))))) -> interp())->equals(new NumVal(4)));
        CHECK( ((new MultExpr((new AddExpr(new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(2)))), new NumExpr(1))), new NumExpr(7))) -> interp())->equals(new NumVal(357))); // 51 * 7
        CHECK( ((new LetExpr("x", new NumExpr(10), new MultExpr( new MultExpr(new VarExpr("x"), new MultExpr(new MultExpr(new NumExpr(10), new NumExpr(10)), new NumExpr(10))), new MultExpr(new NumExpr(10), new NumExpr(10)))))
                      ->interp())->equals(new NumVal(1000000)));
        CHECK( ((new LetExpr("x", new NumExpr(1), new MultExpr( new MultExpr(new VarExpr("x"), new MultExpr(new MultExpr(new NumExpr(10), new NumExpr(10)), new VarExpr("x"))), new MultExpr(new NumExpr(10), new NumExpr(10)))))
                       ->interp())->equals(new NumVal(10000)));
        CHECK_THROWS_WITH( (((new LetExpr("x", new NumExpr(1), new MultExpr( new MultExpr(new VarExpr("x"), new MultExpr(new MultExpr(new NumExpr(10), new NumExpr(10)), new VarExpr("x"))), new MultExpr(new VarExpr("y"), new NumExpr(10)))))
                       -> interp())->equals(new NumVal(10000)) ), "no value for variable");
    }
    SECTION("bypass_middle_let") {
        CHECK( ((new LetExpr("x", new NumExpr(2), new LetExpr("z", new NumExpr(4), new AddExpr(new VarExpr("x"), new NumExpr(10)))))
                -> interp())->equals(new NumVal(12)));
    }
}
