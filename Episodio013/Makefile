all: run_tests.exe unit_test_1.dll

clean:
	del /f /q *.exe *.dll *.lib *.exp *.o *.a

.cpp.o:
	Clang++ --std=c++23 -o $@ -c $?

unit_test_1.dll: unit_test_1.o
	Clang++ -shared -v -o $@ $^

run_tests.exe: main.o
	Clang++ -o $@ $^
