#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#define EXPRESSION "[Expression]"
#define MATCH "[MATCH]"

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
}
