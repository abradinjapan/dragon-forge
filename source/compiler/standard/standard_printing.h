#ifndef DRAGON__compiler__generator__printing
#define DRAGON__compiler__generator__printing

/* Include */
#include "standard_specifications.h"

/* Functions */
// cell types
typedef enum STANDARD__calculate_decimal_digit_count {
	// preserve start
	STANDARD__calculate_decimal_digit_count__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__calculate_decimal_digit_count__value = STANDARD__calculate_decimal_digit_count__preserve__START,
	STANDARD__calculate_decimal_digit_count__digit_count,
	STANDARD__calculate_decimal_digit_count__zero,
	STANDARD__calculate_decimal_digit_count__ten,

	// preserve end
	STANDARD__calculate_decimal_digit_count__preserve__END,

	// inputs
	STANDARD__calculate_decimal_digit_count__input__value = ANVIL__srt__start__function_io,

	// outputs
	STANDARD__calculate_decimal_digit_count__output__digit_count = ANVIL__srt__start__function_io,
} STANDARD__calculate_decimal_digit_count;

// call function
void STANDARD__code__call__calculate_decimal_digit_count(ANVIL__workspace* workspace, STANDARD__offsets* std_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__value, ANVIL__cell_ID output__digit_count) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__value, STANDARD__calculate_decimal_digit_count__input__value);

	// call function
	ANVIL__code__call__static(workspace, flag, (*std_offsets).offsets[STANDARD__sot__calculate_decimal_digit_count__start]);

	// pass outputs
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__calculate_decimal_digit_count__output__digit_count, output__digit_count);

	return;
}

// build function
void STANDARD__code__calculate_decimal_digit_count(ANVIL__workspace* workspace, STANDARD__offsets* std_offsets) {
	// setup function offset
	(*std_offsets).offsets[STANDARD__sot__calculate_decimal_digit_count__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__calculate_decimal_digit_count__preserve__START, STANDARD__calculate_decimal_digit_count__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__calculate_decimal_digit_count__input__value, STANDARD__calculate_decimal_digit_count__value);

    // check for zero
    ANVIL__code__write_cell(workspace, (ANVIL__cell)0, STANDARD__calculate_decimal_digit_count__zero);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)1, STANDARD__calculate_decimal_digit_count__digit_count);
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, STANDARD__calculate_decimal_digit_count__zero, STANDARD__calculate_decimal_digit_count__value, STANDARD__calculate_decimal_digit_count__zero, ANVIL__sft__never_run, (*std_offsets).offsets[STANDARD__sot__calculate_decimal_digit_count__return]);

    // not a zero, set to default
    ANVIL__code__write_cell(workspace, (ANVIL__cell)0, STANDARD__calculate_decimal_digit_count__digit_count);

    // setup remaining variables
    ANVIL__code__write_cell(workspace, (ANVIL__cell)10, STANDARD__calculate_decimal_digit_count__ten);

	// setup loop start
	(*std_offsets).offsets[STANDARD__sot__calculate_decimal_digit_count__loop_start] = ANVIL__get__offset(workspace);

    // check for remaining digits
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, STANDARD__calculate_decimal_digit_count__zero, STANDARD__calculate_decimal_digit_count__value, STANDARD__calculate_decimal_digit_count__zero, ANVIL__sft__never_run, (*std_offsets).offsets[STANDARD__sot__calculate_decimal_digit_count__return]);

    // perform divide
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_division, STANDARD__calculate_decimal_digit_count__value, STANDARD__calculate_decimal_digit_count__ten, ANVIL__unused_cell_ID, STANDARD__calculate_decimal_digit_count__value);

    // increment count
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__calculate_decimal_digit_count__digit_count, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, STANDARD__calculate_decimal_digit_count__digit_count);

    // jump to beginning
    ANVIL__code__jump__static(workspace, ANVIL__sft__always_run, (*std_offsets).offsets[STANDARD__sot__calculate_decimal_digit_count__loop_start]);

	// setup loop return
	(*std_offsets).offsets[STANDARD__sot__calculate_decimal_digit_count__return] = ANVIL__get__offset(workspace);

	// setup outputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__calculate_decimal_digit_count__digit_count, STANDARD__calculate_decimal_digit_count__output__digit_count);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__calculate_decimal_digit_count__preserve__START, STANDARD__calculate_decimal_digit_count__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// cell types
/*typedef enum STANDARD__cell_to_unsigned_integer_string {
	// preserve start
	STANDARD__cell_to_unsigned_integer_string__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__cell_to_unsigned_integer_string__value = STANDARD__cell_to_unsigned_integer_string__preserve__START,
	STANDARD__cell_to_unsigned_integer_string__number_start,
	STANDARD__cell_to_unsigned_integer_string__number_end,
	STANDARD__cell_to_unsigned_integer_string__current,
	STANDARD__cell_to_unsigned_integer_string__digit_count,
	STANDARD__cell_to_unsigned_integer_string__character,
	STANDARD__cell_to_unsigned_integer_string__ten,
	STANDARD__cell_to_unsigned_integer_string__base_digit_character,

	// preserve end
	STANDARD__cell_to_unsigned_integer_string__preserve__END,

	// inputs
	STANDARD__cell_to_unsigned_integer_string__input__value = ANVIL__srt__start__function_io,

	// outputs
	STANDARD__cell_to_unsigned_integer_string__output__number_start = ANVIL__srt__start__function_io,
	STANDARD__cell_to_unsigned_integer_string__output__number_end,
} STANDARD__cell_to_unsigned_integer_string;

// call function
void STANDARD__code__call__cell_to_unsigned_integer_string(ANVIL__workspace* workspace, STANDARD__offsets* std_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__value, ANVIL__cell_ID output__number_start, ANVIL__cell_ID output__number_end) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__value, STANDARD__cell_to_unsigned_integer_string__input__value);

	// call function
	ANVIL__code__call__static(workspace, flag, (*std_offsets).offsets[STANDARD__sot__cell_to_unsigned_integer_string__start]);

	// pass outputs
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__cell_to_unsigned_integer_string__output__number_start, output__number_start);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__cell_to_unsigned_integer_string__output__number_end, output__number_end);

	return;
}

// build function
void STANDARD__code__cell_to_unsigned_integer_string(ANVIL__workspace* workspace, STANDARD__offsets* std_offsets) {
	// setup function offset
	(*std_offsets).offsets[STANDARD__sot__cell_to_unsigned_integer_string__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__preserve__START, STANDARD__cell_to_unsigned_integer_string__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__input__value, STANDARD__cell_to_unsigned_integer_string__value);

	// setup variables
	ANVIL__code__write_cell(workspace, (ANVIL__cell)10, STANDARD__cell_to_unsigned_integer_string__ten);
	ANVIL__code__write_cell(workspace, (ANVIL__cell)'0', STANDARD__cell_to_unsigned_integer_string__base_digit_character);

	// calculate digit count
	STANDARD__code__call__calculate_decimal_digit_count(workspace, std_offsets, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__value, STANDARD__cell_to_unsigned_integer_string__digit_count);

	// allocate output
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_multiply, ANVIL__srt__constant__ascii_character_byte_size, STANDARD__cell_to_unsigned_integer_string__digit_count, ANVIL__unused_cell_ID, ANVIL__srt__temp__length);
	ANVIL__code__request_memory(workspace, ANVIL__srt__temp__length, STANDARD__cell_to_unsigned_integer_string__number_start, STANDARD__cell_to_unsigned_integer_string__number_end);

	// init to zero
	ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__base_digit_character, ANVIL__srt__constant__ascii_character_byte_size, STANDARD__cell_to_unsigned_integer_string__number_start);

	// convert to string
	// setup current
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__number_end, STANDARD__cell_to_unsigned_integer_string__current);

	// setup loop start offset
	std_offsets->offsets[STANDARD__sot__cell_to_unsigned_integer_string__loop_start] = ANVIL__get__offset(workspace);

	// check for loop end
	ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__number_start, STANDARD__cell_to_unsigned_integer_string__current, STANDARD__cell_to_unsigned_integer_string__number_end, ANVIL__sft__always_run, std_offsets->offsets[STANDARD__sot__cell_to_unsigned_integer_string__return]);

	// calculate character
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_modulous, STANDARD__cell_to_unsigned_integer_string__value, STANDARD__cell_to_unsigned_integer_string__ten, ANVIL__unused_cell_ID, STANDARD__cell_to_unsigned_integer_string__character);
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__cell_to_unsigned_integer_string__base_digit_character, STANDARD__cell_to_unsigned_integer_string__character, ANVIL__unused_cell_ID, STANDARD__cell_to_unsigned_integer_string__character);

	// write character
	ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__character, ANVIL__srt__constant__ascii_character_byte_size, STANDARD__cell_to_unsigned_integer_string__current);

	// next character
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_division, STANDARD__cell_to_unsigned_integer_string__value, STANDARD__cell_to_unsigned_integer_string__ten, ANVIL__unused_cell_ID, STANDARD__cell_to_unsigned_integer_string__value);
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_subtract, STANDARD__cell_to_unsigned_integer_string__current, ANVIL__srt__constant__ascii_character_byte_size, ANVIL__unused_cell_ID, STANDARD__cell_to_unsigned_integer_string__current);
	ANVIL__code__jump__static(workspace, ANVIL__sft__always_run, std_offsets->offsets[STANDARD__sot__cell_to_unsigned_integer_string__loop_start]);

	// setup return
	std_offsets->offsets[STANDARD__sot__cell_to_unsigned_integer_string__return] = ANVIL__get__offset(workspace);

	// setup outputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__number_start, STANDARD__cell_to_unsigned_integer_string__output__number_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__number_end, STANDARD__cell_to_unsigned_integer_string__output__number_end);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__cell_to_unsigned_integer_string__preserve__START, STANDARD__cell_to_unsigned_integer_string__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}*/

// cell types
typedef enum STANDARD__print_binary {
	// preserve start
	STANDARD__print_binary__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__print_binary__value = STANDARD__print_binary__preserve__START,
    STANDARD__print_binary__bit,
    STANDARD__print_binary__character,

	// preserve end
	STANDARD__print_binary__preserve__END,

	// inputs
	STANDARD__print_binary__input__value = ANVIL__srt__start__function_io,

	// outputs
} STANDARD__print_binary;

// call function
void STANDARD__code__call__print_binary(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__value) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__value, STANDARD__print_binary__input__value);

	// call function
	ANVIL__code__call__static(workspace, flag, (*standard_offsets).offsets[STANDARD__sot__print_binary__start]);

	return;
}

// build function
void STANDARD__code__print_binary(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets) {
	// setup function offset
	(*standard_offsets).offsets[STANDARD__sot__print_binary__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__print_binary__preserve__START, STANDARD__print_binary__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__print_binary__input__value, STANDARD__print_binary__value);

	// print all bits
    // for each byte
    for (ANVIL__byte_count byte_index = 0; byte_index < sizeof(ANVIL__cell); byte_index++) {
        // for each bit
        for (ANVIL__bit_count bit_index = 0; bit_index < ANVIL__define__bits_in_byte; bit_index++) {
            // get bit
            ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__bits_and, STANDARD__print_binary__value, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, STANDARD__print_binary__bit);

            // get character
            ANVIL__code__write_cell(workspace, (ANVIL__cell)'0', STANDARD__print_binary__character);
            ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__print_binary__character, STANDARD__print_binary__bit, ANVIL__unused_cell_ID, STANDARD__print_binary__character);

            // print character
            ANVIL__code__debug__putchar(workspace, STANDARD__print_binary__character);

            // next bit
            ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__bits_shift_lower, STANDARD__print_binary__value, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, STANDARD__print_binary__value);
        }

        // print separator if not end of loop
        if (byte_index < sizeof(ANVIL__cell) - 1) {
            ANVIL__code__write_cell(workspace, (ANVIL__cell)'_', STANDARD__print_binary__character);
            ANVIL__code__debug__putchar(workspace, STANDARD__print_binary__character);
        }
    }

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__print_binary__preserve__START, STANDARD__print_binary__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// cell types
typedef enum STANDARD__print_buffer_as_string {
	// preserve start
	STANDARD__print_buffer_as_string__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__print_buffer_as_string__buffer_start = STANDARD__print_buffer_as_string__preserve__START,
	STANDARD__print_buffer_as_string__buffer_end,
	STANDARD__print_buffer_as_string__current,
	STANDARD__print_buffer_as_string__character,

	// preserve end
	STANDARD__print_buffer_as_string__preserve__END,

	// inputs
	STANDARD__print_buffer_as_string__input__buffer_start = ANVIL__srt__start__function_io,
	STANDARD__print_buffer_as_string__input__buffer_end,

	// outputs
} STANDARD__print_buffer_as_string;

// call function
void STANDARD__code__call__print_buffer_as_string(ANVIL__workspace* workspace, STANDARD__offsets* std_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__buffer_start, ANVIL__cell_ID input__buffer_end) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__buffer_start, STANDARD__print_buffer_as_string__input__buffer_start);
	ANVIL__code__cell_to_cell(workspace, flag, input__buffer_end, STANDARD__print_buffer_as_string__input__buffer_end);

	// call function
	ANVIL__code__call__static(workspace, flag, (*std_offsets).offsets[STANDARD__sot__print_buffer_as_string__start]);

	return;
}

// build function
void STANDARD__code__print_buffer_as_string(ANVIL__workspace* workspace, STANDARD__offsets* std_offsets) {
	// setup function offset
	(*std_offsets).offsets[STANDARD__sot__print_buffer_as_string__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__print_buffer_as_string__preserve__START, STANDARD__print_buffer_as_string__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__print_buffer_as_string__input__buffer_start, STANDARD__print_buffer_as_string__buffer_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__print_buffer_as_string__input__buffer_end, STANDARD__print_buffer_as_string__buffer_end);

	// setup current
    ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__print_buffer_as_string__buffer_start, STANDARD__print_buffer_as_string__current);

    // setup loop start offset
	(*std_offsets).offsets[STANDARD__sot__print_buffer_as_string__loop__start] = ANVIL__get__offset(workspace);

    // check if loop range is still valid and exit if necessary
    ANVIL__code__write_cell(workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__sft__temp_0, ANVIL__srt__temp__flag_ID_0);
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, STANDARD__print_buffer_as_string__buffer_start, STANDARD__print_buffer_as_string__current, STANDARD__print_buffer_as_string__buffer_end, ANVIL__sft__always_run, (*std_offsets).offsets[STANDARD__sot__print_buffer_as_string__loop__end]);

    // read character
    ANVIL__code__address_to_cell(workspace, ANVIL__sft__always_run, STANDARD__print_buffer_as_string__current, ANVIL__srt__constant__ascii_character_byte_size, STANDARD__print_buffer_as_string__character);

    // print character
    ANVIL__code__debug__putchar(workspace, STANDARD__print_buffer_as_string__character);

    // next character
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__print_buffer_as_string__current, ANVIL__srt__constant__ascii_character_byte_size, ANVIL__unused_cell_ID, STANDARD__print_buffer_as_string__current);

    // jump to beginning of loop
    ANVIL__code__jump__static(workspace, ANVIL__sft__always_run, (*std_offsets).offsets[STANDARD__sot__print_buffer_as_string__loop__start]);

    // setup loop end offset
	(*std_offsets).offsets[STANDARD__sot__print_buffer_as_string__loop__end] = ANVIL__get__offset(workspace);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__print_buffer_as_string__preserve__START, STANDARD__print_buffer_as_string__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// cell types
typedef enum STANDARD__print_tabs {
	// preserve start
	STANDARD__print_tabs__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__print_tabs__tab_count = STANDARD__print_tabs__preserve__START,
	STANDARD__print_tabs__tab_character,

	// preserve end
	STANDARD__print_tabs__preserve__END,

	// inputs
	STANDARD__print_tabs__input__tab_count = ANVIL__srt__start__function_io,
} STANDARD__print_tabs;

// call function
void STANDARD__code__call__print_tabs(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__tab_count) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__tab_count, STANDARD__print_tabs__input__tab_count);

	// call function
	ANVIL__code__call__static(workspace, flag, (*standard_offsets).offsets[STANDARD__ot__print_tabs__start]);

	return;
}

// build function
void STANDARD__code__print_tabs(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets) {
	// setup function offset
	(*standard_offsets).offsets[STANDARD__ot__print_tabs__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__print_tabs__preserve__START, STANDARD__print_tabs__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__print_tabs__input__tab_count, STANDARD__print_tabs__tab_count);

	// setup tab character
	ANVIL__code__write_cell(workspace, (ANVIL__cell)(ANVIL__u64)'\t', STANDARD__print_tabs__tab_character);

    // setup loop start offset
	(*standard_offsets).offsets[STANDARD__ot__print_tabs__loop__start] = ANVIL__get__offset(workspace);

    // check if loop range is still valid and exit if necessary
    ANVIL__code__write_cell(workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__sft__temp_0, ANVIL__srt__temp__flag_ID_0);
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__0, STANDARD__print_tabs__tab_count, ANVIL__srt__constant__0, ANVIL__sft__never_run, (*standard_offsets).offsets[STANDARD__ot__print_tabs__loop__end]);

    // print character
    ANVIL__code__debug__putchar(workspace, STANDARD__print_tabs__tab_character);

    // decrement tab count
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_subtract, STANDARD__print_tabs__tab_count, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, STANDARD__print_tabs__tab_count);

    // jump to beginning of loop
    ANVIL__code__jump__static(workspace, ANVIL__sft__always_run, (*standard_offsets).offsets[STANDARD__ot__print_tabs__loop__start]);

    // setup loop end offset
	(*standard_offsets).offsets[STANDARD__ot__print_tabs__loop__end] = ANVIL__get__offset(workspace);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__print_tabs__preserve__START, STANDARD__print_tabs__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}


#endif
