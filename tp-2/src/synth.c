#include "synth.h"

#include <stdlib.h>
#include <stdio.h>
#define __USE_MISC
#include <math.h>

/**
 * Common signal props
 */
int initSignal(Signal *signal,
               unsigned int sampling_rate,
               double duration)
{
    signal->sampling_rate = sampling_rate;
    signal->samples_count = (unsigned int)ceil(sampling_rate * duration);
    signal->data = malloc(signal->samples_count * sizeof(DATA_TYPE));

    if (signal->data == NULL)
    {
        fprintf(stderr, ERR_MEM_ALLOC, signal->samples_count * sizeof(DATA_TYPE));
        return SYNTH_FAILURE;
    }

    return SYNTH_SUCCESS;
}

/**
 * Sine wave
 */
int setSineSignal(
    Signal *signal,
    unsigned int sampling_rate,
    double duration,
    double frequency,
    double phase)
{
    if (initSignal(signal, sampling_rate, duration) == SYNTH_FAILURE)
    {
        return SYNTH_FAILURE;
    }

    int i;
    double t;
    for (i = 0; i < signal->samples_count; i++)
    {
        t = (double)i / (double)sampling_rate;
        signal->data[i] = sin(2 * M_PI * frequency * t + phase);
    }

    return SYNTH_SUCCESS;
}

/**
 * Exp
 */
int setExpSignal(Signal *signal,
                 unsigned int sampling_rate,
                 double tau)
{
    if (initSignal(signal, sampling_rate, 15 * log(2) * tau) == SYNTH_FAILURE)
    {
        return SYNTH_FAILURE;
    }

    int i;
    double t;
    for (i = 0; i < signal->samples_count; i++)
    {
        t = (double)i / (double)sampling_rate;
        signal->data[i] = exp(-t / tau);
    }

    return SYNTH_SUCCESS;
}

/**
 * ADSR Envelope
 */
int setADSRSignal(Signal *signal,
                  unsigned int sampling_rate,
                  double attack_duration,
                  double attack_amplitude,
                  double decay_duration,
                  double sustain_duration,
                  double release_duration)
{
    if (initSignal(signal, sampling_rate, attack_duration + decay_duration + sustain_duration + release_duration) == SYNTH_FAILURE)
    {
        return SYNTH_FAILURE;
    }

    int i;
    double t;
    double v; /* Value */
    /**
     * After using MODULO operator, devide by Y to normalize the result back to 1
     * Desmos: https://www.desmos.com/calculator/c9vhjxnrzz
     */
    for (i = 0; i < signal->samples_count; i++)
    {
        t = (double)i / (double)sampling_rate;

        /**
         * Attack
         */
        if (t < attack_duration)
        {
            v = fmod(t, attack_duration) / attack_duration * attack_amplitude;
        }
        /**
         * Decay
         */
        else if (t < attack_duration + decay_duration)
        {
            v = attack_amplitude - (attack_amplitude - 1) * fmod(t - attack_duration, decay_duration) / decay_duration;
        }
        /**
         * Sustain
         */
        else if (t < attack_duration + decay_duration + sustain_duration)
        {
            v = 1;
        }
        /**
         * Release
         */
        else
        {
            v = 1 - fmod(t - attack_duration - decay_duration - sustain_duration, release_duration) / release_duration;
        }

        signal->data[i] = v;
    }

    return SYNTH_SUCCESS;
}

/**
 * Clean
 */
void signal_free(Signal *signal)
{
    free(signal->data);
    signal->data = NULL;
    signal->samples_count = 0;
}

/**
 * Signals manipulation
 */
void signal_amplify(Signal *signal, double coef)
{
    int i;
    for (i = 0; i < signal->samples_count; i++)
    {
        signal->data[i] *= coef;
    }
}

int signal_multiply(Signal *output, Signal *s1, Signal *s2)
{
    if (s1->sampling_rate != s2->sampling_rate)
    {
        return SYNTH_FAILURE;
    }

    int samples = s1->samples_count >= s2->samples_count ? s2->samples_count : s1->samples_count;

    if (initSignal(output, s1->sampling_rate, (double)samples / (double)s1->sampling_rate) == SYNTH_FAILURE)
    {
        return SYNTH_FAILURE;
    }

    int i;
    for (i = 0; i < samples; i++)
    {
        output->data[i] = s1->data[i] * s2->data[i];
    }

    return SYNTH_SUCCESS;
}

int signal_sum(Signal *output, Signal *s1, Signal *s2)
{
    if (s1->sampling_rate != s2->sampling_rate)
    {
        return SYNTH_FAILURE;
    }

    int samples = s1->samples_count >= s2->samples_count ? s2->samples_count : s1->samples_count;

    if (initSignal(output, s1->sampling_rate, (double)samples / (double)s1->sampling_rate) == SYNTH_FAILURE)
    {
        return SYNTH_FAILURE;
    }

    int i;

    for (i = 0; i < samples; i++)
    {
        output->data[i] = s1->data[i] + s2->data[i];
    }

    return SYNTH_SUCCESS;
}