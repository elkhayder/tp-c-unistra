#include "track.h"

void track_init(Track *track)
{
    /**
     * Im not checking if malloc was successful, what's the worst that can happen, NULL ?
     */
    track->notes = malloc(0);
    track->notes_count = 0;
    track->BPM = 120;
    track->ClocksPerBeat = 480;
}

void track_free(Track *track)
{
    track->notes_count = 0;
    free(track->notes);
}

void track_note_on(Track *track, __uint8_t id, __uint32_t tick)
{
    Note note;
    note.id = id;
    note.on_tick = tick;
    note.isOff = false;

    track->notes_count++;
    track->notes = realloc(track->notes, track->notes_count * sizeof(Note));
    track->notes[track->notes_count - 1] = note;
}

void track_note_off(Track *track, __uint8_t id, __uint32_t tick)
{
    int i;
    for (i = 0; i < track->notes_count; i++)
    {
        if (track->notes[i].id == id && !track->notes[i].isOff)
        {
            track->notes[i].off_tick = tick;
            track->notes[i].isOff = true;
            break;
        }
    }
}

double note_to_frequency(__uint8_t note)
{
    /**
     * C0 is 16.35Hz
     * C0 note index is 12
     * => Pow: (note - 12)/12 = note/12 - 1
     */
    return 16.35 * pow(2, (double)note / 12.0 - 1);
}

void track_log(Track *track)
{
    FILE *log = fopen("track.log", "w");

    fprintf(log, "Total Notes: %lu\n", track->notes_count);

    int i;
    for (i = 0; i < track->notes_count; i++)
    {
        Note *note = &track->notes[i];
        fprintf(log, "Id: %u, On: %u, Off: %u\n", note->id, note->on_tick, note->off_tick);
    }

    fclose(log);
}

double tickToSecond(double tick, Track *track)
{
    return 60 * tick / (double)(track->ClocksPerBeat * track->BPM);
}

void note_play(Note *note, Track *track, Instrument *instrument, Signal *signal)
{
    double duration = tickToSecond(note->off_tick - note->on_tick, track) + instrument->envelope.release_duration;

    signal_init(signal, SAMPLING_RATE, duration);

    int i;
    double t;
    for (i = 0; i < signal->samples_count; i++)
    {
        t = (double)i / (double)SAMPLING_RATE;
        signal->data[i] = instrument_play(instrument, t, note_to_frequency(note->id), tickToSecond(note->off_tick - note->on_tick, track), 0);
    }
}

void track_play(Track *track, Signal *output, Instrument *instrument)
{
    int i;
    Signal buffer;
    for (i = 0; i < track->notes_count; i++)
    {
        Note *note = &track->notes[i];
        note_play(note, track, instrument, &buffer);
        signal_concatinate(output, &buffer, tickToSecond(note->on_tick, track));
        signal_free(&buffer);
    }
}