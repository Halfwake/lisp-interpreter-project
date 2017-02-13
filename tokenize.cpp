#include <vector>
#include <string>
#include <ostream>
#include <cctype>
#include <cstdio>

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
      case ';':
	if (!word.empty()) {
	  std::string text(word.begin(), word.end());
	  tokens.push_back(Token(ATOM, text, lineNumber));
	  word.clear();
	}
	while(stream.get() != '\n');
	lineNumber++;
	break;
      case ' ':
      case '\t':
      case '\r':
      case '\n':
	if (!word.empty()) {
	  std::string text(word.begin(), word.end());
	  tokens.push_back(Token(ATOM, text, lineNumber));
	  word.clear();
	}
	while (isspace(stream.peek())) {
	  if(stream.get() == '\n') {
	    lineNumber++;
	  }
	}
	break;
      case EOF:
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

  Token::Token(const Token & other) {
    this->type = other.getType();
    this->text = other.getText();
    this->lineNumber = other.getLineNumber();
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


