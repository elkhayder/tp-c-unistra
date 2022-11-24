octave: main.o synth.o save.o midi.o track.o instrument.o
	gcc build/main.o build/synth.o build/save.o build/midi.o build/track.o build/instrument.o -o octave -lm

main.o: src/main.c src/main.h
	gcc -ansi -Wall -c src/main.c -o build/main.o

synth.o: src/synth.c src/synth.h
	gcc -ansi -Wall -c src/synth.c -o build/synth.o

save.o: src/save.c src/save.h
	gcc -ansi -Wall -c src/save.c -o build/save.o

midi.o: src/midi.c src/midi.h
	gcc -ansi -Wall -c src/midi.c -o build/midi.o

track.o: src/track.c src/track.h
	gcc -ansi -Wall -c src/track.c -o build/track.o

instrument.o: src/instrument.c src/instrument.h
	gcc -ansi -Wall -c src/instrument.c -o build/instrument.o

clean:
	rm build/*
	rm octave

run: octave
	./octave