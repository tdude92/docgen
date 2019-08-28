FILE = docgen
VPATH = ../include

main: docgen.o dgparsers.o
	g++ -Wall $(FILE).o dgparsers.o -o main

docgen.o: docgen.cpp
	g++ -Wall -c $(FILE).cpp -I../include

dgparsers.o: dgparsers.cpp
	g++ -Wall -c dgparsers.cpp -I../include

clean:
	rm -rf docgen.o dgparsers.o main documentation.md
