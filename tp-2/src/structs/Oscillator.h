#ifndef _OSCILLATOR_H
#define _OSCILLATOR_H

#include <stdlib.h>
#define __USE_MISC
#include <math.h>

typedef enum Oscillator
{
    Sine = 0,
    Square = 1,
    Triangle = 2,
    Sawtooth = 3,
    Noise = 4
} Oscillator;

double oscillator_play(Oscillator osc, double t, double f, double vibrato_coef, double vibrato_freq);

#endif