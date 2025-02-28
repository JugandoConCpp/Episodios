all: tronco.dll log_tester.exe

include ..\..\Makefile.inc

tronco.dll:
	cd tronco && gnumake
	move tronco\tronco.dll .
	move tronco\tronco.lib .

log_tester.exe:
	cd log_tester && gnumake
	move log_tester\log_tester.exe .
