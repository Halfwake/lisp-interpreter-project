/*
 * The actual interpreter is defined here. See the comment before the
 * main function for usage details.
 * Happy Hacking~
 */

#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>

#include "interpreter.hpp"
#include "expression.hpp"
#include "interpreter_semantic_error.hpp"

/*
 * This is a little helper function for displaying expressions to the
 * REPL. I'm using this instead of the << operator, because the REPL
 * output must be formatted in a specific way. This should probably
 * throw an exception if given an unsimplified type, TODO!
 */
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

/*
 * The main routine. It can run vtscript code in one of three ways
 * depending on how it's called. If the program is called with no
 * arguments, an interactive REPL will start. If the program is called
 * with a file name, the program will attempt to run that
 * file. Finally, if the program is called with '-e' and then a string
 * containting vtscript code, the program will attempt to run that
 * string. This last behavior is similar to 'python -c' or 'perl -e'.
 */
int main(int argc, char * argv[]) {
  Interpreter interpreter;
  // Interpretter case.
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
	  interpreter = Interpreter();
	}
      } else {
	std::cout << "Error" << std::endl;
	return EXIT_FAILURE;
      }
    }
    // File case.
  } else if (argc == 2) {
    std::ifstream stream(argv[1]);
    if (!stream.good()) {
	std::cout << "Error" << std::endl;
	return EXIT_FAILURE;
    }
    std::ifstream & stream_ref = stream;
    if(interpreter.parse(stream_ref)){
      try {
	print_expression(interpreter.eval());
      } catch (InterpreterSemanticError e) {
	interpreter = Interpreter();
      }
    }
    // -e Case
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
