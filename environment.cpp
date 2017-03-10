#include "environment.hpp"

#include <map>
#include <exception>

#include "expression.hpp"

namespace environment {

  Expression & Environment::get(const Symbol symbol) {
    if (map.find(symbol) != map.end()) {
      return map[symbol];
    } else {
      throw LookupException(symbol);
    }
  }

  void Environment::set(const Symbol symbol, Expression expr) {
    if (map.find(symbol) == map.end()) {
      map[symbol] = expr;
    } else {
      throw SetException(symbol);
    }
  }

  void Environment::reset() {
    map.clear();
  }

  Symbol LookupException::getSymbol() {
    return this->symbol;
  }

  const char * LookupException::what () const noexcept {
    return symbol.c_str();
  }

  Symbol SetException::getSymbol() {
    return this->symbol;
  }

  const char * SetException::what () const noexcept {
    return symbol.c_str();
  }

}
