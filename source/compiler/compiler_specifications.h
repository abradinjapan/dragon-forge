#ifndef DRAGON__compiler__specifications
#define DRAGON__compiler__specifications

/* Include */
// anvil instructions
#include "../anvil_coder.h"

/* Define */
// general compiler types
typedef ANVIL__u64 COMPILER__argument_index;
typedef ANVIL__u64 COMPILER__io_count;
typedef COMPILER__io_count COMPILER__input_count;
typedef COMPILER__io_count COMPILER__output_count;
typedef ANVIL__u64 COMPILER__accountling_index;
typedef COMPILER__accountling_index COMPILER__variable_index;
typedef COMPILER__accountling_index COMPILER__call_index;
typedef COMPILER__accountling_index COMPILER__offset_index;
typedef COMPILER__accountling_index COMPILER__flag_index;
typedef COMPILER__accountling_index COMPILER__statement_index;
typedef COMPILER__accountling_index COMPILER__header_index;
typedef COMPILER__accountling_index COMPILER__string_index;
typedef ANVIL__u64 COMPILER__abstraction_index;

// master namespace
#define COMPILER__define__master_namespace "dragon"

// general argument types
typedef enum COMPILER__gat {
    COMPILER__gat__invalid,

    // variables
    COMPILER__gat__variable,
    COMPILER__gat__variable__input,
    COMPILER__gat__variable__output,
    COMPILER__gat__variable__body,
    COMPILER__gat__variable__predefined,

    // offsets
    COMPILER__gat__offset,

    // flags
    COMPILER__gat__flag,
    COMPILER__gat__flag__user_defined,
    COMPILER__gat__flag__predefined,

    // literals
    COMPILER__gat__literal__boolean,
    COMPILER__gat__literal__binary,
    COMPILER__gat__literal__integer,
    COMPILER__gat__literal__hexadecimal,

    // strings
    COMPILER__gat__literal__string,

    // types
    COMPILER__gat__type,
    COMPILER__gat__type__structure,
    COMPILER__gat__type__from_variable, // used for when types are being extracted from variables
    COMPILER__gat__type__predefined,

    // count
    COMPILER__gat__COUNT,
} COMPILER__gat;

// strings
char* COMPILER__global__predefined_cell_name_strings[] = {
    COMPILER__define__master_namespace ".error_code",
    COMPILER__define__master_namespace ".constant.character_byte_size",
    COMPILER__define__master_namespace ".constant.character_bit_size",
    COMPILER__define__master_namespace ".constant.bits_in_byte",
    COMPILER__define__master_namespace ".constant.cell_byte_size",
    COMPILER__define__master_namespace ".constant.cell_bit_size",
    COMPILER__define__master_namespace ".constant.true",
    COMPILER__define__master_namespace ".constant.false",
    COMPILER__define__master_namespace ".constant.0",
    COMPILER__define__master_namespace ".constant.1",
    COMPILER__define__master_namespace ".constant.2",
    COMPILER__define__master_namespace ".constant.4",
    COMPILER__define__master_namespace ".constant.8",
    COMPILER__define__master_namespace ".constant.16",
    COMPILER__define__master_namespace ".constant.24",
    COMPILER__define__master_namespace ".constant.32",
    COMPILER__define__master_namespace ".constant.40",
    COMPILER__define__master_namespace ".constant.48",
    COMPILER__define__master_namespace ".constant.56",
    COMPILER__define__master_namespace ".constant.64",
    COMPILER__define__master_namespace ".constant.program_input.start",
    COMPILER__define__master_namespace ".constant.program_input.end",
    COMPILER__define__master_namespace ".constant.program_output.start",
    COMPILER__define__master_namespace ".constant.program_output.end",
    COMPILER__define__master_namespace ".stack.start",
    COMPILER__define__master_namespace ".stack.current",
    COMPILER__define__master_namespace ".stack.end",
};
char* COMPILER__global__predefined_flag_name_strings[] = {
    COMPILER__define__master_namespace ".always",
    COMPILER__define__master_namespace ".never",
    COMPILER__define__master_namespace ".temporary",
};
char* COMPILER__global__argument_type_name_strings[] = {
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
    "type",
    "structure_type",
    "type_from_variable",
    "predefined_type",
};
char* COMPILER__global__accountling_call_type_name_strings[] = {
    COMPILER__define__master_namespace ".set",
    COMPILER__define__master_namespace ".print.integer.signed",
    COMPILER__define__master_namespace ".print.integer.unsigned",
    COMPILER__define__master_namespace ".print.character",
    COMPILER__define__master_namespace ".print.buffer_as_string",
    COMPILER__define__master_namespace ".print.binary",
    COMPILER__define__master_namespace ".io.cell_to_address",
    COMPILER__define__master_namespace ".io.address_to_cell",
    COMPILER__define__master_namespace ".io.file_to_buffer",
    COMPILER__define__master_namespace ".io.buffer_to_file",
    COMPILER__define__master_namespace ".copy",
    COMPILER__define__master_namespace ".copy.buffer",
    COMPILER__define__master_namespace ".memory.request",
    COMPILER__define__master_namespace ".memory.return",
    COMPILER__define__master_namespace ".buffer.calculate_length",
    COMPILER__define__master_namespace ".cast.cell_to_unsigned_integer_string",
    COMPILER__define__master_namespace ".integer.add",
    COMPILER__define__master_namespace ".integer.subtract",
    COMPILER__define__master_namespace ".integer.multiply",
    COMPILER__define__master_namespace ".integer.divide",
    COMPILER__define__master_namespace ".integer.modulous",
    COMPILER__define__master_namespace ".integer.within_range",
    COMPILER__define__master_namespace ".binary.or",
    COMPILER__define__master_namespace ".binary.invert",
    COMPILER__define__master_namespace ".binary.and",
    COMPILER__define__master_namespace ".binary.xor",
    COMPILER__define__master_namespace ".binary.shift_higher",
    COMPILER__define__master_namespace ".binary.shift_lower",
    COMPILER__define__master_namespace ".binary.overwrite",
    COMPILER__define__master_namespace ".flag.get",
    COMPILER__define__master_namespace ".flag.set",
    COMPILER__define__master_namespace ".flag.invert",
    COMPILER__define__master_namespace ".flag.or",
    COMPILER__define__master_namespace ".flag.and",
    COMPILER__define__master_namespace ".flag.xor",
    COMPILER__define__master_namespace ".jump",
    COMPILER__define__master_namespace ".open.context",
    COMPILER__define__master_namespace ".compile",
    COMPILER__define__master_namespace ".run",
    COMPILER__define__master_namespace ".reset.error_code",
    COMPILER__define__master_namespace ".get.program_inputs",
    COMPILER__define__master_namespace ".set.program_outputs",
    COMPILER__define__master_namespace ".context_buffer.set_inputs",
    COMPILER__define__master_namespace ".context_buffer.get_outputs",
};

// program stage type
typedef enum COMPILER__pst {
    // stages
    COMPILER__pst__invalid, // invalid
    COMPILER__pst__lexing, // lexing files
    COMPILER__pst__parsing, // parsing files
    COMPILER__pst__accounting, // semantics for all files
    COMPILER__pst__generating, // building program
    COMPILER__pst__running, // running program

    // count
    COMPILER__pst__COUNT,
} COMPILER__pst;

/* Translation */
// calculate the amount of digits in a decimal number
ANVIL__length COMPILER__calculate__decimals_digit_count_in_number(ANVIL__u64 number) {
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
ANVIL__buffer COMPILER__translate__integer_value_to_string(ANVIL__u64 number) {
    ANVIL__buffer output;
    ANVIL__length digit_count;

    // count digits
    digit_count = COMPILER__calculate__decimals_digit_count_in_number(number);
    
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
typedef struct COMPILER__character_location {
    ANVIL__file_index file_index;
    ANVIL__line_number line_number;
    ANVIL__character_index character_index;
} COMPILER__character_location;

// create custom character marker location
COMPILER__character_location COMPILER__create__character_location(ANVIL__file_index file_index, ANVIL__line_number line_number, ANVIL__character_index character_index) {
    COMPILER__character_location output;

    output.file_index = file_index;
    output.line_number = line_number;
    output.character_index = character_index;

    return output;
}

// create null character location
COMPILER__character_location COMPILER__create_null__character_location() {
    return COMPILER__create__character_location(-1, -1, -1);
}

/* Errors */
// error information
typedef struct COMPILER__error {
    ANVIL__bt occured;
    ANVIL__buffer message;
    COMPILER__character_location location;
    ANVIL__bt memory_error_occured;
} COMPILER__error;

// create custom error
COMPILER__error COMPILER__create__error(ANVIL__bt occured, ANVIL__buffer message, COMPILER__character_location location, ANVIL__bt memory_error_occured) {
    COMPILER__error output;

    // setup output
    output.occured = occured;
    output.message = message;
    output.location = location;
    output.memory_error_occured = memory_error_occured;

    return output;
}

// create null error
COMPILER__error COMPILER__create_null__error() {
    // return empty
    return COMPILER__create__error(ANVIL__bt__false, ANVIL__create_null__buffer(), COMPILER__create_null__character_location(), ANVIL__bt__false);
}

// open a specific error
COMPILER__error COMPILER__open__error(const char* message, COMPILER__character_location location) {
    return COMPILER__create__error(ANVIL__bt__true, ANVIL__open__buffer_from_string((u8*)message, ANVIL__bt__true, ANVIL__bt__false), location, ANVIL__bt__false);
}

// open a generic memory allocation error
COMPILER__error COMPILER__open__internal_memory_error() {
    return COMPILER__create__error(ANVIL__bt__true, ANVIL__open__buffer_from_string((u8*)"Internal memory error.", ANVIL__bt__true, ANVIL__bt__false), COMPILER__create_null__character_location(), ANVIL__bt__true);
}

// create an error report in json
ANVIL__buffer COMPILER__serialize__error_json(COMPILER__error error, ANVIL__bt* error_occured) {
    ANVIL__buffer output;
    ANVIL__list json;
    ANVIL__buffer temp_buffer;

    // initialize json string builder
    json = ANVIL__open__list(sizeof(ANVIL__character) * 2048, error_occured);

    // generate json
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"{\n\t\"message\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    ANVIL__list__append__buffer_data(&json, error.message, error_occured);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\",\n\t\"file_index\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    temp_buffer = COMPILER__translate__integer_value_to_string(error.location.file_index);
    ANVIL__list__append__buffer_data(&json, temp_buffer, error_occured);
    ANVIL__close__buffer(temp_buffer);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\",\n\t\"line_number\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    temp_buffer = COMPILER__translate__integer_value_to_string(error.location.line_number);
    ANVIL__list__append__buffer_data(&json, temp_buffer, error_occured);
    ANVIL__close__buffer(temp_buffer);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\",\n\t\"character_index\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    temp_buffer = COMPILER__translate__integer_value_to_string(error.location.character_index);
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
ANVIL__bt COMPILER__check__error_occured(COMPILER__error* error) {
    // check for memory error
    if ((*error).memory_error_occured == ANVIL__bt__true) {
        // set error
        *error = COMPILER__open__internal_memory_error();
    }

    return (*error).occured;
}

// close an error
void COMPILER__close__error(COMPILER__error error) {
    // clean up buffers
    ANVIL__close__buffer(error.message);
    
    return;
}

/* List Functions With Errors */
// open a list but the error is a compiler error
ANVIL__list COMPILER__open__list_with_error(ANVIL__list_increase list_increase, COMPILER__error* error) {
    // open with error
    return ANVIL__open__list(list_increase, &((*error).memory_error_occured));
}

// append a buffer but the error is compiler
void COMPILER__append__buffer_with_error(ANVIL__list* list, ANVIL__buffer buffer, COMPILER__error* error) {
    ANVIL__list__append__buffer(list, buffer, &((*error).memory_error_occured));

    return;
}

/* Conversion */
// translate string to boolean
ANVIL__bt COMPILER__translate__string_to_boolean(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    // check possible values
    if (ANVIL__calculate__buffer_contents_equal(string, ANVIL__open__buffer_from_string((u8*)COMPILER__define__master_namespace ".boolean.false", ANVIL__bt__false, ANVIL__bt__false)) == ANVIL__bt__true) {
        *value = (ANVIL__cell_integer_value)(ANVIL__bt__false);

        return ANVIL__bt__true;
    }
    if (ANVIL__calculate__buffer_contents_equal(string, ANVIL__open__buffer_from_string((u8*)COMPILER__define__master_namespace ".boolean.true", ANVIL__bt__false, ANVIL__bt__false)) == ANVIL__bt__true) {
        *value = (ANVIL__cell_integer_value)(ANVIL__bt__true);

        return ANVIL__bt__true;
    }

    return ANVIL__bt__false;
}

// translate string to binary
ANVIL__bt COMPILER__translate__string_to_binary(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    ANVIL__buffer prefix = ANVIL__open__buffer_from_string((u8*)COMPILER__define__master_namespace ".binary.", ANVIL__bt__false, ANVIL__bt__false);
    ANVIL__buffer current;
    ANVIL__u64 character_count_limit = sizeof(ANVIL__u64) * ANVIL__define__bits_in_byte;
    ANVIL__u64 character_count = 0;

    // check for prefix
    if (ANVIL__calculate__buffer_starts_with_buffer(string, prefix) == ANVIL__bt__false) {
        // not a binary literal
        *value = ANVIL__define__null_address;

        return ANVIL__bt__false;
    }

    // setup current
    current = ANVIL__create__buffer(string.start + ANVIL__calculate__buffer_length(prefix), string.end);

    // pre check for all valid characters
    while (ANVIL__check__current_within_range(current)) {
        // check character
        if ((ANVIL__check__character_range_at_current(current, '0', '1') || ANVIL__check__character_range_at_current(current, '_', '_')) == ANVIL__bt__false) {
            // not a binary literal
            *value = ANVIL__define__null_address;

            return ANVIL__bt__false;
        }

        // count binary character
        if (ANVIL__check__character_range_at_current(current, '0', '1') == ANVIL__bt__true) {
            character_count++;
        }

        // advance current
        current.start += sizeof(ANVIL__character);
    }

    // check for sane character limit
    if (character_count > character_count_limit) {
        // binary literal to large, conversion failed
        *value = ANVIL__define__null_address;

        return ANVIL__bt__false;
    }

    // setup bit index
    ANVIL__bit_count bit_index = 0;

    // reset current
    current = ANVIL__create__buffer(string.start + ANVIL__calculate__buffer_length(prefix), string.end);

    // convert binary string to binary number
    while (ANVIL__check__current_within_range(current)) {
        if (ANVIL__check__character_range_at_current(current, '_', '_') == ANVIL__bt__false) {
            // append value
            *value += (((*(ANVIL__character*)current.start) - '0') << bit_index);

            // next bit index
            bit_index++;
        }

        // next character
        current.start += sizeof(ANVIL__character);
    }

    return ANVIL__bt__true;
}

// translate string to integer
ANVIL__bt COMPILER__translate__string_to_integer(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    ANVIL__buffer prefix = ANVIL__open__buffer_from_string((u8*)COMPILER__define__master_namespace ".integer.", ANVIL__bt__false, ANVIL__bt__false);
    ANVIL__buffer suffix;
    ANVIL__u64 digit = 0;

    // check for prefix
    if (ANVIL__calculate__buffer_starts_with_buffer(string, prefix) == ANVIL__bt__false) {
        // not an integer literal
        *value = ANVIL__define__null_address;

        return ANVIL__bt__false;
    }

    // create suffix
    suffix = ANVIL__create__buffer(string.start + ANVIL__calculate__buffer_length(prefix), string.end);

    // translate number
    // if number is negative
    if (*(ANVIL__character*)suffix.start == (ANVIL__character)'n') {
        // for each character
        for (ANVIL__character_index i = ANVIL__calculate__buffer_length(suffix); i > 1; i--) {
            // check for valid character
            if (((((ANVIL__character*)suffix.start)[i - 1] >= '0' && ((ANVIL__character*)suffix.start)[i - 1] <= '9') || ((ANVIL__character*)suffix.start)[i - 1] == '_') == ANVIL__bt__false) {
                // invalid character
                *value = ANVIL__define__null_address;

                return ANVIL__bt__false;
            }

            // if calculable character
            if (((ANVIL__character*)suffix.start)[i - 1] != '_') {
                // add value
                *value += ANVIL__calculate__exponent(10, digit) * (((ANVIL__character*)suffix.start)[i - 1] - '0');

                // next digit power
                digit++;
            }
        }

        // turn number negative using twos compliment
        *value = ~(*value);
        *value = (*value) + 1;
    // if number is positive
    } else {
        // for each character
        for (ANVIL__character_index i = ANVIL__calculate__buffer_length(suffix); i > 0; i--) {
            // check for valid character
            if (((((ANVIL__character*)suffix.start)[i - 1] >= '0' && ((ANVIL__character*)suffix.start)[i - 1] <= '9') || ((ANVIL__character*)suffix.start)[i - 1] == '_') == ANVIL__bt__false) {
                // invalid character
                *value = ANVIL__define__null_address;

                return ANVIL__bt__false;
            }

            // if calculable character
            if (((ANVIL__character*)suffix.start)[i - 1] != '_') {
                // add value
                *value += ANVIL__calculate__exponent(10, digit) * (((ANVIL__character*)suffix.start)[i - 1] - '0');

                // next digit power
                digit++;
            }
        }
    }

    return ANVIL__bt__true;
}

// translate character to hexadecimal
ANVIL__cell_integer_value COMPILER__translate__character_to_hexadecimal(ANVIL__character character, ANVIL__bt* invalid_character) {
    // set character as valid
    *invalid_character = ANVIL__bt__false;

    // translate character
    if (character >= '0' && character <= '9') {
        return character - '0';
    } else if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    } else if (character >= 'A' && character <= 'F') {
        return character - 'A' + 10;
    } else {
        // invalid character
        *invalid_character = ANVIL__bt__true;
    }

    return ANVIL__define__null_address;
}

// translate string to hexedecimal
ANVIL__bt COMPILER__translate__string_to_hexedecimal(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    ANVIL__buffer prefix = ANVIL__open__buffer_from_string((u8*)COMPILER__define__master_namespace ".hexadecimal.", ANVIL__bt__false, ANVIL__bt__false);
    ANVIL__buffer suffix;
    ANVIL__buffer current;
    ANVIL__bt invalid_character;
    ANVIL__cell_integer_value hex_digit;

    // check for prefix
    if (ANVIL__calculate__buffer_starts_with_buffer(string, prefix) == ANVIL__bt__false) {
        // not a hexadecimal literal
        *value = ANVIL__define__null_address;

        return ANVIL__bt__false;
    }

    // create suffix
    suffix = ANVIL__create__buffer(string.start + ANVIL__calculate__buffer_length(prefix), string.end);

    // create current
    current = suffix;

    // setup value
    *value = ANVIL__define__null_address;

    // translate number
    while (ANVIL__check__current_within_range(current)) {
        // check separator
        if (*(ANVIL__character*)current.start == '_') {
            // skip
            current.start += sizeof(ANVIL__character);

            continue;
        }
        
        // check digit
        hex_digit = COMPILER__translate__character_to_hexadecimal(*(ANVIL__character*)current.start, &invalid_character);
        if (invalid_character == ANVIL__bt__true) {
            // invalid digit, invalid hex string
            *value = ANVIL__define__null_address;

            return ANVIL__bt__false;
        }

        // append digit
        *value = (*value) << 4;
        *value = (*value) & (~15);
        *value = (*value) + hex_digit;

        // next character
        current.start += sizeof(ANVIL__character);
    }

    return ANVIL__bt__true;
}

ANVIL__buffer COMPILER__convert__general_argument_type_to_string_buffer(COMPILER__gat argument_type) {
    return ANVIL__open__buffer_from_string((u8*)(COMPILER__global__argument_type_name_strings[argument_type]), ANVIL__bt__false, ANVIL__bt__false);
}

#endif
