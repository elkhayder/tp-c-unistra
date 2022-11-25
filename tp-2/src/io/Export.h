#ifndef _EXPORT_H
#define _EXPORT_H

#include "../misc/Constants.h"

#include "../structs/Signal.h"

int export_csv(Signal *signal, const char *filename);
int export_wav(Signal *signal, const char *filename);

#endif