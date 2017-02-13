#include <vector>
#include <string>
#include <ostream>
#include <cctype>

#include "tokenize.hpp"

namespace token {
  std::vector<Token> tokenize(std::istream & stream) {
    std::vector<Token> tokens;
    std::vector<char> word;
    size_t lineNumber = 1;
    while (!stream.eof()) {
      switch (stream.peek()) {
      case '(':
	if (!word.empty()) {
	  std::string text(word.begin(), word.end());
	  tokens.push_back(Token(ATOM, text, lineNumber));
	  word.clear();
	}
	tokens.push_back(Token(OPEN_PAREN, "(", lineNumber));
	stream.get();
	break;
      case ')':
	if (!word.empty()) {
	  std::string text(word.begin(), word.end());
	  tokens.push_back(Token(ATOM, text, lineNumber));
	  word.clear();
	}
	tokens.push_back(Token(CLOSE_PAREN, ")", lineNumber));
	stream.get();
	break;
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
    }
    return tokens;
  }

  Token::Token(Type type, std::string text, size_t lineNumber) {
    this->type = type;
    this->text = text;
    this->lineNumber = lineNumber;
  }

  bool Token::operator==(const Token & other) const {
    return (type == other.type) && (text == other.text) && (lineNumber == other.lineNumber);
  }
  
  bool Token::operator!=(const Token & other) const {
    return (*this) == other;
  }

  Type Token::getType() const {
    return this->type;
  }

  std::string Token::getText() const {
    return this->text;
  }

  size_t Token::getLineNumber() const {
    return lineNumber;
  }

  std::ostream & operator << (std::ostream & stream, const token::Token & token) {
    stream << "(" << token.getType()
	   << "|" << token.getText()
	   << "|" << token.getLineNumber()
	   << ")";
    return stream;
  }
}


