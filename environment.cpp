#include "environment.hpp"

#include <map>
#include <exception>

#include "tokenize.hpp"

using tokenize::Expression;

namespace environment {

  Expression & Environment::get(const Symbol symbol) {
    if (map.find(symbol) != map.end()) {
      return map[symbol];
    } else {
      throw LookupException(symbol);
    }
  }

  void Environment::set(const Symbol symbol, const Expression expr) {
    map[symbol] = expr;
  }

  Symbol LookupException::getSymbol() {
    return this->symbol;
  }

  const char * LookupException::what () const noexcept {
    return symbol.c_str();
  }

}
