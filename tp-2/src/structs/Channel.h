#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <stdlib.h>

#include "BiquadFilter.h"
#include "Instrument.h"
#include "Track.h"
#include "Note.h"

typedef struct Track Track;

typedef struct Channel
{
    __uint8_t id;
    Instrument *instrument;
    BiquadFilter *filter;
} Channel;

int channel_play(Channel *channel, Track *track, Signal *output);

#endif