FILE = docgen
VPATH = ../include

main: docgen.o dgparsers.o
	g++ -Wall $(FILE).o dgparsers.o -o main

docgen.o: docgen.cpp dgparsers.hpp
	g++ -Wall -c $(FILE).cpp -I../include

dgparsers.o: dgparsers.cpp dgtypes.hpp dgparsers.hpp
	g++ -Wall -c dgparsers.cpp -I../include

clean:
	rm -rf docgen.o dgparsers.o main documentation.md