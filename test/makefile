
# CPPFLAGS = -g -std=c++17 -fopenmp
CPPFLAGS = -g -std=c++17 -lboost_timer -D_unix 
# CPPFLAGS = -g -std=c++17 -lboost_timer -D_intersectTest 
# YSCPPFLAGS = -g -std=c++17 -lboost_timer -D_intersectTest  -D_precise10
# CPPFLAGS = -g -std=c++17  -D_intersectTest 
YSCPPFLAGS = -g -std=c++17  -D_intersectTest  -D_precise10




# CPPFLAGS = -std=c++17

# CPPFLAGS += -fopenmp -D_OMP_

TESTFLAGS =

.PHONY: clean clear test run all doxygen

all : YS simple

simple : plane triangle findneartriangle triintersect prepaste \
	 paste removeoverlap GCS triangulation Yinset \
	 locate SplitGCS
	 
 YS : YSmeet YSjoin SplitGCS
	
YSjoin.exe : catchmain.o YSjoin.cpp 
	g++ -o $@ catchmain.o YSjoin.cpp $(YSCPPFLAGS)
	 
YSmeet.exe : catchmain.o YSmeet.cpp 
	g++ -o $@ catchmain.o YSmeet.cpp $(YSCPPFLAGS) 

locatetest.exe : catchmain.o locatetest.cpp 
	g++ -o $@ catchmain.o locatetest.cpp $(CPPFLAGS)

platest.exe : catchmain.o planetest.cpp
	g++ -o $@ catchmain.o planetest.cpp $(CPPFLAGS)

triangletest.exe : catchmain.o triangletest.cpp
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

triangulationtest.exe : catchmain.o triangulationtest.cpp
	g++ -o $@ catchmain.o triangulationtest.cpp $(CPPFLAGS)

Yinsettest.exe : catchmain.o Yinsettest.cpp
	g++ -o $@ catchmain.o Yinsettest.cpp $(CPPFLAGS)

test1.exe : catchmain.o test1.cpp
	g++ -o $@ catchmain.o test1.cpp $(CPPFLAGS)

SplitGCS.exe : catchmain.o reFacBoundarytest.cpp
	g++ -o $@ catchmain.o reFacBoundarytest.cpp $(CPPFLAGS)

rectangle.exe : catchmain.o rectangletest.cpp	
	g++ -o $@ catchmain.o rectangletest.cpp $(CPPFLAGS)

cuboid.exe : catchmain.o cuboidtest.cpp	
	g++ -o $@ catchmain.o cuboidtest.cpp $(CPPFLAGS)

OctreeTest.exe : catchmain.o OctreeTest.cpp	
	g++ -o $@ catchmain.o OctreeTest.cpp $(CPPFLAGS)

catchmain.o : catchmain.cpp

YSjoin : YSjoin.exe
	./YSjoin.exe $(TESTFLAGS)

YSmeet : YSmeet.exe
	./YSmeet.exe $(TESTFLAGS)

SplitGCS : SplitGCS.exe
	./SplitGCS.exe $(TESTFLAGS)

plane : platest.exe
	./platest.exe

triangle : triangletest.exe
	./triangletest.exe

findneartriangle : findneartriangle.exe
	./findneartriangle.exe

triintersect : triintersecttest.exe
	./triintersecttest.exe

prepaste : prepastetest.exe
	./prepastetest.exe

removeoverlap : removeoverlaptest.exe
	./removeoverlaptest.exe

paste : pastetest.exe
	./pastetest.exe

GCS : GCStest.exe
	./GCStest.exe

triangulation : triangulationtest.exe
	./triangulationtest.exe

Yinset : Yinsettest.exe
	./Yinsettest.exe
	
locate : locatetest.exe 
	./locatetest.exe 


rectangle : rectangle.exe
	./rectangle.exe

cuboid : cuboid.exe	
	./cuboid.exe

Octree : OctreeTest.exe	
	./OctreeTest.exe

test : test.cpp
	g++ -o test.exe test.cpp && ./test.exe

test1 : test1.exe
	./test1.exe

clean:
	rm -f *.exe  core *.ex
	 
	 
