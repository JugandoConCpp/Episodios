all: app.exe

# CCOPTS=-I..\Testie\include

include ..\..\Makefile.inc

# ini__test.dll: ini__test.o section__test.o line__test.o
# 	Clang++ -shared -v -o $@ $^ -lini
# 
# ini.dll: ini.o section.o line.o
# 	Clang++ -shared -v -o $@ $^

app.exe: main.o
	Clang++ -o $@ $^ -luser32
