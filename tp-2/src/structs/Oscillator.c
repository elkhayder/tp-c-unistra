#include "Oscillator.h"

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
