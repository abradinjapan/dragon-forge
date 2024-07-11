#ifndef DRAGON__compiler__expander
#define DRAGON__compiler__expander

/* Include */
// compiler
#include "parser.h"

/* Expander */
/*// one structure
typedef struct COMPILER__expandling_structure {
    COMPILER__parsling_argument name;
    ANVIL__list members; // COMPILER__parsling_argument
} COMPILER__expandling_structure;

// one statement
typedef struct COMPILER__expandling_statement {
    // statement type
    COMPILER__stt type;
} COMPILER__expandling_statement;

// one scope
typedef struct COMPILER__expandling_scope {
    ANVIL__list statements; // COMPILER__expandling_statement
} COMPILER__expandling_scope;

// one function
typedef struct COMPILER__expandling_function {
    COMPILER__parsling_argument name;
    ANVIL__list inputs; // COMPILER__parsling_argument
    ANVIL__list outputs; // COMPILER__parsling_argument
} COMPILER__expandling_function;*/

// one expandling program
typedef struct COMPILER__expandling_program {
    ANVIL__list functions; // COMPILER__parsling_function
    ANVIL__list structures; // COMPILER__parsling_structure
    COMPILER__function_handle main_function_handle;
} COMPILER__expandling_program;

/* Expand Definitions */
// search for structure names
COMPILER__structure_handle COMPILER__find__defined_structure_by_name(ANVIL__list* defined_structures, COMPILER__namespace searching_for, COMPILER__error* error) {
    ANVIL__current current_defined_structure = ANVIL__calculate__current_from_list_filled_index(defined_structures);

    // search for names
    while (ANVIL__check__current_within_range(current_defined_structure)) {
        // check name
        if (COMPILER__check__identical_namespaces((*(COMPILER__parsling_argument*)(*(COMPILER__parsling_structure*)current_defined_structure.start).type_names.buffer.start).name, searching_for) == ANVIL__bt__true) {
            // match!
            return (COMPILER__structure_handle)ANVIL__create__buffer(current_defined_structure.start, current_defined_structure.start + sizeof(COMPILER__parsling_structure) - 1);
        }

        // next structure
        current_defined_structure.start += sizeof(COMPILER__parsling_structure);
    }

    return (COMPILER__structure_handle)ANVIL__create_null__buffer();
}

// expand all files into one group
COMPILER__expandling_program COMPILER__expand__program(ANVIL__list* parsling_programs, COMPILER__error* error) {
    COMPILER__expandling_program output;
    ANVIL__current current_file;
    ANVIL__current current_structure;
    ANVIL__current current_structure_name;

    // record and expand all structures
    // open list
    output.structures = COMPILER__open__list_with_error(sizeof(COMPILER__parsling_structure) * 8, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // setup current file
    current_file = ANVIL__calculate__current_from_list_filled_index(parsling_programs);

    // for each file
    while (ANVIL__check__current_within_range(current_file)) {
        // get file
        COMPILER__parsling_program file = *(COMPILER__parsling_program*)current_file.start;

        // get each structure
        // setup current
        current_structure = ANVIL__calculate__current_from_list_filled_index(&file.structures);

        // for each structure
        while (ANVIL__check__current_within_range(current_structure)) {
            // get structure
            COMPILER__parsling_structure structure = *(COMPILER__parsling_structure*)current_structure.start;

            // create each structure by name
            // setup current
            current_structure_name = ANVIL__calculate__current_from_list_filled_index(&structure.type_names);

            // for each name
            while (ANVIL__check__current_within_range(current_structure_name)) {
                // get name
                COMPILER__parsling_argument name = *(COMPILER__parsling_argument*)current_structure_name.start;

                // check for duplicates
                if (ANVIL__check__empty_buffer(COMPILER__find__defined_structure_by_name(&output.structures, name.name, error)) == ANVIL__bt__false) {
                    // set error
                    *error = COMPILER__open__error("Expansion Error: More than on structure has the same name.", (*(COMPILER__lexling*)name.name.lexlings.buffer.start).location);

                    return output;
                }

                // define a new structure
                // create individual name
                ANVIL__list name_list = COMPILER__open__list_with_error(sizeof(COMPILER__parsling_argument) * 1, error);
                if (COMPILER__check__error_occured(error)) {
                    return output;
                }
                COMPILER__append__parsling_argument(&name_list, name, error);
                if (COMPILER__check__error_occured(error)) {
                    return output;
                }

                // append structure
                COMPILER__append__parsling_structure(&output.structures, COMPILER__create__parsling_structure(name_list, structure.arguments), error);
                if (COMPILER__check__error_occured(error)) {
                    return output;
                }

                // next name
                current_structure_name.start += sizeof(COMPILER__parsling_argument);
            }

            // next structure
            current_structure.start += sizeof(COMPILER__parsling_structure);
        }

        // next file
        current_file.start += sizeof(COMPILER__parsling_program);
    }

    // check structure members for unavailable types
    // TODO

    // search for main
    // TODO

    // starting from main, discover and record all functions connected to it
    // TODO

    return output;
}

// print expandling program
void COMPILER__print__expandling_program(COMPILER__expandling_program program) {
    ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&program.structures);

    // print header
    printf("Expanded Program:\n");

    // print each structure
    while (ANVIL__check__current_within_range(current_structure)) {
        // print structure
        COMPILER__print__parsed_structure(*(COMPILER__parsling_structure*)current_structure.start, 1);

        // advance current
        current_structure.start += sizeof(COMPILER__parsling_structure);
    }

    return;
}

#endif
