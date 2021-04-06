platest : catchmain.o planetest.cpp
	g++ -o $@ catchmain.o planetest.cpp

tritest : catchmain.o triangletest.cpp
	g++ -o $@ catchmain.o triangletest.cpp


catchmain.o : catchmain.cpp
	g++ -c $<


plane : platest
	./platest

triangle : tritest
	./tritest
