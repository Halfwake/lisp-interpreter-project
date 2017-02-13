#include <vector>
#include <string>
#include <iostream>

#IFNDEF TOKEN_H
#define TOKEN_H

namespace token {
  enum Type { OPEN_PAREN, CLOSE_PAREN, ATOM };
  
  class Token {
  public:
    Token(Type type, std::string text, size_t lineNumber);
    Token(const Token & other);
    Type getType() const;
    std::string getText() const;
    size_t getLineNumber() const;
    bool operator==(const Token & other) const;
    bool operator!=(const Token & other) const;
    friend std::ostream & operator << (std::ostream & stream, const Token & token);
  private:
    Type type;
    std::string text;
    size_t lineNumber;
  };

  std::vector<Token> tokenize(std::istream & stream);
}

#endif
