#include <string>
#include <list>
#include <exception>
#include <stdexcept>

#include "tokenize.hpp"

#ifndef EXPRESSION_H
#define EXPRESSION_H

enum AtomType {
  NONE,
  BOOL,
  SYMBOL,
  NUMBER,
  LIST
};

class Expression {
public:
  Expression();
  Expression(const Expression & other);
  Expression(bool value);
  Expression(double value);
  Expression(const std::string value);
  Expression(const std::list<Expression>);
  AtomType getType() const;
  bool operator==(const Expression & other) const noexcept;
  friend std::ostream & operator << (std::ostream & stream, const Expression & expr);
private:
  AtomType type;
  bool bool_value;
  double number_value;
  std::string symbol_value;
  std::list<Expression> children;
};

class InvalidTokenException : public std::exception {
public:
  InvalidTokenException(token::Token token) : token(token) {};
  token::Token getToken();
  const char * what() const noexcept;
private:
  token::Token token;
};

Expression parse_atom(token::Token token);

bool match_open(token::Token token);
bool match_close(token::Token token);
bool match_bool(token::Token token);
bool match_none(token::Token token);
bool match_number(token::Token token);
bool match_symbol(token::Token token);

Expression parse_tokens(std::list<token::Token> tokens);
Expression parse_tokens_iter(std::list<token::Token> & tokens);

#endif
