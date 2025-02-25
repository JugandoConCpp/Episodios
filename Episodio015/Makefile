all: run_tests.exe build_unit_tests

include ..\..\Makefile.inc

build_unit_tests:
	cd unit_tests && gnumake
	move unit_tests\*.dll .

run_tests.exe:
	cd run_tests && gnumake
	move run_tests\run_tests.exe .
	move run_tests\*.dll .
