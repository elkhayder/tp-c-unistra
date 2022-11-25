#ifndef _TRACK_H
#define _TRACK_H

#include "Instrument.h"
#include "Signal.h"
#include "Note.h"

typedef struct Note Note;

typedef struct Track
{
    Note *notes;
    size_t notes_count;
    __uint16_t BPM;
    __uint16_t divisions;
} Track;

int track_init(Track *track);
void track_free(Track *track);
int track_note_on(Track *track, __uint8_t id, __uint32_t tick, __uint8_t velocity);
int track_note_off(Track *track, __uint8_t id, __uint32_t tick);
void track_log(Track *track);
double tickToSecond(__uint32_t tick, Track *track);
int track_play(Track *track, Signal *output, Instrument *instrument);

#endif