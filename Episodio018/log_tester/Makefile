all: log_tester.exe

CCOPTS=-I..\include

include ..\..\..\Makefile.inc

log_tester.exe: main.o
	Clang++ -o $@ $^ -L.. -ltronco
