#include "interpreter.hpp"

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
  return eval_iter(expression, environment);
}

Expression eval_iter(Expression expr, environment::Environment & env) {
  if (expr.getType() != LIST) {
    return expr;
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
    } else if (children.front().getSymbol() == "==") {
      return eval_eq(expr, env);
    } else if (children.front().getSymbol() == "+") {
      return eval_sum(expr, env);
    } else if (children.front().getSymbol() == "-") {
      return eval_diff(expr, env);
    } else if (children.front().getSymbol() == "*") {
      return eval_product(expr, env);
    } else if (children.front().getSymbol() == "/") {
      return eval_diff(expr, env);
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
  bool accum = true;
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
  double accum = 0;
  for (auto & child : simplified_expr) {
    accum += child.getNumber();
  }
  return Expression(accum * 1.0);
}

Expression eval_diff(Expression expr, environment::Environment & env) {
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
  return Expression(expr1.getNumber() - expr2.getNumber() + 0.0);
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
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() * expr2.getNumber() * 1.0);
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
  return Expression(expr1.getNumber() - expr2.getNumber() + 0.0);  
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
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  //TODO! Check that name is symbol.
  std::string symbol = simplified_expr.at(1).getSymbol();
  Expression value = simplified_expr.at(2);
  env.set(symbol, value);
  return Expression();
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
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if (simplified_expr.at(1).getType() != BOOL) {
    throw BadArgumentTypeException(expr);    
  }
  if (simplified_expr.at(1).getBool()) {
    return simplified_expr.at(2);
  } else {
    return simplified_expr.at(3);
  }
}
