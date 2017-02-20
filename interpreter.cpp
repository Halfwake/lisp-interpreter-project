#include "interpreter.hpp"

#include <istream>
#include <vector>
#include <list>

#include "expression.hpp"
#include "environment.hpp"
#include "tokenize.hpp"

Interpreter::Interpreter() {
  
}

bool Interpreter::parse(std::istream & expression) noexcept {
  try {
    std::list<token::Token> tokens = token::tokenize(expression);
    Expression expression = parse_tokens(tokens);
    return true;
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
      //throw error no children
    } else if (children.front().getType() != SYMBOL) {
      //throw error no symbol
    } else if (children.front().getSymbol() == "not") {
      eval_not(expr, env);
    } else if (children.front().getSymbol() == "and") {
      eval_and(expr, env);
    }
  }
}

Expression eval_not(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 2) {
    //throw error not right argument count
  }
  Expression evaluated_child = eval_iter(expr.getChildren().at(1), env);
  if (evaluated_child.getType() != BOOL) {
    //throw error
  }
  return Expression(!evaluated_child.getBool());
}

Expression eval_and(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() < 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if (!is_all_type(BOOL, simplified_expr)) {
    //throw bad type args
  }
  bool accum = true;
  for (auto & child : simplified_expr) {
    accum = accum && child.getBool();
  }
  return Expression(accum);
}

bool is_all_type(AtomType type, std::vector<Expression> expressions) {
  for (auto & expr : expressions) {
    if (expr.getType() != type) {
      return false;
    }
  }
  return true;
}

Expression eval_or(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() < 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if (!is_all_type(BOOL, simplified_expr)) {
    //throw bad type args
  }
  bool accum = true;
  for (auto & child : simplified_expr) {
    accum = accum || child.getBool();
  }
  return Expression(accum);
}

Expression eval_l_than(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    // throw bad type args
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() < expr2.getNumber());  
}

Expression eval_le_than(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    // throw bad type args
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() <= expr2.getNumber());
}

Expression eval_g_than(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    // throw bad type args
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() > expr2.getNumber());
}

Expression eval_ge_than(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    // throw bad type args
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() >= expr2.getNumber());
}

Expression eval_eq(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    // throw bad type args
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() == expr2.getNumber());
}

Expression eval_sum(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() < 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    // throw bad type args
  }
  double accum = 0;
  for (auto & child : simplified_expr) {
    accum += child.getNumber();
  }
  return Expression(accum * 1.0);
}

Expression eval_diff(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() != 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    // throw bad type args
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() - expr2.getNumber() + 0.0);
}

Expression eval_product(Expression expr, environment::Environment & env) {
  if (expr.getChildren().size() < 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    // throw bad type args
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() * expr2.getNumber() * 1.0);
}

Expression eval_ratio(Expression expr, environment::Environment & env) {
if (expr.getChildren().size() != 2) {
    //throw
  }
  std::vector<Expression> simplified_expr;
  for (auto & child : expr.getChildren()) {
    simplified_expr.push_back(eval_iter(child, env));
  }
  if(!is_all_type(NUMBER, simplified_expr)) {
    // throw bad type args
  }
  Expression expr1 = simplified_expr.at(1);
  Expression expr2 = simplified_expr.at(2);
  return Expression(expr1.getNumber() - expr2.getNumber() + 0.0);  
}
