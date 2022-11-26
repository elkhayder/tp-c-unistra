#include "Ins.h"

#define BUFFER_SIZE 100

/**
 * Parse .ins file.
 * .ins format is specified in the README file.
 * I blindly assume that the .ins file is respecting the format,
 * that would make it far easier to  parse the file
 */
Channel *ins_parse(const char *filename)
{
    char buffer[BUFFER_SIZE];

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        fprintf(stderr, ERR_FILE_READ, filename);
        return FAILURE;
    }
    /**
     * 16 channels as per the MIDI specification
     */
    Channel *channels = malloc(16 * sizeof(Channel));

    /**
     * Init Channels
     */
    int i;
    for (i = 0; i < 16; i++)
    {
        channels[i].id = i;
        Instrument *ins = malloc(sizeof(Instrument));
        instrument_init(ins);
        channels[i].instrument = ins;
        channels[i].filter = NULL;
    }

    __uint8_t currentChannelIndex = 0;
    int currentStep = 0;
    while (fgets(buffer, BUFFER_SIZE, file) != NULL)
    {
        /* Comment line */
        if (buffer[0] == '#')
            continue;

        /* Empty line */
        if (buffer[0] == '\n')
        {
            currentChannelIndex++;
            currentStep = 0;
            /* Exit file reading on max Instuments */
            if (currentChannelIndex == 16)
                break;

            continue;
        }

        /* Envelope */
        if (currentStep == 0)
        {
            double AttackDuration, AttackAmplitude, DecayDuration, ReleaseDuration;
            sscanf(strtok(buffer, " "), "%lf", &AttackDuration);
            sscanf(strtok(NULL, " "), "%lf", &AttackAmplitude);
            sscanf(strtok(NULL, " "), "%lf", &DecayDuration);
            sscanf(strtok(NULL, " "), "%lf", &ReleaseDuration);
            instrument_set_envelope(
                channels[currentChannelIndex].instrument,
                AttackDuration,
                AttackAmplitude,
                DecayDuration,
                ReleaseDuration);
        }
        /* Filter */
        else if (currentStep == 1)
        {
            if (buffer[0] == '-')
            {
                currentStep++;
                continue;
            }

            BiquadFilter *filter = malloc(sizeof(BiquadFilter));

            int type;
            double f0, Q, gainDB;

            sscanf(strtok(buffer, " "), "%d", &type);
            sscanf(strtok(NULL, " "), "%lf", &f0);
            sscanf(strtok(NULL, " "), "%lf", &Q);
            sscanf(strtok(NULL, " "), "%lf", &gainDB);

            filter_init(filter, (BiquadFilterType)type, f0, Q, gainDB);

            channels[currentChannelIndex].filter = filter;
        }
        /* Oscillators */
        else
        {
            int oscillator;
            double coef;

            sscanf(strtok(buffer, " "), "%d", &oscillator);
            sscanf(strtok(NULL, " "), "%lf", &coef);

            instrument_append_osc(channels[currentChannelIndex].instrument, (Oscillator)oscillator, coef);
        }

        currentStep++;
    }

    return channels;
}