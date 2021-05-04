CC := g++
CPPFLAGS = -O0 -g -D_GLIBCXX_DEBUG --std=c++17
MATLAB := matlab -nodesktop -nosplash -logfile Y.log -r

.PHONY: clean clear test run all doxygen
.SUFFIXES: .o .cpp .ex


.cpp.o:
	$(CC) -o $@ -c $< $(CPPFLAGS)
.o.exe:
	$(CC) -o $@ $^ $(CPPFLAGS)

main.o: main.cpp 

main.exe: main.o
	$(CC) -o $@ $^ $(CPPFLAGS)

run: main.exe
	./main.exe

test:
	cd test/ && make all

# print:
# make -C code/ print

#doxygen: doxygen.cfg
#	doxygen doxygen.cfg

#-rm main.ex *.o core* *.log

clean:
	-rm main.exe *.o core* *.log
	cd test/  \
	&& make clean
