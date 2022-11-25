#include "Ins.h"

Instrument *ins_parse(const char *filename)
{
    /* I assume that the Ins file is respecting the format */

#define BUFFER_SIZE 100
    char buffer[BUFFER_SIZE];

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        fprintf(stderr, ERR_FILE_READ, filename);
        return FAILURE;
    }

    Instrument *instruments = calloc(16, sizeof(Instrument));
    __uint8_t currentInstrumentIndex = 0;
    int currentStep = 0;

    /* Init First Instrument */
    /* &instruments[0] == instruments */
    instrument_init(instruments);

    while (fgets(buffer, BUFFER_SIZE, file) != NULL)
    {
        /* Comment line */
        if (buffer[0] == '#')
            continue;

        /* Empty line */
        if (buffer[0] == '\n')
        {
            currentInstrumentIndex++;
            currentStep = 0;
            /* Exit file reading on max Instuments */
            if (currentInstrumentIndex == 16)
                break;

            instrument_init(&instruments[currentInstrumentIndex]);
            continue;
        }

        /* Envelope */
        if (currentStep == 0)
        {
            double AttackDuration, AttackAmplitude, DecayDuration, ReleaseDuration;
            /*             printf("%s\n", strtok(buffer, " "));
             */
            sscanf(strtok(buffer, " "), "%lf", &AttackDuration);
            sscanf(strtok(NULL, " "), "%lf", &AttackAmplitude);
            sscanf(strtok(NULL, " "), "%lf", &DecayDuration);
            sscanf(strtok(NULL, " "), "%lf", &ReleaseDuration);
            instrument_set_envelope(
                &instruments[currentInstrumentIndex],
                AttackDuration,
                AttackAmplitude,
                DecayDuration,
                ReleaseDuration);
        }
        /* Filter */
        else if (currentStep == 1)
        {
            BiquadFilter *filter = malloc(sizeof(BiquadFilter));

            int type;
            double f0, Q, gainDB;

            sscanf(strtok(buffer, " "), "%d", &type);
            sscanf(strtok(NULL, " "), "%lf", &f0);
            sscanf(strtok(NULL, " "), "%lf", &Q);
            sscanf(strtok(NULL, " "), "%lf", &gainDB);

            filter_init(filter, (BiquadFilterType)type, f0, Q, gainDB);

            instruments[currentInstrumentIndex].filter = filter;
        }
        /* Oscillators */
        else
        {
            int oscilaltor;
            double coef;

            sscanf(strtok(buffer, " "), "%d", &oscilaltor);
            sscanf(strtok(NULL, " "), "%lf", &coef);

            instrument_append_osc(&instruments[currentInstrumentIndex], (Oscillator)oscilaltor, coef);
        }

        currentStep++;
    }

    return instruments;
}