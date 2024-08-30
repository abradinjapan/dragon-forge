#ifndef DRAGON__compiler__accounter
#define DRAGON__compiler__accounter

/* Notes */
// Compiler backends are just statement translators. Not program understanders.
// Accounting is about setting up your data for how you wanna generate code.
/*
    Steps of accounting everything:
        1) Validate and get structure table.
        2) Get all function call headers.
        3) Validate and account all functions.
    Steps of accounting a function:
        1) Collect all function relevant data (variables, strings, scopes, normal offsets, scoped offsets, statement map, etc).
        2) Translate syntactic data into an indexable blueprint of the function.
*/
/*
    How to access and work with structures:
        Since structures contain sub-structures and the like, it is important to lay out how they work.
        A structure will be itself and all sub-structure-variables layed out in order.
        Structure paths like "data.stuff.oof" are simply names given to the sub-structure-variables.
        So all structures are effectively just multiple variables with each grouping also getting its own name.
*/

/* Include */
#include "compiler_specifications.h"
#include "parser.h"

/* Account */
// one structure member
typedef struct COMPILER__accountling_structure_member {
    COMPILER__structure_index structure_ID; // the ID of the member's type
    COMPILER__namespace name; // the namespace of the member
    ANVIL__bt predefined; // if the member is built in statically
} COMPILER__accountling_structure_member;

// close a predefined accountling structure member
void COMPILER__close__accountling_structure_member(COMPILER__accountling_structure_member member) {
    // close namespace
    if (member.predefined == ANVIL__bt__true) {
        COMPILER__close__parsling_namespace(member.name);
    }

    return;
}

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
    COMPILER__namespace name; // copied from parser when user defined, opened explicitly when predefined
    ANVIL__counted_list members; // COMPILER__accountling_structure_member
    ANVIL__bt predefined;
} COMPILER__accountling_structure;

// close one structure
void COMPILER__close__accountling_structure(COMPILER__accountling_structure structure) {
    // close name
    if (structure.predefined == ANVIL__bt__true) {
        COMPILER__close__parsling_namespace(structure.name);
    }

    // set current
    ANVIL__current current_member = ANVIL__calculate__current_from_list_filled_index(&structure.members.list);

    // for each member
    while (ANVIL__check__current_within_range(current_member)) {
        // get member
        COMPILER__accountling_structure_member member = *(COMPILER__accountling_structure_member*)current_member.start;

        // close member
        COMPILER__close__accountling_structure_member(member);

        // next member
        current_member.start += sizeof(COMPILER__accountling_structure_member);
    }

    // close members
    ANVIL__close__counted_list(structure.members);

    return;
}

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

// close accountling structures
void COMPILER__close__accountling_structures(COMPILER__accountling_structures structures) {
    // close name table
    // close each predefined name
    ANVIL__current current_table_name = ANVIL__calculate__current_from_list_filled_index(&structures.name_table.list);
    COMPILER__structure_index name_index = 0;

    // for each name
    while (ANVIL__check__current_within_range(current_table_name) && name_index < COMPILER__ptt__USER_DEFINED_START) {
        // close name
        COMPILER__close__parsling_namespace(*(COMPILER__namespace*)current_table_name.start);

        // next name
        current_table_name.start += sizeof(COMPILER__namespace);
        name_index++;
    }

    // close list
    ANVIL__close__counted_list(structures.name_table);

    // close structures
    // setup current
    ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&structures.data_table.list);

    // for each structure
    while (ANVIL__check__current_within_range(current_structure)) {
        // get structure
        COMPILER__accountling_structure structure = *(COMPILER__accountling_structure*)current_structure.start;

        // close structure
        COMPILER__close__accountling_structure(structure);

        // next structure
        current_structure.start += sizeof(COMPILER__accountling_structure);
    }

    // close data table
    ANVIL__close__counted_list(structures.data_table);

    return;
}

// one function header
typedef struct COMPILER__accountling_function_header {
    COMPILER__namespace name;
    ANVIL__counted_list input_types; // COMPILER__structure_index
    ANVIL__counted_list output_types; // COMPILER__structure_index
} COMPILER__accountling_function_header;

// append accountling function header
void COMPILER__append__accountling_function_header(ANVIL__list* list, COMPILER__accountling_function_header data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__accountling_function_header), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__accountling_function_header*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__accountling_function_header);

    return;
}

// one function
typedef struct COMPILER__accountling_function {
    COMPILER__accountling_function_header header;
    // TODO
} COMPILER__accountling_function;

// one function header index
typedef struct COMPILER__accountling_function_header_location {
    COMPILER__afht category_index;
    COMPILER__function_header_index header_index;
} COMPILER__accountling_function_header_location;

// create custom index
COMPILER__accountling_function_header_location COMPILER__create__accountling_function_header_location(COMPILER__afht category_index, COMPILER__function_header_index header_index) {
    COMPILER__accountling_function_header_location output;

    // setup output
    output.category_index = category_index;
    output.header_index = header_index;

    return output;
}

// all function headers
typedef struct COMPILER__accountling_function_headers {
    ANVIL__counted_list category[COMPILER__afht__COUNT]; // all headers categories, COMPILER__accountling_function_header
} COMPILER__accountling_function_headers;

// open function headers
COMPILER__accountling_function_headers COMPILER__open__accountling_function_headers(COMPILER__error* error) {
    COMPILER__accountling_function_headers output;

    // null init
    output.category[COMPILER__afht__user_defined] = ANVIL__create_null__counted_list();
    output.category[COMPILER__afht__sets] = ANVIL__create_null__counted_list();
    
    // open all lists
    output.category[COMPILER__afht__user_defined] = COMPILER__open__counted_list_with_error(sizeof(COMPILER__accountling_function_header) * 16, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }
    output.category[COMPILER__afht__sets] = COMPILER__open__counted_list_with_error(sizeof(COMPILER__accountling_function_header) * 16, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }
    
    return output;
}

// close function headers counted list
void COMPILER__close__accountling_function_headers__header_list(ANVIL__counted_list headers, ANVIL__bt close_namespaces) {
    // check for empty list
    if (ANVIL__check__empty_list(headers.list) == ANVIL__bt__true) {
        // nothing to close, quit
        return;
    }

    // close all headers
    // setup current
    ANVIL__current current_function_header = ANVIL__calculate__current_from_list_filled_index(&headers.list);

    // for each header
    while (ANVIL__check__current_within_range(current_function_header)) {
        // get header
        COMPILER__accountling_function_header function_header = *(COMPILER__accountling_function_header*)current_function_header.start;

        // close header
        // if namespace should be closed
        if (close_namespaces == ANVIL__bt__true) {
            // close name
            COMPILER__close__parsling_namespace(function_header.name);
        }

        // close io
        ANVIL__close__counted_list(function_header.input_types);
        ANVIL__close__counted_list(function_header.output_types);

        // next header
        current_function_header.start += sizeof(COMPILER__accountling_function_header);
    }

    // close header buffer
    ANVIL__close__counted_list(headers);

    return;
}

// close function headers
void COMPILER__close__accountling_function_headers(COMPILER__accountling_function_headers headers) {
    // close lists
    COMPILER__close__accountling_function_headers__header_list(headers.category[COMPILER__afht__user_defined], ANVIL__bt__false);
    COMPILER__close__accountling_function_headers__header_list(headers.category[COMPILER__afht__sets], ANVIL__bt__true);

    return;
}

// all functions
typedef struct COMPILER__accountling_functions {
    COMPILER__accountling_function_headers headers;
    ANVIL__counted_list bodies; // COMPILER__accountling_function
} COMPILER__accountling_functions;

// close accountling functions
void COMPILER__close__accountling_functions(COMPILER__accountling_functions functions) {
    // close headers
    COMPILER__close__accountling_function_headers(functions.headers);

    // close bodies
    // TODO

    return;
}

// one program
typedef struct COMPILER__accountling_program {
    COMPILER__accountling_structures structures;
    COMPILER__accountling_functions functions;
} COMPILER__accountling_program;

// close a program
void COMPILER__close__accountling_program(COMPILER__accountling_program program) {
    // close structures
    COMPILER__close__accountling_structures(program.structures);

    // close functions
    COMPILER__close__accountling_functions(program.functions);

    return;
}

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

// check to see if a type member exists
COMPILER__structure_member_index COMPILER__find__accountling_structure_member_name_index(ANVIL__counted_list structures, COMPILER__namespace searching_for) {
    COMPILER__structure_member_index output = 0;

    // check each structure entry or name
    ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&structures.list);

    // check each entry
    while (ANVIL__check__current_within_range(current_structure)) {
        // get structure
        COMPILER__accountling_structure_member member = *(COMPILER__accountling_structure_member*)current_structure.start;

        // check name
        if (COMPILER__check__identical_namespaces(searching_for, member.name) == ANVIL__bt__true) {
            // match!
            return output;
        }

        // next structure
        current_structure.start += sizeof(COMPILER__accountling_structure_member);
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
        accountling_structure.predefined = ANVIL__bt__true;
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
            member_temp.predefined = ANVIL__bt__true;
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

// perform actual recurse
void COMPILER__account__check__recursive_structures__do_search(ANVIL__counted_list structures, COMPILER__stack_index current_stack_index, ANVIL__buffer type_trace, COMPILER__error* error) {
    // get structure ID
    COMPILER__structure_index searching_for = ((COMPILER__structure_index*)type_trace.start)[current_stack_index];

    // search previous structs
    for (COMPILER__structure_index checking_against = 0; checking_against < current_stack_index; checking_against++) {
        // check struct
        if (searching_for == ((COMPILER__structure_index*)type_trace.start)[checking_against]) {
            // match found, compiler error
            *error = COMPILER__open__error("Accounting Error: Recursive structure definition detected.", COMPILER__get__namespace_lexling_location(((COMPILER__accountling_structure*)structures.list.buffer.start)[searching_for].name));

            return;
        }
    }

    // search down tree for each member
    for (COMPILER__structure_member_index member_index = 0; member_index < ((COMPILER__accountling_structure*)structures.list.buffer.start)[searching_for].members.count; member_index++) {
        // get member
        COMPILER__structure_index recursing_for = ((COMPILER__accountling_structure_member*)((COMPILER__accountling_structure*)structures.list.buffer.start)[searching_for].members.list.buffer.start)[member_index].structure_ID;

        // if is a dead end
        if (recursing_for > structures.count) {
            // skip pass
            continue;
        }

        // append structure ID
        ((COMPILER__structure_index*)type_trace.start)[current_stack_index + 1] = recursing_for;

        // otherwise, search for structure
        COMPILER__account__check__recursive_structures__do_search(structures, current_stack_index + 1, type_trace, error);
        if (COMPILER__check__error_occured(error)) {
            return;
        }
    }

    return;
}

// check for recursive types
void COMPILER__account__check__recursive_structures(ANVIL__counted_list structures, COMPILER__error* error) {
    // setup tracker
    ANVIL__buffer type_trace = ANVIL__open__buffer(sizeof(COMPILER__structure_index) * structures.count);

    // check each structure
    for (COMPILER__structure_index top_index = 0; top_index < structures.count; top_index++) {
        // setup starting index
        COMPILER__stack_index starting_index = 0;

        // fresh init trace data
        for (COMPILER__structure_index trace_index = 0; trace_index < structures.count; trace_index++) {
            // init as not found
            ((COMPILER__structure_index*)type_trace.start)[trace_index] = starting_index;
        }

        // push first type onto stack
        ((COMPILER__structure_index*)type_trace.start)[starting_index] = top_index;

        // recurse
        COMPILER__account__check__recursive_structures__do_search(structures, starting_index, type_trace, error);
        if (COMPILER__check__error_occured(error)) {
            goto quit;
        }
    }

    // quit offset
    quit:

    // destroy trace
    ANVIL__close__buffer(type_trace);

    return;
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
        ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&program.structures.list);

        // for each structure
        while (ANVIL__check__current_within_range(current_structure)) {
            // get structure
            COMPILER__parsling_structure structure = *(COMPILER__parsling_structure*)current_structure.start;

            // for each structure name
            ANVIL__current current_structure_name = ANVIL__calculate__current_from_list_filled_index(&structure.type_names.list);

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
        ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&program.structures.list);

        // for each structure
        while (ANVIL__check__current_within_range(current_structure)) {
            // get structure
            COMPILER__parsling_structure parsling_structure = *(COMPILER__parsling_structure*)current_structure.start;

            // for each structure name
            ANVIL__current current_structure_name = ANVIL__calculate__current_from_list_filled_index(&parsling_structure.type_names.list);

            // for each structure name
            while (ANVIL__check__current_within_range(current_structure_name)) {
                // get name
                COMPILER__parsling_argument structure_name = *(COMPILER__parsling_argument*)current_structure_name.start;

                // setup accountling structure structure
                COMPILER__accountling_structure accountling_structure;
                accountling_structure.predefined = ANVIL__bt__false;
                accountling_structure.name = structure_name.name;
                accountling_structure.members = ANVIL__create__counted_list(COMPILER__open__list_with_error(sizeof(COMPILER__accountling_structure_member) * 16, error), 0);
                if (COMPILER__check__error_occured(error)) {
                    return output;
                }

                // for each structure member
                ANVIL__current current_structure_member = ANVIL__calculate__current_from_list_filled_index(&parsling_structure.arguments.list);

                // check for no arguments, this is an error
                if (parsling_structure.arguments.count < 1) {
                    // set error
                    *error = COMPILER__open__error("Accounting Error: A structure cannot have zero members.", COMPILER__get__namespace_lexling_location(structure_name.name));

                    // close members
                    ANVIL__close__counted_list(accountling_structure.members);

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

                        // close members
                        ANVIL__close__counted_list(accountling_structure.members);

                        return output;
                    }

                    // if type is defined in itself (not including longer recursions, just first level ones)
                    if (COMPILER__check__identical_namespaces(accountling_structure.name, parsling_member.type)) {
                        // set error
                        *error = COMPILER__open__error("Accounting Error: A structure has itself as a member.", COMPILER__get__namespace_lexling_location(parsling_member.type));

                        // close members
                        ANVIL__close__counted_list(accountling_structure.members);

                        return output;
                    }

                    // create member
                    COMPILER__accountling_structure_member accountling_member;
                    accountling_member.structure_ID = COMPILER__find__accountling_structure_name_index(output.name_table, parsling_member.type);
                    accountling_member.name = parsling_member.name;
                    accountling_member.predefined = ANVIL__bt__false;

                    // check for duplicate member
                    if (COMPILER__find__accountling_structure_member_name_index(accountling_structure.members, accountling_member.name) < accountling_structure.members.count) {
                        // set error
                        *error = COMPILER__open__error("Accounting Error: A structure has two or more arguments with the same name.", COMPILER__get__namespace_lexling_location(parsling_member.name));

                        // free unused member
                        ANVIL__close__counted_list(accountling_structure.members);

                        return output;
                    }

                    // append member
                    COMPILER__append__accountling_structure_member(&accountling_structure.members.list, accountling_member, error);
                    if (COMPILER__check__error_occured(error)) {
                        return output;
                    }
                    accountling_structure.members.count++;

                    // next member
                    current_structure_member.start += sizeof(COMPILER__parsling_argument);
                }

                // append structure to list
                COMPILER__append__accountling_structure(&output.data_table.list, accountling_structure, error);
                if (COMPILER__check__error_occured(error)) {
                    return output;
                }
                output.data_table.count++;

                // next structure name
                current_structure_name.start += sizeof(COMPILER__parsling_argument);
            }

            // next structure
            current_structure.start += sizeof(COMPILER__parsling_structure);
        }

        // next program
        current_program.start += sizeof(COMPILER__parsling_program);
    }

    // check every type for recursive type definitions
    COMPILER__account__check__recursive_structures(output.data_table, error);

    return output;
}

// check a specific function header
ANVIL__bt COMPILER__account__search_for_function_header__check_headers_equal(COMPILER__accountling_function_header a, COMPILER__accountling_function_header b) {
    // check namespace
    if (COMPILER__check__identical_namespaces(a.name, b.name)) {
        // check inputs
        if (a.input_types.count != b.input_types.count) {
            // not a match
            goto not_equal;
        }
        for (COMPILER__structure_index input_ID = 0; input_ID < a.input_types.count; input_ID++) {
            // check type
            if (((COMPILER__structure_index*)a.input_types.list.buffer.start)[input_ID] != ((COMPILER__structure_index*)b.input_types.list.buffer.start)[input_ID]) {
                // not a match
                goto not_equal;
            }
        }

        // check outputs
        if (a.output_types.count != b.output_types.count) {
            // not a match
            goto not_equal;
        }
        for (COMPILER__structure_index output_ID = 0; output_ID < a.output_types.count; output_ID++) {
            // check type
            if (((COMPILER__structure_index*)a.output_types.list.buffer.start)[output_ID] != ((COMPILER__structure_index*)b.output_types.list.buffer.start)[output_ID]) {
                // not a match
                goto not_equal;
            }
        }

        // everything is equal, match!
        return ANVIL__bt__true;
    }

    not_equal:

    // not equal
    return ANVIL__bt__false;
}

// search for a function header in one list
COMPILER__function_header_index COMPILER__account__search_for_function_header__search_list(ANVIL__counted_list headers_list, COMPILER__accountling_function_header searching_for) {
    // setup index
    COMPILER__function_header_index function_index = 0;

    // search each function header
    // setup current
    ANVIL__current current_header = ANVIL__calculate__current_from_list_filled_index(&headers_list.list);

    // for each header (unless found which quits early)
    while (ANVIL__check__current_within_range(current_header)) {
        // get header
        COMPILER__accountling_function_header header = *(COMPILER__accountling_function_header*)current_header.start;

        // check for match
        if (COMPILER__account__search_for_function_header__check_headers_equal(header, searching_for) == ANVIL__bt__true) {
            // match!
            return function_index;
        }

        // next header
        current_header.start += sizeof(COMPILER__accountling_function_header);
        function_index++;
    }

    // not found
    return function_index;
}

// search for a header across all lists
COMPILER__accountling_function_header_location COMPILER__account__search_for_function_header__search_all_lists(COMPILER__accountling_function_headers headers, COMPILER__accountling_function_header searching_for) {
    COMPILER__accountling_function_header_location output;

    // init output
    output = COMPILER__create__accountling_function_header_location(0, 0);

    // search each list until found
    for (; output.category_index < COMPILER__afht__COUNT; output.category_index++) {
        // search one list
        output.header_index = COMPILER__account__search_for_function_header__search_list(headers.category[output.category_index], searching_for);

        // check search status
        if (output.header_index < headers.category[output.category_index].count) {
            // match!
            return output;
        }
    }

    // not found
    return output;
}

// generate all predefined function headers
COMPILER__accountling_function_headers COMPILER__account__functions__generate_predefined_function_headers(COMPILER__accountling_function_headers headers, COMPILER__error* error) {
    // setup current blueprintling
    ANVIL__current current_blueprintling = ANVIL__create__buffer((ANVIL__address)COMPILER__global__predefined_function_calls, (ANVIL__address)COMPILER__global__predefined_function_calls + sizeof(COMPILER__global__predefined_function_calls) - 1);

    // for each definition
    while (ANVIL__check__current_within_range(current_blueprintling) && COMPILER__read__blueprintling(&current_blueprintling) == COMPILER__abt__define_function_call) {
        // skip past marker
        COMPILER__next__blueprintling(&current_blueprintling);

        // get list it belongs to
        COMPILER__afht list_index = COMPILER__read_and_next__blueprintling(&current_blueprintling);

        // setup variable
        COMPILER__accountling_function_header header;

        // get predefined function call type
        COMPILER__read_and_next__blueprintling(&current_blueprintling);

        // get name ID
        COMPILER__pfcnt name_ID = COMPILER__read_and_next__blueprintling(&current_blueprintling);

        // setup name
        header.name = COMPILER__open__namespace_from_single_lexling(COMPILER__open__lexling_from_string(COMPILER__global__predefined_function_call_names[name_ID], COMPILER__lt__name, COMPILER__create_null__character_location()), error);
        if (COMPILER__check__error_occured(error)) {
            return headers;
        }

        // count inputs
        COMPILER__input_count input_count = COMPILER__read_and_next__blueprintling(&current_blueprintling);

        // allocate inputs
        header.input_types = ANVIL__create__counted_list(COMPILER__open__list_with_error(sizeof(COMPILER__structure_index) * 8, error), 0);

        // get inputs
        for (COMPILER__input_count input_ID = 0; input_ID < input_count; input_ID++) {
            // get type
            COMPILER__append__structure_index(&header.input_types.list, (COMPILER__structure_index)COMPILER__read_and_next__blueprintling(&current_blueprintling), error);
            if (COMPILER__check__error_occured(error)) {
                return headers;
            }
        }

        // count outputs
        COMPILER__output_count output_count = COMPILER__read_and_next__blueprintling(&current_blueprintling);

        // allocate outputs
        header.output_types = ANVIL__create__counted_list(COMPILER__open__list_with_error(sizeof(COMPILER__structure_index) * 8, error), 0);

        // get outputs
        for (COMPILER__output_count output_ID = 0; output_ID < output_count; output_ID++) {
            // get type
            COMPILER__append__structure_index(&header.output_types.list, (COMPILER__structure_index)COMPILER__read_and_next__blueprintling(&current_blueprintling), error);
            if (COMPILER__check__error_occured(error)) {
                return headers;
            }
        }

        // append function header
        COMPILER__append__accountling_function_header(&headers.category[list_index].list, header, error);
        if (COMPILER__check__error_occured(error)) {
            return headers;
        }
        headers.category[list_index].count++;
    }

    // done
    return headers;
}

// account all user defined function header arguments
ANVIL__counted_list COMPILER__account__functions__user_defined_function_header_arguments(ANVIL__counted_list structure_names, ANVIL__counted_list parsling_header_arguments, COMPILER__error* error) {
    // allocate structure index list
    ANVIL__counted_list output = COMPILER__open__counted_list_with_error(sizeof(COMPILER__structure_index) * 8, error);

    // setup current
    ANVIL__current current_parsling_argument = ANVIL__calculate__current_from_list_filled_index(&parsling_header_arguments.list);

    // for each argument
    while (ANVIL__check__current_within_range(current_parsling_argument)) {
        // get argument
        COMPILER__parsling_argument parsling_argument = *(COMPILER__parsling_argument*)current_parsling_argument.start;

        // validate and check the argument for a valid argument type
        // get index
        COMPILER__structure_index structure_index = COMPILER__find__accountling_structure_name_index(structure_names, parsling_argument.type);

        // if index deems invalid
        if (structure_index >= structure_names.count) {
            // set error
            *error = COMPILER__open__error("Accounting Error: A function header argument has an unknown type.", COMPILER__get__namespace_lexling_location(parsling_argument.type));

            return output;
        }

        // record structure index
        COMPILER__append__structure_index(&output.list, structure_index + COMPILER__aat__COUNT, error);
        if (COMPILER__check__error_occured(error)) {
            return output;
        }

        // next argument
        current_parsling_argument.start += sizeof(COMPILER__parsling_argument);
        output.count++;
    }

    return output;
}

// account all user defined function headers
COMPILER__accountling_function_headers COMPILER__account__functions__user_defined_function_headers(COMPILER__accountling_function_headers headers, ANVIL__list parsling_programs, ANVIL__counted_list structure_names, COMPILER__error* error) {
    // for each parsling program
    // setup current
    ANVIL__current current_program = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // for each program
    while (ANVIL__check__current_within_range(current_program)) {
        // get parsling program
        COMPILER__parsling_program parsling_program = *(COMPILER__parsling_program*)current_program.start;

        // for each function
        // setup current
        ANVIL__current current_function = ANVIL__calculate__current_from_list_filled_index(&parsling_program.functions.list);

        // for each function
        while (ANVIL__check__current_within_range(current_function)) {
            // get parsling function
            COMPILER__parsling_function parsling_function = *(COMPILER__parsling_function*)current_function.start;

            // account header
            {
                // setup variable
                COMPILER__accountling_function_header header;

                // get name
                header.name = parsling_function.header.name.name;

                // get inputs
                header.input_types = COMPILER__account__functions__user_defined_function_header_arguments(structure_names, parsling_function.header.inputs, error);
                if (COMPILER__check__error_occured(error)) {
                    return headers;
                }

                // get outputs
                header.output_types = COMPILER__account__functions__user_defined_function_header_arguments(structure_names, parsling_function.header.outputs, error);
                if (COMPILER__check__error_occured(error)) {
                    return headers;
                }

                // check if function header is already used
                // check namespace
                if (COMPILER__account__search_for_function_header__search_all_lists(headers, header).category_index < COMPILER__afht__COUNT) {
                    // already defined, error
                    *error = COMPILER__open__error("Accounting Error: Two or more functions have the same header (name & io).", COMPILER__get__namespace_lexling_location(header.name));

                    return headers;
                }

                // append header
                COMPILER__append__accountling_function_header(&headers.category[COMPILER__afht__user_defined].list, header, error);
                headers.category[COMPILER__afht__user_defined].count++;
                if (COMPILER__check__error_occured((error))) {
                    return headers;
                }
            }

            // next function
            current_function.start += sizeof(COMPILER__parsling_function);
        }

        // next parsling program
        current_program.start += sizeof(COMPILER__parsling_program);
    }

    return headers;
}

/*// account all functions
ANVIL__counted_list COMPILER__account__functions__user_defined_function_bodies(ANVIL__counted_list function_headers, COMPILER__accountling_structures structures, ANVIL__list parsling_programs, COMPILER__error* error) {
    // open function list
    ANVIL__counted_list output = ANVIL__create__counted_list(COMPILER__open__list_with_error(sizeof(COMPILER__accountling_function) * 32, error), 0);

    // account each program
    ANVIL__current current_parsling_program = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // for each parsling program
    while (ANVIL__check__current_within_range(current_parsling_program)) {
        // get parsling program
        COMPILER__parsling_program parsling_program = *(COMPILER__parsling_program*)current_parsling_program.start;

        // account each function
        ANVIL__current current_parsling_function = ANVIL__calculate__current_from_list_filled_index(&parsling_program.functions.list);

        // for each function
        while (ANVIL__check__current_within_range(current_parsling_function)) {
            // get function
            COMPILER__parsling_function parsling_function = *(COMPILER__parsling_function*)current_parsling_function.start;

            // account function
            {
                COMPILER__accountling_function function;
            }

            // next function
            current_parsling_function.start += sizeof(COMPILER__parsling_function);
        }

        // next program
        current_parsling_program.start += sizeof(COMPILER__parsling_program);
    }

    return output;
}*/

// account all functions & headers
COMPILER__accountling_functions COMPILER__account__functions(ANVIL__list parsling_programs, COMPILER__accountling_structures structures, COMPILER__error* error) {
    COMPILER__accountling_functions output;

    // open output
    output.headers = COMPILER__open__accountling_function_headers(error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // generate predefined headers
    output.headers = COMPILER__account__functions__generate_predefined_function_headers(output.headers, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // account user defined function headers
    output.headers = COMPILER__account__functions__user_defined_function_headers(output.headers, parsling_programs, structures.name_table, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // account user defined function bodies
    //output.bodies = COMPILER__account__functions__user_defined_function_bodies(output.headers, structures, parsling_programs, error);

    return output;
}

// account all files into one accountling program
COMPILER__accountling_program COMPILER__account__program(ANVIL__list parsling_programs, COMPILER__error* error) {
    COMPILER__accountling_program output;

    // get all structures
    output.structures = COMPILER__account__structures(parsling_programs, error);

    // get all functions & function headers
    output.functions = COMPILER__account__functions(parsling_programs, output.structures, error);

    return output;
}

// print accountling structure
void COMPILER__print__accountling_structure(COMPILER__accountling_structures accountling_structures, COMPILER__accountling_structure accountling_structure, COMPILER__structure_index index, ANVIL__tab_count tab_depth) {
    // print header
    ANVIL__print__tabs(tab_depth);
    printf("[ %lu ] Structure '!", index);
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
            printf("(internal||error)");
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
    printf("Structure Name Table (%lu):\n", structures.name_table.count);

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
    printf("Structures (%lu):\n", structures.data_table.count);

    // print all structures
    // setup current
    ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&structures.data_table.list);
    COMPILER__structure_index structure_ID = 0;

    // for each structure
    while (ANVIL__check__current_within_range(current_structure)) {
        // get structure
        COMPILER__accountling_structure accountling_structure = *(COMPILER__accountling_structure*)current_structure.start;

        // print structure
        COMPILER__print__accountling_structure(structures, accountling_structure, structure_ID, tab_depth + 1);

        // next structure
        current_structure.start += sizeof(COMPILER__accountling_structure);
        structure_ID++;
    }

    return;
}

// print accountling function header arguments
void COMPILER__print__accountling_function_header_arguments(ANVIL__counted_list arguments) {
    // print start of arguments
    printf("(");

    // setup current
    ANVIL__current current_structure_index = ANVIL__calculate__current_from_list_filled_index(&arguments.list);

    // for each structure index
    while (ANVIL__check__current_within_range(current_structure_index)) {
        // get structure index
        COMPILER__structure_index structure_index = *(COMPILER__structure_index*)current_structure_index.start;

        // print start of argument
        printf("[ raw: %lu, ", structure_index);

        // check argument for embedded type
        if (structure_index < COMPILER__aat__COUNT) {
            // print aat
            printf("aat: %lu", structure_index);
        } else {
            // print structure index
            printf("structure_index: %lu", structure_index - COMPILER__aat__COUNT);
        }

        // print end of argument
        printf(" ]");

        // next argument
        current_structure_index.start += sizeof(COMPILER__structure_index);
    }

    // print end of arguments
    printf(")");

    return;
}

// print accountling functions
void COMPILER__print__accountling_functions(COMPILER__accountling_functions functions, ANVIL__tab_count tab_depth) {
    // print section header
    ANVIL__print__tabs(tab_depth);
    printf("Function Header Table:\n");

    // print each header table
    for (COMPILER__afht category_index = 0; category_index < COMPILER__afht__COUNT; category_index++) {
        // print section header
        ANVIL__print__tabs(tab_depth + 1);
        printf("Section (%i):\n", category_index);

        // print header table
        // setup current
        ANVIL__current current_header = ANVIL__calculate__current_from_list_filled_index(&functions.headers.category[category_index].list);
        COMPILER__function_header_index header_index = 0;

        // for each header
        while (ANVIL__check__current_within_range(current_header)) {
            // get header
            COMPILER__accountling_function_header header = *(COMPILER__accountling_function_header*)current_header.start;

            // print header
            ANVIL__print__tabs(tab_depth + 2);
            printf("[ %lu ] ", header_index);
            COMPILER__print__namespace(header.name);

            // print io
            COMPILER__print__accountling_function_header_arguments(header.input_types);
            COMPILER__print__accountling_function_header_arguments(header.output_types);

            // next header
            printf("\n");
            current_header.start += sizeof(COMPILER__accountling_function_header);
            header_index++;
        }
    }

    return;
}

// print accountling program
void COMPILER__print__accountling_program(COMPILER__accountling_program program) {
    // print header
    printf("Accounted Program:\n");

    // print structures
    COMPILER__print__accountling_structures(program.structures, 1);

    // print functions
    COMPILER__print__accountling_functions(program.functions, 1);

    return;
}

#endif
