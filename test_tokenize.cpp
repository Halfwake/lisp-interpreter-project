#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <string>
#include <sstream>
#include <algorithm>

#define TOKENIZE "[Tokenize]"

#include "tokenize.hpp"

using token::Token;
using token::tokenize;

TEST_CASE("Test empty stream.", TOKENIZE) {
  std::stringstream stream;
  std::vector<Token> tokens = tokenize(stream);
  REQUIRE(tokens.empty());
}

TEST_CASE("Test typical case.", TOKENIZE) {
  std::stringstream stream("(+ 12 34 (* 56 78))");
  std::vector<Token> tokens = tokenize(stream);
  std::vector<Token> expected = { "(", "12", "34", "(", "56", "78", ")", ")" };
  REQUIRE(std::equal(tokens.begin(), tokens.end(), expected.begin()));
}
