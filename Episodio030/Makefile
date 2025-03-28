all: edit.exe

CCOPTS=-I..\Ini -Iwindows_wrappers

include ..\..\Makefile.inc

edit.exe: main.o main_window.o
	Clang++ -o $@ $^ -L..\Ini -luser32 -lIni
