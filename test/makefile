
.PHONY: clean clear test run all doxygen

all : plane triangle findneartriangle triintersect


platest.ex : catchmain.o planetest.cpp
	g++ -o $@ catchmain.o planetest.cpp -g

tritest.ex : catchmain.o triangletest.cpp
	g++ -o $@ catchmain.o triangletest.cpp -g

findneartriangle.ex : catchmain.o findneartritest.cpp
	g++ -o $@ catchmain.o findneartritest.cpp -g

triintersecttest.ex : catchmain.o triintersecttest.cpp
	g++ -o $@ catchmain.o triintersecttest.cpp -g

prepastetest.exe : catchmain.o prepastetest.cpp
	g++ -o $@ catchmain.o prepastetest.cpp -g

catchmain.o : catchmain.cpp
	g++ -c $< -g


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
	 
	 
