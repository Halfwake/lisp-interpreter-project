CC=g++
CFLAGS=	-std=c++11

test_environment: environment.o
	$(CC) $(CFLAGS) -o $@ environment.o test_environment.cpp

environment.o: environment.cpp environment.hpp
	$(CC) $(CFLAGS) -c environment.cpp



