#include <stdlib.h>

#include "main.h"
#include "synth.h"
#include "save.h"

int main(int argc, char **argv)
{
    Signal sine;
    Signal adsr;
    Signal exp;

    Signal output1;
    Signal output2;

    if (setExpSignal(&exp, SAMPLING_RATE, 1) == SYNTH_FAILURE)
    {
        return EXIT_FAILURE;
    }

    if (setSineSignal(&sine, SAMPLING_RATE, exp.samples_count / SAMPLING_RATE, 440, 0) == SYNTH_FAILURE)
    {
        return EXIT_FAILURE;
    }

    if (setADSRSignal(&adsr, SAMPLING_RATE, 0.02, 1.1, 0.01, 1, 0.1) == SYNTH_FAILURE)
    {
        return EXIT_FAILURE;
    }

    signal_amplify(&sine, 29000);
    signal_multiply(&output1, &sine, &exp);
    signal_multiply(&output2, &output1, &adsr);

    if (wavSaveSignal(&output2, "output.wav") == SAVE_FAILURE)
    {
        return EXIT_FAILURE;
    }

    signal_free(&sine);
    signal_free(&exp);
    signal_free(&adsr);
    signal_free(&output1);
    signal_free(&output2);

    return EXIT_SUCCESS;
}