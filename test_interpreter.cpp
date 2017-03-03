#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "test_config.hpp"

#define TOKENIZE "[Tokenize]"

#include "tokenize.hpp"

using token::Token;
using token::tokenize;
using token::OPEN_PAREN;
using token::CLOSE_PAREN;
using token::ATOM;

TEST_CASE("Test Token equality.", TOKENIZE) {
  REQUIRE(Token(ATOM, "ABC", 1) == Token(ATOM, "ABC", 1));
  REQUIRE_FALSE(Token(OPEN_PAREN, "(", 1) == Token(CLOSE_PAREN, ")", 1));
  REQUIRE_FALSE(Token(OPEN_PAREN, "(", 1) == Token(OPEN_PAREN, "(", 2));
  REQUIRE_FALSE(Token(ATOM, "", 1) == Token(CLOSE_PAREN, "", 1));
  REQUIRE(Token(ATOM, "", 1) != Token(CLOSE_PAREN, "", 1));
}

TEST_CASE("Test list<Token> equality.", TOKENIZE) {
  std::list<Token> first  = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "12", 1),
    Token(ATOM, "34", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  std::list<Token> same  = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "12", 1),
    Token(ATOM, "34", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  std::list<Token> altered  = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "12", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  REQUIRE(first == same);
  REQUIRE_FALSE(first == altered);
  std::list<Token> tiny = {};
  std::list<Token> big = { Token(OPEN_PAREN, "(", 1) };
  REQUIRE_FALSE(tiny == big);
  REQUIRE_FALSE(big == tiny);
}

TEST_CASE("Test logic.", TOKENIZE) {
  std::stringstream stream("(not True)");
  std::list<Token> expected = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "not", 1),
    Token(ATOM, "True", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  std::list<Token> tokens = tokenize(stream);
  REQUIRE(tokens == expected);
}

TEST_CASE("Test empty stream.", TOKENIZE) {
  std::stringstream stream;
  std::list<Token> tokens = tokenize(stream);
  REQUIRE(tokens.empty());
}

TEST_CASE("Test typical case.", TOKENIZE) {
  std::stringstream stream("(+ 12 34 (* 56 78))");
  std::list<Token> tokens = tokenize(stream);
  std::list<Token> expected = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "+", 1),
    Token(ATOM, "12", 1),
    Token(ATOM, "34", 1),
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "*", 1),
    Token(ATOM, "56", 1),
    Token(ATOM, "78", 1),
    Token(CLOSE_PAREN, ")", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  REQUIRE(tokens == expected);
}

TEST_CASE("Test different amounts of whitespace.", TOKENIZE) {
  std::stringstream stream("  \r\n\t  abc\n\ndef\t\tghi");
  std::list<Token> tokens = tokenize(stream);
  std::list<Token> expected = {
    Token(ATOM, "abc", 2),
    Token(ATOM, "def", 4),
    Token(ATOM, "ghi", 4)
  };
  REQUIRE(tokens == expected);
}

TEST_CASE("Test comments.", TOKENIZE) {
  std::stringstream stream("abc ;\n efg\nghi;asdasd asdasd\n");
  std::list<Token> expected = {
    Token(ATOM, "abc", 1),
    Token(ATOM, "efg", 2),
    Token(ATOM, "ghi", 3)
  };
  std::list<Token> tokens = tokenize(stream);
  REQUIRE(tokens == expected);
}

TEST_CASE("Test single value.", TOKENIZE) {
  std::stringstream stream("(True)");
  std::list<Token> expected = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "True", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  std::list<Token> tokens = tokenize(stream);
  REQUIRE(tokens == expected);
}

#define EXPRESSION "[Expression]"
#define MATCH "[MATCH]"
#define PARSE "[PARSE]"
#define EXPR "[Expression]"

#include "tokenize.hpp"
#include "expression.hpp"

using token::Token;
using token::Type;
using token::OPEN_PAREN;
using token::CLOSE_PAREN;
using token::ATOM;

TEST_CASE("Matching paren tokens.", MATCH) {
  REQUIRE(match_open(Token(OPEN_PAREN, "(", 1)));
  REQUIRE_FALSE(match_open(Token(CLOSE_PAREN, ")", 1)));
  REQUIRE_FALSE(match_open(Token(ATOM, "ABC", 3)));
  REQUIRE_THROWS_AS(match_open(Token(OPEN_PAREN, ")", 1)), InvalidTokenException);

  REQUIRE(match_close(Token(CLOSE_PAREN, ")", 1)));
  REQUIRE_FALSE(match_close(Token(OPEN_PAREN, "(", 1)));
  REQUIRE_FALSE(match_close(Token(ATOM, "ABC", 3)));
  REQUIRE_THROWS_AS(match_open(Token(CLOSE_PAREN, "(", 1)), InvalidTokenException);
}

TEST_CASE("Matching boolean tokens.", MATCH) {
  REQUIRE(match_bool(Token(ATOM, "True", 1)));
  REQUIRE(match_bool(Token(ATOM, "False", 1)));
  REQUIRE_FALSE(match_bool(Token(ATOM, "true", 1)));
  REQUIRE_FALSE(match_bool(Token(ATOM, "false", 3)));
  REQUIRE_FALSE(match_bool(Token(ATOM, "TRUE", 1)));
  REQUIRE_FALSE(match_bool(Token(ATOM, "FALSE", 3)));
  REQUIRE_FALSE(match_bool(Token(ATOM, "notabool", 1)));
}

TEST_CASE("Match none tokens.", MATCH) {
  REQUIRE(match_none(Token(ATOM, "None", 1)));
  REQUIRE_FALSE(match_none(Token(ATOM, "none", 1)));
  REQUIRE_FALSE(match_none(Token(ATOM, "NONE", 1)));
}

TEST_CASE("Match number tokens.", MATCH) {
  REQUIRE(match_number(Token(ATOM, "3", 1)));
  REQUIRE(match_number(Token(ATOM, "314", 1)));
  REQUIRE(match_number(Token(ATOM, "3.14", 1)));
  REQUIRE(match_number(Token(ATOM, "314.", 1)));
  REQUIRE_FALSE(match_number(Token(ATOM, "abc", 1)));
  REQUIRE_FALSE(match_number(Token(ATOM, ".3", 1)));
}

TEST_CASE("Check symbol type", EXPR) {
  REQUIRE(Expression(std::string("some-symbol")).getType() == SYMBOL);
}

TEST_CASE("Check expression type.", EXPR) {
  REQUIRE(Expression().getType() == NONE);
  REQUIRE(Expression(true).getType() == BOOL);
  REQUIRE(Expression(std::string("begin")).getType() == SYMBOL);
  REQUIRE(Expression(3.14).getType() == NUMBER);
  std::vector<Expression> children = {
    Expression()
  };
  REQUIRE(Expression(children).getType() == LIST);
}

// We want to support bare symbols or not?
/*
TEST_CASE("Bare symbol.", PARSE) {
  std::list<Token> tokens = {
    Token(ATOM, "abc", 1)
  };
  std::vector<Expression> shallow_children = {
    Expression(std::string("abc"))
  };
  std::vector<Expression> children = {
    Expression(shallow_children)
  };
  std::vector<Expression> top = {
    Expression(std::string("begin")),
    children
  };
  Expression expected = Expression(top);
  REQUIRE(expected == parse_tokens(tokens));
}
*/

TEST_CASE("Typical case.", PARSE) {
  std::list<Token> tokens = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "+", 1),
    Token(ATOM, "12", 1),
    Token(ATOM, "18", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  std::vector<Expression> children = {
    Expression(std::string("+")),
    Expression(12 * 1.0),
    Expression(18 * 1.0),
  };
  std::vector<Expression> top = {
    Expression(std::string("begin")),
    Expression(children)
  };
  Expression expected = Expression(top);
  REQUIRE(expected == parse_tokens(tokens));
}

TEST_CASE("Harder case.", PARSE) {
  std::list<Token> tokens = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "+", 1),
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "-", 1),
    Token(ATOM, "pi", 1),
    Token(ATOM, "1.5", 1),
    Token(CLOSE_PAREN, ")", 1),
    Token(ATOM, "3", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  std::vector<Expression> deep_children = {
    Expression(std::string("-")),
    Expression(std::string("pi")),
    Expression(1.5 * 1.0)
  };
  std::vector<Expression> shallow_children = {
    Expression(std::string("+")),
    Expression(deep_children),
    Expression(3 * 1.0)
  };
  std::vector<Expression> top = {
    Expression(std::string("begin")),
    Expression(shallow_children)
  };
  Expression expected = Expression(top);
  REQUIRE(expected == parse_tokens(tokens));
}

TEST_CASE("Boolean expression.", PARSE) {
  std::list<Token> tokens = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "not", 1),
    Token(ATOM, "True", 1),
    Token(CLOSE_PAREN, ")", 1)
  };

  std::vector<Expression> children = {
    Expression(std::string("not")),
    Expression(true)
  };

  std::vector<Expression> top = {
    Expression(std::string("begin")),
    Expression(children)
  };

  Expression expected = Expression(top);
  REQUIRE(expected == parse_tokens(tokens));
}

TEST_CASE("Simple value.", PARSE) {
  std::list<Token> tokens = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "True", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  std::vector<Expression> children = {
    Expression(true)
  };
  std::vector<Expression> top = {
    Expression(std::string("begin")),
    Expression(children)
  };
  Expression expected = Expression(top);
  REQUIRE(expected == parse_tokens(tokens));
}


Expression run(const std::string & program){
  
  std::istringstream iss(program);
    
  Interpreter interp;
    
  bool ok = interp.parse(iss);
  if(!ok){
    std::cerr << "Failed to parse: " << program << std::endl; 
  }
  REQUIRE(ok == true);

  Expression result;
  REQUIRE_NOTHROW(result = interp.eval());

  return result;
}

Expression runfile(const std::string & fname){

  std::ifstream ifs(fname);
  REQUIRE(ifs.good() == true);
  
  Interpreter interp;
    
  bool ok = interp.parse(ifs);
  if(!ok){
    std::cerr << "Failed to parse file: " << fname << std::endl; 
  }
  REQUIRE(ok == true);

  Expression result;
  REQUIRE_NOTHROW(result = interp.eval());

  return result;
}

TEST_CASE( "Test Interpreter parser with expected input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);
 
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == true);
}

TEST_CASE( "Test Interpreter parser with truncated input", "[interpreter]" ) {

  {
    std::string program = "(f";
    std::istringstream iss(program);
  
    Interpreter interp;
    bool ok = interp.parse(iss);
    REQUIRE(ok == false);
  }
  
  {
    std::string program = "(begin (define r 10) (* pi (* r r";
    std::istringstream iss(program);

    Interpreter interp;
    bool ok = interp.parse(iss);
    REQUIRE(ok == false);
  }
}

TEST_CASE( "Test Interpreter parser with extra input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r))) )";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with single non-keyword", "[interpreter]" ) {

  std::string program = "hello";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with empty input", "[interpreter]" ) {

  std::string program;
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with empty expression", "[interpreter]" ) {

  std::string program = "( )";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with bad number string", "[interpreter]" ) {

  std::string program = "(1abc)";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter result with literal expressions", "[interpreter]" ) {

  { // Boolean True
    std::string program = "(True)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  { // Boolean False
    std::string program = "(False)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }

  { // Number
    std::string program = "(4)";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }

  { // Symbol
    std::string program = "(pi)";
    Expression result = run(program);
    REQUIRE(result == Expression(atan2(0, -1)));
  }

}


TEST_CASE( "Test Interpreter result with simple procedures (add)", "[interpreter]" ) {

  { // add, binary case
    std::string program = "(+ 1 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(3.));
  }
  
  { // add, 3-ary case
    std::string program = "(+ 1 2 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(6.));
  }

  { // add, 6-ary case
    std::string program = "(+ 1 2 3 4 5 6)";
    Expression result = run(program);
    REQUIRE(result == Expression(21.));
  }
}
  
TEST_CASE( "Test Interpreter special form: if", "[interpreter]" ) {

  {
    std::string program = "(if True (4) (-4))";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }
  
  {
    std::string program = "(if False (4) (-4))";
    Expression result = run(program);
    REQUIRE(result == Expression(-4.));
  }
}

TEST_CASE( "Test Interpreter special forms: begin and define", "[interpreter]" ) {

  {
    std::string program = "(define answer 42)";
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }

  {
    std::string program = "(begin (define answer 42)\n(answer))";
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }
  
  {
    std::string program = "(begin (define answer (+ 9 11)) (answer))";
    Expression result = run(program);
    REQUIRE(result == Expression(20.));
  }

  {
    std::string program = "(begin (define a 1) (define b 1) (+ a b))";
    Expression result = run(program);
    REQUIRE(result == Expression(2.));
  }
}

TEST_CASE( "Test a complex expression", "[interpreter]" ) {

  {
    std::string program = "(+ (+ 10 1) (+ 30 (+ 1 1)))";
    Expression result = run(program);
    REQUIRE(result == Expression(43.));
  }
}

TEST_CASE( "Test relational procedures", "[interpreter]" ) {

  {
    std::vector<std::string> programs = {"(< 1 2)",
					 "(<= 1 2)",
					 "(<= 1 1)",
					 "(> 2 1)",
					 "(>= 2 1)",
					 "(>= 2 2)",
					 "(= 4 4)"};
    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(true));
    }
  }

  {
    std::vector<std::string> programs = {"(< 2 1)",
					 "(<= 2 1)",
					 "(<= 1 0)",
					 "(> 1 2)",
					 "(>= 1 2)",
					 "(>= 2 3)",
					 "(= 0 4)"};
    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(false));
    }
  }
}

TEST_CASE( "Test arithmetic procedures", "[interpreter]" ) {

  {
    std::vector<std::string> programs = {"(+ 1 -2)",
					 "(+ -3 1 1)",
					 "(- 1)",
					 "(- 1 2)",
					 "(* 1 -1)",
					 "(* 1 1 -1)",
					 "(/ -1 1)",
					 "(/ 1 -1)"};

    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(-1.));
    }
  }
}

TEST_CASE( "Test logical procedures", "[interpreter]" ) {

  REQUIRE(run("(not True)") == Expression(false));
  REQUIRE(run("(not False)") == Expression(true));

  REQUIRE(run("(and True True)") == Expression(true));
  REQUIRE(run("(and True False)") == Expression(false));
  REQUIRE(run("(and False True)") == Expression(false));
  REQUIRE(run("(and False False)") == Expression(false));
  REQUIRE(run("(and True True False)") == Expression(false));

  REQUIRE(run("(or True True)") == Expression(true));
  REQUIRE(run("(or True False)") == Expression(true));
  REQUIRE(run("(or False True)") == Expression(true));
  REQUIRE(run("(or False False)") == Expression(false));
  REQUIRE(run("(or True True False)") == Expression(true));
}

TEST_CASE( "Test some semantically invalid expresions", "[interpreter]" ) {
  
  std::vector<std::string> programs = {"(@ none)", // so such procedure
				       "(- 1 1 2)", // too many arguments
				       "(define if 1)", // redefine special form
				       "(define pi 3.14)"}; // redefine builtin symbol
    for(auto s : programs){
      Interpreter interp;

      std::istringstream iss(s);
      
      bool ok = interp.parse(iss);
      REQUIRE(ok == true);

      REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
    }
}

TEST_CASE( "Test file tests/test0.vts", "[interpreter]" ) {

  std::string fname = TEST_FILE_DIR + "/test0.vts";

  std::ifstream ifs(fname);
  REQUIRE(ifs.good() == true);
  
  Interpreter interp;
    
  bool ok = interp.parse(ifs);
  REQUIRE(ok == false);
}

TEST_CASE( "Test syntactically INCORRECT files", "[interpreter]" ) {

  std::string fname = TEST_FILE_DIR + "/test1.vts";

  std::ifstream ifs(fname);
  REQUIRE(ifs.good() == true);
  
  Interpreter interp;
    
  bool ok = interp.parse(ifs);
  REQUIRE(ok == false);
}

// Comment out temporarily, since it's been hanging.

TEST_CASE( "Test all syntactically and semantically CORRECT files.", "[interpreter]" ) {

  const int START_TEST = 2;
  const int LAST_TEST = 4;
  std::string base = TEST_FILE_DIR + "/test";
  
  for(int i = START_TEST; i <= LAST_TEST; ++i){
    std::string input = base + std::to_string(i) + ".vts";
    std::string expect = input + ".expected";
    Expression result = runfile(input);
    Expression expected_result = runfile(expect);
  
    REQUIRE(result == expected_result);
  }
}


