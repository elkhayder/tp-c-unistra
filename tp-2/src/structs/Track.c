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

void track_log(Track *track)
{
    FILE *log = fopen("track.log", "w");

    fprintf(log, "Total Notes: %lu\n", track->notes_count);

    int i;
    for (i = 0; i < track->notes_count; i++)
    {
        Note *note = &track->notes[i];
        fprintf(log, "Id: %u, On: %u, Off: %u, Velocity: %u\n", note->id, note->on_tick, note->off_tick, note->velocity);
    }

    fclose(log);
}

double tickToSecond(__uint32_t tick, Track *track)
{
    return 60.0 * ((double)tick) / (double)(track->divisions * track->BPM);
}

int track_play(Track *track, Signal *output, Channel *channels)
{
    int i;
    Signal buffer;

    for (i = 0; i < 16; i++)
    {
        signal_init(&buffer, SAMPLING_RATE, 0);
        channel_play(&channels[i], track, &buffer);
        signal_concatinate(output, &buffer, 0);
        signal_free(&buffer);
    }

    return SUCCESS;
}