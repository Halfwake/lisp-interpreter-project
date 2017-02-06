#include "environment.hpp"
#include <iostream>

using environment::Environment;
using environment::LookupException;

int main() {
  auto env = Environment();
  std::cout << "Created environment."  << std::endl;
  env.set("work", 3);
  std::cout << "Set [work] to [3]." << std::endl;
  std::cout << "Work was [" << env.get("work") << "]" << std::endl;
}
