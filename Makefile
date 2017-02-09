CC=g++
CFLAGS=	-std=c++11

test: test_tokenize test_environment
	./test_tokenize
	./test_environment

test_tokenize: tokenize.o test_tokenize.cpp
	$(CC) $(CFLAGS) -o $@ tokenize.o test_tokenize.cpp

test_environment: environment.o test_environment.cpp
	$(CC) $(CFLAGS) -o $@ environment.o test_environment.cpp

tokenize.o: tokenize.cpp tokenize.hpp
	$(CC) $(CFLAGS) -c tokenize.cpp

environment.o: environment.cpp environment.hpp
	$(CC) $(CFLAGS) -c environment.cpp





