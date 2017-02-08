#include <vector>
#include <string>
#include <iostream>

namespace token {
  enum Type { OPEN_PAREN, CLOSE_PAREN, ATOM };
  
  class Token {
  public:
    Token(Type type, std::string text, size_t lineNumber);
    Type getType();
    std::string getText();
    size_t getLineNumber();
    bool operator==(const Token & other);
    bool operator!=(const Token & other);
  private:
    Type type;
    std::string text;
    size_t lineNumber;
  };

  std::vector<Token> tokenize(std::istream & stream);
}
