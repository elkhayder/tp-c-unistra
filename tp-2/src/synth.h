#ifndef _SYNTH_H
#define _SYNTH_H

#define SYNTH_SUCCESS -1
#define SYNTH_FAILURE 0

#define ERR_MEM_ALLOC "Error allocating %ld bytes\n"

typedef struct Signal
{
    unsigned int sampling_rate;
    unsigned int samples_count;
    double *data;
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
void signal_amplify(Signal *signal, double gain);

int signal_multiply(Signal *destination, Signal *s1, Signal *s2);

int signal_sum(Signal *destination, Signal *s1, Signal *s2);

int signal_concatinate(Signal *destination, Signal *input, double t);

#endif