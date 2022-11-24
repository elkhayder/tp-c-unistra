#ifndef _MIDI_H
#define _MIDI_H

typedef struct Note
{
    double on_time;
    double off_time;
    unsigned char number;
} Note;

typedef enum EventName
{
    VoiceNoteOff = 0x80,
    VoiceNoteOn = 0x90,
    VoiceAftertouch = 0xA0,
    VoiceControlChange = 0xB0,
    VoiceProgramChange = 0xC0,
    VoiceChannelPressure = 0xD0,
    VoicePitchBend = 0xE0,
    SystemExclusive = 0xF0,
} EventName;

typedef enum MetaEventName
{
    MetaSequence = 0x00,
    MetaText = 0x01,
    MetaCopyright = 0x02,
    MetaTrackName = 0x03,
    MetaProgramName = 0x08,
    MetaInstrumentName = 0x04,
    MetaLyrics = 0x05,
    MetaMarker = 0x06,
    MetaCuePoint = 0x07,
    MetaChannelPrefix = 0x20,
    MetaEndOfTrack = 0x2F,
    MetaSetTempo = 0x51,
    MetaSMPTEOffset = 0x54,
    MetaTimeSignature = 0x58,
    MetaKeySignature = 0x59,
    MetaSequencerSpecific = 0x7F,
} MetaEventName;

#endif