#ifndef __ADSREnvelope_H
#define __ADSREnvelope_H

typedef struct ADSREnvelope
{
    double attack_duration;
    double attack_amplitude;
    double decay_duration;
    double release_duration;
} ADSREnvelope;

double envelope_play(ADSREnvelope *env, double t, double duration);

#endif