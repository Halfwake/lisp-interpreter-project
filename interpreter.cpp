Interpreter::Interpreter() {
  
}

bool Interpreter::parse(std::istream & expression) noexcept {
  try {
    std::list<tokens> tokens = tokenize(expression);
    expression = parse_tokens(tokens);
    return true;
  } catch (InvalidTokenException e) {
    return false;
  }
}

Expression Interpreter::Expression eval() {
  
}
