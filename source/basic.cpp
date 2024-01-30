#pragma once

#include <stdint.h>

/* Basic Types */
namespace basic {
    // basic types
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef int8_t s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;

    typedef void* address;

    typedef u8 character;

    // buffer type
    typedef struct buffer {
        address start;
        address end;
    } buffer;

    // create a buffer
    buffer create_buffer(address start, address end) {
        buffer output;

        // setup output
        output.start = start;
        output.end = end;

        return output;
    }

    // create a null buffer
    buffer create_null_buffer() {
        // return empty
        return create_buffer(0, 0);
    }
}
