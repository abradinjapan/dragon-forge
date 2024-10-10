#ifndef DRAGON__compiler__generator__specifications
#define DRAGON__compiler__generator__specifications

/* Include */
#include "../compiler_specifications.h"

/* Define */
// standard offset type
typedef enum STANDARD__sot {
    // offsets
    STANDARD__sot__package_start,

    // translation functions
    STANDARD__sot__calculate_decimal_digit_count__start,
    STANDARD__sot__calculate_decimal_digit_count__loop_start,
    STANDARD__sot__calculate_decimal_digit_count__return,
    STANDARD__sot__cell_to_unsigned_integer_string__start,
    STANDARD__sot__cell_to_unsigned_integer_string__loop_start,
    STANDARD__sot__cell_to_unsigned_integer_string__return,

    // printing functions
    STANDARD__sot__print_binary__start,
    STANDARD__sot__print_buffer_as_string__start,
    STANDARD__sot__print_buffer_as_string__loop__start,
    STANDARD__sot__print_buffer_as_string__loop__end,

    // context functions
    STANDARD__sot__set_inputs_in_context_buffer__start,
    STANDARD__sot__get_outputs_from_context_buffer__start,

    // list functions
    STANDARD__ot__open_list__start,
    STANDARD__ot__close_list__start,

    // count
    STANDARD__sot__COUNT,
} STANDARD__sot;

// offsets
typedef struct STANDARD__offsets {
    ANVIL__offset offsets[STANDARD__sot__COUNT];
} STANDARD__offsets;

#endif
