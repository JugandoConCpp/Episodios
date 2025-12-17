all: big_integer__unit_tests.dll

CCOPTS=-I..\testie\include

include ..\Makefile.inc

big_integer__unit_tests.dll: big_integer__test.o
	Clang++ -shared -v -o $@ $^
