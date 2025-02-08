all: hello.exe

clean:
	del hello.exe
	del *.o

.cpp.o:
	clang++ -c $? -o $@

hello.exe: main.o hello.o
	clang++ -o $@ $?
