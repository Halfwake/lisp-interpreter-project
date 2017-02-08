#include <vector>
#include <string>
#include <iostream>
#include <cctype>

#include "tokenize.hpp"

namespace token {
  std::vector<Token> tokenize(std::istream & stream) {
    std::vector<Token> tokens;
    Token word;
    switch (stream.get()) {
    case '(':
      tokens.push_back("(");
      break;
    case ')':
      tokens.push_back(")");
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      tokens.push_back(word);
      word = "";
      while (isspace(stream.peek()));	
    }
    return tokens;
  }  
}
