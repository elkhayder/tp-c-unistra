#ifndef _INS_H
#define _INS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../structs/Instrument.h"
#include "../misc/Constants.h"

Instrument *ins_parse(const char *filename);

#endif