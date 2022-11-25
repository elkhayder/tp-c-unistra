#ifndef _INSTRUMENT_H
#define _INSTRUMENT_H

#include <stdlib.h>
#include <stdio.h>

#include "Oscillator.h"
#include "ADSREnvelope.h"

#include "../misc/Constants.h"

typedef struct Instrument
{
    Oscillator *oscs;
    double *oscs_coefs;
    size_t oscs_count;
    ADSREnvelope *envelope;
} Instrument;

int instrument_init(Instrument *instrument);

int instrument_append_osc(Instrument *instrument, Oscillator osc, double coef);

int instrument_set_envelope(
    Instrument *instrument,
    double attack_duration,
    double attack_amplitude,
    double decay_duration,
    double release_duration);

double instrument_play(Instrument *instrument, double t, double f, double duration, double phase);

#endif