all: riff_tester.exe

clean:
	del /f /q *.exe *.o *.a

.cpp.o:
	Clang++ --std=c++20 -o $@ -c $?

riff_tester.exe: main.o riff.o
	Clang++ -o $@ $^
