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

bool Expression::operator==(const Expression & other) const noexcept {
  if (type != other.type) {
    return false;
  }
  if (type == NUMBER) {
    return number_value == other.number_value;
  }
  if (type == BOOL) {
    return bool_value == other.bool_value;
  }
  if (type == SYMBOL) {
    return symbol_value == other.symbol_value;
  }
  if (type == NONE) {
    return true; // There's only one NONE.
  }
  if (type == LIST) {
    return children == other.children;
  }
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

Expression::Expression(const std::list<Expression> children) {
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

Expression parse_tokens_iter(std::list<token::Token> & tokens) {
  std::list<Expression> top;
  /*
  while(token.front().getType() != token::CLOSE_PAREN) {
    top.push_back(token.pop());
  } 
  */
  return Expression(top);
}

Expression parse_tokens(std::list<token::Token> tokens) {
  std::list<Expression> top;
  top.push_back(Expression("begin"));
  return Expression(top);
}

std::ostream & operator << (std::ostream & stream, const Expression & expr) {
  if (expr.type == NONE) {
    stream << "(None|None)";
  } else if (expr.type == BOOL) {
    stream << "(Bool|";
    if (expr.bool_value) {
      stream << "True";
    } else {
      stream << "False";
    }
    stream << ")";
  } else if (expr.type == SYMBOL) {
    stream << "(Symbol|" << expr.symbol_value << ")";
  } else if (expr.type == NUMBER) {
    stream << "(Number|" << expr.number_value << ")";
  } else if (expr.type == LIST) {
    stream << "(Parent|{";
    for (auto const & child : expr.children) {
      stream << child << "|";
    }
    stream << "}";
  }
  return stream;
}
