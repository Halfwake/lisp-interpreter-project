CC=g++
CFLAGS=	-std=c++11 -Wall

test: test_tokenize  test_expression #test_environment
	./test_tokenize
#	./test_environment
	./test_expression

test_tokenize: tokenize.o test_tokenize.cpp
	$(CC) $(CFLAGS) -o $@ tokenize.o test_tokenize.cpp

#test_environment: environment.o tokenize.o test_environment.cpp
#	$(CC) $(CFLAGS) -o $@ environment.o tokenize.o test_environment.cpp

test_expression: expression.o tokenize.o test_expression.cpp
	$(CC) $(CFLAGS) -o $@ expression.o tokenize.o test_expression.cpp

tokenize.o: tokenize.cpp tokenize.hpp
	$(CC) $(CFLAGS) -c tokenize.cpp

environment.o: environment.cpp environment.hpp
	$(CC) $(CFLAGS) -c environment.cpp

expression.o: expression.cpp expression.hpp
	$(CC) $(CFLAGS) -c expression.cpp





