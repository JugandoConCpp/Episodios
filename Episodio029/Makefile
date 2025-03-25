all: edit.exe

CCOPTS=-I..\Ini

include ..\..\Makefile.inc

edit.exe: main.o
	Clang++ -o $@ $^ -L..\Ini -luser32 -lIni
