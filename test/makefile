
.PHONY: clean clear test run all doxygen

all : plane triangle findneartriangle triintersect

platest.ex : catchmain.o planetest.cpp
	g++ -o $@ catchmain.o planetest.cpp

tritest.ex : catchmain.o triangletest.cpp
	g++ -o $@ catchmain.o triangletest.cpp

findneartriangle.ex : catchmain.o findneartritest.cpp
	g++ -o $@ catchmain.o findneartritest.cpp

triintersecttest.ex : catchmain.o triintersecttest.cpp
	g++ -o $@ catchmain.o triintersecttest.cpp

catchmain.o : catchmain.cpp
	g++ -c $<


plane : platest.ex
	./platest.ex

triangle : tritest.ex
	./tritest.ex

findneartriangle : findneartriangle.ex
	./findneartriangle.ex

triintersect : triintersecttest.ex
	./triintersecttest.ex
clean:
	 rm -f *.ex  core
	 
	 
