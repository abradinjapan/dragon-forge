#ifndef DRAGON__compiler__generator__context_related
#define DRAGON__compiler__generator__context_related

/* Include */
#include "standard_specifications.h"

/* Functions */
/* Convert Cells to Integer Literals */
// cell types
typedef enum STANDARD__calculate_digit_count {
	// preserve start
	STANDARD__calculate_digit_count__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__calculate_digit_count__base = STANDARD__calculate_digit_count__preserve__START,
	STANDARD__calculate_digit_count__value,
	STANDARD__calculate_digit_count__count,

	// preserve end
	STANDARD__calculate_digit_count__preserve__END,

	// inputs
	STANDARD__calculate_digit_count__input__base = ANVIL__srt__start__function_io,
	STANDARD__calculate_digit_count__input__value,

	// outputs
	STANDARD__calculate_digit_count__output__count = ANVIL__srt__start__function_io,
} STANDARD__calculate_digit_count;

// call function
void STANDARD__code__call__calculate_digit_count(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__base, ANVIL__cell_ID input__value, ANVIL__cell_ID output__count) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__base, STANDARD__calculate_digit_count__input__base);
	ANVIL__code__cell_to_cell(workspace, flag, input__value, STANDARD__calculate_digit_count__input__value);

	// call function
	ANVIL__code__call__static(workspace, flag, (*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__start]);

	// pass outputs
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__calculate_digit_count__output__count, output__count);

	return;
}

// build function
void STANDARD__code__calculate_digit_count(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets) {
	// setup function offset
	(*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__calculate_digit_count__preserve__START, STANDARD__calculate_digit_count__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__calculate_digit_count__input__base, STANDARD__calculate_digit_count__base);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__calculate_digit_count__input__value, STANDARD__calculate_digit_count__value);

	// check for zero value
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__0, STANDARD__calculate_digit_count__value, ANVIL__srt__constant__0, ANVIL__sft__never_run, (*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__value_is_zero_exit]);

    // check for invalid base
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__0, STANDARD__calculate_digit_count__base, ANVIL__srt__constant__1, ANVIL__sft__never_run, (*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__invalid_base_exit]);

    // calculate digit count
    // setup counter
    ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__0, STANDARD__calculate_digit_count__count);

    // setup loop header
    (*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__loop_start] = ANVIL__get__offset(workspace);

    // check or zero value
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__0, STANDARD__calculate_digit_count__value, ANVIL__srt__constant__0, ANVIL__sft__never_run, (*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__normal_exit]);

    // divide
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_division, STANDARD__calculate_digit_count__value, STANDARD__calculate_digit_count__base, ANVIL__unused_cell_ID, STANDARD__calculate_digit_count__value);

    // increment count
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__calculate_digit_count__count, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, STANDARD__calculate_digit_count__count);

    // jump to loop start
    ANVIL__code__jump__static(workspace, ANVIL__sft__always_run, (*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__loop_start]);

    // normal calculation exit
    {
        // setup offset
        (*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__normal_exit] = ANVIL__get__offset(workspace);

        // setup outputs
        ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__calculate_digit_count__count, STANDARD__calculate_digit_count__output__count);

        // setup function epilogue
        ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__calculate_digit_count__preserve__START, STANDARD__calculate_digit_count__preserve__END);

        // return to caller
    	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);
    }

    // zero value calculation exit
    {
        // setup offset
        (*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__value_is_zero_exit] = ANVIL__get__offset(workspace);

        // setup output
        ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__1, STANDARD__calculate_digit_count__count);

        // setup outputs
        ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__calculate_digit_count__count, STANDARD__calculate_digit_count__output__count);

        // setup function epilogue
        ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__calculate_digit_count__preserve__START, STANDARD__calculate_digit_count__preserve__END);

        // return to caller
    	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);
    }

    // invalid base calculation exit
    {
        // setup offset
        (*standard_offsets).offsets[STANDARD__sot__calculate_digit_count__invalid_base_exit] = ANVIL__get__offset(workspace);

        // setup output
        ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__0, STANDARD__calculate_digit_count__count);

        // setup outputs
        ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__calculate_digit_count__count, STANDARD__calculate_digit_count__output__count);

        // setup function epilogue
        ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__calculate_digit_count__preserve__START, STANDARD__calculate_digit_count__preserve__END);

        // return to caller
    	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);
    }

	return;
}

// cell types
typedef enum STANDARD__cell_to_text {
	// preserve start
	STANDARD__cell_to_text__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__cell_to_text__digits_start = STANDARD__cell_to_text__preserve__START,
	STANDARD__cell_to_text__digits_end,
	STANDARD__cell_to_text__base,
	STANDARD__cell_to_text__value,
	STANDARD__cell_to_text__direction,
	STANDARD__cell_to_text__text_start,
	STANDARD__cell_to_text__text_end,
	STANDARD__cell_to_text__digit_count,
    STANDARD__cell_to_text__current_character,
    STANDARD__cell_to_text__remainder,
    STANDARD__cell_to_text__character_source_address,
    STANDARD__cell_to_text__character,

	// preserve end
	STANDARD__cell_to_text__preserve__END,

	// inputs
	STANDARD__cell_to_text__input__digits_start = ANVIL__srt__start__function_io,
	STANDARD__cell_to_text__input__digits_end,
	STANDARD__cell_to_text__input__base,
	STANDARD__cell_to_text__input__value,
	STANDARD__cell_to_text__input__direction,

	// outputs
	STANDARD__cell_to_text__output__text_start = ANVIL__srt__start__function_io,
	STANDARD__cell_to_text__output__text_end,
} STANDARD__cell_to_text;

// call function
void STANDARD__code__call__cell_to_text(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__digits_start, ANVIL__cell_ID input__digits_end, ANVIL__cell_ID input__base, ANVIL__cell_ID input__value, ANVIL__cell_ID input__direction, ANVIL__cell_ID output__text_start, ANVIL__cell_ID output__text_end) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__digits_start, STANDARD__cell_to_text__input__digits_start);
	ANVIL__code__cell_to_cell(workspace, flag, input__digits_end, STANDARD__cell_to_text__input__digits_end);
	ANVIL__code__cell_to_cell(workspace, flag, input__base, STANDARD__cell_to_text__input__base);
	ANVIL__code__cell_to_cell(workspace, flag, input__value, STANDARD__cell_to_text__input__value);
	ANVIL__code__cell_to_cell(workspace, flag, input__direction, STANDARD__cell_to_text__input__direction);

	// call function
	ANVIL__code__call__static(workspace, flag, (*standard_offsets).offsets[STANDARD__sot__cell_to_text__start]);

	// pass outputs
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__cell_to_text__output__text_start, output__text_start);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__cell_to_text__output__text_end, output__text_end);

	return;
}

// build function
void STANDARD__code__cell_to_text(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets) {
	// setup function offset
	(*standard_offsets).offsets[STANDARD__sot__cell_to_text__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__preserve__START, STANDARD__cell_to_text__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__input__digits_start, STANDARD__cell_to_text__digits_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__input__digits_end, STANDARD__cell_to_text__digits_end);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__input__base, STANDARD__cell_to_text__base);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__input__value, STANDARD__cell_to_text__value);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__input__direction, STANDARD__cell_to_text__direction);

	// calculate digit count
    STANDARD__code__call__calculate_digit_count(workspace, standard_offsets, ANVIL__sft__always_run, STANDARD__cell_to_text__base, STANDARD__cell_to_text__value, STANDARD__cell_to_text__digit_count);

    // allocate string
    ANVIL__code__request_memory(workspace, STANDARD__cell_to_text__digit_count, STANDARD__cell_to_text__text_start, STANDARD__cell_to_text__text_end);

    // check for direction
    // todo

    // lower to higher
    {
        // setup current
        ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__text_start, STANDARD__cell_to_text__current_character);

        // setup loop offset
        (*standard_offsets).offsets[STANDARD__sot__cell_to_text__loop_start__lower_to_higher] = ANVIL__get__offset(workspace);

        // check for current within range
        

        // get digit
        ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_modulous, STANDARD__cell_to_text__value, STANDARD__cell_to_text__base, ANVIL__unused_cell_ID, STANDARD__cell_to_text__remainder);
        ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__cell_to_text__digits_start, STANDARD__cell_to_text__remainder, ANVIL__unused_cell_ID, STANDARD__cell_to_text__character_source_address);
        ANVIL__code__address_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__character_source_address, ANVIL__srt__constant__ascii_character_byte_size, STANDARD__cell_to_text__character);

        // write digit
        ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__character, ANVIL__srt__constant__ascii_character_byte_size, STANDARD__cell_to_text__current_character);

        // next digit
        ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_division, STANDARD__cell_to_text__value, STANDARD__cell_to_text__base, ANVIL__unused_cell_ID, STANDARD__cell_to_text__value);
        ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__cell_to_text__current_character, ANVIL__srt__constant__ascii_character_byte_size, ANVIL__unused_cell_ID, STANDARD__cell_to_text__current_character);

        // jump to loop start

    }

	// setup outputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__text_start, STANDARD__cell_to_text__output__text_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__text_end, STANDARD__cell_to_text__output__text_end);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__cell_to_text__preserve__START, STANDARD__cell_to_text__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

#endif
