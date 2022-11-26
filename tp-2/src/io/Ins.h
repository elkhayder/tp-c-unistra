#ifndef _INS_H
#define _INS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../structs/Channel.h"
#include "../structs/Instrument.h"
#include "../misc/Constants.h"

Channel *ins_parse(const char *filename);

#endif