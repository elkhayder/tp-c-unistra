lzw: lzw.o
	gcc build/lzw.o -o lzw

lzw.o: src/lzw.c src/lzw.h
	gcc -ansi -Wall -c src/lzw.c -o build/lzw.o

clean:
	rm build/*

run: lzw
	./lzw input.txt out.lzw