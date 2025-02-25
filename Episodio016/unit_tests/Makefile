all: unit_test_1.dll

CCOPTS=-I..\include

include ..\..\..\Makefile.inc

unit_test_1.dll: unit_test_1.o
	Clang++ -shared -v -o $@ $^
