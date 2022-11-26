#include "Channel.h"

int channel_play(Channel *channel, Track *track, Signal *output)
{
    int i;
    Signal buffer;

    for (i = 0; i < track->notes_count; i++)
    {
        Note *note = &track->notes[i];

        if (channel->instrument == NULL || channel->id != note->channel)
        {
            continue;
        }

        if (note_play(note, track, channel->instrument, &buffer) == FAILURE)
        {
            fprintf(stderr, ERR_TRACK_PLAY);
            return FAILURE;
        };

        if (signal_concatinate(output, &buffer, tickToSecond(note->on_tick, track)) == FAILURE)
        {
            fprintf(stderr, ERR_TRACK_PLAY);
            return FAILURE;
        };

        signal_free(&buffer);
    }

    if (channel->filter != NULL)
    {
        filter_apply(channel->filter, output);
    }

    return SUCCESS;
}