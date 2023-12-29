#pragma once

#include <stdint.h>

namespace basic {
    // base types
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef int8_t s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;
    typedef void* address;
    typedef s8 character;

    // buffer
    class buffer {
    public:
        address start;
        address end;
        
        buffer(address _start, address _end) {
            start = _start;
            end = _end;
        }
    };
}
