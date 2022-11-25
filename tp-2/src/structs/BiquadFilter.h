#ifndef _BIQUAD_FILTER
#define _BIQUAD_FILTER

#define __USE_MISC
#include <math.h>

#include <stdlib.h>
#include <string.h>

#include "../misc/Constants.h"
#include "Signal.h"

typedef enum BiquadFilterType
{
    LowPass,
    HighPass,
    BandPass,
    Notch,
    LowShelf,
    HighShelf,
    /* AllPass,
    PeakingEQ */
} BiquadFilterType;

typedef struct BiquadFilter
{
    double b0, b1, b2;
    double a0, a1, a2;
} BiquadFilter;

int filter_init(BiquadFilter *filter, BiquadFilterType type, double f0, double Q, double gainDB);

int filter_apply(BiquadFilter *filter, Signal *signal);

#endif