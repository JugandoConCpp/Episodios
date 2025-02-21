all: RunTests.exe Unit_Test_1.dll

clean:
	del /f /q *.dll *.o *.a

.cpp.o:
	Clang++ --std=c++23 -o $@ -c $?

Unit_Test_1.dll: Unit_Test_1.o
	Clang++ -shared -v -o $@ $^

RunTests.exe: main.o
	Clang++ -o $@ $^
