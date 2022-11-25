#include "Signal.h"

/**
 * Common signal props
 */
int signal_init(Signal *signal,
                unsigned int sampling_rate,
                double duration)
{
    signal->sampling_rate = sampling_rate;
    signal->samples_count = (unsigned int)ceil(sampling_rate * duration);
    signal->data = malloc(signal->samples_count * sizeof(double));

    if (signal->data == NULL)
    {
        fprintf(stderr, ERR_MEM_ALLOC);
        fprintf(stderr, ERR_SIGNAL_INIT);
        return FAILURE;
    }

    return SUCCESS;
}

/**
 * Clean
 */
void signal_free(Signal *signal)
{
    free(signal->data);
    signal->data = NULL;
    signal->samples_count = 0;
}

int signal_concatinate(Signal *destination, Signal *input, double t)
{
    /**
     * Fail if the two signals are sampled with different frequencies
     * TODO: Implement a resampling functionnality
     */
    if (destination->sampling_rate != input->sampling_rate)
    {
        return FAILURE;
    }

    int index = t * destination->sampling_rate;

    int samples_count = MAX(destination->samples_count, index + input->samples_count);

    int i;

    destination->samples_count = samples_count;
    destination->data = (double *)realloc(destination->data, samples_count * sizeof(double));

    if (destination->data == NULL)
    {
        fprintf(stderr, ERR_MEM_ALLOC);
        fprintf(stderr, ERR_SIGNAL_CONCATINATE);
        return FAILURE;
    }

    for (i = 0; i < input->samples_count; i++)
    {
        destination->data[i + index] += input->data[i];
    }

    return SUCCESS;
}