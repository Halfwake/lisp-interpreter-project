#include "expression.hpp"
#include "environment.hpp"

#ifndef INTERPRETER_H
#define INTERPRETER_H

/*
 * A class for interpreting code. To use it, create one, call parse on
 * a text stream, check the result of parse to see if the text was
 * valid, and if it is, call eval. Curious about why eval doesn't call
 * parse internally and automatically? Me too.
 */
class Interpreter {
public:
  Interpreter();
  bool parse(std::istream & expression) noexcept;
  Expression eval();
private:
  Expression expression;
  environment::Environment environment;
};

/*
 * A helper function that checks if every element in a vector has a
 * certain type. It's used for type checking in eval.
 */
bool is_all_type(AtomType type, std::vector<Expression> expressions);

/*
 * These functions all evaluate small forms. The main eval functions
 * dispatches them.
 */
Expression eval_iter(Expression expr, environment::Environment & env);
Expression eval_not(Expression expr, environment::Environment & env);
Expression eval_and(Expression expr, environment::Environment & env);
Expression eval_or(Expression expr, environment::Environment & env);
Expression eval_l_than(Expression expr, environment::Environment & env);
Expression eval_le_than(Expression expr, environment::Environment & env);
Expression eval_g_than(Expression expr, environment::Environment & env);
Expression eval_ge_than(Expression expr, environment::Environment & env);
Expression eval_eq(Expression expr, environment::Environment & env);
Expression eval_sum(Expression expr, environment::Environment & env);
Expression eval_diff(Expression expr, environment::Environment & env);
Expression eval_product(Expression expr, environment::Environment & env);
Expression eval_ratio(Expression expr, environment::Environment & env);

Expression eval_define(Expression expr, environment::Environment & env);
Expression eval_begin(Expression expr, environment::Environment & env);
Expression eval_if(Expression expr, environment::Environment & env);


/*
 * Throw if an invalid type is passed to a form.
 */
class BadArgumentTypeException : public std::exception {
public:
  BadArgumentTypeException (Expression expression) : expression(expression) {};
  Expression getExpression();
  const char * what() const noexcept;
private:
  Expression expression;
};

/*
 * Throw if an invalid amount of arguments is thrown to a form.
 */
class BadArgumentCountException : public std::exception {
public:
  BadArgumentCountException (Expression expression) : expression(expression) {};
  Expression getExpression();
  const char * what() const noexcept;
private:
  Expression expression;
};

/*
 * Throw if an expression matches no known evaluation form.
 */
class InvalidExpressionException : public std::exception {
public:
  InvalidExpressionException (Expression expression) : expression(expression) {};
  Expression getExpression();
  const char * what() const noexcept;
private:
  Expression expression;
};

#endif
