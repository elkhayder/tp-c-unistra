#include "Instrument.h"

int instrument_init(Instrument *instrument)
{
    instrument->oscs_count = 0;
    instrument->oscs_coefs = 0;
    instrument->oscs = malloc(0);
    instrument->envelope = malloc(sizeof(ADSREnvelope));

    if (instrument->envelope == NULL)
    {
        fprintf(stderr, ERR_MEM_ALLOC);
        fprintf(stderr, ERR_INSTRUMENT_INIT);
        return FAILURE;
    }

    return SUCCESS;
}

int instrument_append_osc(Instrument *instrument, Oscillator osc, double coef)
{
    instrument->oscs_count++;

    instrument->oscs = realloc(instrument->oscs, instrument->oscs_count * sizeof(Oscillator));
    if (instrument->oscs == NULL)
    {
        fprintf(stderr, ERR_MEM_ALLOC);
        fprintf(stderr, ERR_INSTRUMENT_APPEND_OSC);
        return FAILURE;
    }

    instrument->oscs_coefs = realloc(instrument->oscs_coefs, instrument->oscs_count * sizeof(double));
    if (instrument->oscs_coefs == NULL)
    {
        fprintf(stderr, ERR_MEM_ALLOC);
        fprintf(stderr, ERR_INSTRUMENT_APPEND_OSC);
        return FAILURE;
    }

    instrument->oscs[instrument->oscs_count - 1] = osc;
    instrument->oscs_coefs[instrument->oscs_count - 1] = coef;

    return SUCCESS;
}

int instrument_set_envelope(
    Instrument *instrument,
    double attack_duration,
    double attack_amplitude,
    double decay_duration,
    double release_duration)
{
    ADSREnvelope *env = instrument->envelope;
    env->attack_duration = attack_duration;
    env->attack_amplitude = attack_amplitude;
    env->decay_duration = decay_duration;
    env->release_duration = release_duration;

    return SUCCESS;
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

    return v * envelope_play(instrument->envelope, t, duration) / oscsCoefsSum;
}