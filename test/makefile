
.PHONY: clean clear test run all doxygen

all : plane triangle findneartriangle triintersect

platest.exe : catchmain.o planetest.cpp
	g++ -o $@ catchmain.o planetest.cpp

tritest.exe : catchmain.o triangletest.cpp
	g++ -o $@ catchmain.o triangletest.cpp

findneartriangle.exe : catchmain.o findneartritest.cpp
	g++ -o $@ catchmain.o findneartritest.cpp

triintersecttest.exe : catchmain.o triintersecttest.cpp
	g++ -o $@ catchmain.o triintersecttest.cpp

prepastetest.exe : catchmain.o prepastetest.cpp
	g++ -o $@ catchmain.o prepastetest.cpp

catchmain.o : catchmain.cpp
	g++ -c $<


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
	 rm -f *.exe  core
	 
	 
