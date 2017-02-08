#include <vector>
#include <string>
#include <iostream>
#include <cctype>

#include "tokenize.hpp"

namespace token {
  std::vector<Token> tokenize(std::istream & stream) {
    std::vector<Token> tokens;
    std::vector<char> word;
    size_t lineNumber = 1;
    switch (stream.peek()) {
    case '(':
      tokens.push_back(Token(OPEN_PAREN, "(", lineNumber));
      break;
    case ')':
      tokens.push_back(Token(CLOSE_PAREN, ")", lineNumber));
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      while (isspace(stream.peek())) {
	if(stream.get() == '\n') {
	  lineNumber++;
	}
      }
      if (!word.empty()) {
	std::string text(word.begin(), word.end());
	tokens.push_back(Token(ATOM, text, lineNumber));
	word.clear();
      }
      break;
    default:
      word.push_back(stream.get());
    }
    return tokens;
  }

  Token::Token(Type type, std::string text, size_t lineNumber) {
    this->type = type;
    this->text = text;
    this->lineNumber = lineNumber;
  }

  bool Token::operator==(const Token & other) {
    return (type == other.type) && (text == other.text) && (lineNumber == other.lineNumber);
  }
  
  bool Token::operator!=(const Token & other) {
    return !(*this == other);
  }

  Type Token::getType() {
    return this->type;
  }

  std::string Token::getText() {
    return this->text;
  }

  size_t Token::getLineNumber() {
    return lineNumber;
  }
}
