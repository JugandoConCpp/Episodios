all: hello.exe

clean:
	del *.exe
	del *.a
	del *.o

.cpp.o:
	clang++ -c $? -o $@

libhello.a: hello.o
	llvm-ar r libhello.a $?

hello.exe: main.o libhello.a
	clang++ -o $@ $?
