
# CPPFLAGS = -g -std=c++17
CPPFLAGS = -std=c++17

.PHONY: clean clear test run all doxygen

all : plane triangle findneartriangle triintersect prepaste


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

removeoverlaptest.exe : catchmain.o removeoverlaptest.cpp
	g++ -o $@ catchmain.o removeoverlaptest.cpp $(CPPFLAGS)

pastetest.exe : catchmain.o pastetest.cpp
	g++ -o $@ catchmain.o pastetest.cpp $(CPPFLAGS)

GCStest.exe : catchmain.o GCStest.cpp
	g++ -o $@ catchmain.o GCStest.cpp $(CPPFLAGS)

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

test : test.cpp
	g++ -o test.exe test.cpp && ./test.exe

removeoverlap : removeoverlaptest.exe
	./removeoverlaptest.exe

paste : pastetest.exe
	./pastetest.exe

GCS : GCStest.exe
	./GCStest.exe

clean:
	rm -f *.exe  core *.ex
	 
	 
