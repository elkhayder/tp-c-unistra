#ifndef _OSCILLATOR_H
#define _OSCILLATOR_H

#include <stdlib.h>
#define __USE_MISC
#include <math.h>

typedef enum Oscillator
{
    Sine,
    Square,
    Triangle,
    Sawtooth,
    Noise
} Oscillator;

double oscillator_play(Oscillator osc, double t, double f, double phase);

#endif