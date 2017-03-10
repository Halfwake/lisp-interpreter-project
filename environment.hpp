#include <map>
#include <string>
#include <exception>
#include <stdexcept>

#include "expression.hpp"

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

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
   void set(const Symbol symbol, Expression expr);
   void reset();
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

  /*
   * Throw when a variable has already been defined.
   */
  class SetException : public std::exception {
  public:
    SetException (Symbol symbol) : symbol(symbol) {};
    Symbol getSymbol();
    const char * what() const noexcept;
  private:
    Symbol symbol;
  };

}

#endif
