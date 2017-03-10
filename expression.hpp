#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

#include "tokenize.hpp"

#ifndef EXPRESSION_H
#define EXPRESSION_H

/*
 * An expression can be one of several types. If it's an atom
 * expression, it can be None, a boolean, a symbol, or a number. If
 * the expressions is a sexpr then it's a list.
 */
enum AtomType {
  NONE,
  BOOL,
  SYMBOL,
  NUMBER,
  LIST
};

/*
 * An expression object. Expressions are a kind of tree represented by
 * vectors of vectors. They can be simplified by eval functions.
 */
class Expression {
public:
  Expression();
  Expression(const Expression & other);
  Expression(bool value);
  Expression(double value);
  Expression(const std::string value);
  Expression(const std::vector<Expression>);
  AtomType getType() const;
  std::vector<Expression> getChildren() const;
  bool getBool() const;
  double getNumber() const;
  std::string getSymbol() const;
  bool operator==(const Expression & other) const noexcept;
  friend std::ostream & operator << (std::ostream & stream, const Expression & expr);
private:
  AtomType type;
  bool bool_value;
  double number_value;
  std::string symbol_value;
  std::vector<Expression> children;
};

/*
 * Throw if a token cannot be parsed into an expression. For instance,
 * the token with text "34ff" could not be parsed.
 */
class InvalidTokenException : public std::exception {
public:
  InvalidTokenException(token::Token token) : token(token) {};
  token::Token getToken();
  const char * what() const noexcept;
private:
  token::Token token;
};

/*
 * Take an atom token and return an atom expression.
 */
Expression parse_atom(token::Token token);

/*
 * Utility functions. They return true if the given token matches a certain type.
 */
bool match_open(token::Token token);
bool match_close(token::Token token);
bool match_bool(token::Token token);
bool match_none(token::Token token);
bool match_number(token::Token token);
bool match_symbol(token::Token token);

/*
 * Take a list of tokens and return an expression tree.
 */
Expression parse_tokens(std::list<token::Token> tokens);

/*
 * A recursive helper function for parse_tokens. The start of parse is
 * a special case, so parse_tokens_iter has to be wrapped by
 * parse_tokens.
 */
Expression parse_tokens_iter(std::list<token::Token> & tokens);

#endif
