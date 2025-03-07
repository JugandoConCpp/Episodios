all: ini_tester.exe ini__test.dll

CCOPTS=-I..\Testie\include

include ..\..\Makefile.inc

ini__test.dll: ini__test.o line__test.o
	Clang++ -shared -v -o $@ $^

ini_tester.exe: main.o
	Clang++ -o $@ $^
