#define CATCH_CONFIG_MAIN

#include "catch.hpp"

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
  std::list<Expression> children = {
    Expression()
  };
  REQUIRE(Expression(children).getType() == LIST);
}

TEST_CASE("Typical case.", PARSE) {
  std::list<Token> tokens = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "+", 1),
    Token(ATOM, "12", 1),
    Token(ATOM, "18", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  std::list<Expression> children = {
    Expression(std::string("+")),
    Expression(12 * 1.0),
    Expression(18 * 1.0),
  };
  std::list<Expression> top = {
    Expression(std::string("begin")),
    Expression(children)
  };
  Expression expected = Expression(top);
  REQUIRE(expected == parse_tokens(tokens));
}
