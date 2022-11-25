#include "Note.h"

double note_to_frequency(__uint8_t note)
{
    /**
     * C0 is 16.35Hz
     * C0 note index is 12
     * => Pow: (note - 12)/12 = note/12 - 1
     */
    return 16.35 * pow(2, (double)note / 12.0 - 1);
}

int note_play(Note *note, Track *track, Instrument *instrument, Signal *signal)
{
    double duration = tickToSecond(note->off_tick - note->on_tick, track);

    if (signal_init(signal, SAMPLING_RATE, duration + instrument->envelope->release_duration) == FAILURE)
    {
        fprintf(stderr, ERR_NOTE_PLAY);
        return FAILURE;
    };

    int i;
    double t;
    for (i = 0; i < signal->samples_count; i++)
    {
        t = (double)i / (double)SAMPLING_RATE;
        signal->data[i] = ((double)note->velocity / 80.0) *
                          instrument_play(instrument,
                                          t,
                                          note_to_frequency(note->id),
                                          duration,
                                          0);
    }

    return SUCCESS;
}
