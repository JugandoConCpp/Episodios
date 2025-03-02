all: tronco.dll

CCOPTS=-I..\include -DTRONCO_API=__declspec(dllexport)

include ..\..\..\Makefile.inc

tronco.dll: tronco.o
	Clang++ -shared -v -o $@ $^
