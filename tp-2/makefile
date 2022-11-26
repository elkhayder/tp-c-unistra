octave: main.o ADSREnvelope.o BiquadFilter.o Channel.o Instrument.o Note.o Oscillator.o Signal.o Track.o Export.o Ins.o Midi.o
	gcc build/main.o build/ADSREnvelope.o build/BiquadFilter.o build/Channel.o build/Instrument.o build/Note.o build/Oscillator.o build/Signal.o build/Track.o build/Export.o build/Ins.o build/Midi.o -o octave -lm

main.o: src/main.c
	gcc -ansi -Wall -c src/main.c -o build/main.o

# Structs

ADSREnvelope.o: src/structs/ADSREnvelope.c src/structs/ADSREnvelope.h
	gcc -ansi -Wall -c src/structs/ADSREnvelope.c -o build/ADSREnvelope.o

BiquadFilter.o: src/structs/BiquadFilter.c src/structs/BiquadFilter.h
	gcc -ansi -Wall -c src/structs/BiquadFilter.c -o build/BiquadFilter.o

Channel.o: src/structs/Channel.c src/structs/Channel.h
	gcc -ansi -Wall -c src/structs/Channel.c -o build/Channel.o

Instrument.o: src/structs/Instrument.c src/structs/Instrument.h
	gcc -ansi -Wall -c src/structs/Instrument.c -o build/Instrument.o

Note.o: src/structs/Note.c src/structs/Note.h
	gcc -ansi -Wall -c src/structs/Note.c -o build/Note.o

Oscillator.o: src/structs/Oscillator.c src/structs/Oscillator.h
	gcc -ansi -Wall -c src/structs/Oscillator.c -o build/Oscillator.o

Signal.o: src/structs/Signal.c src/structs/Signal.h
	gcc -ansi -Wall -c src/structs/Signal.c -o build/Signal.o

Track.o: src/structs/Track.c src/structs/Track.h
	gcc -ansi -Wall -c src/structs/Track.c -o build/Track.o

# IO

Export.o: src/io/Export.c src/io/Export.h
	gcc -ansi -Wall -c src/io/Export.c -o build/Export.o

Ins.o: src/io/Ins.c src/io/Ins.h
	gcc -ansi -Wall -c src/io/Ins.c -o build/Ins.o

Midi.o: src/io/Midi.c src/io/Midi.h
	gcc -ansi -Wall -c src/io/Midi.c -o build/Midi.o

# Routines

clear:
	rm build/*
	rm octave

clean:
	rm *.wav *.log *.csv

run: octave
	./octave SingleNote