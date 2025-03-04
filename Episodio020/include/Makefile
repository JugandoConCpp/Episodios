all: tronco_headers_tests.dll

CCOPTS=-I..\..\Testie\include

include ..\..\..\Makefile.inc

tronco_headers_tests.dll: channel__test.o
	Clang++ -shared -v -o $@ $^ -L.. -ltronco
