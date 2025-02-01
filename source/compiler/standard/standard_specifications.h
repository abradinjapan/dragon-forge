#ifndef DRAGON__compiler__generator__specifications
#define DRAGON__compiler__generator__specifications

/* Include */
#include "../compiler_specifications.h"

/* Define */
// standard offset type
typedef enum STANDARD__sot {
    // offsets
    STANDARD__sot__package_start,

    // context functions
    STANDARD__sot__set_inputs_in_context_buffer__start,
    STANDARD__sot__get_outputs_from_context_buffer__start,

    // list functions
    STANDARD__sot__open_list__start,
    STANDARD__sot__close_list__start,
    STANDARD__sot__append_buffer__start,
    STANDARD__sot__append_buffer__skip_reallocation,


    // count
    STANDARD__sot__COUNT,
} STANDARD__sot;

// offsets
typedef struct STANDARD__offsets {
    ANVIL__offset offsets[STANDARD__sot__COUNT];
} STANDARD__offsets;

#endif
