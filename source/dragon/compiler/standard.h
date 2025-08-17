#ifndef DRAGON__standard__buffering
#define DRAGON__standard__buffering

/* Include */
// anvil
#include "../../anvil.h"

// standard files
#include "../../../build_temps/dragon-print.c"
#include "../../../build_temps/dragon-cast.c"
#include "../../../build_temps/dragon-buffer.c"
#include "../../../build_temps/dragon-current.c"
#include "../../../build_temps/dragon-list.c"
#include "../../../build_temps/dragon-context.c"
#include "../../../build_temps/dragon-check.c"
#include "../../../build_temps/dragon-error.c"
#include "../../../build_temps/dragon-json.c"
#include "../../../build_temps/dragon-time.c"
#include "../../../build_temps/dragon-anvil.c"
#include "../../../build_temps/dragon-compile.c"
#include "../../../build_temps/dragon-just_run.c"

/* Turn C Files Into Buffers */
// bufferify any file
ANVIL__buffer STANDARD__bufferify__any_file(unsigned char* buffer, unsigned int length) {
    return ANVIL__create__buffer(buffer, ANVIL__calculate__address_from_buffer_index(buffer, length - 1));
}

#endif
