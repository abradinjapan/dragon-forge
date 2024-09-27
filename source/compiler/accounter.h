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
    Updated notes:
        Structures are not expanded in accounting, they are expanded in generation.
            Since you only need to know if variable types align, and not what variables go where, you don't need to mark every single sub-structure down.
    Possible new name:
        Descripting.
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
    ANVIL__cell_count cell_count; // the count of all cells
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
    ANVIL__counted_list category[COMPILER__afht__COUNT]; // all headers categories, COMPILER__accountling_function_header, COMPILER__afht
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

// one statement argument
typedef struct COMPILER__accountling_statement_argument {
    // type
    COMPILER__aat type;

    // offset
    COMPILER__offset_index offset_index;

    // scope index
    COMPILER__scope_index scope_index;

    // variable member
    COMPILER__variable_index variable__variable_index;
    COMPILER__variable_member_index variable__member_index;
} COMPILER__accountling_statement_argument;

// one function scope
typedef struct COMPILER__accountling_scope {
    COMPILER__accountling_statement_argument condition;
    ANVIL__counted_list statements; // COMPILER__accountling_statement
} COMPILER__accountling_scope;

// one statement
typedef struct COMPILER__accountling_statement {
    // type
    COMPILER__ast statement_type;
    COMPILER__accountling_function_header_location associated_header;

    // set cell data
    ANVIL__cell_integer_value set_cell__raw_value;
    COMPILER__variable_index set_cell__variable_index;

    // set string data
    COMPILER__string_index set_string__string_value_index;
    COMPILER__variable_index set_string__variable_index;

    // printing data
    COMPILER__variable_index print__variable_index;

    // offset data
    COMPILER__offset_index offset_index; // if the statement is a regular offset, use this

    // scope data
    COMPILER__scope_index scope_index;
    COMPILER__accountling_scope scope_data; // COMPILER__accountling_statement

    // user defined function call inputs and outputs
    ANVIL__counted_list function_call__inputs; // COMPILER__accountling_statement_argument
    ANVIL__counted_list function_call__outputs; // COMPILER__accountling_statement_argument
} COMPILER__accountling_statement;

// create null accountling statement
COMPILER__accountling_statement COMPILER__create_null__accountling_statement() {
    COMPILER__accountling_statement output;

    // setup output
    output.statement_type = COMPILER__ast__invalid;
    output.associated_header= COMPILER__create__accountling_function_header_location(0, 0);
    output.set_cell__raw_value = -1;
    output.set_cell__variable_index = -1;
    output.set_string__string_value_index = -1;
    output.set_string__variable_index = -1;
    output.offset_index = -1;
    output.scope_index = -1;
    output.scope_data.statements = ANVIL__create_null__counted_list();
    output.function_call__inputs = ANVIL__create_null__counted_list();
    output.function_call__outputs = ANVIL__create_null__counted_list();

    return output;
}

// append accountling statement
void COMPILER__append__accountling_statement(ANVIL__list* list, COMPILER__accountling_statement data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__accountling_statement), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__accountling_statement*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__accountling_statement);

    return;
}

// scope header
typedef struct COMPILER__accountling_scope_header {
    COMPILER__namespace name;
} COMPILER__accountling_scope_header;

// append accountling scope header
void COMPILER__append__accountling_scope_header(ANVIL__list* list, COMPILER__accountling_scope_header data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__accountling_scope_header), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__accountling_scope_header*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__accountling_scope_header);

    return;
}

// variable member range
typedef struct COMPILER__accountling_variable_range {
    COMPILER__variable_member_index start;
    COMPILER__variable_member_index end;
} COMPILER__accountling_variable_range;

// create index
COMPILER__accountling_variable_range COMPILER__create__accountling_variable_range(COMPILER__variable_member_index start, COMPILER__variable_member_index end) {
    COMPILER__accountling_variable_range output;

    // setup output
    output.start = start;
    output.end = end;

    return output;
}

// variable
typedef struct COMPILER__accountling_variable {
    COMPILER__lexling name;
    COMPILER__variable_type_index type;
    COMPILER__accountling_variable_range members;
    COMPILER__cell_range cells;
} COMPILER__accountling_variable;

// create an accountling variable
COMPILER__accountling_variable COMPILER__create__accountling_variable(COMPILER__lexling name, COMPILER__variable_type_index type, COMPILER__accountling_variable_range members, COMPILER__cell_range cells) {
    COMPILER__accountling_variable output;

    // setup output
    output.name = name;
    output.type = type;
    output.members = members;
    output.cells = cells;

    return output;
}

// create null accountling variable
COMPILER__accountling_variable COMPILER__create_null__accountling_variable() {
    return COMPILER__create__accountling_variable(COMPILER__create_null__lexling(), 0, COMPILER__create__accountling_variable_range(0, 0), COMPILER__create__cell_range(0, 0));
}

// append accountling variable
void COMPILER__append__accountling_variable(ANVIL__list* list, COMPILER__accountling_variable data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__accountling_variable), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__accountling_variable*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__accountling_variable);

    return;
}

// all variable data for one function
typedef struct COMPILER__accountling_variables {
    ANVIL__counted_list masters; // COMPILER__accountling_variable
    ANVIL__counted_list members; // COMPILER__accountling_variable
} COMPILER__accountling_variables;

// open variables
COMPILER__accountling_variables COMPILER__open__variables(COMPILER__error* error) {
    COMPILER__accountling_variables output;

    // open function level variable lists
    output.masters = COMPILER__open__counted_list_with_error(sizeof(COMPILER__accountling_variable) * 256, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }
    output.members = COMPILER__open__counted_list_with_error(sizeof(COMPILER__accountling_variable) * 1024, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    return output;
}

// close variables
void COMPILER__close__accountling_variables(COMPILER__accountling_variables variables) {
    // close data
    ANVIL__close__counted_list(variables.masters);
    ANVIL__close__counted_list(variables.members);

    return;
}

// one function
typedef struct COMPILER__accountling_function {
    // program data
    ANVIL__counted_list offsets; // COMPILER__namespace
    COMPILER__accountling_variables variables;
    ANVIL__counted_list scope_headers; // COMPILER__accountling_scope_header
    ANVIL__counted_list strings; // ANVIL__buffer
    COMPILER__accountling_scope scope;

    // statistics
    ANVIL__cell_count next_available_workspace_cell;
} COMPILER__accountling_function;

// append accountling function
void COMPILER__append__accountling_function(ANVIL__list* list, COMPILER__accountling_function data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__accountling_function), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__accountling_function*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__accountling_function);

    return;
}

// close accountling scope
void COMPILER__close__accountling_scope(COMPILER__accountling_scope scope) {
    // close statements
    for (COMPILER__statement_index index = 0; index < scope.statements.count; index++) {
        // get statement
        COMPILER__accountling_statement statement = ((COMPILER__accountling_statement*)scope.statements.list.buffer.start)[index];

        // check statement type
        if (statement.statement_type == COMPILER__ast__scope) {
            // close scope
            COMPILER__close__accountling_scope(statement.scope_data);
        } else if (statement.statement_type == COMPILER__ast__user_defined_function_call) {
            // close io
            // TODO
        }
    }

    // close list
    ANVIL__close__counted_list(scope.statements);
    
    return;
}

// close function
void COMPILER__close__accountling_function(COMPILER__accountling_function function) {
    // close offsets
    ANVIL__close__counted_list(function.offsets);

    // close variables
    COMPILER__close__accountling_variables(function.variables);

    // close scope headers
    ANVIL__close__counted_list(function.scope_headers);

    // close strings
    for (COMPILER__string_index index = 0; index < function.strings.count; index++) {
        // close string
        ANVIL__close__buffer(((ANVIL__buffer*)function.strings.list.buffer.start)[index]);
    }
    ANVIL__close__counted_list(function.strings);

    // close scope
    COMPILER__close__accountling_scope(function.scope);

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
    for (COMPILER__function_index index = 0; index < functions.bodies.count; index++) {
        // close one body
        COMPILER__close__accountling_function(((COMPILER__accountling_function*)functions.bodies.list.buffer.start)[index]);
    }
    
    // close list
    ANVIL__close__counted_list(functions.bodies);

    return;
}

// one program
typedef struct COMPILER__accountling_program {
    COMPILER__accountling_structures structures;
    COMPILER__accountling_functions functions;
    COMPILER__function_index entry_point;
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
COMPILER__structure_member_index COMPILER__find__accountling_structure_member_name_index(COMPILER__accountling_structure structure, COMPILER__namespace searching_for) {
    COMPILER__structure_member_index output = 0;

    // check each structure entry or name
    ANVIL__current current_member = ANVIL__calculate__current_from_list_filled_index(&structure.members.list);

    // check each entry
    while (ANVIL__check__current_within_range(current_member)) {
        // get member
        COMPILER__accountling_structure_member member = *(COMPILER__accountling_structure_member*)current_member.start;

        // check name
        if (COMPILER__check__identical_namespaces(searching_for, member.name) == ANVIL__bt__true) {
            // match!
            return output;
        }

        // next structure
        current_member.start += sizeof(COMPILER__accountling_structure_member);
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
            accountling_structure.members.count++;
        }

        // append structure
        COMPILER__append__accountling_structure(&structure_data.list, accountling_structure, error);
        if (COMPILER__check__error_occured(error)) {
            return structure_data;
        }
        structure_data.count++;
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

// search one structure for its cell count
void COMPILER__account__structures__calculate_structure_cell_counts__search(ANVIL__cell_count* cell_count, COMPILER__accountling_structures structures, COMPILER__structure_index searching, ANVIL__tab_count tabs) {
    // get structure
    COMPILER__accountling_structure structure = ((COMPILER__accountling_structure*)structures.data_table.list.buffer.start)[searching];

    // tally based on sub structures cell count
    for (COMPILER__structure_member_index index = 0; index < structure.members.count; index++) {
        // get structure index
        COMPILER__structure_index current_structure_index = ((COMPILER__accountling_structure_member*)structure.members.list.buffer.start)[index].structure_ID;

        // determine if should search based on structure index
        if (current_structure_index < structures.data_table.count) {
            // search sub-structures
            COMPILER__account__structures__calculate_structure_cell_counts__search(cell_count, structures, current_structure_index, tabs + 1);
        } else {
            // cell found
            (*cell_count)++;
        }
    }

    return;
}

// get the cell counts for all structures
void COMPILER__account__structures__calculate_structure_cell_counts(COMPILER__accountling_structures structures) {
    // for each structure
    for (COMPILER__structure_index index = 0; index < structures.data_table.count; index++) {
        // update structure
        // setup count
        ANVIL__cell_count cell_count = 0;

        // calculate count
        COMPILER__account__structures__calculate_structure_cell_counts__search(&cell_count, structures, index, 1);

        // apply count
        ((COMPILER__accountling_structure*)structures.data_table.list.buffer.start)[index].cell_count = cell_count;
    }

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
                    if (COMPILER__find__accountling_structure_member_name_index(accountling_structure, accountling_member.name) < accountling_structure.members.count) {
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
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // apply a cell count for each structure
    COMPILER__account__structures__calculate_structure_cell_counts(output);

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

// check scope headers for name
COMPILER__scope_index COMPILER__account__functions__get_scope_index(COMPILER__accountling_function* accountling_function, COMPILER__namespace scope_name) {
    COMPILER__scope_index output = 0;

    // search each scope header for name
    while (output < (*accountling_function).scope_headers.count) {
        // check name
        if (COMPILER__check__identical_namespaces(((COMPILER__accountling_scope_header*)(*accountling_function).scope_headers.list.buffer.start)[output].name, scope_name) == ANVIL__bt__true) {
            // match!
            return output;
        }

        // not a match, next one
        output++;
    }

    // no match found
    return output;
}

// check offsets for name
COMPILER__offset_index COMPILER__account__functions__get_offset_index(COMPILER__accountling_function* accountling_function, COMPILER__namespace offset_name) {
    COMPILER__offset_index output = 0;

    // search each offset for name
    while (output < (*accountling_function).offsets.count) {
        // check name
        if (COMPILER__check__identical_namespaces(((COMPILER__namespace*)(*accountling_function).offsets.list.buffer.start)[output], offset_name) == ANVIL__bt__true) {
            // match!
            return output;
        }

        // not a match, next one
        output++;
    }

    // no match found
    return output;
}

// get all data that is function wide (except variables)
void COMPILER__account__functions__get_function_level_data(COMPILER__accountling_function* accountling_function, COMPILER__parsling_scope parsling_scope, COMPILER__error* error) {
    // search through each statement
    // setup current
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_scope.statements.list);

    // for each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // get statement
        COMPILER__parsling_statement parsling_statement = *(COMPILER__parsling_statement*)current_statement.start;

        // check type
        if (parsling_statement.type == COMPILER__stt__subscope) {
            // check for scope name
            if (COMPILER__account__functions__get_scope_index(accountling_function, parsling_statement.name.name) < (*accountling_function).scope_headers.count) {
                // error, scope already exists
                *error = COMPILER__open__error("Accounting Error: A function has two scopes with the same name.", COMPILER__get__namespace_lexling_location(parsling_statement.name.name));

                return;
            }

            // check for offset name
            if (COMPILER__account__functions__get_offset_index(accountling_function, parsling_statement.name.name) < (*accountling_function).offsets.count) {
                // error, offset already exists
                *error = COMPILER__open__error("Accounting Error: A scope has the name of an offset.", COMPILER__get__namespace_lexling_location(parsling_statement.name.name));

                return;
            }

            // create new scope header
            COMPILER__accountling_scope_header scope_header;
            scope_header.name = parsling_statement.name.name;

            // if scope name does not exist, append
            COMPILER__append__accountling_scope_header(&(*accountling_function).scope_headers.list, scope_header, error);
            (*accountling_function).scope_headers.count++;

            // search for more headers in sub-scope
            COMPILER__account__functions__get_function_level_data(accountling_function, parsling_statement.subscope, error);
            if (COMPILER__check__error_occured(error)) {
                return;
            }
        } else if (parsling_statement.type == COMPILER__stt__offset) {
            // check for scope name
            if (COMPILER__account__functions__get_scope_index(accountling_function, parsling_statement.name.name) < (*accountling_function).scope_headers.count) {
                // error, scope already exists
                *error = COMPILER__open__error("Accounting Error: A function has a scope with the same name as an offset.", COMPILER__get__namespace_lexling_location(parsling_statement.name.name));

                return;
            }

            // check for offset name
            if (COMPILER__account__functions__get_offset_index(accountling_function, parsling_statement.name.name) < (*accountling_function).offsets.count) {
                // error, offset already exists
                *error = COMPILER__open__error("Accounting Error: An offset is declared twice.", COMPILER__get__namespace_lexling_location(parsling_statement.name.name));

                return;
            }

            // append offset
            COMPILER__append__namespace(&(*accountling_function).offsets.list, parsling_statement.name.name, error);
            if (COMPILER__check__error_occured(error)) {
                return;
            }
            (*accountling_function).offsets.count++;
        }

        // next statement
        current_statement.start += sizeof(COMPILER__parsling_statement);
    }

    return;
}

// convert string to converted string
ANVIL__buffer COMPILER__translate__string_literal_to_translated_buffer(COMPILER__lexling string_literal, COMPILER__error* error) {
    ANVIL__buffer output = ANVIL__create_null__buffer();
    ANVIL__length character_count = 0;

    // count characters
    for (ANVIL__character_index i = 1; i < ANVIL__calculate__buffer_length(string_literal.value) - 1; i++) {
        // check for escape sequence
        if (((ANVIL__character*)string_literal.value.start)[i] == '%') {
            // check for characters available
            if (i + 3 <= ANVIL__calculate__buffer_length(string_literal.value) - 1) {
                // validate characters
                ANVIL__bt invalid_hexadecimal_character_1;
                ANVIL__bt invalid_hexadecimal_character_2;
                ANVIL__bt semi_colon_missing;
                COMPILER__translate__character_to_hexadecimal(((ANVIL__character*)string_literal.value.start)[i + 1], &invalid_hexadecimal_character_1);
                COMPILER__translate__character_to_hexadecimal(((ANVIL__character*)string_literal.value.start)[i + 2], &invalid_hexadecimal_character_2);
                semi_colon_missing = (ANVIL__bt)(((ANVIL__character*)string_literal.value.start)[i + 3] != ';');

                // determine validity
                if (invalid_hexadecimal_character_1 != ANVIL__bt__false || invalid_hexadecimal_character_2 != ANVIL__bt__false || semi_colon_missing != ANVIL__bt__false) {
                    // invalid escape sequence
                    *error = COMPILER__open__error("Accounting Error: String literal has invalid escape sequences.", string_literal.location);
                    
                    return output;
                }

                // skip past characters
                i += 3;
            // error
            } else {
                *error = COMPILER__open__error("Accounting Error: String literal has invalid escape sequences.", string_literal.location);

                return output;
            }
        }

        // next character
        character_count++;
    }

    // check for empty string
    if (character_count == 0) {
        // return empty string
        return output;
    }

    // allocate string
    output = ANVIL__open__buffer(character_count);
    if (ANVIL__check__empty_buffer(output)) {
        *error = COMPILER__open__internal_memory_error();

        return output;
    }

    // translate string
    character_count = 0;
    for (ANVIL__character_index i = 1; i < ANVIL__calculate__buffer_length(string_literal.value) - 1; i++) {
        // check for escape sequence
        if (((ANVIL__character*)string_literal.value.start)[i] == '%') {
            // validate characters
            ANVIL__bt invalid_hexadecimal_character_1;
            ANVIL__bt invalid_hexadecimal_character_2;
            ANVIL__character a = COMPILER__translate__character_to_hexadecimal(((ANVIL__character*)string_literal.value.start)[i + 1], &invalid_hexadecimal_character_1);
            ANVIL__character b = COMPILER__translate__character_to_hexadecimal(((ANVIL__character*)string_literal.value.start)[i + 2], &invalid_hexadecimal_character_2);

            // write character
            ((ANVIL__character*)output.start)[character_count] = b + (a << 4);
            
            // skip past characters
            i += 3;
        } else {
            // write character
            ((ANVIL__character*)output.start)[character_count] = ((ANVIL__character*)string_literal.value.start)[i];
        }

        // next character
        character_count++;
    }

    return output;
}

// get a variable by index
COMPILER__accountling_variable COMPILER__account__functions__get_variable_by_variable_index(ANVIL__counted_list variables, COMPILER__variable_index index) {
    return ((COMPILER__accountling_variable*)variables.list.buffer.start)[index];
}

// get a variable index by name
COMPILER__variable_index COMPILER__account__functions__get_variable_index_by_name(ANVIL__counted_list variables, COMPILER__lexling name) {
    COMPILER__variable_index index = 0;

    // search for variable
    while (index < variables.count) {
        // get variable
        COMPILER__accountling_variable variable = COMPILER__account__functions__get_variable_by_variable_index(variables, index);

        // check name
        if (ANVIL__calculate__buffer_contents_equal(variable.name.value, name.value)) {
            // match!
            return index;
        }

        // next name
        index++;
    }

    // not a match
    return index;
}

// calculate cell range
COMPILER__cell_range COMPILER__calculate_and_advance__cells(COMPILER__accountling_structures structures, COMPILER__accountling_function* function, COMPILER__variable_type_index type) {
    // get length
    ANVIL__cell_count length = ((COMPILER__accountling_structure*)structures.data_table.list.buffer.start)[type].cell_count;

    // calculate cell range
    COMPILER__cell_range output = COMPILER__create__cell_range((*function).next_available_workspace_cell, (*function).next_available_workspace_cell + length - 1);

    // adjust cell range
    (*function).next_available_workspace_cell += length;

    return output;
}

// generate variables from source variable
COMPILER__accountling_variable_range COMPILER__account__functions__mark_variable__generate_structure_variable_members(COMPILER__accountling_structures structures, COMPILER__accountling_function* accountling_function, ANVIL__cell_ID* current_sub_cell, COMPILER__variable_type_index type, COMPILER__error* error) {
    COMPILER__accountling_variable_range output;

    // get structure
    COMPILER__accountling_structure structure = ((COMPILER__accountling_structure*)structures.data_table.list.buffer.start)[type];

    // setup output
    output = COMPILER__create__accountling_variable_range((*accountling_function).variables.members.count, (*accountling_function).variables.members.count + structure.members.count - 1);

    // advance members
    // TODO

    // reserve members
    for (COMPILER__variable_member_index index = output.start; index <= output.end; index++) {
        // reserve member
        COMPILER__append__accountling_variable(&(*accountling_function).variables.members.list, COMPILER__create_null__accountling_variable(), error);
        if (COMPILER__check__error_occured(error)) {
            return output;
        }
        (*accountling_function).variables.members.count++;
    }

    // for each member
    for (COMPILER__structure_member_index index = 0; index < structure.members.count; index++) {
        // get member
        COMPILER__accountling_structure_member member = ((COMPILER__accountling_structure_member*)structure.members.list.buffer.start)[index];

        // recurse through types or declare new variable
        if (member.structure_ID >= structures.data_table.count) {
            // declare variable
            ((COMPILER__accountling_variable*)(*accountling_function).variables.members.list.buffer.start)[output.start + index] = COMPILER__create__accountling_variable(COMPILER__get__lexling_by_index(member.name.lexlings, 0), member.structure_ID, COMPILER__create__accountling_variable_range(output.start + index, output.start + index), COMPILER__create__cell_range(*current_sub_cell, *current_sub_cell));

            // next cell
            (*current_sub_cell)++;
        // recurse
        } else {
            // setup range start
            COMPILER__cell_range cell_range;
            cell_range.start = *current_sub_cell;

            // recurse
            COMPILER__accountling_variable_range variable_range = COMPILER__account__functions__mark_variable__generate_structure_variable_members(structures, accountling_function, current_sub_cell, member.structure_ID, error);

            // set range end
            cell_range.end = (*current_sub_cell) - 1;

            // append variable
            ((COMPILER__accountling_variable*)(*accountling_function).variables.members.list.buffer.start)[output.start + index] = COMPILER__create__accountling_variable(COMPILER__get__lexling_by_index(member.name.lexlings, 0), member.structure_ID, variable_range, cell_range);
        }
    }

    return output;
}

// generate variables from source variable
void COMPILER__account__functions__mark_variable__generate_master_variable_and_sub_variables(COMPILER__accountling_structures structures, COMPILER__accountling_function* accountling_function, COMPILER__parsling_argument name, COMPILER__variable_type_index type, COMPILER__error* error) {
    // reserve cells
    COMPILER__cell_range cell_range = COMPILER__calculate_and_advance__cells(structures, accountling_function, type);

    // declare sub-variables
    ANVIL__cell_ID current_sub_cell = cell_range.start;
    COMPILER__accountling_variable_range member_range = COMPILER__account__functions__mark_variable__generate_structure_variable_members(structures, accountling_function, &current_sub_cell, type, error);

    // declare master variable
    COMPILER__append__accountling_variable(&(*accountling_function).variables.masters.list, COMPILER__create__accountling_variable(COMPILER__get__lexling_by_index(name.name.lexlings, 0), type, member_range, cell_range), error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }
    (*accountling_function).variables.masters.count++;

    return;
}

// mark variable
COMPILER__variable_index COMPILER__account__functions__mark_variable(COMPILER__accountling_structures structures, COMPILER__accountling_function* accountling_function, COMPILER__parsling_argument name, COMPILER__variable_type_index expected_type, COMPILER__asvt argument_location, COMPILER__error* error) {
    // search for master variable
    COMPILER__variable_index index = COMPILER__account__functions__get_variable_index_by_name((*accountling_function).variables.masters, COMPILER__get__lexling_by_index(name.name.lexlings, 0));

    // if variable is a master namespace only
    if (name.name.lexlings.count < 2) {
        // check for variable declaration
        // is already declared
        if (index < (*accountling_function).variables.masters.count) {
            // if types match, found correct function
            if (COMPILER__account__functions__get_variable_by_variable_index((*accountling_function).variables.masters, index).type == expected_type) {
                goto success;
            // if types don't match, maybe another function matches.
            } else {
                goto failure;
            }
        // is not declared
        } else {
            // if argument is in an output slot
            if (argument_location == COMPILER__asvt__output) {
                // declare variable and all its sub-variables
                COMPILER__account__functions__mark_variable__generate_master_variable_and_sub_variables(structures, accountling_function, name, expected_type, error);
                if (COMPILER__check__error_occured(error)) {
                    goto failure;
                }
            // variables cannot be declared in statement inputs
            } else {
                *error = COMPILER__open__error("Accounting Error: Variables cannot be declared in statement inputs.", COMPILER__get__namespace_lexling_location(name.name));

                goto failure;
            }

            // check to see if cell range is out of bounds
            if ((*accountling_function).next_available_workspace_cell >= ANVIL__srt__start__function_io) {
                // error, too many cells were used
                *error = COMPILER__open__error("Accounting Error: A function has too many variables.", COMPILER__get__namespace_lexling_location(name.name));

                goto failure;
            }

            // return index
            goto success;
        }
    // otherwise, the variable MUST already be declared to access its members
    } else {
        // if already declared
        if (index < (*accountling_function).variables.masters.count) {
            // if types match, found correct function
            if (COMPILER__account__functions__get_variable_by_variable_index((*accountling_function).variables.masters, index).type == expected_type) {
                goto success;
            // if types don't match, maybe another function matches
            } else {
                goto failure;
            }
        // error, variable sub-structure being used on non-exsistant master namespace || variable is not found
        } else {
            // open error
            *error = COMPILER__open__error("Accounting Error: An argument is trying to access a variable that doesn't exist.", COMPILER__get__namespace_lexling_location(name.name));

            goto failure;
        }
    }

    success:
    return index;

    failure:
    return (*accountling_function).variables.masters.count;
}

// check for set
ANVIL__bt COMPILER__account__functions__check_and_get_statement_translation__set(COMPILER__accountling_structures structures, COMPILER__accountling_function* accountling_function, COMPILER__parsling_statement parsling_statement, COMPILER__accountling_statement* accountling_statement, COMPILER__error* error) {
    // setup valid name
    COMPILER__namespace valid_name = COMPILER__open__namespace_from_single_lexling(COMPILER__open__lexling_from_string(COMPILER__define__master_namespace ".set", COMPILER__lt__name, COMPILER__create_null__character_location()), error);
    if (COMPILER__check__error_occured(error)) {
        goto failure;
    }

    // check for name & argument counts
    if (COMPILER__check__identical_namespaces(parsling_statement.name.name, valid_name) && parsling_statement.inputs.count == 1 && parsling_statement.outputs.count == 1) {
        // check for literal type
        // check for string literal
        if (COMPILER__get__parsling_argument_by_index(parsling_statement.inputs, 0).category == COMPILER__pat__string_literal) {
            // check output variable type
            // get index
            COMPILER__variable_index variable_ID = COMPILER__account__functions__mark_variable(structures, accountling_function, COMPILER__get__parsling_argument_by_index(parsling_statement.outputs, 0), COMPILER__ptt__dragon_buffer, COMPILER__asvt__output, error);
            if (COMPILER__check__error_occured(error) || variable_ID >= (*accountling_function).variables.masters.count) {
                goto failure;
            }

            // add string to library
            COMPILER__append__buffer_with_error(&(*accountling_function).strings.list, COMPILER__translate__string_literal_to_translated_buffer(COMPILER__get__lexling_by_index(COMPILER__get__parsling_argument_by_index(parsling_statement.inputs, 0).name.lexlings, 0), error), error);
            (*accountling_function).strings.count++;
            if (COMPILER__check__error_occured(error)) {
                goto failure;
            }

            // setup output statement
            (*accountling_statement).set_string__variable_index = variable_ID;
            (*accountling_statement).set_string__string_value_index = (*accountling_function).strings.count - 1;
            (*accountling_statement).statement_type = COMPILER__ast__predefined__set__string;

            // variable declared / reused
            goto match;
        // check for integer literal
        } else if (COMPILER__get__parsling_argument_by_index(parsling_statement.inputs, 0).category == COMPILER__pat__name) {
            // temporaries
            ANVIL__cell_integer_value integer_value = 0;
            ANVIL__buffer integer_string = COMPILER__get__lexling_by_index(COMPILER__get__parsling_argument_by_index(parsling_statement.inputs, 0).name.lexlings, 0).value;

            // get index
            COMPILER__variable_index variable_ID = COMPILER__account__functions__mark_variable(structures, accountling_function, COMPILER__get__parsling_argument_by_index(parsling_statement.outputs, 0), COMPILER__ptt__dragon_cell, COMPILER__asvt__output, error);
            if (COMPILER__check__error_occured(error) || variable_ID >= (*accountling_function).variables.masters.count) {
                goto failure;
            }

            // attempt translation
            if (COMPILER__translate__string_to_boolean(integer_string, &integer_value) || COMPILER__translate__string_to_binary(integer_string, &integer_value) || COMPILER__translate__string_to_integer(integer_string, &integer_value) || COMPILER__translate__string_to_hexedecimal(integer_string, &integer_value)) {
                // create statement
                (*accountling_statement).statement_type = COMPILER__ast__predefined__set__cell;
                (*accountling_statement).set_cell__raw_value = integer_value;
                (*accountling_statement).set_cell__variable_index = variable_ID;

                // valid variable declaration
                goto match;
            }

            // no valid literal was found, error
            *error = COMPILER__open__error("Accounting Error: A set function call did not have a valid literal.", COMPILER__get__namespace_lexling_location(COMPILER__get__parsling_argument_by_index(parsling_statement.inputs, 0).name));
            goto failure;
        }

        // statement is not a setter, but not yet known to be invalid
        goto failure;
    }

    // not a match
    failure:
    COMPILER__close__parsling_namespace(valid_name);
    return ANVIL__bt__false;

    // match!
    match:
    COMPILER__close__parsling_namespace(valid_name);
    return ANVIL__bt__true;
}

// check for printing
ANVIL__bt COMPILER__account__functions__check_and_get_statement_translation__print(COMPILER__accountling_structures structures, COMPILER__accountling_function* accountling_function, COMPILER__parsling_statement parsling_statement, COMPILER__accountling_statement* accountling_statement, COMPILER__error* error) {
    // setup valid name
    COMPILER__namespace print_buffer_as_string_name = COMPILER__open__namespace_from_single_lexling(COMPILER__open__lexling_from_string(COMPILER__define__master_namespace ".print.buffer_as_string", COMPILER__lt__name, COMPILER__create_null__character_location()), error);
    if (COMPILER__check__error_occured(error)) {
        goto failure;
    }

    // if is a print buffer as string
    if (COMPILER__check__identical_namespaces(parsling_statement.name.name, print_buffer_as_string_name) && parsling_statement.inputs.count == 1 && parsling_statement.outputs.count == 0) {
        // if inputs are correct parsing type
        if (COMPILER__get__parsling_argument_by_index(parsling_statement.inputs, 0).category == COMPILER__pat__name) {
            // check input variable type
            // get index
            COMPILER__variable_index variable_ID = COMPILER__account__functions__mark_variable(structures, accountling_function, COMPILER__get__parsling_argument_by_index(parsling_statement.inputs, 0), COMPILER__ptt__dragon_buffer, COMPILER__asvt__input, error);
            if (COMPILER__check__error_occured(error) || variable_ID >= (*accountling_function).variables.masters.count) {
                goto failure;
            }

            // setup output statement
            (*accountling_statement).statement_type = COMPILER__ast__predefined__print__buffer_as_string;
            (*accountling_statement).print__variable_index = variable_ID;

            // match
            goto match;
        // not the right argument type
        } else {
            goto failure;
        }
    }

    // not a match
    failure:
    COMPILER__close__parsling_namespace(print_buffer_as_string_name);
    return ANVIL__bt__false;

    // match!
    match:
    COMPILER__close__parsling_namespace(print_buffer_as_string_name);
    return ANVIL__bt__true;
}

// get all statements & variables
void COMPILER__account__functions__function_sequential_information__one_scope(COMPILER__accountling_structures structures, COMPILER__accountling_function* accountling_function, COMPILER__accountling_scope* accountling_scope, COMPILER__parsling_scope scope, COMPILER__error* error) {
    // translate all statements
    // setup current statement
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&scope.statements.list);

    // setup statements
    (*accountling_scope).statements = COMPILER__open__counted_list_with_error(sizeof(COMPILER__accountling_statement) * 32, error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // for each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // get parsling statement
        COMPILER__parsling_statement parsling_statement = *(COMPILER__parsling_statement*)current_statement.start;

        // setup accountling statement
        COMPILER__accountling_statement accountling_statement;

        // null init type
        accountling_statement = COMPILER__create_null__accountling_statement();

        // determine statement type
        if (parsling_statement.type == COMPILER__stt__function_call) {
            // find definition
            // find sets
            if (COMPILER__account__functions__check_and_get_statement_translation__set(structures, accountling_function, parsling_statement, &accountling_statement, error)) {
                // append statement
                COMPILER__append__accountling_statement(&(*accountling_scope).statements.list, accountling_statement, error);
                if (COMPILER__check__error_occured(error)) {
                    return;
                }
                (*accountling_scope).statements.count++;
                goto next_statement;
            }
            if (COMPILER__check__error_occured(error)) {
                return;
            }

            // find prints
            if (COMPILER__account__functions__check_and_get_statement_translation__print(structures, accountling_function, parsling_statement, &accountling_statement, error)) {
                // append statement
                COMPILER__append__accountling_statement(&(*accountling_scope).statements.list, accountling_statement, error);
                if (COMPILER__check__error_occured(error)) {
                    return;
                }
                (*accountling_scope).statements.count++;
                goto next_statement;
            }
            if (COMPILER__check__error_occured(error)) {
                return;
            }
        } else if (parsling_statement.type == COMPILER__stt__offset) {
            // setup offset data
            accountling_statement.offset_index = COMPILER__account__functions__get_offset_index(accountling_function, parsling_statement.name.name);
            accountling_statement.statement_type = COMPILER__ast__offset;
            
            // append statement
            COMPILER__append__accountling_statement(&(*accountling_scope).statements.list, accountling_statement, error);
            if (COMPILER__check__error_occured(error)) {
                return;
            }
            (*accountling_scope).statements.count++;
        } else if (parsling_statement.type == COMPILER__stt__subscope) {
            // get scope data
            COMPILER__account__functions__function_sequential_information__one_scope(structures, accountling_function, &accountling_statement.scope_data, parsling_statement.subscope, error);
            if (COMPILER__check__error_occured(error)) {
                return;
            }
            accountling_statement.scope_index = COMPILER__account__functions__get_scope_index(accountling_function, parsling_statement.name.name);
            accountling_statement.statement_type = COMPILER__ast__scope;

            // append statement
            COMPILER__append__accountling_statement(&(*accountling_scope).statements.list, accountling_statement, error);
            if (COMPILER__check__error_occured(error)) {
                return;
            }
            (*accountling_scope).statements.count++;
        // unrecognized statement type
        } else {
            *error = COMPILER__open__error("Internal Error: Unrecognized statement type in accounter, oops.", COMPILER__get__namespace_lexling_location(parsling_statement.name.name));

            return;
        }

        // next statement
        next_statement:
        current_statement.start += sizeof(COMPILER__parsling_statement);
    }

    return;
}

// account all functions
COMPILER__accountling_functions COMPILER__account__functions__user_defined_function_bodies(COMPILER__accountling_functions functions, COMPILER__accountling_structures structures, ANVIL__list parsling_programs, COMPILER__error* error) {
    // open function body list
    functions.bodies = COMPILER__open__counted_list_with_error(sizeof(COMPILER__accountling_function) * functions.headers.category[COMPILER__afht__user_defined].count, error);

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
                // setup function variable
                COMPILER__accountling_function accountling_function;

                // intialize statistics
                accountling_function.next_available_workspace_cell = ANVIL__srt__start__workspace;

                // allocate scope headers list
                accountling_function.scope_headers = COMPILER__open__counted_list_with_error(sizeof(COMPILER__accountling_scope_header) * 16, error);
                if (COMPILER__check__error_occured(error)) {
                    return functions;
                }
                // allocate offsets list
                accountling_function.offsets = COMPILER__open__counted_list_with_error(sizeof(COMPILER__namespace) * 16, error);
                if (COMPILER__check__error_occured(error)) {
                    return functions;
                }

                // get scope headers & offsets
                COMPILER__account__functions__get_function_level_data(&accountling_function, parsling_function.scope, error);
                if (COMPILER__check__error_occured(error)) {
                    return functions;
                }

                // allocate variables list
                accountling_function.variables = COMPILER__open__variables(error);
                if (COMPILER__check__error_occured(error)) {
                    return functions;
                }

                // open strings
                accountling_function.strings = COMPILER__open__counted_list_with_error(sizeof(ANVIL__buffer) * 16, error);
                if (COMPILER__check__error_occured(error)) {
                    return functions;
                }

                // get variables & statements
                COMPILER__account__functions__function_sequential_information__one_scope(structures, &accountling_function, &accountling_function.scope, parsling_function.scope, error);
                if (COMPILER__check__error_occured(error)) {
                    return functions;
                }

                // append function
                COMPILER__append__accountling_function(&functions.bodies.list, accountling_function, error);
                if (COMPILER__check__error_occured(error)) {
                    return functions;
                }
                functions.bodies.count++;
            }

            // next function
            current_parsling_function.start += sizeof(COMPILER__parsling_function);
        }

        // next program
        current_parsling_program.start += sizeof(COMPILER__parsling_program);
    }

    return functions;
}

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
    output = COMPILER__account__functions__user_defined_function_bodies(output, structures, parsling_programs, error);

    return output;
}

// search for main
COMPILER__function_index COMPILER__find__entry_point(COMPILER__accountling_functions functions, COMPILER__error* error) {
    // setup main name
    COMPILER__namespace searching_for = COMPILER__open__namespace_from_single_lexling(COMPILER__open__lexling_from_string(COMPILER__define__master_namespace ".main", COMPILER__lt__name, COMPILER__create_null__character_location()), error);
    if (COMPILER__check__error_occured(error)) {
        return -1;
    }

    // search for main
    for (COMPILER__function_index index = 0; index < functions.headers.category[COMPILER__afht__user_defined].count; index++) {
        // get header
        COMPILER__accountling_function_header header = ((COMPILER__accountling_function_header*)functions.headers.category[COMPILER__afht__user_defined].list.buffer.start)[index];

        // check header
        if (COMPILER__check__identical_namespaces(header.name, searching_for) == ANVIL__bt__true && header.input_types.count == 0 && header.output_types.count == 0) {
            // match!
            COMPILER__close__parsling_namespace(searching_for);
            return index;
        }
    }

    // not found
    *error = COMPILER__open__error("Accounting Error: The entry point '" COMPILER__define__master_namespace ".main()()' could not be found.", COMPILER__create_null__character_location());
    
    // return
    COMPILER__close__parsling_namespace(searching_for);

    return -1;
}

// account all files into one accountling program
COMPILER__accountling_program COMPILER__account__program(ANVIL__list parsling_programs, COMPILER__error* error) {
    COMPILER__accountling_program output;

    // get all structures
    output.structures = COMPILER__account__structures(parsling_programs, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // get all functions & function headers
    output.functions = COMPILER__account__functions(parsling_programs, output.structures, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // search for main
    output.entry_point = COMPILER__find__entry_point(output.functions, error);

    return output;
}

// print accountling structure
void COMPILER__print__accountling_structure(COMPILER__accountling_structures accountling_structures, COMPILER__accountling_structure accountling_structure, COMPILER__structure_index index, ANVIL__tab_count tab_depth) {
    // print header
    ANVIL__print__tabs(tab_depth);
    printf("[ ID: %lu, Cell Count: %lu ] Structure '!", index, accountling_structure.cell_count);
    COMPILER__print__namespace(accountling_structure.name);
    printf("'\n");

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

// print an accountling function header
void COMPILER__print__accountling_function_header(COMPILER__accountling_function_header header) {
    // print name
    COMPILER__print__namespace(header.name);

    // print io
    COMPILER__print__accountling_function_header_arguments(header.input_types);
    COMPILER__print__accountling_function_header_arguments(header.output_types);

    return;
}

// print an accounting variable
void COMPILER__print__accountling_variable(COMPILER__accountling_function function, COMPILER__accountling_structures structures, COMPILER__accountling_variable variable, COMPILER__variable_index master_variable_index, ANVIL__tab_count tab_depth) {
    // print variable data
    ANVIL__print__tabs(tab_depth);
    printf("[ master_index: %lu, type: %lu, name: '", master_variable_index, variable.type);
    ANVIL__print__buffer(variable.name.value);
    printf("' cells: [ %lu->%lu ]]\n", (ANVIL__u64)variable.cells.start, (ANVIL__u64)variable.cells.end);

    // print all members
    for (COMPILER__variable_member_index index = variable.members.start; index <= variable.members.end; index++) {
        // get member variable
        COMPILER__accountling_variable member_variable = ((COMPILER__accountling_variable*)function.variables.members.list.buffer.start)[index];
        // only print if variable is not end node
        if (member_variable.type < structures.data_table.count) {
            // print member
            COMPILER__print__accountling_variable(function, structures, member_variable, master_variable_index, tab_depth + 1);
        }
    }

    return;
}

// print an accountling scope
void COMPILER__print__accountling_scope(COMPILER__accountling_scope statements, ANVIL__tab_count tab_depth) {
    // print each statement
    for (COMPILER__statement_index statement_index = 0; statement_index < statements.statements.count; statement_index++) {
        // get statement
        COMPILER__accountling_statement statement = ((COMPILER__accountling_statement*)statements.statements.list.buffer.start)[statement_index];

        // print statement start
        ANVIL__print__tabs(tab_depth + 1);

        // print statement data based on type
        if (statement.statement_type == COMPILER__ast__predefined__set__cell) {
            printf("COMPILER__ast__predefined__set__cell(raw_value: %lu, variable_index: %lu)", statement.set_cell__raw_value, statement.set_cell__variable_index);

            // statement finisher
            printf("\n");
        } else if (statement.statement_type == COMPILER__ast__predefined__set__string) {
            printf("COMPILER__ast__predefined__set__string(string_index: %lu, variable_index: %lu)", statement.set_string__string_value_index, statement.set_string__variable_index);

            // statement finisher
            printf("\n");
        } else if (statement.statement_type == COMPILER__ast__predefined__print__buffer_as_string) {
            printf("COMPILER__ast__predefined__print__buffer_as_string(variable_index: %lu)", statement.print__variable_index);

            // statement finisher
            printf("\n");
        } else if (statement.statement_type == COMPILER__ast__offset) {
            printf("COMPILER__ast__offset(offset_index: %lu)", statement.offset_index);

            // statement finisher
            printf("\n");
        } else if (statement.statement_type == COMPILER__ast__scope) {
            printf("COMPILER__ast__scope(scope_index: %lu)", statement.scope_index);
            if (statement.scope_data.statements.count > 0) {
                printf("\n");
                COMPILER__print__accountling_scope(statement.scope_data, tab_depth + 1);
            }
        } else {
            printf("[ Internal Issue: Debug info not implemented for statement type '%lu' ]\n", (ANVIL__u64)statement.statement_type);
        }
    }

    return;
}

// print accountling functions
void COMPILER__print__accountling_functions(COMPILER__accountling_structures structures, COMPILER__accountling_functions functions, ANVIL__tab_count tab_depth) {
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
            COMPILER__print__accountling_function_header(header);

            // next header
            printf("\n");
            current_header.start += sizeof(COMPILER__accountling_function_header);
            header_index++;
        }
    }

    // if there function bodies, print them
    if (functions.bodies.count > 0) {
        // print section header
        ANVIL__print__tabs(tab_depth);
        printf("Function Bodies Table [ function_body_count: %lu ]:\n", functions.bodies.count);

        // print each function
        for (COMPILER__function_index function_index = 0; function_index < functions.bodies.count; function_index++) {
            // get function
            COMPILER__accountling_function function = ((COMPILER__accountling_function*)functions.bodies.list.buffer.start)[function_index];

            // print data
            {
                // print name
                ANVIL__print__tabs(tab_depth + 1);
                printf("Function '");
                COMPILER__print__accountling_function_header(((COMPILER__accountling_function_header*)functions.headers.category[COMPILER__afht__user_defined].list.buffer.start)[function_index]);
                printf("':\n");

                // print scopes
                if (function.scope_headers.count > 0) {
                    // print scope headers header
                    ANVIL__print__tabs(tab_depth + 2);
                    printf("Scopes:\n");
                    
                    // print each scope header
                    for (COMPILER__scope_index scope_index = 0; scope_index < function.scope_headers.count; scope_index++) {
                        // get scope
                        COMPILER__accountling_scope_header scope_header = ((COMPILER__accountling_scope_header*)function.scope_headers.list.buffer.start)[scope_index];

                        // print scope data
                        ANVIL__print__tabs(tab_depth + 3);
                        COMPILER__print__namespace(scope_header.name);
                        printf("\n");
                    }
                }

                // print offsets
                if (function.offsets.count > 0) {
                    // print offsets header
                    ANVIL__print__tabs(tab_depth + 2);
                    printf("Offsets:\n");
                    
                    // print each offset
                    for (COMPILER__offset_index offset_index = 0; offset_index < function.offsets.count; offset_index++) {
                        // get offset
                        COMPILER__namespace offset = ((COMPILER__namespace*)function.offsets.list.buffer.start)[offset_index];

                        // print scope data
                        ANVIL__print__tabs(tab_depth + 3);
                        COMPILER__print__namespace(offset);
                        printf("\n");
                    }
                }

                // print variable declarations
                if (function.variables.masters.count > 0) {
                    // print header
                    ANVIL__print__tabs(tab_depth + 2);
                    printf("Variables [ cells_used: %lu/%lu (%lu->%lu) ]:\n", function.next_available_workspace_cell - (ANVIL__u64)ANVIL__srt__start__workspace, (ANVIL__u64)ANVIL__srt__workspace__COUNT, (ANVIL__u64)ANVIL__srt__start__workspace, (ANVIL__u64)ANVIL__srt__start__function_io - 1);

                    // print each master & member variables
                    for (COMPILER__variable_index index = 0; index < function.variables.masters.count; index++) {
                        // get variable
                        COMPILER__accountling_variable master_variable = ((COMPILER__accountling_variable*)function.variables.masters.list.buffer.start)[index];

                        // print master & member variables
                        COMPILER__print__accountling_variable(function, structures, master_variable, index, tab_depth + 3);
                    }
                }

                // print statements
                if (function.scope.statements.count > 0) {
                    // print statements header
                    ANVIL__print__tabs(tab_depth + 2);
                    printf("Statements:\n");
                    
                    // print statements
                    COMPILER__print__accountling_scope(function.scope, tab_depth + 2);
                }

                // print strings
                if (function.strings.count > 0) {
                    // print strings header
                    ANVIL__print__tabs(tab_depth + 2);
                    printf("Strings:\n");
                    
                    // print each string
                    for (COMPILER__string_index string_index = 0; string_index < function.strings.count; string_index++) {
                        // get string
                        ANVIL__buffer string = ((ANVIL__buffer*)function.strings.list.buffer.start)[string_index];

                        // print string data
                        ANVIL__print__tabs(tab_depth + 3);
                        printf("[ %lu ] \"", string_index);
                        ANVIL__print__buffer(string);
                        printf("\"\n");
                    }
                }
            }
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
    COMPILER__print__accountling_functions(program.structures, program.functions, 1);

    return;
}

#endif
