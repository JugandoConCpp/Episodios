all: tronco.dll log_tester.exe header_unit_tests

CLEAN=tronco\*.lib tronco\*.dll tronco\*.exp tronco\*.o log_tester\*.o log_tester\*.exe

include ..\..\Makefile.inc

header_unit_tests:
	cd include && gnumake
	move include\*.dll .
	move include\*.lib .

tronco.dll:
	cd tronco && gnumake
	move tronco\tronco.dll .
	move tronco\tronco.lib .

log_tester.exe:
	cd log_tester && gnumake
	move log_tester\log_tester.exe .
