#include <sstream>

#include "expression.hpp"

#include "tokenize.hpp"

bool match_open(token::Token token) {
  bool correct_type = token.getType() == token::OPEN_PAREN;
  bool correct_string = token.getText() == "(";
  if (correct_type && !correct_string) {
    throw InvalidTokenException(token);
  }
  if (!correct_type && correct_string) {
    throw InvalidTokenException(token);
  }
  return correct_type && correct_string;
}

bool match_close(token::Token token) {
  bool correct_type = token.getType() == token::CLOSE_PAREN;
  bool correct_string = token.getText() == ")";
  if (correct_type && !correct_string) {
    throw InvalidTokenException(token);
  }
  if (!correct_type && correct_string) {
    throw InvalidTokenException(token);
  }
  return correct_type && correct_string;
}

bool match_bool(token::Token token) {
  bool correct_type = token.getType() == token::ATOM;
  bool true_string  = token.getText() == "True";
  bool false_string = token.getText() == "False";
  return correct_type && (true_string || false_string);
}

bool match_none(token::Token token) {
  bool correct_type = token.getType() == token::ATOM;
  bool none_string = token.getText() == "None";
  return correct_type && none_string;
}

bool match_number(token::Token token) {
  bool first_char = true;
  bool hit_dot = false;
  bool hit_e = false;
  bool number_after_e = false;
  for (auto character : token.getText()) {
    switch (character) {
    case '+':
    case '-':
      if (!first_char) {
	return false;
      }
      break;
    case '.':
      if (first_char || hit_dot || hit_e) {
	return false;
      }
      break;
    case 'e':
      if (first_char || hit_e) {
	return false;
      }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      if (hit_e) {
	number_after_e = true;
      }
      break;
    default:
      return false;
    }
    first_char = false;
  }


  if (hit_e) {
    return number_after_e;
  } else {
    return true;
  }
}


Expression::Expression() {
  this->type = NONE;
}

Expression::Expression(double value) {
  this->type = NUMBER;
  this->number_value = value;
}

Expression::Expression(bool value) {
  this->type = BOOL;
  this->bool_value = value;
}

Expression::Expression(const std::string value) {
  this->type = SYMBOL;
  this->symbol_value = value;
}

Expression::Expression(const std::vector<Expression> children) {
  this->type = LIST;
  this->children = children;
}

const char * InvalidTokenException::what () const noexcept {
  std::stringstream stream;
  stream << token;
  std::string output = stream.str();
  return output.c_str();
}

Expression::Expression(const Expression & other) {
  type = other.type;
  bool_value = other.bool_value;
  number_value = other.number_value;
  symbol_value = other.symbol_value;
  children = other.children;
}

Expression parse_tokens(std::vector<token::Token> tokens) {
  std::vector<Expression> top;
  
  return Expression(top);
}
