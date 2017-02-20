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
  if (!correct_type && (true_string || false_string)) {
    throw InvalidTokenException(token);
  }
  return correct_type && (true_string || false_string);
}

bool match_none(token::Token token) {
  bool correct_type = token.getType() == token::ATOM;
  bool none_string = token.getText() == "None";
  if (!correct_type && none_string) {
    throw InvalidTokenException(token);
  }
  return correct_type && none_string;
}

bool match_number(token::Token token) {
  // TODO! Make this less of a hack.
  if (token.getText() == "+" or token.getText() == "-") {
    return false;
  }
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

bool match_symbol(token::Token token) {
  // TODO! Check if symbol name begins with digit.
  bool not_number = !match_number(token);
  bool not_none = !match_none(token);
  bool not_bool = !match_bool(token);
  return not_number && not_none && not_bool;
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
  return false;
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

AtomType Expression::getType() const {
  return type;
}

Expression parse_tokens_iter(std::list<token::Token> & tokens) {
  std::list<Expression> top;
  while (!tokens.empty()) {
    if (match_open(tokens.front())) {
      tokens.pop_front();
      top.push_back(parse_tokens_iter(tokens));
    } else if (match_close(tokens.front())) {
      tokens.pop_front();
      return Expression(top);
    } else {
      top.push_back(parse_atom(tokens.front()));
      tokens.pop_front();
    }
  }
  //TODO! throw InvalidTokenException(token);
}

Expression parse_atom(token::Token token) {
  if (match_bool(token)) {
    if (token.getText() == "True") {
      return Expression(true);
    } else if (token.getText() == "False") {
      return Expression(false);
    } else {
      throw InvalidTokenException(token);
    }
  } else if (match_none(token)) {
    return Expression();
  } else if (match_number(token)) {
    std::stringstream stream(token.getText());
    double value;
    stream >> value;
    // TODO! This should check if we were actually given a
    // double, since match_number might not be valid.
    return Expression(value);
  } else if (match_symbol(token)) {
    return Expression(std::string(token.getText()));
  }
  throw InvalidTokenException(token);
}

Expression parse_tokens(std::list<token::Token> tokens) {
  std::list<Expression> top;
  top.push_back(Expression(std::string("begin")));
  while (!tokens.empty()) {
    if (match_open(tokens.front())) {
      tokens.pop_front();
      top.push_back(parse_tokens_iter(tokens));
    } else {
      top.push_back(parse_atom(tokens.front()));
      tokens.pop_front();
    }
  }
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

std::list<Expression> Expression::getChildren() const {
  return this->children;
}

bool Expression::getBool() const {
  return bool_value;
}
double Expression::getNumber() const {
  return number_value;
}

std::string Expression::getSymbol() const {
  return symbol_value;
}
