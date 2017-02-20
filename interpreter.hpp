#include "expression.hpp"
#include "environment.hpp"

#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
public:
  Interpreter();
  bool parse(std::istream & expression) noexcept;
  Expression eval();
private:
  Expression expression;
  environment::Environment environment;
};

bool is_all_type(AtomType type, std::vector<Expression> expressions);

Expression eval_iter(Expression expr, environment::Environment & env);
Expression eval_not(Expression expr, environment::Environment & env);
Expression eval_and(Expression expr, environment::Environment & env);
Expression eval_or(Expression expr, environment::Environment & env);
Expression eval_l_than(Expression expr, environment::Environment & env);
Expression eval_le_than(Expression expr, environment::Environment & env);
Expression eval_g_than(Expression expr, environment::Environment & env);
Expression eval_ge_than(Expression expr, environment::Environment & env);
Expression eval_eq_than(Expression expr, environment::Environment & env);
Expression eval_sum_than(Expression expr, environment::Environment & env);
Expression eval_diff(Expression expr, environment::Environment & env);
Expression eval_product(Expression expr, environment::Environment & env);
Expression eval_ratio(Expression expr, environment::Environment & env);


class BadArgumentTypeException : public std::exception {
public:
  BadArgumentTypeException (Expression expression) : expression(expression) {};
  Expression getExpression();
  const char * what() const noexcept;
private:
  Expression expression;
};

class BadArgumentCountException : public std::exception {
public:
  BadArgumentCountException (Expression expression) : expression(expression) {};
  Expression getExpression();
  const char * what() const noexcept;
private:
  Expression expression;
};

class InvalidExpressionException : public std::exception {
public:
  InvalidExpressionException (Expression expression) : expression(expression) {};
  Expression getExpression();
  const char * what() const noexcept;
private:
  Expression expression;
};

#endif
