#include "expression.hpp"

class Interpreter {
public:
  Interpreter();
  bool parse(std::istream & expression) noexcept;
  Expression eval();
private:
  Expression expression;
};
