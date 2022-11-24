#include "instrument.h"

#define __USE_MISC
#include <math.h>

void instrument_init(Instrument *instrument)
{
    instrument->oscs_count = 0;
    instrument->oscs_coefs = 0;
    instrument->oscs = malloc(0);
    instrument->envelope = *(ADSREnvelope *)malloc(sizeof(ADSREnvelope));
}

void instrument_append_osc(Instrument *instrument, Oscillator osc, double coef)
{
    instrument->oscs_count++;
    instrument->oscs = realloc(instrument->oscs, instrument->oscs_count * sizeof(Oscillator));
    instrument->oscs_coefs = realloc(instrument->oscs_coefs, instrument->oscs_count * sizeof(double));

    instrument->oscs[instrument->oscs_count - 1] = osc;
    instrument->oscs_coefs[instrument->oscs_count - 1] = coef;
}

void instrument_set_envelope(
    Instrument *instrument,
    double attack_duration,
    double attack_amplitude,
    double decay_duration,
    double release_duration)
{
    ADSREnvelope *env = &instrument->envelope;
    env->attack_duration = attack_duration;
    env->attack_amplitude = attack_amplitude;
    env->decay_duration = decay_duration;
    env->release_duration = release_duration;
}

double envelope_play(ADSREnvelope *env, double t, double duration)
{
    /**
     * Attack
     */
    if (t < env->attack_duration)
    {
        return fmod(t, env->attack_duration) / env->attack_duration * env->attack_amplitude;
    }
    /**
     * Decay
     */
    else if (t < env->attack_duration + env->decay_duration)
    {
        return env->attack_amplitude - (env->attack_amplitude - 1) * fmod(t - env->attack_duration, env->decay_duration) / env->decay_duration;
    }
    /**
     * Sustain
     */
    else if (t < env->attack_duration + env->decay_duration + duration)
    {
        return 1;
    }
    /**
     * Release
     */
    else
    {
        return 1 - fmod(t - env->attack_duration - env->decay_duration - duration, env->release_duration) / env->release_duration;
    }
}

double oscillator_play(Oscillator osc, double t, double f, double phase)
{
    double T = 1 / f;
    t += phase / (2 * M_PI * f); /* Add phase */
    double mod = fmod(t, T);
    /*
     * Desmos: https://www.desmos.com/calculator/2xswrci3s0
     */
    switch (osc)
    {
    case Sine:
        return sin(2 * M_PI * f * t);

    case Triangle:
        if (mod < T / 4.0)
        {
            return 4 * f * fmod(t, T / 2.0);
        }
        else if (mod < 3.0 * T / 4.0)
        {
            return -4 * f * mod + 2;
        }
        else
        {
            return 4 * f * fmod(t, T / 2.0) - 2;
        }

    case Square:
        if (mod < T / 2.0)
        {
            return 1;
        }
        else
        {
            return -1;
        }

    case Sawtooth:
        return 2 * fmod(t - T / 2.0, T) - 1;

    case Noise:
        return 2 * (rand() / (float)RAND_MAX) - 1;
    }

    return 0;
}

double instrument_play(Instrument *instrument, double t, double f, double duration, double phase)
{
    double v = 0;
    double oscsCoefsSum = 0;
    int i;
    for (i = 0; i < instrument->oscs_count; i++)
    {
        v += oscillator_play(instrument->oscs[i], t, f, phase) * instrument->oscs_coefs[i];
        oscsCoefsSum += instrument->oscs_coefs[i];
    }

    return v * envelope_play(&instrument->envelope, t, duration) / oscsCoefsSum;
}