#include "Track.h"

int track_init(Track *track)
{
    /**
     * Im not checking if malloc was successful, what's the worst that can happen, NULL ?
     */
    track->notes = malloc(0);
    track->notes_count = 0;
    track->BPM = 120;
    track->divisions = 90;

    return SUCCESS;
}

void track_free(Track *track)
{
    track->notes_count = 0;
    free(track->notes);
}

/**
 * Append a new Note
 */
int track_note_on(Track *track, __uint8_t id, __uint32_t tick, __uint8_t velocity, __uint8_t channel)
{
    Note note;
    note.id = id;
    note.on_tick = tick;
    note.isOff = false;
    note.velocity = velocity;
    note.channel = channel;

    track->notes_count++;
    track->notes = realloc(track->notes, track->notes_count * sizeof(Note));

    if (track->notes == NULL)
    {
        fprintf(stderr, ERR_TRACK_NOTE_ON);
        return FAILURE;
    }

    track->notes[track->notes_count - 1] = note;

    return SUCCESS;
}

/**
 * Turn off previously started note
 */
int track_note_off(Track *track, __uint8_t id, __uint32_t tick, __uint8_t channel)
{
    int i;
    for (i = 0; i < track->notes_count; i++)
    {
        if (track->notes[i].id == id && track->notes[i].channel == channel && !track->notes[i].isOff)
        {
            track->notes[i].off_tick = tick;
            track->notes[i].isOff = true;
            return SUCCESS;
        }
    }

    fprintf(stderr, ERR_TRACK_NOTE_OFF);
    return FAILURE;
}

/**
 * Export track to log
 */
void track_log(Track *track)
{
    FILE *log = fopen("track.log", "w");

    fprintf(log, "Total Notes: %lu\n", track->notes_count);
    fprintf(log, "BPM: %u\n", track->BPM);
    fprintf(log, "Divisions: %u\n", track->divisions);

    int i;
    for (i = 0; i < track->notes_count; i++)
    {
        Note *note = &track->notes[i];
        fprintf(log, "Id: %u, Channel: %d, On: %u, Off: %u, Velocity: %u\n", note->id, note->channel, note->on_tick, note->off_tick, note->velocity);
    }

    fclose(log);
}

/**
 * Convert MIDI tick to realtime seconds with respect to track properties
 */
double tickToSecond(__uint32_t tick, Track *track)
{
    return 60.0 * ((double)tick) / (double)(track->divisions * track->BPM);
}

/**
 * Play track.
 * Basically loops through all channels and play them individually, then stacks all the signals on top of each other
 */
int track_play(Track *track, Signal *output, Channel *channels)
{
    size_t i;
    Signal buffer;

    for (i = 0; i < 16; i++)
    {
        signal_init(&buffer, SAMPLING_RATE, 0);

        if (channel_play(&channels[i], track, &buffer) == FAILURE)
        {
            fprintf(stderr, ERR_TRACK_PLAY);
            return FAILURE;
        }

        if (signal_concatinate(output, &buffer, 0) == FAILURE)
        {
            fprintf(stderr, ERR_TRACK_PLAY);
            return FAILURE;
        }

        signal_free(&buffer);
    }

    /**
     * This block of code was previously responsible of removing the blank on the start, it compiles and all, but im not sure what went wrong. I'm too young to deal with this
     */
    /* size_t startOfSongIndex = 0;
    for (i = 0; i < output->samples_count; i++)
    {
        if (output->data[i] != 0.0)
        {
            startOfSongIndex = i;
            break;
        }
    }
    output->samples_count -= startOfSongIndex;
    double *temp = malloc(output->samples_count * sizeof(double));
    memcpy(temp, output->data + startOfSongIndex, output->samples_count);
    free(output->data);
    output->data = temp; */

    return SUCCESS;
}