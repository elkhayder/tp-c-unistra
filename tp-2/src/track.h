#ifndef _TRACK_H
#define _TRACK_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "synth.h"
#include "instrument.h"
#include "main.h"

typedef struct Note
{
    __uint8_t id;
    __uint32_t on_tick;
    __uint32_t off_tick;
    bool isOff;
} Note;

typedef struct Track
{
    Note *notes;
    size_t notes_count;
    __uint32_t BPM;
    __uint16_t ClocksPerBeat;
} Track;

void track_init(Track *track);
void track_free(Track *track);
void track_note_on(Track *track, __uint8_t id, __uint32_t tick);
void track_note_off(Track *track, __uint8_t id, __uint32_t tick);
void track_log(Track *track);
double note_to_frequency(__uint8_t note);
double tickToSecond(double tick, Track *track);
void note_play(Note *note, Track *track, Instrument *instrument, Signal *signal);
void track_play(Track *track, Signal *output, Instrument *instrument);
#endif