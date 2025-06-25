#ifndef DRAGON__compiler
#define DRAGON__compiler

/* Include */
// compiler subparts
#include "lexer.h"
#include "parser.h"
#include "accounter.h"
#include "generator.h"
#include "./standard/standard.h"

/* Compilation Unit */
// one compiled object across multiple stages
typedef struct COMPILER__compilation_unit {
    ANVIL__buffer user_codes;
    ANVIL__list standard_files;
    COMPILER__lexlings lexlings;
    COMPILER__parsling_program parslings;
    COMPILER__accountling_program accountlings;
    COMPILER__pst stages_completed;
} COMPILER__compilation_unit;

// close a compilation unit
void COMPILER__close__compilation_unit(COMPILER__compilation_unit unit, COMPILER__error* error) {
    // check for standard include files
    if (ANVIL__check__empty_list(unit.standard_files) == ANVIL__bt__false) {
        // close list
        ANVIL__close__list(unit.standard_files);
    }

    // close lexling buffer
    if (unit.stages_completed > COMPILER__pst__invalid) {
        COMPILER__close__lexlings(unit.lexlings);
    }

    // close parsling buffer
    if (unit.stages_completed > COMPILER__pst__lexing) {
        COMPILER__close__parsling_program(unit.parslings);
    }

    // close accountling data
    if (unit.stages_completed > COMPILER__pst__parsing) {
        COMPILER__close__accountling_program(unit.accountlings, error);
    }

    return;
}

/* Generate Debug */
// append a string
void COMPILER__generate__debug_information__string(ANVIL__list* json, char* string_data, COMPILER__error* error) {
    // append string
    ANVIL__list__append__buffer_data(json, ANVIL__open__buffer_from_string((u8*)string_data, ANVIL__bt__false, ANVIL__bt__false), &(*error).memory_error_occured);

    return;
}

// append tab depth
void COMPILER__generate__debug_information__tabs(ANVIL__list* json, ANVIL__tab_count tab_depth, COMPILER__error* error) {
    // append tabs
    for (ANVIL__tab_count index = 0; index < tab_depth; index++) {
        // append tab
        COMPILER__generate__debug_information__string(json, "\t", error);
        if (COMPILER__check__error_occured(error)) {
            return;
        }
    }

    return;
}

// write string to list
void COMPILER__generate__debug_information__buffer_as_string(ANVIL__list* json, ANVIL__buffer string_data, COMPILER__error* error) {
    // append starting quote
    COMPILER__generate__debug_information__string(json, "\"", error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // append string data quote
    ANVIL__list__append__buffer_data(json, string_data, &(*error).memory_error_occured);
    if (COMPILER__check__error_occured(error)) {
        return;
    }
    
    // append ending quote
    COMPILER__generate__debug_information__string(json, "\"", error);

    return;
}

// write a value as an integer to a list
void COMPILER__generate__debug_information__integer_literal(ANVIL__list* json, ANVIL__u64 value, COMPILER__error* error) {
    // generate the integer
    ANVIL__buffer string_value = ANVIL__cast__integer_to_unsigned_base_10(value);

    // write value
    ANVIL__list__append__buffer_data(json, string_value, &(*error).memory_error_occured);

    // deallocate string
    ANVIL__close__buffer(string_value);

    return;
}

// generate and write variable header to list
void COMPILER__generate__debug_information__json_variable_header(ANVIL__list* json, ANVIL__buffer variable_name, COMPILER__error* error) {
    // print string
    COMPILER__generate__debug_information__buffer_as_string(json, variable_name, error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // print colon
    COMPILER__generate__debug_information__string(json, ": ", error);

    return;
}

// generate an integer value with a variable header
void COMPILER__generate__debug_information__integer_variable(ANVIL__list* json, const char* variable_name, ANVIL__u64 value, COMPILER__error* error) {
    // append header
    COMPILER__generate__debug_information__json_variable_header(json, ANVIL__open__buffer_from_string((u8*)variable_name, ANVIL__bt__false, ANVIL__bt__false), error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // append value
    COMPILER__generate__debug_information__integer_literal(json, value, error);

    return;
}

// generate one lexling's debug information
void COMPILER__generate__debug_information__lexling(ANVIL__list* json, COMPILER__lexling lexling, ANVIL__bt is_last, ANVIL__tab_count tabs, COMPILER__error* error) {
    // print tabs
    COMPILER__generate__debug_information__tabs(json, tabs, error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // print lexling
    // print opener
    ANVIL__list__append__buffer_data(json, ANVIL__open__buffer_from_string((u8*)"{ ", ANVIL__bt__false, ANVIL__bt__false), &(*error).memory_error_occured);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // setup separator
    ANVIL__buffer separator = ANVIL__open__buffer_from_string((u8*)", ", ANVIL__bt__false, ANVIL__bt__false);

    // print values & separators
    COMPILER__generate__debug_information__integer_variable(json, "type", (ANVIL__u64)lexling.type, error);
    ANVIL__list__append__buffer_data(json, separator, &(*error).memory_error_occured);
    COMPILER__generate__debug_information__integer_variable(json, "value.start", (ANVIL__u64)lexling.value.start, error);
    ANVIL__list__append__buffer_data(json, separator, &(*error).memory_error_occured);
    COMPILER__generate__debug_information__integer_variable(json, "value.end", (ANVIL__u64)lexling.value.end, error);
    ANVIL__list__append__buffer_data(json, separator, &(*error).memory_error_occured);
    COMPILER__generate__debug_information__integer_variable(json, "location.file_index", (ANVIL__u64)lexling.location.file_index, error);
    ANVIL__list__append__buffer_data(json, separator, &(*error).memory_error_occured);
    COMPILER__generate__debug_information__integer_variable(json, "location.line_number", (ANVIL__u64)lexling.location.line_number, error);
    ANVIL__list__append__buffer_data(json, separator, &(*error).memory_error_occured);
    COMPILER__generate__debug_information__integer_variable(json, "location.character_index", (ANVIL__u64)lexling.location.character_index, error);

    // print close
    if (is_last == ANVIL__bt__false) {
        ANVIL__list__append__buffer_data(json, ANVIL__open__buffer_from_string((u8*)" },\n", ANVIL__bt__false, ANVIL__bt__false), &(*error).memory_error_occured);
    } else {
        ANVIL__list__append__buffer_data(json, ANVIL__open__buffer_from_string((u8*)" }\n", ANVIL__bt__false, ANVIL__bt__false), &(*error).memory_error_occured);
    }

    return;
}

// generate all lexlings information
void COMPILER__generate__debug_information__lexlings(ANVIL__list* json, COMPILER__lexlings lexlings, ANVIL__tab_count tabs, COMPILER__error* error) {
    // print header tabs
    COMPILER__generate__debug_information__tabs(json, tabs, error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // print lexlings name
    COMPILER__generate__debug_information__buffer_as_string(json, ANVIL__open__buffer_from_string((u8*)"lexlings", ANVIL__bt__false, ANVIL__bt__false), error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // print value setting
    ANVIL__list__append__buffer_data(json, ANVIL__open__buffer_from_string((u8*)": [\n", ANVIL__bt__false, ANVIL__bt__false), &(*error).memory_error_occured);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // for each lexling
    for (COMPILER__lexling_index index = 0; index < lexlings.data.count; index++) {
        // print lexling
        COMPILER__generate__debug_information__lexling(json, COMPILER__get__lexling_by_index(lexlings.data, index), index == lexlings.data.count - 1, tabs + 1, error);
        if (COMPILER__check__error_occured(error)) {
            return;
        }
    }

    // print closer
    // print footer tabs
    COMPILER__generate__debug_information__tabs(json, tabs, error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // print end
    ANVIL__list__append__buffer_data(json, ANVIL__open__buffer_from_string((u8*)"]\n", ANVIL__bt__false, ANVIL__bt__false), &(*error).memory_error_occured);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    return;
}

// serialize debug information to json
ANVIL__buffer COMPILER__generate__debug_information(COMPILER__compilation_unit unit, ANVIL__tab_count tabs, COMPILER__error* error) {
    // setup list
    ANVIL__list json_data = COMPILER__open__list_with_error(sizeof(ANVIL__character) * 65536, error);
    ANVIL__list* json = &json_data;
    if (COMPILER__check__error_occured(error)) {
        return ANVIL__create_null__buffer();
    }

    // setup master opening bracket
    ANVIL__list__append__buffer_data(json, ANVIL__open__buffer_from_string((u8*)"{\n", ANVIL__bt__false, ANVIL__bt__false), &(*error).memory_error_occured);
    if (COMPILER__check__error_occured(error)) {
        goto bufferify;
    }

    // serialize json for lexer
    COMPILER__generate__debug_information__lexlings(json, unit.lexlings, tabs + 1, error);

    // setup master closing bracket
    ANVIL__list__append__buffer_data(json, ANVIL__open__buffer_from_string((u8*)"}\n", ANVIL__bt__false, ANVIL__bt__false), &(*error).memory_error_occured);
    if (COMPILER__check__error_occured(error)) {
        goto bufferify;
    }

    // turn list into new buffer and return
    bufferify:
    ANVIL__buffer json_buffer_temp = ANVIL__calculate__list_current_buffer(json);
    ANVIL__buffer output = ANVIL__open__buffer(ANVIL__calculate__buffer_length(json_buffer_temp));
    ANVIL__copy__buffer(json_buffer_temp, output, &(*error).memory_error_occured);

    // deallocate old list
    ANVIL__close__list(json_data);

    // return buffer
    return output;
}

/* Compile */
// compile a program
void COMPILER__compile__files(ANVIL__buffer user_codes, ANVIL__bt include_standard_files, ANVIL__bt generate_kickstarter, ANVIL__bt print_debug, ANVIL__bt generate_debug, ANVIL__buffer* final_program, ANVIL__buffer* debug_information, COMPILER__error* error) {
    COMPILER__compilation_unit compilation_unit;

    // setup blank error
    *error = COMPILER__create_null__error();

    // check for empty buffer
    if (ANVIL__check__empty_buffer(user_codes)) {
        // set error
        *error = COMPILER__open__error("Compilation Error: No source files were passed.", COMPILER__create_null__character_location());

        goto quit;
    }

    // setup compilation unit
    compilation_unit.user_codes = user_codes;
    compilation_unit.stages_completed = COMPILER__pst__invalid;

    // setup preincluded files buffer
    ANVIL__buffer standard_files = ANVIL__create_null__buffer();

    // setup standard files
    if (include_standard_files == ANVIL__bt__true) {
        // create list
        compilation_unit.standard_files = ANVIL__open__list(sizeof(ANVIL__buffer) * 16, &(*error).memory_error_occured);
        if (COMPILER__check__error_occured(error)) {
            goto quit;
        }

        // append files
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_print_dragon, __source_compiler_standard_print_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_cast_dragon, __source_compiler_standard_cast_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_buffer_dragon, __source_compiler_standard_buffer_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_current_dragon, __source_compiler_standard_current_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_list_dragon, __source_compiler_standard_list_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_context_dragon, __source_compiler_standard_context_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_check_dragon, __source_compiler_standard_check_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_error_dragon, __source_compiler_standard_error_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_json_dragon, __source_compiler_standard_json_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_time_dragon, __source_compiler_standard_time_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_forge_dragon, __source_compiler_standard_forge_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_anvil_dragon, __source_compiler_standard_anvil_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_compile_dragon, __source_compiler_standard_compile_dragon_len), &(*error).memory_error_occured);
        ANVIL__list__append__buffer(&compilation_unit.standard_files, STANDARD__bufferify__any_file(__source_compiler_standard_just_run_dragon, __source_compiler_standard_just_run_dragon_len), &(*error).memory_error_occured);

        // create content buffer
        standard_files = ANVIL__calculate__list_current_buffer(&compilation_unit.standard_files);
    // standard files are not included
    } else {
        compilation_unit.standard_files = ANVIL__create_null__list();
    }

    // print compilation message
    if (print_debug) {
        printf("Compiling Files.\n");
    }

    // lex file
    compilation_unit.lexlings = COMPILER__compile__lex(standard_files, include_standard_files, compilation_unit.user_codes, error);
    compilation_unit.stages_completed = COMPILER__pst__lexing;

    // print lexlings
    if (print_debug) {
        COMPILER__debug__print_lexlings(compilation_unit.lexlings);
    }
    // check for error
    if (COMPILER__check__error_occured(error)) {
        goto quit;
    }

    // parse
    compilation_unit.parslings = COMPILER__parse__program(compilation_unit.lexlings, error);
    compilation_unit.stages_completed = COMPILER__pst__parsing;

    // print parslings
    if (print_debug) {
        COMPILER__print__parsed_program(compilation_unit.parslings);
    }
    // check for error
    if (COMPILER__check__error_occured(error)) {
        goto quit;
    }

    // account
    compilation_unit.accountlings = COMPILER__account__program(compilation_unit.parslings, error);
    compilation_unit.stages_completed = COMPILER__pst__accounting;

    // check for errors
    if (COMPILER__check__error_occured(error)) {
        goto quit;
    }

    // print accountlings
    if (print_debug) {
        COMPILER__print__accountling_program(compilation_unit.accountlings);
    }

    // inform user of code generation
    if (print_debug) {
        printf("Generating code...\n");
    }

    // generate program
    COMPILER__generate__program(final_program, compilation_unit.accountlings, generate_kickstarter, 65536, error);

    // quit label
    quit:

    // generate debug information
    if (generate_debug) {
        // setup temp error
        COMPILER__error json_generation_error = COMPILER__create_null__error();

        // generate debug json
        *debug_information = COMPILER__generate__debug_information(compilation_unit, 0, &json_generation_error);

        // if error occured with json serialization, return failure buffer
        if (COMPILER__check__error_occured(&json_generation_error)) {
            // check for allocated buffer
            if (ANVIL__check__empty_buffer(*debug_information) == ANVIL__bt__false) {
                // close buffer
                ANVIL__close__buffer(*debug_information);
            }

            // setup dummy string
            *debug_information = ANVIL__open__buffer_from_string((u8*)"\"message\": \"Debug information failed to serialize. Oops...\"", ANVIL__bt__true, ANVIL__bt__false);
        }
    // otherwise, generate blank debug info
    } else {
        // setup no information
        *debug_information = ANVIL__create_null__buffer();
    }

    // clean up
    // setup clean up error
    COMPILER__error closing_error = COMPILER__create_null__error();
    COMPILER__close__compilation_unit(compilation_unit, &closing_error);

    return;
}

#endif
