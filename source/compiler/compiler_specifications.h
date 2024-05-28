#ifndef DRAGON__compiler__specifications
#define DRAGON__compiler__specifications

/* Include */
// anvil instructions
#include "../anvil_coder.h"

/* Define */
// general compiler types
typedef ANVIL__u64 ANVIL__argument_index;
typedef ANVIL__u64 ANVIL__io_count;
typedef ANVIL__io_count ANVIL__input_count;
typedef ANVIL__io_count ANVIL__output_count;
typedef ANVIL__u64 ANVIL__accountling_index;
typedef ANVIL__accountling_index ANVIL__variable_index;
typedef ANVIL__accountling_index ANVIL__call_index;
typedef ANVIL__accountling_index ANVIL__offset_index;
typedef ANVIL__accountling_index ANVIL__flag_index;
typedef ANVIL__accountling_index ANVIL__statement_index;
typedef ANVIL__accountling_index ANVIL__header_index;
typedef ANVIL__accountling_index ANVIL__string_index;
typedef ANVIL__u64 ANVIL__abstraction_index;

// strings
char* ANVIL__global__predefined_cell_name_strings[] = {
    "dragon.error_code",
    "dragon.constant.character_byte_size",
    "dragon.constant.character_bit_size",
    "dragon.constant.bits_in_byte",
    "dragon.constant.cell_byte_size",
    "dragon.constant.cell_bit_size",
    "dragon.constant.true",
    "dragon.constant.false",
    "dragon.constant.0",
    "dragon.constant.1",
    "dragon.constant.2",
    "dragon.constant.4",
    "dragon.constant.8",
    "dragon.constant.16",
    "dragon.constant.24",
    "dragon.constant.32",
    "dragon.constant.40",
    "dragon.constant.48",
    "dragon.constant.56",
    "dragon.constant.64",
    "dragon.constant.program_input.start",
    "dragon.constant.program_input.end",
    "dragon.constant.program_output.start",
    "dragon.constant.program_output.end",
    "dragon.stack.start",
    "dragon.stack.current",
    "dragon.stack.end",
};
char* ANVIL__global__predefined_flag_name_strings[] = {
    "dragon.always",
    "dragon.never",
    "dragon.temporary",
};
char* ANVIL__global__argument_type_name_strings[] = {
    "invalid",
    "variable",
    "input_variable",
    "output_variable",
    "body_variable",
    "predefined_variable",
    "offset",
    "flag",
    "flag_user_defined",
    "flag_predefined",
    "boolean",
    "binary",
    "integer",
    "hexadecimal",
    "string",
};
char* ANVIL__global__accountling_call_type_name_strings[] = {
    "dragon.set",
    "dragon.print.integer.signed",
    "dragon.print.integer.unsigned",
    "dragon.print.character",
    "dragon.print.buffer_as_string",
    "dragon.print.binary",
    "dragon.io.cell_to_address",
    "dragon.io.address_to_cell",
    "dragon.io.file_to_buffer",
    "dragon.io.buffer_to_file",
    "dragon.copy",
    "dragon.copy.buffer",
    "dragon.memory.request",
    "dragon.memory.return",
    "dragon.buffer.calculate_length",
    "dragon.cast.cell_to_unsigned_integer_string",
    "dragon.integer.add",
    "dragon.integer.subtract",
    "dragon.integer.multiply",
    "dragon.integer.divide",
    "dragon.integer.modulous",
    "dragon.integer.within_range",
    "dragon.binary.or",
    "dragon.binary.invert",
    "dragon.binary.and",
    "dragon.binary.xor",
    "dragon.binary.shift_higher",
    "dragon.binary.shift_lower",
    "dragon.binary.overwrite",
    "dragon.flag.get",
    "dragon.flag.set",
    "dragon.flag.invert",
    "dragon.flag.or",
    "dragon.flag.and",
    "dragon.flag.xor",
    "dragon.jump",
    "dragon.open.context",
    "dragon.compile",
    "dragon.run",
    "dragon.reset.error_code",
    "dragon.get.program_inputs",
    "dragon.set.program_outputs",
    "dragon.context_buffer.set_inputs",
    "dragon.context_buffer.get_outputs",
};

// program stage type
typedef enum ANVIL__pst {
    // stages
    ANVIL__pst__invalid, // invalid
    ANVIL__pst__lexing, // lexing files
    ANVIL__pst__parsing, // parsing files
    ANVIL__pst__accounting, // semantics for all files
    ANVIL__pst__generating, // building program
    ANVIL__pst__running, // running program

    // count
    ANVIL__pst__COUNT,
} ANVIL__pst;

/* Current */
// define
typedef ANVIL__buffer ANVIL__current;

// check if a current buffer is still valid
ANVIL__bt ANVIL__check__current_within_range(ANVIL__current current) {
    return (current.start <= current.end);
}

// calculate a current buffer from a list // NOTE: buffer cannot be null or calculation fails!
ANVIL__current ANVIL__calculate__current_from_list_filled_index(ANVIL__list* list) {
    return ANVIL__create__buffer((*list).buffer.start, (*list).buffer.start + (*list).filled_index - 1);
}

// check for a character at a current
ANVIL__bt ANVIL__check__character_range_at_current(ANVIL__current current, ANVIL__character range_start, ANVIL__character range_end) {
    return ((*(ANVIL__character*)current.start) >= range_start) && ((*(ANVIL__character*)current.start) <= range_end);
}

// calculate the amounnt of items in one list (assumes all items are same size!)
ANVIL__list_filled_index ANVIL__calculate__list_content_count(ANVIL__list list, size_t item_size) {
    return list.filled_index / item_size;
}

/* Translation */
// calculate the amount of digits in a decimal number
ANVIL__length ANVIL__calculate__decimals_digit_count_in_number(ANVIL__u64 number) {
    ANVIL__length output = 0;

    // check for zero
    if (number == 0) {
        return 1;
    }

    // while there are digits left
    while (number > 0) {
        // divide by ten
        number /= 10;

        // increment digit count
        output++;
    }

    return output;
}

// create buffer string from number
ANVIL__buffer ANVIL__translate__integer_value_to_string(ANVIL__u64 number) {
    ANVIL__buffer output;
    ANVIL__length digit_count;

    // count digits
    digit_count = ANVIL__calculate__decimals_digit_count_in_number(number);
    
    // allocate output
    output = ANVIL__open__buffer(sizeof(ANVIL__character) * digit_count);

    // if allocation succeded
    if (ANVIL__check__empty_buffer(output) == ANVIL__bt__false) {
        // calculate characters
        for (ANVIL__u64 i = digit_count; i > 0; i--) {
            // create character
            ((ANVIL__character*)output.start)[i - 1] = '0' + (number % 10);

            // next character
            number /= 10;
        }
    }

    return output;
}

/* Character Locations */
// parsing character location
typedef struct ANVIL__character_location {
    ANVIL__file_index file_index;
    ANVIL__line_number line_number;
    ANVIL__character_index character_index;
} ANVIL__character_location;

// create custom character marker location
ANVIL__character_location ANVIL__create__character_location(ANVIL__file_index file_index, ANVIL__line_number line_number, ANVIL__character_index character_index) {
    ANVIL__character_location output;

    output.file_index = file_index;
    output.line_number = line_number;
    output.character_index = character_index;

    return output;
}

// create null character location
ANVIL__character_location ANVIL__create_null__character_location() {
    return ANVIL__create__character_location(-1, -1, -1);
}

/* Errors */
// error information
typedef struct ANVIL__error {
    ANVIL__bt occured;
    ANVIL__buffer message;
    ANVIL__character_location location;
    ANVIL__bt memory_error_occured;
} ANVIL__error;

// create custom error
ANVIL__error ANVIL__create__error(ANVIL__bt occured, ANVIL__buffer message, ANVIL__character_location location, ANVIL__bt memory_error_occured) {
    ANVIL__error output;

    // setup output
    output.occured = occured;
    output.message = message;
    output.location = location;
    output.memory_error_occured = memory_error_occured;

    return output;
}

// create null error
ANVIL__error ANVIL__create_null__error() {
    // return empty
    return ANVIL__create__error(ANVIL__bt__false, ANVIL__create_null__buffer(), ANVIL__create_null__character_location(), ANVIL__bt__false);
}

// open a specific error
ANVIL__error ANVIL__open__error(const char* message, ANVIL__character_location location) {
    return ANVIL__create__error(ANVIL__bt__true, ANVIL__open__buffer_from_string((u8*)message, ANVIL__bt__true, ANVIL__bt__false), location, ANVIL__bt__false);
}

// open a generic memory allocation error
ANVIL__error ANVIL__open__internal_memory_error() {
    return ANVIL__create__error(ANVIL__bt__true, ANVIL__open__buffer_from_string((u8*)"Internal memory error.", ANVIL__bt__true, ANVIL__bt__false), ANVIL__create_null__character_location(), ANVIL__bt__true);
}

// create an error report in json
ANVIL__buffer ANVIL__serialize__error_json(ANVIL__error error, ANVIL__bt* error_occured) {
    ANVIL__buffer output;
    ANVIL__list json;
    ANVIL__buffer temp_buffer;

    // initialize json string builder
    json = ANVIL__open__list(sizeof(ANVIL__character) * 2048, error_occured);

    // generate json
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"{\n\t\"message\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    ANVIL__list__append__buffer_data(&json, error.message, error_occured);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\",\n\t\"file_index\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    temp_buffer = ANVIL__translate__integer_value_to_string(error.location.file_index);
    ANVIL__list__append__buffer_data(&json, temp_buffer, error_occured);
    ANVIL__close__buffer(temp_buffer);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\",\n\t\"line_number\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    temp_buffer = ANVIL__translate__integer_value_to_string(error.location.line_number);
    ANVIL__list__append__buffer_data(&json, temp_buffer, error_occured);
    ANVIL__close__buffer(temp_buffer);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\",\n\t\"character_index\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    temp_buffer = ANVIL__translate__integer_value_to_string(error.location.character_index);
    ANVIL__list__append__buffer_data(&json, temp_buffer, error_occured);
    ANVIL__close__buffer(temp_buffer);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\"\n}\n", ANVIL__bt__false, ANVIL__bt__false), error_occured);

    // create buffer from list
    output = ANVIL__list__open_buffer_from_list(&json, error_occured);

    // clean up list
    ANVIL__close__list(json);

    return output;
}

// check to see if an error occured
ANVIL__bt ANVIL__check__error_occured(ANVIL__error* error) {
    // check for memory error
    if ((*error).memory_error_occured == ANVIL__bt__true) {
        // set error
        *error = ANVIL__open__internal_memory_error();
    }

    return (*error).occured;
}

// close an error
void ANVIL__close__error(ANVIL__error error) {
    // clean up buffers
    ANVIL__close__buffer(error.message);
    
    return;
}

/* List Functions With Errors */
// open a list but the error is a compiler error
ANVIL__list ANVIL__open__list_with_error(ANVIL__list_increase list_increase, ANVIL__error* error) {
    // open with error
    return ANVIL__open__list(list_increase, &((*error).memory_error_occured));
}

// append a buffer but the error is compiler
void ANVIL__append__buffer_with_error(ANVIL__list* list, ANVIL__buffer buffer, ANVIL__error* error) {
    ANVIL__list__append__buffer(list, buffer, &((*error).memory_error_occured));

    return;
}

#endif
