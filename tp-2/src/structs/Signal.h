#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../misc/Helpers.h"
#include "../misc/Constants.h"

typedef struct Signal
{
    unsigned int sampling_rate;
    unsigned int samples_count;
    double *data;
} Signal;

/**
 * Init common signal params
 */
int signal_init(Signal *signal,
                unsigned int sampling_rate,
                double duration);

/**
 * Clean
 */
void signal_free(Signal *signal);

/**
 * Concatination
 */
int signal_concatinate(Signal *destination, Signal *input, double t);

#endif