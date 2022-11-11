unlzw: unlzw.o
	gcc build/unlzw.o -o unlzw

unlzw.o: src/unlzw.c src/unlzw.h
	gcc -ansi -Wall -c src/unlzw.c -o build/unlzw.o

clean:
	rm build/*

run: unlzw
	./unlzw ../lzw/out.lzw