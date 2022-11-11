#ifndef _SYNTH_H
#define _SYNTH_H

#define SYNTH_SUCCESS -1
#define SYNTH_FAILURE 0

#define DATA_TYPE double

#define ERR_MEM_ALLOC "Error allocating %ld bytes\n"

typedef struct Signal
{
    unsigned int sampling_rate;
    unsigned int samples_count;
    DATA_TYPE *data;
} Signal;

/**
 * Init common signal params
 */
int initSignal(Signal *signal,
               unsigned int sampling_rate,
               double duration);

/**
 * Sine
 */
int setSineSignal(
    Signal *signal,
    unsigned int sampling_rate,
    double duration,
    double frequency,
    double phase);

/**
 * ADSR Envelope
 */
int setADSRSignal(Signal *signal,
                  unsigned int sampling_rate,
                  double attack_duration,
                  double attack_amplitude,
                  double decay_duration,
                  double sustain_duration,
                  double release_duration);

/**
 * Exp
 */
int setExpSignal(Signal *signal,
                 unsigned int sampling_rate,
                 double tau);

/**
 * Clean
 */
void signal_free(Signal *signal);

/**
 * Signals manipulation
 */
void signal_amplify(Signal *signal, double coef);

int signal_multiply(Signal *output, Signal *s1, Signal *s2);

int signal_sum(Signal *output, Signal *s1, Signal *s2);

#endif