#include "BiquadFilter.h"

/**
 * Resources:
 *
 * - Cookbook formulae for audio EQ biquad filter coefficients
 *      https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
 */

int filter_init(BiquadFilter *filter, BiquadFilterType type, double f0, double Q, double gainDB)
{
    const double A = pow(10, gainDB / 40);
    const double w0 = 2 * M_PI * f0 / (double)SAMPLING_RATE;
    /* Sin & Cos are frequently calculated, it would be best to pre calculate them once */
    const double w0cos = cos(w0);
    const double w0sin = sin(w0);
    /* a: α */
    const double a = w0sin / (2.0 * Q);

    printf("Q = %f, A = %f, w0 = %f, α = %f\n", Q, A, w0, a);

    switch (type)
    {
    case LowPass:
        filter->b0 = (1.0 - w0cos) / 2.0;
        filter->b1 = 1.0 - w0cos;
        filter->b2 = (1.0 - w0cos) / 2.0;

        filter->a0 = 1 + a;
        filter->a1 = -2 * w0cos;
        filter->a2 = 1 - a;
        break;

    case HighPass:
        filter->b0 = (1.0 + w0cos) / 2.0;
        filter->b1 = 1.0 - w0cos;
        filter->b2 = (1.0 + w0cos) / 2.0;

        filter->a0 = 1 + a;
        filter->a1 = -2 * w0cos;
        filter->a2 = 1 - a;
        break;

    case BandPass:
        filter->b0 = a;
        filter->b1 = 0;
        filter->b2 = -a;

        filter->a0 = 1 + a;
        filter->a1 = -2 * w0cos;
        filter->a2 = 1 - a;
        break;

    case Notch:
        filter->b0 = 1;
        filter->b1 = -2 * w0cos;
        filter->b2 = 1;

        filter->a0 = 1 + a;
        filter->a1 = -2 * w0cos;
        filter->a2 = 1 - a;
        break;

    case LowShelf:
        filter->b0 = A * (A + 1 - (A - 1) * w0cos + 2 * sqrt(A) * a);
        filter->b1 = 2 * A * (A - 1 - (A + 1) * w0cos);
        filter->b2 = A * (A + 1 - (A - 1) * w0cos - 2 * sqrt(A) * a);

        filter->a0 = A + 1 + (A - 1) * w0cos + 2 * sqrt(A) * a;
        filter->a1 = -2 * (A - 1 + (A + 1) * w0cos);
        filter->a2 = A + 1 + (A - 1) * w0cos - 2 * sqrt(A) * a;
        break;

    case HighShelf:
        filter->b0 = A * (A + 1 + (A - 1) * w0cos + 2 * sqrt(A) * a);
        filter->b1 = -2 * A * (A - 1 + (A + 1) * w0cos);
        filter->b2 = A * (A + 1 + (A - 1) * w0cos - 2 * sqrt(A) * a);

        filter->a0 = A + 1 - (A - 1) * w0cos + 2 * sqrt(A) * a;
        filter->a1 = 2 * (A - 1 - (A + 1) * w0cos);
        filter->a2 = A + 1 - (A - 1) * w0cos - 2 * sqrt(A) * a;
        break;

    default:
        return FAILURE;
    }

    printf("Coefs: %f %f %f %f %f %f\n", filter->b0, filter->b1, filter->b2, filter->a0, filter->a1, filter->a2);

    return SUCCESS;
}

int filter_apply(BiquadFilter *filter, Signal *signal)
{
    double *y = signal->data;
    /**
     * I could  allocate memory for only the 2 previous values, but I guess it is simpler to be like this
     */
    double *x = malloc(signal->samples_count * sizeof(double));
    if (x == NULL)
    {
        fprintf(stderr, ERR_MEM_ALLOC);
        fprintf(stderr, ERR_APPLYING_FILTER);
        return FAILURE;
    }
    memcpy(x, y, signal->samples_count * sizeof(double));

    int n;
    for (n = 2; n < signal->samples_count; n++)
    {
        y[n] = x[n] * filter->b0 +
               x[n - 1] * filter->b1 +
               x[n - 2] * filter->b2 -
               y[n - 1] * filter->a1 -
               y[n - 2] * filter->a2;

        y[n] /= filter->a0;
    }

    free(x);

    return SUCCESS;
}