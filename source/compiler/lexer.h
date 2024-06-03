#ifndef DRAGON__compiler__lexer
#define DRAGON__compiler__lexer

/* Include */
#include "compiler_specifications.h"

/* Lexer */
// lexing types
typedef ANVIL__u8 ANVIL__lexling_type;
typedef ANVIL__address ANVIL__lexling_address;
typedef ANVIL__lexling_address ANVIL__lexling_start;
typedef ANVIL__lexling_address ANVIL__lexling_end;
typedef ANVIL__u64 ANVIL__lexling_index;
typedef ANVIL__u64 ANVIL__lexling_depth; // used for comments and strings

// lexling types
typedef enum ANVIL__lt {
    ANVIL__lt__invalid,
    ANVIL__lt__left_parenthesis,
    ANVIL__lt__right_parenthesis,
    ANVIL__lt__left_curly_bracket,
    ANVIL__lt__right_curly_bracket,
    ANVIL__lt__name,
    ANVIL__lt__colon,
    ANVIL__lt__at,
    ANVIL__lt__hashtag,
    ANVIL__lt__equals,
    ANVIL__lt__string_literal,
    ANVIL__lt__end_of_file,
    ANVIL__lt__COUNT,
} ANVIL__lt;

// lexling type
typedef struct ANVIL__lexling {
    ANVIL__lexling_type type;
    ANVIL__buffer value;
    ANVIL__character_location location;
} ANVIL__lexling;

// create custom lexling
ANVIL__lexling ANVIL__create__lexling(ANVIL__lexling_type type, ANVIL__buffer value, ANVIL__character_location location) {
    ANVIL__lexling output;

    // setup output
    output.type = type;
    output.value = value;
    output.location = location;

    return output;
}

// create null lexling
ANVIL__lexling ANVIL__create_null__lexling() {
    // return empty
    return ANVIL__create__lexling(ANVIL__lt__invalid, ANVIL__create_null__buffer(), ANVIL__create_null__character_location());
}

// lexlings
typedef struct ANVIL__lexlings {
    ANVIL__list data;
} ANVIL__lexlings;

// create custom lexlings
ANVIL__lexlings ANVIL__create__lexlings(ANVIL__list list) {
    ANVIL__lexlings output;

    // setup output
    output.data = list;

    return output;
}

// create null lexlings
ANVIL__lexlings ANVIL__create_null__lexlings() {
    // return empty
    return ANVIL__create__lexlings(ANVIL__create_null__list());
}

// close lexlings
void ANVIL__close__lexlings(ANVIL__lexlings lexlings) {
    // close buffer
    ANVIL__close__list(lexlings.data);

    return;
}

// append a lexling to the list
void ANVIL__append__lexling(ANVIL__list* list, ANVIL__lexling lexling, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__lexling), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__lexling*)ANVIL__calculate__list_current_address(list)) = lexling;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__lexling);

    return;
}

// append a lexlings to the list
void ANVIL__append__lexlings(ANVIL__list* list, ANVIL__lexlings lexlings, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__lexlings), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__lexlings*)ANVIL__calculate__list_current_address(list)) = lexlings;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__lexlings);

    return;
}

// read a lexling from an address
ANVIL__lexling ANVIL__read__lexling_from_current(ANVIL__current current) {
    // return struct
    return *(ANVIL__lexling*)current.start;
}

// check one character
ANVIL__bt ANVIL__calculate__valid_character_range(ANVIL__current current, ANVIL__character start, ANVIL__character end) {
    // perform calculation
    return (ANVIL__bt)((*(ANVIL__character*)(current.start) >= start) && (*(ANVIL__character*)(current.start) <= end));
}

// check one character for a name character
ANVIL__bt ANVIL__calculate__valid_name_character(ANVIL__current current) {
    return (ANVIL__calculate__valid_character_range(current, 'a', 'z') || ANVIL__calculate__valid_character_range(current, 'A', 'Z') || ANVIL__calculate__valid_character_range(current, '0', '9') || ANVIL__calculate__valid_character_range(current, '_', '_') || ANVIL__calculate__valid_character_range(current, '.', '.'));
}

// calculate character index
ANVIL__character_index ANVIL__calculate__character_index(ANVIL__buffer main_buffer, ANVIL__buffer current) {
    return (ANVIL__character_index)(current.start - main_buffer.start);
}

// lex a program
ANVIL__lexlings ANVIL__compile__lex(ANVIL__buffer user_code, ANVIL__file_index file_index, ANVIL__error* error) {
    ANVIL__lexlings output;
    ANVIL__current current;
    ANVIL__line_number current_line_number;
    ANVIL__lexling_start temp_start;
    ANVIL__lexling_end temp_end;

    // setup output
    output.data = ANVIL__open__list_with_error(sizeof(ANVIL__lexling) * 64, error);

    // check for error
    if (ANVIL__check__error_occured(error)) {
        // return empty
        return ANVIL__create_null__lexlings();
    }

    // setup current & locations
    current = user_code;
    current_line_number = 1;

    // lex program
    while (ANVIL__check__current_within_range(current)) {
        // skip comments and whitespace
        while (ANVIL__check__current_within_range(current) && (ANVIL__calculate__valid_character_range(current, 0, 32) || ANVIL__calculate__valid_character_range(current, '[', '['))) {
            // skip whitespace
            while (ANVIL__check__current_within_range(current) && ANVIL__calculate__valid_character_range(current, 0, 32)) {
                // check for new line
                if (ANVIL__calculate__valid_character_range(current, '\n', '\n') || ANVIL__calculate__valid_character_range(current, '\r', '\r')) {
                    // next line
                    current_line_number++;
                }

                // next character
                current.start += sizeof(ANVIL__character);
            }

            // skip comments
            if (ANVIL__check__current_within_range(current) && ANVIL__calculate__valid_character_range(current, '[', '[')) {
                ANVIL__lexling_depth depth = 1;

                // next character
                current.start += sizeof(ANVIL__character);

                // skip past characters
                while (ANVIL__check__current_within_range(current) && depth > 0) {
                    // check for new line
                    if (ANVIL__calculate__valid_character_range(current, '\n', '\n') || ANVIL__calculate__valid_character_range(current, '\r', '\r')) {
                        // next line
                        current_line_number++;
                    }
                    // check for opening comment
                    if (ANVIL__calculate__valid_character_range(current, '[', '[')) {
                        // increase depth
                        depth++;
                    }
                    // check for closing comment
                    if (ANVIL__calculate__valid_character_range(current, ']', ']')) {
                        // decrease depth
                        depth--;
                    }

                    // next character
                    current.start += sizeof(ANVIL__character);
                }

                // check for unfinished comment
                if (depth > 0) {
                    // set error
                    *error = ANVIL__open__error("Lexing Error: Comment ended with end of file instead of proper closing.", ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current)));

                    goto quit;
                }
            }
        }

        // check for out of range
        if (ANVIL__check__current_within_range(current) == ANVIL__bt__false) {
            goto quit;
        }

        // check for lexlings
        if (ANVIL__calculate__valid_character_range(current, '(', '(')) {
            // add lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__left_parenthesis, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, ')', ')')) {
            // add lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__right_parenthesis, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '{', '{')) {
            // add lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__left_curly_bracket, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '}', '}')) {
            // add lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__right_curly_bracket, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_name_character(current)) {
            // get lexling start and setup temp end
            temp_start = current.start;
            temp_end = temp_start - 1;

            // get lexling size
            while (ANVIL__check__current_within_range(current) && ANVIL__calculate__valid_name_character(current)) {
                // next character
                current.start += sizeof(ANVIL__character);
                temp_end += sizeof(ANVIL__character);
            }

            // record lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__name, ANVIL__create__buffer(temp_start, temp_end), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, ANVIL__create__buffer(temp_start, temp_end)))), error);
        } else if (ANVIL__calculate__valid_character_range(current, ':', ':')) {
            // add lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__colon, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '@', '@')) {
            // add lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__at, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '#', '#')) {
            // add lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__hashtag, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '=', '=')) {
            // add lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__equals, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '"', '"')) {
            ANVIL__buffer data;

            // get string start
            data.start = current.start;

            // advance current
            current.start += sizeof(ANVIL__character);

            // search for string end
            while (ANVIL__check__current_within_range(current) && ANVIL__calculate__valid_character_range(current, '\"', '\"') == ANVIL__bt__false) {
                // next character
                current.start += sizeof(ANVIL__character);
            }

            // check for end of file
            if (ANVIL__check__current_within_range(current) == ANVIL__bt__false) {
                // string ended abruptly
                *error = ANVIL__open__error("Lexical Error: String ended at the end of a file and not with a (\").", ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current)));

                goto quit;
            }

            // finish string data
            data.end = current.start;

            // append lexling
            ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__string_literal, data, ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, data))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        // no lexling found
        } else {
            // open error
            *error = ANVIL__open__error("Lexical Error: Invalid character.", ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current)));

            // quit
            goto quit;
        }

        // check for error
        if (ANVIL__check__error_occured(error)) {
            // return lexlings as they are
            goto quit;
        }
    }

    // quit
    quit:

    // append eof lexling
    ANVIL__append__lexling(&output.data, ANVIL__create__lexling(ANVIL__lt__end_of_file, ANVIL__open__buffer_from_string((u8*)"[EOF]", ANVIL__bt__false, ANVIL__bt__false), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

    return output;
}

// print lexlings
void ANVIL__debug__print_lexlings(ANVIL__lexlings lexlings) {
    ANVIL__current current;
    ANVIL__lexling temp;

    // setup current
    current = lexlings.data.buffer;

    // print header
    printf("Lexlings:\n");

    // print each lexling
    while (current.start < lexlings.data.buffer.start + lexlings.data.filled_index) {
        // get lexling
        temp = ANVIL__read__lexling_from_current(current);

        // next lexling
        current.start += sizeof(ANVIL__lexling);

        // print lexling type
        printf("\t%lu [ %lu, %lu ] [ file_index: %lu, line_number: %lu, character_index: %lu ] : ", (ANVIL__u64)temp.type, (ANVIL__u64)temp.value.start, (ANVIL__u64)temp.value.end, temp.location.file_index, (ANVIL__u64)temp.location.line_number, (ANVIL__u64)temp.location.character_index);
        fflush(stdout);

        // print lexling string
        ANVIL__print__buffer(temp.value);

        // print new line
        printf("\n");
    }

    return;
}

#endif
