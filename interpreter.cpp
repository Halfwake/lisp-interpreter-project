#include "interpreter.hpp"
#include "interpreter_semantic_error.hpp"

#include <istream>
#include <vector>
#include <list>
#include <sstream>
#include <math.h>

#include "expression.hpp"
#include "environment.hpp"
#include "tokenize.hpp"

Interpreter::Interpreter() {
  environment.set("pi", atan2(0, -1));
}

bool Interpreter::parse(std::istream & expr) noexcept {
  try {
    std::list<token::Token> tokens = token::tokenize(expr);
    expression = parse_tokens(tokens);
    return expression.getChildren().size() != 0;
  } catch (InvalidTokenException e) {
    return false;
  }
}

Expression Interpreter::eval() {
  try {
    return eval_iter(expression, environment);
  } catch (InvalidExpressionException e) {
    throw InterpreterSemanticError("Expression could not be evaluated.");
  } catch (BadArgumentCountException e) {
    throw InterpreterSemanticError("Bad argument count.");
  } catch (BadArgumentTypeException e) {
    throw InterpreterSemanticError("Argumetn of wrong type.");
  } catch (environment::LookupException e) {
    throw InterpreterSemanticError("Unbound variable.");
  }
}

bool reserved_symbol(std::string symbol) {
  std::vector<std::string> reserved = {
    "not",
    "and",
    "or",
    "<",
    "<=",
    ">",
    ">=",
    "=",
    "+",
    "-",
    "*",
    "/",
    "define",
    "begin",
    "if"
  };
  for (auto reserved_symbol : reserved) {
    if (symbol == reserved_symbol) {
      return true;
    }
  }
  return false;
}

Expression eval_iter(Expression expr, environment::Environment & env) {
  if (expr.getType() != LIST) {
    if ((expr.getType() == SYMBOL) && (!reserved_symbol(expr.getSymbol()))) {
      return env.get(expr.getSymbol());
    } else {
      return expr;
    }
  } else {
    std::vector<Expression> children = expr.getChildren();
    if (children.size() == 0) {
      throw InvalidExpressionException(expr);
    } else if (children.size() == 1) {
      return eval_iter(children.front(), env);
    } else if (children.front().getType() != SYMBOL) {
      throw InvalidExpressionException(expr);
    } else if (children.front().getSymbol() == "not") {
      return eval_not(expr, env);
    } else if (children.front().getSymbol() == "and") {
      return eval_and(expr, env);
    } else if (children.front().getSymbol() == "or") {
      return eval_or(expr, env);
    } else if (children.front().getSymbol() == "<") {
      return eval_l_than(expr, env);
    } else if (children.front().getSymbol() == "<=") {
      return eval_le_than(expr, env);
    } else if (children.front().getSymbol() == ">") {
      return eval_g_than(expr, env);
    } else if (children.front().getSymbol() == ">=") {
      return eval_ge_than(expr, env);
    } else if (children.front().getSymbol() == "=") {
      return eval_eq(expr, env);
    } else if (children.front().getSymbol() == "+") {
      return eval_sum(expr, env);
    } else if (children.front().getSymbol() == "-") {
      return eval_diff(expr, env);
    } else if (children.front().getSymbol() == "*") {
      return eval_product(expr, env);
    } else if (children.front().getSymbol() == "/") {
      return eval_ratio(expr, env);
    } else if (children.front().getSymbol() == "define") {
      return eval_define(expr, env);
    } else if (children.front().getSymbol() == "begin") {
      return eval_begin(expr, env);
    } else if (children.front().getSymbol() == "if") {
      return eval_if(expr, env);
    } else {
      throw InvalidExpressionException(expr);
    }
  }
  throw InvalidExpressionException(expr);
}

Expression eval_not(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 2) {
    throw BadArgumentCountException(expr);
  }
  Expression evaluated_child = eval_iter(expr.getChildren().at(1), env);
  if (evaluated_child.getType() != BOOL) {
    throw BadArgumentTypeException(expr);
  }
  return Expression(!evaluated_child.getBool());
}

Expression eval_and(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() < 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if (!is_all_type(BOOL, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  simplified_expr.erase(simplified_expr.begin());
  bool accum = true;
  for (auto & child : simplified_expr) {
    accum = accum && child.getBool();
  }
  return Expression(accum);
}

bool is_all_type(AtomType type, std::vector<Expression> expressions) {
  bool skip_first = true;
  for (auto & expr : expressions) {
    if (skip_first) {
      skip_first = false;
      continue;
    }
    if (expr.getType() != type) {
      return false;
    }
  }
  return true;
}

Expression eval_or(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() < 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if (!is_all_type(BOOL, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  simplified_expr.erase(simplified_expr.begin());
  bool accum = false;
  for (auto & child : simplified_expr) {
    accum = accum || child.getBool();
  }
  return Expression(accum);
}

Expression eval_l_than(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() < expr2.getNumber());  
}

Expression eval_le_than(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() <= expr2.getNumber());
}

Expression eval_g_than(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() > expr2.getNumber());
}

Expression eval_ge_than(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() >= expr2.getNumber());
}

Expression eval_eq(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() == expr2.getNumber());
}

Expression eval_sum(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() < 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  simplified_expr.erase(simplified_expr.begin());
  double accum = 0;
  for (auto & child : simplified_expr) {
    accum += child.getNumber();
  }
  return Expression(accum * 1.0);
}

Expression eval_diff(Expression expr, environment::Environment & env) {
  if ((expr.getChildren().size() == 3) || (expr.getChildren().size() == 2)) {

    std::vector<Expression> simplified_expr;
    for (auto & child : expr.getChildren()) {
      simplified_expr.push_back(eval_iter(child, env));
    }
    if(!is_all_type(NUMBER, simplified_expr)) {
      throw BadArgumentTypeException(expr);
    }
    if (expr.getChildren().size() == 3) {
      Expression expr1 = simplified_expr.at(1);
      Expression expr2 = simplified_expr.at(2);
      return Expression(expr1.getNumber() - expr2.getNumber() + 0.0);
    } else {
      Expression expr1 = simplified_expr.at(1);
      return Expression(expr1.getNumber() * -1.0);
    }
  } else {
      throw BadArgumentCountException(expr);
  }
}

Expression eval_product(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() < 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  simplified_expr.erase(simplified_expr.begin());
  double accum = 1;
  for (auto & child : simplified_expr) {
    accum *= child.getNumber();
  }
  return Expression(accum * 1.0);
}

Expression eval_ratio(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 3) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    throw BadArgumentTypeException(expr);
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() / expr2.getNumber());  
}

const char * BadArgumentTypeException::what () const noexcept {
  std::stringstream stream;
  stream << expression;
  std::string output = stream.str();
  return output.c_str();
}

const char * BadArgumentCountException::what () const noexcept {
  std::stringstream stream;
  stream << expression;
  std::string output = stream.str();
  return output.c_str();
}


const char * InvalidExpressionException::what () const noexcept {
std::stringstream stream;
  stream << expression;
  std::string output = stream.str();
  return output.c_str();
}

Expression eval_define(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 3) {
    throw BadArgumentCountException(expr);
  }

  if (expr.getChildren().at(1).getType() != SYMBOL) {
    throw BadArgumentTypeException(expr);
  }

  if (reserved_symbol(expr.getChildren().at(1).getSymbol())) {
    throw BadArgumentTypeException(expr);
  }

  std::string symbol = expr.getChildren().at(1).getSymbol();
  Expression value = eval_iter(expr.getChildren().at(2), env);
  env.set(symbol, value);
  return value;
}

Expression eval_begin(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() < 2) {
    throw BadArgumentCountException(expr);
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  return simplified_expr.back();
}

Expression eval_if(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 4) {
    throw BadArgumentCountException(expr);
  }

  std::vector<Expression> children = expr.getChildren();
  Expression test_expr = eval_iter(children.at(1), env);
  if (test_expr.getType() != BOOL) {
    throw BadArgumentTypeException(expr);    
  }
  if (test_expr.getBool()) {
    return eval_iter(children.at(2), env);
  } else {
    return eval_iter(children.at(3), env);
  }
}
