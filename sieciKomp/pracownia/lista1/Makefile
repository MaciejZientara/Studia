default:
	g++ -std=c++17 -c traceroute.cpp -Wall -Wextra
	g++ -o trace traceroute.o

test:
	(sudo traceroute -I 8.8.8.8 &) && (sudo ./trace 8.8.8.8)
	sleep 1
	printf "\n"
	(sudo ./trace 8.8.8.8 &) && (sudo ./trace 8.8.8.8)
	sleep 1
	printf "\n"
	(sudo ./trace 8.8.8.8 &) && (sudo ./trace 156.17.4.1)

clean:
	rm *.o

distclean:
	rm trace
	rm *.o