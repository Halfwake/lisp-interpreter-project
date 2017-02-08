#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <string>
#include <sstream>

#define TOKENIZE "[Tokenize]"

#include "tokenize.hpp"

using token::Token;
using token::tokenize;

TEST_CASE("Test empty stream.", TOKENIZE) {
  std::stringstream stream;
  std::vector<Token> tokens = tokenize(stream);
  REQUIRE(tokens.empty());
}
