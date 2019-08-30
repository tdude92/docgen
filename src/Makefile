FILE = docgen
VPATH = ../include

docgen: docgen.o dgparsers.o
	g++ -Wall $(FILE).o dgparsers.o -o docgen -lboost_system -lboost_filesystem

docgen.o: docgen.cpp
	g++ -Wall -c $(FILE).cpp -I../include

dgparsers.o: dgparsers.cpp
	g++ -Wall -c dgparsers.cpp -I../include

clean:
	rm -rf docgen.o dgparsers.o docgen
