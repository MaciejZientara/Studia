CC = g++ 
CFLAGS = -Wall -Wextra --std=c++11 -O2

all: transport

transport: transport.o receiver.o
transport.o: transport.cpp
receiver.o: receiver.h receiver.cpp

.PHONY: clean distclean test

IP = 127.0.0.1
PORT = 40001
FILE = output
SIZE = 8273645#15250
test:
	rm -f $(FILE) ./transport-binaries/$(FILE)
	time -v ./transport $(IP) $(PORT) $(FILE) $(SIZE) > /dev/null && (printf "\n\n") && time -v ./transport-binaries/transport-client-fast $(IP) $(PORT) transport-binaries/$(FILE) $(SIZE) > /dev/null
	diff $(FILE) ./transport-binaries/$(FILE)
	rm -f $(FILE) ./transport-binaries/$(FILE)
# brak informacji, ze pliki sie roznia po diff oznacza, ze sa identyczne!

clean: 
	rm -f *.o
	rm -f $(FILE) ./transport-binaries/$(FILE)

distclean:
	rm -f transport *.o
	rm -f $(FILE) ./transport-binaries/$(FILE)