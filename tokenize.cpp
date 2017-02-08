#include <vector>
#include <string>
#include <iostream>
#include <cctype>

#include "tokenize.hpp"

namespace token {
  std::vector<Token> tokenize(std::istream & stream) {
    std::vector<Token> tokens;
    std::vector<char> word;
    switch (stream.peek()) {
    case '(':
      tokens.push_back("(");
      break;
    case ')':
      tokens.push_back(")");
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      while (isspace(stream.peek())) {
	stream.get();
      }
      if (!word.empty()) {
	std::string token(word.begin(), word.end());
	tokens.push_back(token);
	word.clear();
      }
      break;
    default:
      word.push_back(stream.get());
    }
    return tokens;
  }  
}
