#include "Export.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int export_csv(Signal *signal, const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (!file)
    {
        fprintf(stderr, ERR_FILE_CREATE, filename);
        return FAILURE;
    }

    int i;

    fprintf(file, "i, t, s\n");

    for (i = 0; i < signal->samples_count; i++)
    {
        fprintf(file, "%d, %f, %f\n", i, (double)i / (double)signal->sampling_rate, signal->data[i]);
    }

    fclose(file);

    return SUCCESS;
}

int export_wav(Signal *signal, const char *filename)
{
    FILE *file = fopen(filename, "w");

    int buffer[1];

    int dataBytesCount = signal->samples_count * 2;

    if (!file)
    {
        fprintf(stderr, ERR_FILE_CREATE, filename);
        return FAILURE;
    }

    fprintf(file, "RIFF");
    buffer[0] = 36 + dataBytesCount;
    fwrite(buffer, 1, 4, file);
    fprintf(file, "WAVE");

    fprintf(file, "fmt ");
    /* Subchunk1Size */
    buffer[0] = 16;
    fwrite(buffer, 4, 1, file);
    /* AudioFormat */
    buffer[0] = 1;
    fwrite(buffer, 2, 1, file);
    /* NumChannels       */
    fwrite(buffer, 2, 1, file);
    /* Sample rate */
    buffer[0] = signal->sampling_rate;
    fwrite(buffer, 4, 1, file);
    /* Byte rate */
    buffer[0] = signal->sampling_rate * 2;
    fwrite(buffer, 4, 1, file);
    /* Block Align */
    buffer[0] = 2;
    fwrite(buffer, 2, 1, file);
    /* BitsPerSample */
    buffer[0] = 16;
    fwrite(buffer, 2, 1, file);

    fprintf(file, "data");
    /* Subchunk2Size */
    buffer[0] = dataBytesCount;
    fwrite(buffer, 4, 1, file);

    int i;
    double maxValue = 0;

    for (i = 0; i < signal->samples_count; i++)
    {
        maxValue = MAX(abs(signal->data[i]), maxValue);
    }

    for (i = 0; i < signal->samples_count; i++)
    {
        buffer[0] = (__int16_t)((signal->data[i] / maxValue) * pow(2, 15) / 2);
        fwrite(buffer, 2, 1, file);
    }

    return SUCCESS;
}
