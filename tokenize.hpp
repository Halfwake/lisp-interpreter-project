#include <vector>
#include <string>
#include <iostream>

namespace token {
  typedef std::string Token;
  std::vector<Token> tokenize(std::istream & stream);
}
