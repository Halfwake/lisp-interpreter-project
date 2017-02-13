#include <string>
#include <vector>

class Expression {
public:
  Expression();
  Expression(bool value);
  Expression(double value);
  Expression(const std::string & value);
  Expression(const std::vector<Expression>);
  bool operator==(const Expression & other) const noexcept;
};

