octave: main.o synth.o save.o
	gcc build/main.o build/synth.o build/save.o -o octave -lm

main.o: src/main.c
	gcc -ansi -Wall -c src/main.c -o build/main.o

synth.o: src/synth.c src/synth.h
	gcc -ansi -Wall -c src/synth.c -o build/synth.o

save.o: src/save.c src/save.h
	gcc -ansi -Wall -c src/save.c -o build/save.o

clean:
	rm build/*
	rm octave

run: octave
	./octave