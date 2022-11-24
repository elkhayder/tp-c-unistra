#ifndef _INSTRUMENT_H
#define _INSTRUMENT_H

#include <stdlib.h>

#include "synth.h"

typedef enum Oscillator
{
    Sine,
    Square,
    Triangle,
    Sawtooth,
    Noise
} Oscillator;

typedef struct ADSREnvelope
{
    double attack_duration;
    double attack_amplitude;
    double decay_duration;
    double release_duration;
} ADSREnvelope;

typedef struct Instrument
{
    Oscillator *oscs;
    double *oscs_coefs;
    size_t oscs_count;
    ADSREnvelope envelope;
} Instrument;

void instrument_init(Instrument *instrument);

void instrument_append_osc(Instrument *instrument, Oscillator osc, double coef);

void instrument_set_envelope(
    Instrument *instrument,
    double attack_duration,
    double attack_amplitude,
    double decay_duration,
    double release_duration);

double envelope_play(ADSREnvelope *env, double t, double duration);

double oscillator_play(Oscillator osc, double t, double f, double phase);

double instrument_play(Instrument *instrument, double t, double f, double duration, double phase);

#endif