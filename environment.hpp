#include <map>
#include <string>
#include <exception>
#include <stdexcept>

#include "expression.hpp"

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

using expression::Expression;

namespace environment {

  typedef std::string Symbol;

/*
 * Represents a mapping between the identifiers in a variable and the
 * variable's value. This is a global environment and has no support
 * for local scoping.
 */
 class Environment {
 private:
   std::map<Symbol, Expression> map;
 public:
   Expression & get(const Symbol symbol);
   void set(const Symbol symbol, const Expression expr);
 };


 /*
  * Throw when a variable appears that has not been defined. The
  * constructor should be called with the missing symbol.
  */
 class LookupException : public std::exception {
 public:
   LookupException (Symbol symbol) : symbol(symbol) {};
   Symbol getSymbol();
   const char * what() const noexcept;
 private:
   Symbol symbol;
 };

}

#endif
