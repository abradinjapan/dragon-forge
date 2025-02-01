#ifndef DRAGON__standard__buffering
#define DRAGON__standard__buffering

/* Include */
// anvil
#include "../../anvil.h"

// standard files
#include "../../../build_temps/print.c"
#include "../../../build_temps/cast.c"
#include "../../../build_temps/buffer.c"

/* Turn C Files Into Buffers */
// bufferify any file
ANVIL__buffer STANDARD__bufferify__any_file(unsigned char* buffer, unsigned int length) {
    return ANVIL__create__buffer(buffer, ANVIL__calculate__address_from_buffer_index(buffer, length - 1));
}

#endif
