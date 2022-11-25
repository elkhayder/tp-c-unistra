#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define SAMPLING_RATE (int)44100

#define SUCCESS -1
#define FAILURE 0

/* Errors */

#define ERR_PROG_SYNTAX "%s Midi_Filename [Output_Filename]\n"

#define ERR_FILE_CREATE "Error creating file: %s\n"
#define ERR_FILE_READ "Error reading file: %s\n"

#define ERR_MEM_ALLOC "Error allocating memory\n"

#define ERR_TRACK_PLAY "Error playing track\n"
#define ERR_TRACK_NOTE_ON "Error Creating Note\n"
#define ERR_TRACK_NOTE_OFF "Error turning Note off\n"

#define ERR_SIGNAL_INIT "Error initiating signal\n"
#define ERR_SIGNAL_CONCATINATE "Error concatinating signals\n"

#define ERR_NOTE_PLAY "Error playing note\n"

#define ERR_INSTRUMENT_INIT "Error initiating instrument\n"
#define ERR_INSTRUMENT_APPEND_OSC "Error appending oscilaltor to instrument\n"

#endif