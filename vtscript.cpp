#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>

#include "interpreter.hpp"
#include "expression.hpp"

void print_expression(Expression expr) {
  std::cout << "(";
  switch (expr.getType()) {
  case NONE:
    std::cout << "None";
    break;
  case BOOL:
    std::cout << expr.getBool();
    break;
  case NUMBER:
    std::cout << expr.getNumber();
    break;
  default:
    std::cout << "Error: bad return.";
  }
  std::cout << ")" << std::endl;
}

int main(int argc, char * argv[]) {
  Interpreter interpreter;
  if (argc == 1) {
    std::string line;
    while (true) {
      std::getline(std::cin, line);
      std::stringstream stream(line);
      if (interpreter.parse(stream)) {
	print_expression(interpreter.eval());
      }
    }
  } else if (argc == 2) {
    /*
    std::ifstream ifs(std::string(argv[1]));
    std::istream & stream = ifs;
    if(interpreter.parse(stream)){
      std::cerr << "Failed to parse file: " << argv[1] << std::endl; 
    }
    std::cout << "(" << interpreter.eval() << ")" << std::endl;
    */
  } else if ((argc == 3) && (argv[1] == "-e")) {
    std::stringstream stream(argv[2]);
    if (interpreter.parse(stream)) {
      std::cout << "(" << interpreter.eval() << ")" << std::endl;
      return EXIT_SUCCESS;
    } else {
      return EXIT_FAILURE;
    }
  }
}
