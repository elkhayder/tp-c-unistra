#include "Midi.h"

/**

 * Resources:

 * - The MIDI Protocol: MIDI Messages | Simon Hutchinson
        https://www.youtube.com/watch?v=2BccxWkUgaU

 * - Standard MIDI-File Format Spec. 1.1, updated
        http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html

 * - Programming MIDI | javidx9
        https://www.youtube.com/watch?v=040BKtnDdg0

 * - Parts of the code were ported from javidx9 C++ implementation:
        https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_MIDI.cpp

*/

/**
 * Read 2 bytes big endian uint
 */
__uint16_t read16(FILE *file)
{
    __uint16_t n;
    fread(&n, sizeof(n), 1, file);
    return ((n >> 8) | (n << 8));
}

/**
 * Read 4 bytes big endian uint
 */
__uint32_t read32(FILE *file)
{
    __uint32_t n;
    fread(&n, sizeof(n), 1, file);
    /**
     * Good old bit shifts & masks
     */
    return (((n >> 24) & 0xff) | ((n << 8) & 0xff0000) | ((n >> 8) & 0xff00) | ((n << 24) & 0xff000000));
}

/**
 * reads a compressed MIDI value. This can be up to 32 bits long. Essentially if the first byte, first bit is set to 1, that indicates that the next byte is required to construct the full word
 * Only the bottom 7 bits of each byte are used to construct the final word value. Each successive byte that has MSB set, indicates a further byte needs to be read.
 */
__uint32_t readValue(FILE *file)
{
    __uint8_t nByte = 0;

    /* read byte */
    __uint32_t nValue = fgetc(file);

    /* Check MSB, if set, more bytes need reading */
    if (nValue & 0x80)
    {
        /* Extract bottom 7 bits of read byte */
        nValue &= 0x7F;
        do
        {
            /* read next byte */
            nByte = fgetc(file);
            /* Construct value by setting bottom 7 bits, then shifting 7 bits */
            nValue = (nValue << 7) | (nByte & 0x7F);
        } while (nByte & 0x80); /* Loop whilst read byte MSB is 1 */
    }

    /* Return final construction (always 32-bit unsigned integer internally) */
    return nValue;
}

/**
 * Read MIDI string
 */
char *readString(FILE *file, unsigned int length)
{
    /**
     * N + 1 for null terminating char
     */
    char *string = calloc(length + 1, sizeof(char));

    int i;
    for (i = 0; i < length; i++)
    {
        string[i] = fgetc(file);
    }

    string[i + 1] = '\0';

    return string;
}

/**
 * Ignore the annoying warnings
 * https://stackoverflow.com/a/3394268/11755388
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
/**
 * TODO: Errors checking
 */
int midi_parse(const char *filename, Track *track)
{
    FILE *file = fopen(filename, "rb");
    FILE *log = fopen("midi.log", "w");

    if (file == NULL)
    {
        fprintf(stderr, ERR_FILE_READ, filename);
        return FAILURE;
    }

    char *fileID = readString(file, 4);
    __uint32_t headerLength = read32(file);
    __uint16_t format = read16(file);
    __uint16_t trackChunksCount = read16(file);

    track->divisions = read16(file);

    __uint32_t tempo = 0;

    fprintf(log, "Format: %u, Tracks Count: %u, Divisions: %u\n", format, trackChunksCount, track->divisions);

    int chunk;
    for (chunk = 0; chunk < trackChunksCount; chunk++)
    {
        fprintf(log, "New Track\n");

        char *trackId = readString(file, 4);
        __uint32_t trackLength = read32(file);

        fprintf(log, "Track id: %s, TrackLength: %u\n", trackId, trackLength);

        bool isEndOfTrack = false;

        __uint8_t prevStatus = 0;
        __uint32_t wallTime = 0;

        while (!feof(file) && !isEndOfTrack)
        {
            __uint32_t deltaTime = readValue(file);
            __uint8_t status = fgetc(file);

            wallTime += deltaTime;

            if (status < 0x80)
            {
                status = prevStatus;
                fseek(file, -1, SEEK_CUR);
            }

            if ((status & 0xF0) == VoiceNoteOff)
            {
                prevStatus = status;
                __uint8_t nChannel = status & 0x0F;
                __uint8_t nNoteID = fgetc(file);
                __uint8_t nNoteVelocity = fgetc(file);
                fprintf(log, "Note OFF - ID: %u, Velocity: %u, Δt: %u, Channel: %u\n", nNoteID, nNoteVelocity, deltaTime, nChannel);
                track_note_off(track, nNoteID, wallTime, nChannel);
            }
            else if ((status & 0xF0) == VoiceNoteOn)
            {
                prevStatus = status;
                __uint8_t nChannel = status & 0x0F;
                __uint8_t nNoteID = fgetc(file);
                __uint8_t nNoteVelocity = fgetc(file);
                if (nNoteVelocity == 0)
                {
                    fprintf(log, "Note OFF - ID: %u, Velocity: %u, Δt: %u, Channel: %u\n", nNoteID, nNoteVelocity, deltaTime, nChannel);
                    track_note_off(track, nNoteID, wallTime, nChannel);
                }
                else
                {
                    fprintf(log, "Note ON - ID: %u, Velocity: %u, Δt: %u, Channel: %u\n", nNoteID, nNoteVelocity, deltaTime, nChannel);
                    track_note_on(track, nNoteID, wallTime, nNoteVelocity, nChannel);
                }
            }
            else if ((status & 0xF0) == VoiceAftertouch)
            {
                prevStatus = status;
                __uint8_t nChannel = status & 0x0F;
                __uint8_t nNoteID = fgetc(file);
                __uint8_t nNoteVelocity = fgetc(file);
                fprintf(log, "VoiceAftertouch: %u %u\n", nNoteID, nNoteVelocity);
            }

            else if ((status & 0xF0) == VoiceControlChange)
            {
                prevStatus = status;
                __uint8_t nChannel = status & 0x0F;
                __uint8_t nControlID = fgetc(file);
                __uint8_t nControlValue = fgetc(file);
                fprintf(log, "VoiceControlChange: %u %u\n", nControlID, nControlValue);
            }

            else if ((status & 0xF0) == VoiceProgramChange)
            {
                prevStatus = status;
                __uint8_t nChannel = status & 0x0F;
                __uint8_t nProgramID = fgetc(file);
                fprintf(log, "VoiceProgramChange: %u\n", nProgramID);
            }

            else if ((status & 0xF0) == VoiceChannelPressure)
            {
                prevStatus = status;
                __uint8_t nChannel = status & 0x0F;
                __uint8_t nChannelPressure = fgetc(file);
                fprintf(log, "VoiceChannelPressure: %u\n", nChannelPressure);
            }

            else if ((status & 0xF0) == VoicePitchBend)
            {
                prevStatus = status;
                __uint8_t nChannel = status & 0x0F;
                __uint8_t nLS7B = fgetc(file);
                __uint8_t nMS7B = fgetc(file);
                fprintf(log, "VoicePitchBend: %u %u\n", nLS7B, nMS7B);
            }

            else if ((status & 0xF0) == SystemExclusive)
            {
                prevStatus = 0;
                if (status == 0xFF)
                {
                    __uint8_t nType = fgetc(file);
                    __uint8_t nLength = readValue(file);

                    switch (nType)
                    {
                    case MetaSequence:
                        fprintf(log, "Sequence Number: %d%d\n", fgetc(file), fgetc(file));
                        break;
                    case MetaText:
                        fprintf(log, "Text: %s\n", readString(file, nLength));
                        break;
                    case MetaCopyright:
                        fprintf(log, "Copyright: %s\n", readString(file, nLength));
                        break;
                    case MetaTrackName:
                        fprintf(log, "Track Name: %s\n", readString(file, nLength));
                        break;
                    case MetaProgramName:
                        fprintf(log, "Program Name: %s\n", readString(file, nLength));
                        break;
                    case MetaInstrumentName:
                        fprintf(log, "Instrument Name: %s\n", readString(file, nLength));
                        break;
                    case MetaLyrics:
                        fprintf(log, "Lyrics: %s\n", readString(file, nLength));
                        break;
                    case MetaMarker:
                        fprintf(log, "Marker: %s\n", readString(file, nLength));
                        break;
                    case MetaCuePoint:
                        fprintf(log, "Cue: %s\n", readString(file, nLength));
                        break;
                    case MetaChannelPrefix:
                        fprintf(log, "Prefix: %c\n", fgetc(file));
                        break;
                    case MetaEndOfTrack:
                        fprintf(log, "End of track\n");
                        isEndOfTrack = true;
                        break;
                    case MetaSetTempo:
                        /* Tempo is in microseconds per quarter note */
                        if (tempo == 0)
                        {
                            (tempo |= (fgetc(file) << 16));
                            (tempo |= (fgetc(file) << 8));
                            (tempo |= (fgetc(file) << 0));
                        }
                        __uint16_t BPM = (60000000 / tempo);
                        fprintf(log, "Tempo: %u, BPM: %u\n", tempo, BPM);
                        track->BPM = BPM;
                        break;
                    case MetaSMPTEOffset:
                        fprintf(log, "SMPTE: H: %d, M: %u, S: %u, FR: %u, FF: %u, \n", fgetc(file), fgetc(file), fgetc(file), fgetc(file), fgetc(file));
                        break;
                    case MetaTimeSignature:
                        fprintf(log, "Time Signature: %u/%u\n", fgetc(file), (2 << fgetc(file)));
                        fprintf(log, "ClocksPerTick: %u\n", fgetc(file));
                        /* A MIDI "Beat" is 24 ticks, so specify how many 32nd notes constitute a beat */
                        fprintf(log, "32per24Clocks: %u\n", fgetc(file));
                        break;
                    case MetaKeySignature:
                        fprintf(log, "Key Signature: %u\n", fgetc(file));
                        fprintf(log, "Minor Key: %u\n", fgetc(file));
                        break;
                    case MetaSequencerSpecific:
                        fprintf(log, "Sequencer Specific: %s\n", readString(file, nLength));
                        break;
                    default:
                        fprintf(log, "Unrecognised MetaEvent: 0x%02X\n", nType);
                    }
                }

                else if (status == 0xF0)
                {
                    /* System Exclusive Message Begin */
                    fprintf(log, "System Exclusive Begin: %s\n", readString(file, readValue(file)));
                }

                else if (status == 0xF7)
                {
                    /* System Exclusive Message Begin */
                    fprintf(log, "System Exclusive End: %s\n", readString(file, readValue(file)));
                }

                else
                {
                    fprintf(log, "Unknown Status: 0x%02X\n", status);
                }
            }
            else
            {
                fprintf(log, "Unknown Status: 0x%02X\n", status);
            }
        }
    }
    fclose(file);
    fclose(log);

    /**
     * Substract the initial Delta time where the headers were transmitted
     * First note in Track notes list should be the origin of t axis
     *
     * Update:
     * Previously I naivly assumed that teh first note is the start of t origin, it introduced a very annoying bug,
     * now I loop through all the notes to find the smallest on_tick ans take it as my origin
     *
     * Update:
     * I now just ignore this, I fix the leading blank later when exporting the data
     */

    /*int i;
     __uint32_t offset = (__uint32_t)HUGE_VAL;
    for (i = 0; i < track->notes_count; i++)
    {
        offset = MIN(offset, track->notes[i].on_tick);
    }

    for (i = 0; i < track->notes_count; i++)
    {
        track->notes[i].on_tick -= offset;
        track->notes[i].off_tick -= offset;
    }*/

    return SUCCESS;
}
#pragma GCC diagnostic pop
