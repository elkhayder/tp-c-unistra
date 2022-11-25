#ifndef _NOTE_H
#define _NOTE_H

#include "Signal.h"
#include "Instrument.h"
#include "Track.h"
#include "../misc/Constants.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct Track Track;

typedef struct Note
{
    __uint8_t id;
    __uint32_t on_tick;
    __uint32_t off_tick;
    __uint8_t velocity;
    bool isOff;
} Note;

int note_play(Note *note, Track *track, Instrument *instrument, Signal *signal);
double note_to_frequency(__uint8_t note);

#endif