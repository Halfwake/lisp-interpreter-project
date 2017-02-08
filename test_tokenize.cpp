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

TEST_CASE("Test empty stream.", TOKENIZE) {
  std::stringstream stream;
  std::vector<Token> tokens = tokenize(stream);
  REQUIRE(tokens.empty());
}

TEST_CASE("Test typical case.", TOKENIZE) {
  std::stringstream stream("(+ 12 34 (* 56 78))");
  std::vector<Token> tokens = tokenize(stream);
  std::vector<Token> expected = {
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "12", 1),
    Token(ATOM, "34", 1),
    Token(OPEN_PAREN, "(", 1),
    Token(ATOM, "56", 1),
    Token(ATOM, "78", 1),
    Token(CLOSE_PAREN, ")", 1),
    Token(CLOSE_PAREN, ")", 1)
  };
  REQUIRE(std::equal(tokens.begin(), tokens.end(), expected.begin()));
}

TEST_CASE("Test different amounts of whitespace.", TOKENIZE) {
  std::stringstream stream("  \r\n\t  abc\n\ndef\t\tghi");
  std::vector<Token> tokens = tokenize(stream);
  std::vector<Token> expected = {
    Token(ATOM, "abc", 2),
    Token(ATOM, "def", 4),
    Token(ATOM, "ghi", 4)
  };
  REQUIRE(std::equal(tokens.begin(), tokens.end(), expected.begin()));
}
