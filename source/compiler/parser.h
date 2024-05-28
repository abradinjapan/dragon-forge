#ifndef DRAGON__compiler__parser
#define DRAGON__compiler__parser

/* Include */
#include "lexer.h"

/* Names */
// name struct
typedef struct ANVIL__name {
    ANVIL__lexling lexling;
} ANVIL__name;

// create name
ANVIL__name ANVIL__create__name(ANVIL__lexling lexling) {
    ANVIL__name output;

    output.lexling = lexling;

    return output;
}

// create null name
ANVIL__name ANVIL__create_null__name() {
    return ANVIL__create__name(ANVIL__create_null__lexling());
}

// append name
void ANVIL__append__name(ANVIL__list* list, ANVIL__name data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__name), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__name*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__name);

    return;
}

/* Parser */
// parsing argument type
typedef enum ANVIL__pat {
    ANVIL__pat__invalid,
    ANVIL__pat__variable,
    ANVIL__pat__variable__input,
    ANVIL__pat__variable__output,
    ANVIL__pat__variable__body,
    ANVIL__pat__variable__predefined,
    ANVIL__pat__offset,
    ANVIL__pat__flag,
    ANVIL__pat__flag__user_defined,
    ANVIL__pat__flag__predefined,
    ANVIL__pat__literal__boolean,
    ANVIL__pat__literal__binary,
    ANVIL__pat__literal__integer,
    ANVIL__pat__literal__hexadecimal,
    ANVIL__pat__literal__string,

    // count
    ANVIL__pat__COUNT,
} ANVIL__pat;

// parsling argument
typedef struct ANVIL__parsling_argument {
    ANVIL__pat type;
    ANVIL__name text;
    ANVIL__cell_integer_value value;
} ANVIL__parsling_argument;

// create a custom argument
ANVIL__parsling_argument ANVIL__create__parsling_argument(ANVIL__pat type, ANVIL__name text, ANVIL__cell_integer_value value) {
    ANVIL__parsling_argument output;

    // setup output
    output.type = type;
    output.text = text;
    output.value = value;

    return output;
}

// check if arguments have the same text
ANVIL__bt ANVIL__check__parsling_arguments_have_same_text(ANVIL__parsling_argument a, ANVIL__parsling_argument b) {
    return ANVIL__calculate__buffer_contents_equal(a.text.lexling.value, b.text.lexling.value);
}

// find parsling argument index in list
ANVIL__argument_index ANVIL__find__parsling_argument_index__by_name(ANVIL__list list, ANVIL__parsling_argument argument) {
    ANVIL__argument_index output = 0;

    // setup current
    ANVIL__current current_argument = ANVIL__calculate__current_from_list_filled_index(&list);

    // find argument
    while (ANVIL__check__current_within_range(current_argument)) {
        // check arg
        if (ANVIL__check__parsling_arguments_have_same_text(*(ANVIL__parsling_argument*)current_argument.start, argument)) {
            // found
            return output;
        }

        // next arg
        output++;
        current_argument.start += sizeof(ANVIL__parsling_argument);
    }

    // not found
    return output;
}

// translate string to boolean
ANVIL__bt ANVIL__translate__string_to_boolean(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    // check possible values
    if (ANVIL__calculate__buffer_contents_equal(string, ANVIL__open__buffer_from_string((u8*)"dragon.boolean.false", ANVIL__bt__false, ANVIL__bt__false)) == ANVIL__bt__true) {
        *value = (ANVIL__cell_integer_value)(ANVIL__bt__false);

        return ANVIL__bt__true;
    }
    if (ANVIL__calculate__buffer_contents_equal(string, ANVIL__open__buffer_from_string((u8*)"dragon.boolean.true", ANVIL__bt__false, ANVIL__bt__false)) == ANVIL__bt__true) {
        *value = (ANVIL__cell_integer_value)(ANVIL__bt__true);

        return ANVIL__bt__true;
    }

    return ANVIL__bt__false;
}

// translate string to binary
ANVIL__bt ANVIL__translate__string_to_binary(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    ANVIL__buffer prefix = ANVIL__open__buffer_from_string((u8*)"dragon.binary.", ANVIL__bt__false, ANVIL__bt__false);
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
ANVIL__bt ANVIL__translate__string_to_integer(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    ANVIL__buffer prefix = ANVIL__open__buffer_from_string((u8*)"dragon.integer.", ANVIL__bt__false, ANVIL__bt__false);
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
ANVIL__cell_integer_value ANVIL__translate__character_to_hexadecimal(ANVIL__character character, ANVIL__bt* invalid_character) {
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
ANVIL__bt ANVIL__translate__string_to_hexedecimal(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    ANVIL__buffer prefix = ANVIL__open__buffer_from_string((u8*)"dragon.hexadecimal.", ANVIL__bt__false, ANVIL__bt__false);
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
        hex_digit = ANVIL__translate__character_to_hexadecimal(*(ANVIL__character*)current.start, &invalid_character);
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

// setup null argument
ANVIL__parsling_argument ANVIL__create_null__parsling_argument() {
    return ANVIL__create__parsling_argument(ANVIL__pat__invalid, ANVIL__create_null__name(), 0);
}

// statement type
typedef enum ANVIL__stt {
    ANVIL__stt__invalid,
    ANVIL__stt__abstraction_call,
    ANVIL__stt__abstraction_header,
    ANVIL__stt__offset,

    // count
    ANVIL__stt__COUNT,
} ANVIL__stt;

// one statement
typedef struct ANVIL__parsling_statement {
    ANVIL__stt type;

    // name
    ANVIL__parsling_argument name;

    // abstraction call data
    ANVIL__list inputs; // ANVIL__parsling_argument
    ANVIL__list outputs; // ANVIL__parsling_argument

    // metadata
    ANVIL__input_count input_count;
    ANVIL__output_count output_count;
} ANVIL__parsling_statement;

// create a custom statement
ANVIL__parsling_statement ANVIL__create__parsling_statement(ANVIL__stt type, ANVIL__parsling_argument name, ANVIL__list inputs, ANVIL__list outputs, ANVIL__input_count input_count, ANVIL__output_count output_count) {
    ANVIL__parsling_statement output;

    // setup output
    output.type = type;
    output.name = name;
    output.inputs = inputs;
    output.outputs = outputs;
    output.input_count = input_count;
    output.output_count = output_count;

    return output;
}

// create a null statement
ANVIL__parsling_statement ANVIL__create_null__parsling_statement() {
    // return empty
    return ANVIL__create__parsling_statement(ANVIL__stt__invalid, ANVIL__create_null__parsling_argument(), ANVIL__create_null__list(), ANVIL__create_null__list(), 0, 0);
}

// one abstraction
typedef struct ANVIL__parsling_abstraction {
    ANVIL__parsling_statement header;
    ANVIL__list statements; // ANVIL__parsling_statement
} ANVIL__parsling_abstraction;

// create a custom abstraction
ANVIL__parsling_abstraction ANVIL__create__parsling_abstraction(ANVIL__parsling_statement header, ANVIL__list statements) {
    ANVIL__parsling_abstraction output;

    // setup output
    output.header = header;
    output.statements = statements;

    return output;
}

// create a null abstraction
ANVIL__parsling_abstraction ANVIL__create_null__parsling_abstraction() {
    // return empty
    return ANVIL__create__parsling_abstraction(ANVIL__create_null__parsling_statement(), ANVIL__create_null__list());
}

// one program
typedef struct ANVIL__parsling_program {
    ANVIL__list abstractions; // ANVIL__parsling_abstraction
} ANVIL__parsling_program;

// create a custom program
ANVIL__parsling_program ANVIL__create__parsling_program(ANVIL__list abstractions) {
    ANVIL__parsling_program output;

    // setup output
    output.abstractions = abstractions;

    return output;
}

// create a null program
ANVIL__parsling_program ANVIL__create_null__parsling_program() {
    // return empty
    return ANVIL__create__parsling_program(ANVIL__create_null__list());
}

// append parsling argument
void ANVIL__append__parsling_argument(ANVIL__list* list, ANVIL__parsling_argument data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_argument), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_argument*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_argument);

    return;
}

// append parsling statement
void ANVIL__append__parsling_statement(ANVIL__list* list, ANVIL__parsling_statement data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_statement), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_statement*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_statement);

    return;
}

// append parsling abstraction
void ANVIL__append__parsling_abstraction(ANVIL__list* list, ANVIL__parsling_abstraction data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_abstraction), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_abstraction*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_abstraction);

    return;
}

// append parsling program
void ANVIL__append__parsling_program(ANVIL__list* list, ANVIL__parsling_program data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_program), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_program*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_program);

    return;
}

// close statement
void ANVIL__close__parsling_statement(ANVIL__parsling_statement statement) {
    // close io
    if (ANVIL__check__empty_list(statement.inputs) == ANVIL__bt__false) {
        ANVIL__close__list(statement.inputs);
    }
    if (ANVIL__check__empty_list(statement.outputs) == ANVIL__bt__false) {
        ANVIL__close__list(statement.outputs);
    }

    return;
}

// close statements
void ANVIL__close__parsling_statements(ANVIL__list list) {
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&list);

    // clean up each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // clean up statement
        ANVIL__close__parsling_statement(*(ANVIL__parsling_statement*)current_statement.start);

        // next statement
        current_statement.start += sizeof(ANVIL__parsling_statement);
    }

    // clean up statements buffer
    ANVIL__close__list(list);

    return;
}

// close abstraction
void ANVIL__close__parsling_abstraction(ANVIL__parsling_abstraction abstraction) {
    // close header
    ANVIL__close__parsling_statement(abstraction.header);
    
    // close statements
    if (ANVIL__check__empty_list(abstraction.statements) == ANVIL__bt__false) {
        ANVIL__close__parsling_statements(abstraction.statements);
    }

    return;
}

// close parsling program
void ANVIL__close__parsling_program(ANVIL__parsling_program program) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&(program.abstractions));

    // free each abstraction
    while (ANVIL__check__current_within_range(current)) {
        // free abstraction
        ANVIL__close__parsling_abstraction(*(ANVIL__parsling_abstraction*)current.start);

        // next abstraction
        current.start += sizeof(ANVIL__parsling_abstraction);
    }

    // free the abstraction list
    ANVIL__close__list(program.abstractions);

    return;
}

// create a name from a lexing
ANVIL__name ANVIL__create__name_from_lexling_current(ANVIL__current lexling_current) {
    ANVIL__name output;

    // setup
    output.lexling = ANVIL__read__lexling_from_current(lexling_current);

    return output;
}

// advance the current buffer by N number of lexlings
void ANVIL__advance__lexling_current(ANVIL__current* current, ANVIL__lexling_index lexling_count) {
    // advance the pointer
    (*current).start += sizeof(ANVIL__lexling) * lexling_count;

    return;
}

// revert lexling current by N number of lexlings
void ANVIL__revert__lexling_current(ANVIL__current* current, ANVIL__lexling_index lexling_count) {
    (*current).start -= sizeof(ANVIL__lexling) * lexling_count;

    return;
}

// parse arguments
ANVIL__list ANVIL__parse__arguments(ANVIL__current* current, ANVIL__io_count* count, ANVIL__bt is_header, ANVIL__error* error) {
    ANVIL__list output;
    ANVIL__cell_integer_value value = 0;

    // init count
    *count = 0;

    // open names list
    output = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 8, error);

    // check for error
    if (ANVIL__check__error_occured(error)) {
        return output;
    }
    
    // check for opening parenthesis
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__left_parenthesis) {
        // next lexling
        ANVIL__advance__lexling_current(current, 1);
    // not found, error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: Arguments is missing opening parenthesis.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    // get arguments
    while (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type != ANVIL__lt__right_parenthesis) {
        ANVIL__parsling_argument argument = ANVIL__create_null__parsling_argument();

        // check type
        // is variable / literal
        if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
            // determine value / is variable
            if (ANVIL__translate__string_to_boolean(ANVIL__read__lexling_from_current(*current).value, &value)) {
                argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__boolean, ANVIL__create__name_from_lexling_current(*current), value);
            } else if (ANVIL__translate__string_to_binary(ANVIL__read__lexling_from_current(*current).value, &value)) {
                argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__binary, ANVIL__create__name_from_lexling_current(*current), value);
            } else if (ANVIL__translate__string_to_integer(ANVIL__read__lexling_from_current(*current).value, &value)) {
                argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__integer, ANVIL__create__name_from_lexling_current(*current), value);
            } else if (ANVIL__translate__string_to_hexedecimal(ANVIL__read__lexling_from_current(*current).value, &value)) {
                argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__hexadecimal, ANVIL__create__name_from_lexling_current(*current), value);
            } else {
                // must be a variable
                argument = ANVIL__create__parsling_argument(ANVIL__pat__variable, ANVIL__create__name_from_lexling_current(*current), 0);
            }
        // offset
        } else if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__at) {
            // advance current past at
            ANVIL__advance__lexling_current(current, 1);

            // if correct type
            if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
                // get name
                argument = ANVIL__create__parsling_argument(ANVIL__pat__offset, ANVIL__create__name_from_lexling_current(*current), 0);
            // error
            } else {
                *error = ANVIL__open__error("Parse Error: Offset is missing name.", ANVIL__read__lexling_from_current(*current).location);

                return output;
            }
        // flag
        } else if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__hashtag) {
            // advance current past hashtag
            ANVIL__advance__lexling_current(current, 1);

            // if correct type
            if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
                // get name
                argument = ANVIL__create__parsling_argument(ANVIL__pat__flag, ANVIL__create__name_from_lexling_current(*current), 0);
            // error
            } else {
                *error = ANVIL__open__error("Parse Error: Flag is missing name.", ANVIL__read__lexling_from_current(*current).location);

                return output;
            }
        // string literal
        } else if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__string_literal) {
            // get argument
            argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__string, ANVIL__create__name_from_lexling_current(*current), 0);
        // error
        } else {
            *error = ANVIL__open__error("Parse Error: Unrecognized argument type.", ANVIL__read__lexling_from_current(*current).location);

            return output;
        }

        // check argument for variable only
        if (is_header == ANVIL__bt__true && argument.type != ANVIL__pat__variable) {
            // set error
            *error = ANVIL__open__error("Parse Error: A non-variable argument was detected in a header.", ANVIL__read__lexling_from_current(*current).location);

            return output;
        }

        // append argument
        ANVIL__append__parsling_argument(&output, argument, error);
        
        // increment count
        *count = *count + 1;

        // check for error
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // next current
        ANVIL__advance__lexling_current(current, 1);
    }

    // check for closing parenthesis
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__right_parenthesis) {
        // next lexling
        ANVIL__advance__lexling_current(current, 1);
    // not found, error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: Arguments is missing closing parenthesis.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    return output;
}

// parse one statement
ANVIL__parsling_statement ANVIL__parse__statement(ANVIL__current* current, ANVIL__bt is_header, ANVIL__error* error) {
    ANVIL__parsling_statement output = ANVIL__create_null__parsling_statement();
    ANVIL__input_count input_count;
    ANVIL__output_count output_count;

    // check for offset
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__at) {
        // advance current
        ANVIL__advance__lexling_current(current, 1);

        // check for offset name
        if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
            // set name
            output.name = ANVIL__create__parsling_argument(ANVIL__pat__offset, ANVIL__create__name_from_lexling_current(*current), 0);

            // advance current
            ANVIL__advance__lexling_current(current, 1);
        // invalid syntax
        } else {
            // set error
            *error = ANVIL__open__error("Parse Error: Offset statement name is an invalid lexling.", ANVIL__read__lexling_from_current(*current).location);

            return output;
        }

        // null initialize unused data
        output.inputs = ANVIL__create_null__list();
        output.outputs = ANVIL__create_null__list();

        // setup type
        output.type = ANVIL__stt__offset;
    // is an abstraction call
    } else if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
        // get name
        output.name = ANVIL__create__parsling_argument(ANVIL__pat__offset, ANVIL__create__name_from_lexling_current(*current), 0);

        // advance current
        ANVIL__advance__lexling_current(current, 1);

        // get inputs
        output.inputs = ANVIL__parse__arguments(current, &input_count, is_header, error);
        output.input_count = input_count;

        // check for error
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // get outputs
        output.outputs = ANVIL__parse__arguments(current, &output_count, is_header, error);
        output.output_count = output_count;

        // check for error
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // set type
        if (is_header == ANVIL__bt__true) {
            output.type = ANVIL__stt__abstraction_header;
        } else {
            output.type = ANVIL__stt__abstraction_call;
        }
    // error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: Unrecognized statement type.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    return output;
}

// parse an abstraction
ANVIL__parsling_abstraction ANVIL__parse__abstraction(ANVIL__current* current, ANVIL__error* error) {
    ANVIL__parsling_abstraction output = ANVIL__create_null__parsling_abstraction();

    // check for eof
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__end_of_file) {
        return output;
    }

    // parse header
    output.header = ANVIL__parse__statement(current, ANVIL__bt__true, error);
    
    // check for error
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // check for equals sign
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__equals) {
        // no saving data necessary, next lexling
        ANVIL__advance__lexling_current(current, 1);
    // error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: An abstraction definition has an equals sign missing.", ANVIL__read__lexling_from_current(*current).location);

        // quit
        return output;
    }

    // check for scope opener
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__left_curly_bracket) {
        // advance current
        ANVIL__advance__lexling_current(current, 1);
    // scope opener not found, error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: Scope is missing left curly bracket.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    // parse statements
    // open statements list
    output.statements = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_statement) * 16, error);
    
    // get statements
    while (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type != ANVIL__lt__right_curly_bracket) {
        // parse statement
        ANVIL__parsling_statement statement = ANVIL__parse__statement(current, ANVIL__bt__false, error);

        // add statement
        ANVIL__append__parsling_statement(&output.statements, statement, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
    }

    // check for scope closer
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__right_curly_bracket) {
        // advance current
        ANVIL__advance__lexling_current(current, 1);
    // scope opener not found, error
    } else {
        // setup error
        *error = ANVIL__open__error("Parse Error: Scope is missing right curly bracket.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    return output;
}

// parse a file (program)
ANVIL__parsling_program ANVIL__parse__program(ANVIL__lexlings lexlings, ANVIL__error* error) {
    ANVIL__parsling_program output;
    ANVIL__parsling_abstraction temp;
    ANVIL__current current;

    // setup current
    current = ANVIL__calculate__current_from_list_filled_index(&lexlings.data);

    // open the abstraction list
    output.abstractions = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_abstraction) * 64, error);

    // check for memory error
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // parse abstractions
    while (ANVIL__check__current_within_range(current)) {
        // if end of file
        if (ANVIL__read__lexling_from_current(current).type == ANVIL__lt__end_of_file) {
            // finished parsing
            goto quit;
        }
        
        // parse abstraction
        temp = ANVIL__parse__abstraction(&current, error);

        // append abstraction
        ANVIL__append__parsling_abstraction(&(output.abstractions), temp, error);

        // check for error
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            goto quit;
        }
    }

    // quit
    quit:

    return output;
}

ANVIL__buffer ANVIL__convert__parsed_argument_type_to_string_buffer(ANVIL__pat argument_type) {
    return ANVIL__open__buffer_from_string((u8*)(ANVIL__global__argument_type_name_strings[argument_type]), ANVIL__bt__false, ANVIL__bt__false);
}

// print argument
void ANVIL__print__parsling_argument(ANVIL__parsling_argument argument) {
    // print type
    printf("[");
    ANVIL__print__buffer(ANVIL__convert__parsed_argument_type_to_string_buffer(argument.type));
    printf("]");

    // print data
    if (argument.type == ANVIL__pat__variable || ANVIL__pat__variable__input || ANVIL__pat__variable__output || ANVIL__pat__variable__body || ANVIL__pat__variable__predefined || ANVIL__pat__offset || ANVIL__pat__flag || ANVIL__pat__literal__string) {
        ANVIL__print__buffer(argument.text.lexling.value);
    } else if (argument.type == ANVIL__pat__literal__boolean || ANVIL__pat__literal__binary || ANVIL__pat__literal__hexadecimal) {
        ANVIL__print__buffer(argument.text.lexling.value);
        printf("[%lu]", argument.value);
    } else if (argument.type == ANVIL__pat__literal__integer) {
        ANVIL__print__buffer(argument.text.lexling.value);
        printf("[%lu, %li]", argument.value, argument.value);
    }

    return;
}

// print arguments
void ANVIL__print__parsling_arguments(ANVIL__list* arguments) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(arguments);

    // print opener
    printf("(");

    // for each name
    while (ANVIL__check__current_within_range(current)) {
        // print separator
        if (current.start != (*arguments).buffer.start) {
            // print
            printf(" ");
        }

        // print argument
        ANVIL__print__parsling_argument(*(ANVIL__parsling_argument*)current.start);

        // next buffer
        current.start += sizeof(ANVIL__parsling_argument);
    }

    // print closer
    printf(")");

    return;
}

// print statement
void ANVIL__print__parsed_statement(ANVIL__parsling_statement statement) {
    // print statement
    if (statement.type == ANVIL__stt__offset) {
        // print offset information
        printf("@");
        ANVIL__print__buffer(statement.name.text.lexling.value);
    } else if (statement.type == ANVIL__stt__abstraction_call || statement.type == ANVIL__stt__abstraction_header) {
        // print statement
        ANVIL__print__buffer(statement.name.text.lexling.value);

        // print inputs
        ANVIL__print__parsling_arguments(&statement.inputs);

        // print outputs
        ANVIL__print__parsling_arguments(&statement.outputs);
    }

    return;
}

// print an abstraction
void ANVIL__print__parsed_abstraction(ANVIL__parsling_abstraction abstraction) {
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&abstraction.statements);

    // print header
    printf("\tAbstraction: ");
    if (abstraction.header.type != ANVIL__stt__invalid) {
        ANVIL__print__parsed_statement(abstraction.header);
    }

    // new line for statements
    printf("\n");

    // print statements
    if (ANVIL__check__empty_list(abstraction.statements) == ANVIL__bt__false) {
        // print each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // print formatting
            printf("\t\t");

            // print statement
            ANVIL__print__parsed_statement(*(ANVIL__parsling_statement*)current_statement.start);

            // print new line
            printf("\n");

            // advance current
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    return;
}

// print a program
void ANVIL__print__parsed_program(ANVIL__parsling_program program) {
    ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&(program.abstractions));
    
    // print header
    printf("Parsed Program:\n");

    // print each function
    while (ANVIL__check__current_within_range(current_abstraction)) {
        // print function
        ANVIL__print__parsed_abstraction(*(ANVIL__parsling_abstraction*)current_abstraction.start);

        // advance current
        current_abstraction.start += sizeof(ANVIL__parsling_abstraction);
    }

    return;
}

#endif
