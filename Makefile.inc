CC=clang++ $(CCOPTS) -std=c++23 

clean:
	del *.o *.exe *.dll

.cpp.o:
	$(CC) -o $@ -c $^
