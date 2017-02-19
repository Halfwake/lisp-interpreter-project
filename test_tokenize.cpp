#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

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
