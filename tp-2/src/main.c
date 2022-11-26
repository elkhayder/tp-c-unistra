/**
 * Author: @elkhayder
 *
 * Please read the README.
 *
 * MIT License
 */

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "misc/Constants.h"

#include "structs/Signal.h"
#include "structs/Instrument.h"
#include "structs/Track.h"
#include "structs/BiquadFilter.h"

#include "io/Midi.h"
#include "io/Export.h"
#include "io/Ins.h"

int main(int argc, char **argv)
{
    char midiFilename[1024];
    char insFilename[1024];
    char wavFilename[1024];

    if (argc != 4)
    {
        fprintf(stderr, ERR_PROG_SYNTAX, argv[0]);
        return EXIT_FAILURE;
    }

    strcpy(midiFilename, argv[1]);
    strcpy(insFilename, argv[2]);
    strcpy(wavFilename, argv[3]);

    Channel *channels = ins_parse(insFilename);

    Signal output;
    signal_init(&output, SAMPLING_RATE, 0);

    Track track;
    track_init(&track);
    midi_parse(midiFilename, &track);

    track_play(&track, &output, channels);

    export_wav(&output, wavFilename);

    track_log(&track);
    track_free(&track);

    signal_free(&output);

    return EXIT_SUCCESS;
}

/*
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

    parseMidiFile("Mario Bros. - Super Mario Bros. Theme.mid");
    parseMidiFile("Cymatics - August - 90 BPM A Min.mid");

    return EXIT_SUCCESS;
} */