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
//  I struggle with tasks I percieve as difficult.
//      Both feelings and the project need to be balanced.
/* Include */
#include "parser.h"

/* Account */
// one structure member
typedef struct COMPILER__accountling_structure_member {
    COMPILER__structure_index structure_ID; // the ID of the member's type
    COMPILER__namespace name; // the namespace of the member
} COMPILER__accountling_structure_member;

// append accountling structure member
void COMPILER__append__accountling_structure_member(ANVIL__list* list, COMPILER__accountling_structure_member data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__accountling_structure_member), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__accountling_structure_member*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__accountling_structure_member);

    return;
}

// one structure
typedef struct COMPILER__accountling_structure {
    COMPILER__namespace name; // copied from parser
    ANVIL__counted_list members; // COMPILER__structure_index
} COMPILER__accountling_structure;

// append accountling structure
void COMPILER__append__accountling_structure(ANVIL__list* list, COMPILER__accountling_structure data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__accountling_structure), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__accountling_structure*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__accountling_structure);

    return;
}

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
        COMPILER__namespace name = COMPILER__open__namespace_from_single_lexling(COMPILER__open__lexling_from_string(COMPILER__global__predefined_type_names[type], COMPILER__lt__name, COMPILER__create_null__character_location()), error);

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

            // get the member name
            COMPILER__read_and_next__blueprintling(&current_blueprintling);
        }
    }

    return names;
}

// generate predefined type data
ANVIL__counted_list COMPILER__account__structures__generate_predefined_type_data(ANVIL__counted_list structure_data, COMPILER__error* error) {
    // loop through blueprint and generate structures
    // setup current
    ANVIL__current current_blueprintling = ANVIL__create__buffer((ANVIL__address)COMPILER__global__predefined_types, (ANVIL__address)(COMPILER__global__predefined_types + sizeof(COMPILER__global__predefined_types) - 1));

    // loop until end of blueprint
    while (ANVIL__check__current_within_range(current_blueprintling) && COMPILER__read__blueprintling(&current_blueprintling) == COMPILER__abt__define_type) {
        // skip past define type marker
        COMPILER__next__blueprintling(&current_blueprintling);

        // read predefined type definition
        COMPILER__ptt definition_type = COMPILER__read_and_next__blueprintling(&current_blueprintling);

        // setup accountling structure structure
        COMPILER__accountling_structure accountling_structure;
        accountling_structure.name = COMPILER__open__namespace_from_single_lexling(COMPILER__open__lexling_from_string(COMPILER__global__predefined_type_names[definition_type], COMPILER__lt__name, COMPILER__create_null__character_location()), error);
        if (COMPILER__check__error_occured(error)) {
            return structure_data;
        }
        accountling_structure.members = ANVIL__create__counted_list(COMPILER__open__list_with_error(sizeof(COMPILER__accountling_structure_member) * 16, error), 0);
        if (COMPILER__check__error_occured(error)) {
            return structure_data;
        }

        // next structure index
        structure_data.count++;

        // generate and append structure members
        // get member count
        COMPILER__structure_member_count member_count = COMPILER__read_and_next__blueprintling(&current_blueprintling);

        // record members
        for (COMPILER__structure_member_index member_index = 0; member_index < member_count; member_index++) {
            // get the member ID
            COMPILER__ptt member_type = COMPILER__read_and_next__blueprintling(&current_blueprintling);
            COMPILER__ptmnt member_name_ID = COMPILER__read_and_next__blueprintling(&current_blueprintling);

            // setup member
            COMPILER__accountling_structure_member member_temp;
            member_temp.structure_ID = member_type;
            member_temp.name = COMPILER__open__namespace_from_single_lexling(COMPILER__open__lexling_from_string(COMPILER__global__predefined_type_member_names[member_name_ID], COMPILER__lt__name, COMPILER__create_null__character_location()), error);
            if (COMPILER__check__error_occured(error)) {
                return structure_data;
            }

            // record type
            COMPILER__append__accountling_structure_member(&accountling_structure.members.list, member_temp, error);
            if (COMPILER__check__error_occured(error)) {
                return structure_data;
            }
        }

        // append structure
        COMPILER__append__accountling_structure(&structure_data.list, accountling_structure, error);
        if (COMPILER__check__error_occured(error)) {
            return structure_data;
        }
    }

    return structure_data;
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
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

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

    // setup predefined structures
    output.data_table = COMPILER__account__structures__generate_predefined_type_data(output.data_table, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // get all structure data
    // for each program
    current_program = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // for each program
    while (ANVIL__check__current_within_range(current_program)) {
        // get program
        COMPILER__parsling_program program = *(COMPILER__parsling_program*)current_program.start;

        // for each structure
        ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&program.structures);

        // for each structure
        while (ANVIL__check__current_within_range(current_structure)) {
            // get structure
            COMPILER__parsling_structure parsling_structure = *(COMPILER__parsling_structure*)current_structure.start;

            // for each structure name
            ANVIL__current current_structure_name = ANVIL__calculate__current_from_list_filled_index(&parsling_structure.type_names);

            // for each structure name
            while (ANVIL__check__current_within_range(current_structure_name)) {
                // get name
                COMPILER__parsling_argument structure_name = *(COMPILER__parsling_argument*)current_structure_name.start;

                // setup accountling structure structure
                COMPILER__accountling_structure accountling_structure;
                accountling_structure.name = structure_name.name;
                accountling_structure.members = ANVIL__create__counted_list(COMPILER__open__list_with_error(sizeof(COMPILER__accountling_structure_member) * 16, error), 0);
                if (COMPILER__check__error_occured(error)) {
                    return output;
                }

                // for each structure member
                ANVIL__current current_structure_member = ANVIL__calculate__current_from_list_filled_index(&parsling_structure.arguments);

                // check for no arguments, this is an error
                if (parsling_structure.arguments.filled_index < 1) {
                    *error = COMPILER__open__error("Accounting Error: A structure cannot have zero members.", COMPILER__get__namespace_lexling_location(structure_name.name));

                    return output;
                }

                // for each structure member
                while (ANVIL__check__current_within_range(current_structure_member)) {
                    // get member
                    COMPILER__parsling_argument parsling_member = *(COMPILER__parsling_argument*)current_structure_member.start;

                    // if unknown type
                    if (COMPILER__find__accountling_structure_name_index(output.name_table, parsling_member.type) >= output.name_table.count) {
                        // set error
                        *error = COMPILER__open__error("Accounting Error: A structure member is of unknown type.", COMPILER__get__namespace_lexling_location(parsling_member.type));

                        // DEBUG
                        ANVIL__print__tabs(1);
                        COMPILER__print__namespace(parsling_member.type);
                        printf("\n");

                        return output;
                    }

                    // create member
                    COMPILER__accountling_structure_member accountling_member;
                    accountling_member.structure_ID = COMPILER__find__accountling_structure_name_index(output.name_table, parsling_member.type);
                    accountling_member.name = parsling_member.name;

                    // append member
                    COMPILER__append__accountling_structure_member(&accountling_structure.members.list, accountling_member, error);
                    if (COMPILER__check__error_occured(error)) {
                        return output;
                    }

                    // next member
                    current_structure_member.start += sizeof(COMPILER__parsling_argument);
                }

                // append structure to list
                COMPILER__append__accountling_structure(&output.data_table.list, accountling_structure, error);
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

    // check for recursive type definitions
    // TODO

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

// print accountling structure
void COMPILER__print__accountling_structure(COMPILER__accountling_structures accountling_structures, COMPILER__accountling_structure accountling_structure, ANVIL__tab_count tab_depth) {
    // print header
    ANVIL__print__tabs(tab_depth);
    printf("Structure '!");
    COMPILER__print__namespace(accountling_structure.name);
    printf("':\n");

    // print members
    // setup current
    ANVIL__current current_member = ANVIL__calculate__current_from_list_filled_index(&accountling_structure.members.list);

    // for each member
    while (ANVIL__check__current_within_range(current_member)) {
        // get member
        COMPILER__accountling_structure_member member = *(COMPILER__accountling_structure_member*)current_member.start;

        // print header
        ANVIL__print__tabs(tab_depth + 1);

        // print index, member name and member type
        COMPILER__print__namespace(member.name);
        printf(" !");
        if (member.structure_ID >= accountling_structures.name_table.count) {
            printf("(internal/error)");
        } else {
            COMPILER__print__namespace(((COMPILER__namespace*)accountling_structures.name_table.list.buffer.start)[member.structure_ID]);
        }
        printf("[ %lu ]\n", member.structure_ID);

        // next member
        current_member.start += sizeof(COMPILER__accountling_structure_member);
    }

    return;
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
        printf("!");
        COMPILER__print__namespace(name);
        printf("\n");

        // next name
        current_name.start += sizeof(COMPILER__namespace);
    }

    // print header
    ANVIL__print__tabs(tab_depth);
    printf("Structures:\n");

    // print all structures
    // setup current
    ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&structures.data_table.list);

    // for each structure
    while (ANVIL__check__current_within_range(current_structure)) {
        // get structure
        COMPILER__accountling_structure accountling_structure = *(COMPILER__accountling_structure*)current_structure.start;

        // print structure
        COMPILER__print__accountling_structure(structures, accountling_structure, tab_depth + 1);

        // next structure
        current_structure.start += sizeof(COMPILER__accountling_structure);
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
