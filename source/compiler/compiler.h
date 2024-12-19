#ifndef DRAGON__compiler
#define DRAGON__compiler

/* Include */
// compiler subparts
#include "lexer.h"
#include "parser.h"
#include "accounter.h"
#include "generator.h"

/* Compile */
// one compiled object across multiple stages
typedef struct COMPILER__compilation_unit {
    ANVIL__buffer user_codes;
    ANVIL__list lexling_buffers; // COMPILER__lexlings
    COMPILER__parsling_program parslings;
    COMPILER__accountling_program accountlings;
    COMPILER__pst stages_completed;
} COMPILER__compilation_unit;

// close a compilation unit
void COMPILER__close__compilation_unit(COMPILER__compilation_unit unit) {
    // close lexling buffers
    if (unit.stages_completed > COMPILER__pst__invalid) {
        // setup current
        ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&unit.lexling_buffers);

        // close each buffer
        while (ANVIL__check__current_within_range(current)) {
            // close parsling buffer
            COMPILER__close__lexlings(*(COMPILER__lexlings*)current.start);

            // next program
            current.start += sizeof(COMPILER__lexlings);
        }

        // close parslings buffer
        ANVIL__close__list(unit.lexling_buffers);
    }

    // close parsling buffers
    if (unit.stages_completed > COMPILER__pst__lexing) {
        COMPILER__close__parsling_program(unit.parslings);
    }

    // close accountling data
    if (unit.stages_completed > COMPILER__pst__parsing) {
        COMPILER__close__accountling_program(unit.accountlings);
    }

    return;
}

// compile a program
void COMPILER__compile__files(ANVIL__buffer user_codes, ANVIL__bt generate_kickstarter, ANVIL__bt print_debug, ANVIL__buffer* final_program, COMPILER__error* error) {
    COMPILER__compilation_unit compilation_unit;
    ANVIL__current current;
    ANVIL__file_index current_file_index = 0;

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
    compilation_unit.lexling_buffers = COMPILER__open__list_with_error(sizeof(COMPILER__lexlings) * 5, error);
    compilation_unit.stages_completed = COMPILER__pst__invalid;

    // check for error
    if (COMPILER__check__error_occured(error)) {
        goto quit;
    }

    // setup current
    current = compilation_unit.user_codes;

    // print compilation message
    if (print_debug) {
        printf("Compiling Files.\n");
    }

    // lex each file
    while (ANVIL__check__current_within_range(current)) {
        // get file
        ANVIL__buffer user_code = *(ANVIL__buffer*)current.start;

        // inform user of compilation start
        if (print_debug) {
            printf("Compiling User Code:\n");
            ANVIL__print__buffer(user_code);
            printf("\n");
        }

        // lex file
        COMPILER__lexlings lexlings = COMPILER__compile__lex(user_code, current_file_index, error);

        // append lexlings
        COMPILER__append__lexlings(&compilation_unit.lexling_buffers, lexlings, error);

        // print lexlings
        if (print_debug) {
            COMPILER__debug__print_lexlings(lexlings);
        }

        // mark as done (unfinished input acceptable)
        if (compilation_unit.stages_completed < COMPILER__pst__lexing) {
            compilation_unit.stages_completed = COMPILER__pst__lexing;
        }

        // check for error
        if (COMPILER__check__error_occured(error)) {
            goto quit;
        }

        // advance current
        current.start += sizeof(ANVIL__buffer);
    }

    // parse
    compilation_unit.parslings = COMPILER__parse__program(compilation_unit.lexling_buffers, error);

    // print parslings
    if (print_debug) {
        COMPILER__print__parsed_program(compilation_unit.parslings);
    }

    // mark as done (unfinished input acceptable)
    if (compilation_unit.stages_completed < COMPILER__pst__parsing) {
        compilation_unit.stages_completed = COMPILER__pst__parsing;
    }

    // check for error
    if (COMPILER__check__error_occured(error)) {
        goto quit;
    }

    // account
    compilation_unit.accountlings = COMPILER__account__program(compilation_unit.parslings, error);

    // check for errors
    if (COMPILER__check__error_occured(error)) {
        goto quit;
    }

    // print accountlings
    if (print_debug) {
        COMPILER__print__accountling_program(compilation_unit.accountlings);
    }

    // mark as done
    compilation_unit.stages_completed = COMPILER__pst__accounting;

    // inform user of code generation
    if (print_debug) {
        printf("Generating code...\n");
    }

    // generate program
    COMPILER__generate__program(final_program, compilation_unit.accountlings, generate_kickstarter, 65536, error);

    // quit label
    quit:

    // clean up
    COMPILER__close__compilation_unit(compilation_unit);

    return;
}

#endif
