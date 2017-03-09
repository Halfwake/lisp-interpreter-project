#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>

#include "interpreter.hpp"
#include "expression.hpp"
#include "interpreter_semantic_error.hpp"

void print_expression(Expression expr) {
  std::cout << "(";
  switch (expr.getType()) {
  case NONE:
    std::cout << "None";
    break;
  case BOOL:
    if (expr.getBool()) {
      std::cout << "True";
    } else {
      std::cout << "False";
    }
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
      std::cout << "vtscript>";
      std::getline(std::cin, line);
      std::stringstream stream(line);
      if (interpreter.parse(stream)) {
	try {
	  print_expression(interpreter.eval());
	} catch (InterpreterSemanticError e) {
	  std::cout << "Error" << std::endl;
	  return EXIT_FAILURE;
	}
      } else {
	std::cout << "Error" << std::endl;
	return EXIT_FAILURE;
      }
    }
  } else if (argc == 2) {
    std::ifstream stream(argv[1]);
    if (!stream.good()) {
	std::cout << "Error" << std::endl;
	return EXIT_FAILURE;
    }
    std::ifstream & stream_ref = stream;
    if(interpreter.parse(stream_ref)){
      print_expression(interpreter.eval());
    } 
  } else if ((argc == 3) && (std::string(argv[1]) == "-e")) {
    std::stringstream stream(argv[2]);
    if (interpreter.parse(stream)) {
      try {
	print_expression(interpreter.eval());
      } catch (InterpreterSemanticError e) {
	std::cout << "Error" << std::endl;
	return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
    } else {
      std::cout << "Error" << std::endl;
      return EXIT_FAILURE;
    }
  }
}
