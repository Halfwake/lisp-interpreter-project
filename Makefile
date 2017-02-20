CC=g++
CFLAGS=	-std=c++11 -Wall -g

test: test_tokenize  test_expression test_environment test_interpreter
	./test_tokenize
	./test_environment
	./test_expression
	./test_interpreter

test_tokenize: tokenize.o test_tokenize.cpp
	$(CC) $(CFLAGS) -o $@ tokenize.o test_tokenize.cpp

test_environment: environment.o tokenize.o test_environment.cpp expression.o interpreter.o
	$(CC) $(CFLAGS) -o $@ environment.o expression.o tokenize.o test_environment.cpp

test_interpreter: expression.o environment.o test_interpreter.cpp tokenize.o interpreter.o
	$(CC) $(CFLAGS) -o $@ environment.o expression.o test_interpreter.cpp tokenize.o interpreter.o

test_expression: expression.o tokenize.o test_expression.cpp
	$(CC) $(CFLAGS) -o $@ expression.o tokenize.o test_expression.cpp

interpreter.o: interpreter.cpp interpreter.hpp
	$(CC) $(CFLAGS) -c interpreter.cpp

tokenize.o: tokenize.cpp tokenize.hpp
	$(CC) $(CFLAGS) -c tokenize.cpp

environment.o: environment.cpp environment.hpp
	$(CC) $(CFLAGS) -c environment.cpp

expression.o: expression.cpp expression.hpp
	$(CC) $(CFLAGS) -c expression.cpp





