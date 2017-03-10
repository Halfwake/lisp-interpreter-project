#include <list>
#include <string>
#include <iostream>

#ifndef TOKEN_H
#define TOKEN_H

namespace token {
  /*
   * A token can either be an opening paren, a closing paren, or an atom.
   */
  enum Type { OPEN_PAREN, CLOSE_PAREN, ATOM };

  /*
   * This class represents a token for the parser. It includes the
   * line number the token appeard on for debugging purposes.
   */
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

  /*
   * Take a text stream and return a list of tokens. A list is
   * returned so that the parser can pull tokens off the list.
   */
  std::list<Token> tokenize(std::istream & stream);
}

#endif
