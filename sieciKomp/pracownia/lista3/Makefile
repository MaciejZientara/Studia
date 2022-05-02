CC = g++ 
CFLAGS = -Wall -Wextra --std=c++11 -O2

all: transport

transport: transport.o receiver.o
transport.o: transport.cpp
receiver.o: receiver.h receiver.cpp

.PHONY: clean distclean

clean: 
	rm -f *.o

distclean:
	rm -f transport *.o