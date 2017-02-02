#include <map>

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


  LookupException (const Symbol symbol) {
    this.symbol = symbol;
  }

}
