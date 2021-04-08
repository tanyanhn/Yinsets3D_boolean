
.PHONY: clean clear test run all doxygen

all : plane triangle

platest.ex : catchmain.o planetest.cpp
	g++ -o $@ catchmain.o planetest.cpp

tritest.ex : catchmain.o triangletest.cpp
	g++ -o $@ catchmain.o triangletest.cpp


catchmain.o : catchmain.cpp
	g++ -c $<


plane : platest.ex
	./platest.ex

triangle : tritest.ex
	./tritest.ex
	
clean:
	 rm -f *.ex  core
	 
	 
