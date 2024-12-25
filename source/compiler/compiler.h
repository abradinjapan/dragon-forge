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
    COMPILER__lexlings lexlings;
    COMPILER__parsling_program parslings;
    COMPILER__accountling_program accountlings;
    COMPILER__pst stages_completed;
} COMPILER__compilation_unit;

// close a compilation unit
void COMPILER__close__compilation_unit(COMPILER__compilation_unit unit) {
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
        COMPILER__close__accountling_program(unit.accountlings);
    }

    return;
}

// append tab depth
void COMPILER__generate__debug_information__tabs(ANVIL__list* json, ANVIL__tab_count tab_depth, COMPILER__error* error) {
    // setup string
    ANVIL__buffer data = ANVIL__open__buffer_from_string("\t", ANVIL__bt__false, ANVIL__bt__false);

    // append tabs
    for (ANVIL__tab_count index = 0; index < tab_depth; index++) {
        // append tab
        ANVIL__list__append__buffer_data(json, data, &(*error).memory_error_occured);
        if (COMPILER__check__error_occured(error)) {
            return;
        }
    }

    return;
}

// generate one lexling's debug information
void COMPILER__generate__debug_information__lexling(COMPILER__lexling lexling, ANVIL__tab_count tab_depth, COMPILER__error* error) {
    
}

// generate all lexlings information
void COMPILER__generate__debug_information__lexlings() {

}

// serialize debug information to json
ANVIL__buffer COMPILER__generate__debug_information(COMPILER__compilation_unit unit, COMPILER__error* error) {
    // setup list
    ANVIL__list json = COMPILER__open__list_with_error(sizeof(ANVIL__character) * 65536, error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // serialize json for lexer


    return;
}

// compile a program
void COMPILER__compile__files(ANVIL__buffer user_codes, ANVIL__bt generate_kickstarter, ANVIL__bt print_debug, ANVIL__bt generate_debug, ANVIL__buffer* final_program, ANVIL__buffer* debug_information, COMPILER__error* error) {
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

    // print compilation message
    if (print_debug) {
        printf("Compiling Files.\n");
    }

    // lex file
    compilation_unit.lexlings = COMPILER__compile__lex(compilation_unit.user_codes, error);
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

    // generate debug information
    if (generate_debug) {
        COMPILER__generate__debug_information(compilation_unit, error);
    }

    // quit label
    quit:

    // clean up
    COMPILER__close__compilation_unit(compilation_unit);

    return;
}

#endif
