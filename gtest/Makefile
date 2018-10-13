GTEST = /usr/local/lib/libgtest.a /usr/local/lib/libgtest_main.a

CC = g++

CPPFLAGS = -std=c++11 -Wall

TARGET = calctest

RM = rm -rf

all: $(TARGET)

calctest: calculator.t.o calculator.o
					$(CC) $(CPPFLAGS) -o calctest.o calculator.t.o calculator.o $(GTEST)

calculator.t.o: calculator.t.cpp
							  $(CC) $(CPPFLAGS) -c calculator.t.cpp $(GTEST)

calculator.o: calculator.h calculator.cpp
						  $(CC) $(CPPFLAGS) -c calculator.cpp

clean: #
			 $(RM) *.o
