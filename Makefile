FILE = docgen

main: docgen.o
	g++ -Wall $(FILE).o -o main

docgen.o: docgen.cpp
	g++ -Wall -c $(FILE).cpp

clean:
	rm -rf docgen.o main