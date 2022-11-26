#include "ADSREnvelope.h"

#include <math.h>

/**
 * Return envelope gain at time t
 * Needs the note duration parameter to calculate the sustain duration
 */
double envelope_play(ADSREnvelope *env, double t, double duration)
{
    /**
     * Attack
     */
    if (t < env->attack_duration)
    {
        return fmod(t, env->attack_duration) / env->attack_duration * env->attack_amplitude;
    }
    /**
     * Decay
     */
    else if (t < env->attack_duration + env->decay_duration)
    {
        return env->attack_amplitude - (env->attack_amplitude - 1) * fmod(t - env->attack_duration, env->decay_duration) / env->decay_duration;
    }
    /**
     * Sustain
     */
    else if (t < duration)
    {
        /* Should this be Exp decay ?*/
        return 1;
    }
    /**
     * Release
     */
    else if (t < duration + env->release_duration)
    {
        return 1 - fmod(t - duration, env->release_duration) / env->release_duration;
    }
    /**
     * Inactive
     */
    else
    {
        return 0;
    }
}
