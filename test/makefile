
CPPFLAGS = -O0 -g -D_GLIBCXX_DEBUG -std=c++17

.PHONY: clean clear test run all doxygen

all : plane triangle findneartriangle triintersect


platest.exe : catchmain.o planetest.cpp
	g++ -o $@ catchmain.o planetest.cpp $(CPPFLAGS)

tritest.exe : catchmain.o triangletest.cpp
	g++ -o $@ catchmain.o triangletest.cpp $(CPPFLAGS)

findneartriangle.exe : catchmain.o findneartritest.cpp
	g++ -o $@ catchmain.o findneartritest.cpp $(CPPFLAGS)

triintersecttest.exe : catchmain.o triintersecttest.cpp
	g++ -o $@ catchmain.o triintersecttest.cpp $(CPPFLAGS)

prepastetest.exe : catchmain.o prepastetest.cpp
	g++ -o $@ catchmain.o prepastetest.cpp $(CPPFLAGS)

catchmain.o : catchmain.cpp


plane : platest.exe
	./platest.exe

triangle : tritest.exe
	./tritest.exe

findneartriangle : findneartriangle.exe
	./findneartriangle.exe

triintersect : triintersecttest.exe
	./triintersecttest.exe

prepaste : prepastetest.exe
	./prepastetest.exe


clean:
	 rm -f *.exe  core *.ex
	 
	 
