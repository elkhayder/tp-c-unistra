#include <stdlib.h>

#include "main.h"
#include "synth.h"
#include "save.h"
#include "midi.h"

#include <math.h>

int main(int argc, char **argv)
{
    Signal sine, adsr, exp, exp_adsr;
    Signal output, temp;

    if (setExpSignal(&exp, SAMPLING_RATE, 1) == SYNTH_FAILURE)
    {
        return EXIT_FAILURE;
    }

    if (setADSRSignal(&adsr, SAMPLING_RATE, 0.02, 1.1, 0.01, 1, 0.1) == SYNTH_FAILURE)
    {
        return EXIT_FAILURE;
    }

    initSignal(&output, SAMPLING_RATE, 0);

    signal_multiply(&exp_adsr, &exp, &adsr);

    int i;
    int base_frequency = 220;
    for (i = 0; i < 12; i++)
    {
        if (setSineSignal(&sine,
                          SAMPLING_RATE,
                          exp.samples_count / SAMPLING_RATE,
                          base_frequency * pow(2, (double)i / 12.0),
                          0) == SYNTH_FAILURE)
        {
            return EXIT_FAILURE;
        }

        signal_amplify(&sine, 29000);
        signal_multiply(&temp, &sine, &exp_adsr);

        signal_concatinate(&output, &temp, 0.2 * i);

        signal_free(&temp);
        signal_free(&sine);
    }

    if (wavSaveSignal(&output, "output.wav") == SAVE_FAILURE)
    {
        return EXIT_FAILURE;
    }

    signal_free(&exp);
    signal_free(&adsr);
    signal_free(&output);
    signal_free(&exp_adsr);

    readMidiFile("Mario Bros. - Super Mario Bros. Theme.mid");
    /* readMidiFile("Cymatics - August - 90 BPM A Min.mid"); */

    return EXIT_SUCCESS;
}