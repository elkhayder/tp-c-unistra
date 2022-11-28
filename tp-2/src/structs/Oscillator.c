#include "Oscillator.h"

/*
 * Oscillator at t
 * Desmos: https://www.desmos.com/calculator/2xswrci3s0
 */
double oscillator_play(Oscillator osc, double t, double f, double vibrato_coef, double vibrato_freq)
{
    double T = 1 / f;
    /* t += phase / (2 * M_PI * f); */ /* Add phase */
    f += vibrato_coef * sin(2 * M_PI * vibrato_freq);
    double mod = fmod(t, T);

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
