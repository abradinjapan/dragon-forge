#ifndef DRAGON__compiler__accounter
#define DRAGON__compiler__accounter

/* Notes */
// Compiler backends are just statement translators. Not program understanders.
// Accounting is about setting up your data for how you wanna generate code.
// Next language can be called Sword.
/*
    All variables will have one type and one value.
    Arguments are different from variables.
    Is there a difference between using a variable and declaring a variable?
        Yes.
        Do variables exist independently of arguments?
            Yes.
    Three things: Arguments, variables, calls.
*/
/*
    One thing to note is that structures are not arranged in a straight line, they build upon one another.
        Building types by hierarchy and not by index.
            Or do both.
            Have types point to other types in an array of types and have end nodes point to a "!dragon.cell" structure.
    Another thing to note is that types and function call arguments are different types of data (aka different structs entirely.).
*/
/*
    Steps of accounting everything:
        1) Validate and get structure table.
        2) Get all function call headers.
        3) Validate and account all functions.
    Steps of accounting a function:
        1) Collect all function relevant data (variables, strings, scopes, normal offsets, scoped offsets, statement map, etc).
        2) Translate syntactic data into an indexable blueprint of the function.
*/

/* Include */
#include "parser.h"

/* Account */
// one structure
typedef struct COMPILER__accountling_structure {
    COMPILER__parsling_argument name; // copied from parser
    ANVIL__counted_list members; // COMPILER__structure_index
} COMPILER__accountling_structure;

// all structures
typedef struct COMPILER__accountling_structures {
    ANVIL__counted_list name_table; // COMPILER__namespace
    ANVIL__counted_list data_table; // COMPILER__accountling_structure
} COMPILER__accountling_structures;

// one program
typedef struct COMPILER__accountling_program {
    COMPILER__accountling_structures structures;
    ANVIL__counted_list functions; // COMPILER__accountling_function
} COMPILER__accountling_program;

// check to see if a type name exists
COMPILER__structure_index COMPILER__find__accountling_structure_name_index(ANVIL__counted_list structure_names, COMPILER__namespace searching_for) {
    COMPILER__structure_index output = 0;

    // check each structure entry or name
    ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&structure_names.list);

    // check each entry
    while (ANVIL__check__current_within_range(current_structure)) {
        // get structure
        COMPILER__namespace name = *(COMPILER__namespace*)current_structure.start;

        // check name
        if (COMPILER__check__identical_namespaces(searching_for, name) == ANVIL__bt__true) {
            // match!
            return output;
        }

        // next structure
        current_structure.start += sizeof(COMPILER__namespace);
        output++;
    }

    // not found
    return output;
}

// generate predefined type names
ANVIL__counted_list COMPILER__account__structures__generate_predefined_type_names(ANVIL__counted_list names, COMPILER__error* error) {
    // loop through blueprint and generate structures
    // setup current
    ANVIL__current current_blueprintling = ANVIL__create__buffer((ANVIL__address)COMPILER__global__predefined_types, (ANVIL__address)(COMPILER__global__predefined_types + sizeof(COMPILER__global__predefined_types) - 1));

    // loop until end of blueprint
    while (ANVIL__check__current_within_range(current_blueprintling) && COMPILER__read__blueprintling(&current_blueprintling) == COMPILER__abt__define_type) {
        // skip past define type marker
        COMPILER__next__blueprintling(&current_blueprintling);

        // read predefined type definition
        COMPILER__ptt type = COMPILER__read_and_next__blueprintling(&current_blueprintling);

        // generate and append structure name
        // get the name of the function
        COMPILER__namespace name = COMPILER__open__namespace_from_single_lexling(COMPILER__open__lexling_from_string(COMPILER__global__predefined_type_names[type - COMPILER__ptt__START], COMPILER__lt__name, COMPILER__create_null__character_location()), error);

        // next name index
        names.count++;

        // append the name
        COMPILER__append__namespace(&names.list, name, error);
        if (COMPILER__check__error_occured(error)) {
            return names;
        }

        // generate and append structure members
        // get member count
        COMPILER__structure_member_count member_count = COMPILER__read_and_next__blueprintling(&current_blueprintling);

        // skip past members
        for (COMPILER__structure_member_index member_index = 0; member_index < member_count; member_index++) {
            // get the member ID
            COMPILER__read_and_next__blueprintling(&current_blueprintling);
        }
    }

    return names;
}

// generate and get all types
COMPILER__accountling_structures COMPILER__account__structures(ANVIL__list parsling_programs, COMPILER__error* error) {
    COMPILER__accountling_structures output;

    // open tables
    output.name_table = ANVIL__create__counted_list(COMPILER__open__list_with_error(sizeof(COMPILER__namespace) * 32, error), 0);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }
    output.data_table = ANVIL__create__counted_list(COMPILER__open__list_with_error(sizeof(COMPILER__accountling_structure) * 32, error), 0);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // setup predefined structure names
    output.name_table = COMPILER__account__structures__generate_predefined_type_names(output.name_table, error);

    // get all names
    // for each program
    ANVIL__current current_program = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // for each program
    while (ANVIL__check__current_within_range(current_program)) {
        // get program
        COMPILER__parsling_program program = *(COMPILER__parsling_program*)current_program.start;

        // for each structure
        ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&program.structures);

        // for each structure
        while (ANVIL__check__current_within_range(current_structure)) {
            // get structure
            COMPILER__parsling_structure structure = *(COMPILER__parsling_structure*)current_structure.start;

            // for each structure name
            ANVIL__current current_structure_name = ANVIL__calculate__current_from_list_filled_index(&structure.type_names);

            // for each structure name
            while (ANVIL__check__current_within_range(current_structure_name)) {
                // get name
                COMPILER__parsling_argument name = *(COMPILER__parsling_argument*)current_structure_name.start;

                // check if structure name is taken
                if (COMPILER__find__accountling_structure_name_index(output.name_table, name.name) < output.name_table.count) {
                    // setup error
                    *error = COMPILER__open__error("Accounting Error: Structure name is defined more than once.", COMPILER__get__namespace_lexling_location(name.name));

                    return output;
                }

                // append name
                COMPILER__append__namespace(&output.name_table.list, name.name, error);
                output.name_table.count++;
                if (COMPILER__check__error_occured(error)) {
                    return output;
                }

                // next structure name
                current_structure_name.start += sizeof(COMPILER__parsling_argument);
            }

            // next structure
            current_structure.start += sizeof(COMPILER__parsling_structure);
        }

        // next program
        current_program.start += sizeof(COMPILER__parsling_program);
    }

    return output;
}

// account all files into one accountling program
COMPILER__accountling_program COMPILER__account__program(ANVIL__list parsling_programs, COMPILER__error* error) {
    COMPILER__accountling_program output;

    // get all structures
    output.structures = COMPILER__account__structures(parsling_programs, error);

    // generate predefined variables
    // TODO

    // generate predefined function headers
    // TODO

    return output;
}

// print accountling structures
void COMPILER__print__accountling_structures(COMPILER__accountling_structures structures, ANVIL__tab_count tab_depth) {
    // print header
    ANVIL__print__tabs(tab_depth);
    printf("Structure Name Table:\n");

    // print name table
    // setup current
    ANVIL__current current_name = ANVIL__calculate__current_from_list_filled_index(&structures.name_table.list);

    // for each name
    while (ANVIL__check__current_within_range(current_name)) {
        // get name
        COMPILER__namespace name = *(COMPILER__namespace*)current_name.start;

        // print name
        ANVIL__print__tabs(tab_depth + 1);
        COMPILER__print__namespace(name);
        printf("\n");

        // next name
        current_name.start += sizeof(COMPILER__namespace);
    }

    return;
}

// print accountling program
void COMPILER__print__accountling_program(COMPILER__accountling_program program) {
    // print header
    printf("Accounted Program:\n");

    // print name table
    COMPILER__print__accountling_structures(program.structures, 1);

    return;
}

#endif
