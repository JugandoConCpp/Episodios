all: run_tests.exe run_tests__unit_tests.dll

CCOPTS=-I..\include

include ..\..\..\Makefile.inc

run_tests.exe: main.o
	Clang++ -o $@ $^

run_tests__unit_tests.dll: test_run_results__test.o library__test.o util__test.o
	Clang++ -shared -v -o $@ $^
