#ifndef _SAVE_H
#define _SAVE_H

#include "synth.h"

#define ERR_FILE_CREATE "Error creating output file: %s\n"

#define SAVE_SUCCESS -1
#define SAVE_FAILURE 0

int csvSaveSignal(Signal *signal, const char *filename);
int wavSaveSignal(Signal *signal, const char *filename);

#endif