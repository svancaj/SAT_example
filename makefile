run: build
	./coloring

build:
	g++ main.cpp libkissat.a -o coloring

clean:
	rm -f coloring
