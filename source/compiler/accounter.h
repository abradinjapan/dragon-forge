#ifndef DRAGON__compiler__accounter
#define DRAGON__compiler__accounter

/* Include */
#include "parser.h"

/* Accounting */
// predefined variable type
typedef enum ANVIL__pvt {
    // variables
    ANVIL__pvt__error_code,
    ANVIL__pvt__constant__character_byte_size,
    ANVIL__pvt__constant__character_bit_size,
    ANVIL__pvt__constant__bits_in_byte,
    ANVIL__pvt__constant__cell_byte_size,
    ANVIL__pvt__constant__cell_bit_size,
    ANVIL__pvt__constant__true,
    ANVIL__pvt__constant__false,
    ANVIL__pvt__constant__0,
    ANVIL__pvt__constant__1,
    ANVIL__pvt__constant__2,
    ANVIL__pvt__constant__4,
    ANVIL__pvt__constant__8,
    ANVIL__pvt__constant__16,
    ANVIL__pvt__constant__24,
    ANVIL__pvt__constant__32,
    ANVIL__pvt__constant__40,
    ANVIL__pvt__constant__48,
    ANVIL__pvt__constant__56,
    ANVIL__pvt__constant__64,
    ANVIL__pvt__constant__input_start,
    ANVIL__pvt__constant__input_end,
    ANVIL__pvt__constant__output_start,
    ANVIL__pvt__constant__output_end,
    ANVIL__pvt__stack_start,
    ANVIL__pvt__stack_current,
    ANVIL__pvt__stack_end,

    // count
    ANVIL__pvt__COUNT,
} ANVIL__pvt;

// predefined flag type
typedef enum ANVIL__pft {
    // flags
    ANVIL__pft__always_run,
    ANVIL__pft__never_run,
    ANVIL__pft__temporary,

    // count
    ANVIL__pft__COUNT,
} ANVIL__pft;

// accountling argument
typedef struct ANVIL__accountling_argument {
    ANVIL__pat type;
    ANVIL__accountling_index index;
    ANVIL__parsling_argument value;
} ANVIL__accountling_argument;

// create accountling argument
ANVIL__accountling_argument ANVIL__create__accountling_argument(ANVIL__pat type, ANVIL__accountling_index index, ANVIL__parsling_argument value) {
    ANVIL__accountling_argument output;

    output.type = type;
    output.index = index;
    output.value = value;

    return output;
}

// create null accountling argument
ANVIL__accountling_argument ANVIL__create_null__accountling_argument() {
    return ANVIL__create__accountling_argument(ANVIL__pat__invalid, ANVIL__define__null_call_ID, ANVIL__create_null__parsling_argument());
}

// append accountling argument
void ANVIL__append__accountling_argument(ANVIL__list* list, ANVIL__accountling_argument data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__accountling_argument), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__accountling_argument*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__accountling_argument);

    return;
}

// blueprintling
typedef ANVIL__u64 ANVIL__blueprintling;

// accountling call type
typedef enum ANVIL__act {
    // start
    ANVIL__act__START = 0,

    // acts
    // sets
    ANVIL__act__set__boolean = ANVIL__act__START,
    ANVIL__act__set__binary,
    ANVIL__act__set__integer,
    ANVIL__act__set__hexadecimal,
    ANVIL__act__set__offset,
    ANVIL__act__set__flag_ID,
    ANVIL__act__set__string,

    // io
    ANVIL__act__io__cell_to_address,
    ANVIL__act__io__address_to_cell,
    ANVIL__act__io__file_to_buffer,
    ANVIL__act__io__buffer_to_file,

    // copy
    ANVIL__act__copy__cell,
    ANVIL__act__copy__buffer,

    // memory
    ANVIL__act__memory__request_memory,
    ANVIL__act__memory__return_memory,

    // buffers
    ANVIL__act__buffer__calculate_length,

    // casts
    ANVIL__act__cast__cell_to_unsigned_integer_string,

    // prints
    ANVIL__act__print__binary,
    ANVIL__act__print__signed_integer,
    ANVIL__act__print__unsigned_integer,
    ANVIL__act__print__character,
    ANVIL__act__print__buffer_as_string,

    // integers
    ANVIL__act__integer__add,
    ANVIL__act__integer__subtract,
    ANVIL__act__integer__multiply,
    ANVIL__act__integer__divide,
    ANVIL__act__integer__modulous,
    ANVIL__act__integer__within_range,

    // binary
    ANVIL__act__binary__or,
    ANVIL__act__binary__invert,
    ANVIL__act__binary__and,
    ANVIL__act__binary__xor,
    ANVIL__act__binary__shift_higher,
    ANVIL__act__binary__shift_lower,
    ANVIL__act__binary__overwrite,

    // flags
    ANVIL__act__flag__get,
    ANVIL__act__flag__set,
    ANVIL__act__flag__invert,
    ANVIL__act__flag__or,
    ANVIL__act__flag__and,
    ANVIL__act__flag__xor,

    // jumps
    ANVIL__act__jump,

    // rtcg
    ANVIL__act__open_context,
    ANVIL__act__compile,
    ANVIL__act__run,

    // etc
    ANVIL__act__reset_error_code_cell,
    ANVIL__act__get_program_inputs,
    ANVIL__act__set_program_outputs,
    ANVIL__act__set_context_buffer_inputs,
    ANVIL__act__get_context_buffer_outputs,

    // end
    ANVIL__act__END,

    // user defined
    ANVIL__act__user_defined = ANVIL__act__END,

    // invalid
    ANVIL__act__invalid,

    // count
    ANVIL__act__COUNT = ANVIL__act__END - ANVIL__act__START,
} ANVIL__act;

// calculate user defined call ID
ANVIL__abstraction_index ANVIL__calculate__abstraction_index_from_call_index(ANVIL__call_index call_index) {
    return call_index - ANVIL__act__user_defined;
}

// accountling variable type (all predefined variables)
typedef enum ANVIL__avt {
    // start
    ANVIL__avt__START = 0,

    // avts
    ANVIL__avt__invalid = ANVIL__avt__START,
    ANVIL__avt__error_code,

    // end
    ANVIL__avt__END,

    // user defined
    ANVIL__avt__user_defined = ANVIL__avt__END,

    // count
    ANVIL__avt__COUNT = ANVIL__avt__END - ANVIL__avt__START,
} ANVIL__avt;

// accountling header
typedef struct ANVIL__accountling_abstraction_header {
    ANVIL__parsling_statement header;
    ANVIL__call_index call_index;
} ANVIL__accountling_abstraction_header;

// create custom accountling abstraction header
ANVIL__accountling_abstraction_header ANVIL__create__accountling_abstraction_header(ANVIL__parsling_statement header, ANVIL__call_index call_index) {
    ANVIL__accountling_abstraction_header output;

    output.header = header;
    output.call_index = call_index;

    return output;
}

// create null accountling abstraction header
ANVIL__accountling_abstraction_header ANVIL__create_null__accountling_abstraction_header() {
    return ANVIL__create__accountling_abstraction_header(ANVIL__create_null__parsling_statement(), ANVIL__act__invalid);
}

// close accountling abstraction header
void ANVIL__close__accountling_abstraction_header(ANVIL__accountling_abstraction_header header) {
    // close header
    ANVIL__close__parsling_statement(header.header);

    return;
}

// accountling statement
typedef struct ANVIL__accountling_statement {
    // type
    ANVIL__stt type;

    // call
    ANVIL__accountling_abstraction_header header;
    ANVIL__list inputs; // ANVIL__accountling_argument
    ANVIL__list outputs; // ANVIL__accountling_argument

    // offset
    ANVIL__offset_index offset_ID;
} ANVIL__accountling_statement;

// setup custom accountling statement
ANVIL__accountling_statement ANVIL__create__accountling_statement(ANVIL__stt type, ANVIL__accountling_abstraction_header header, ANVIL__list inputs, ANVIL__list outputs, ANVIL__offset_index offset_ID) {
    ANVIL__accountling_statement output;

    output.type = type;
    output.header = header;
    output.inputs = inputs;
    output.outputs = outputs;
    output.offset_ID = offset_ID;

    return output;
}

// create null accountling statement
ANVIL__accountling_statement ANVIL__create_null__accountling_statement() {
    return ANVIL__create__accountling_statement(ANVIL__stt__invalid, ANVIL__create_null__accountling_abstraction_header(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__define__null_offset_ID);
}

// close statement
void ANVIL__close__accountling_statement(ANVIL__accountling_statement statement) {
    // close io
    if (statement.type == ANVIL__stt__abstraction_call) {
        ANVIL__close__list(statement.inputs);
        ANVIL__close__list(statement.outputs);
    }

    return;
}

// get statement input by index
ANVIL__accountling_argument ANVIL__get__abstractling_statement_argument_by_index(ANVIL__list list, ANVIL__argument_index index) {
    return ((ANVIL__accountling_argument*)list.buffer.start)[index];
}

// accountling abstraction
typedef struct ANVIL__accountling_abstraction {
    ANVIL__parsling_statement header;
    ANVIL__list* predefined_variables; // ANVIL__parsling_argument
    ANVIL__list* predefined_flags; // ANVIL__parsling_argument
    ANVIL__list strings; // ANVIL__parsling_argument
    ANVIL__list converted_strings; // ANVIL__buffer
    ANVIL__list inputs; // ANVIL__parsling_argument
    ANVIL__list outputs; // ANVIL__parsling_argument
    ANVIL__list variables; // ANVIL__parsling_argument
    ANVIL__list offsets; // ANVIL__parsling_argument
    ANVIL__list flags; // ANVIL__parsling_argument
    ANVIL__list statements; // ANVIL__accountling_statement
} ANVIL__accountling_abstraction;

// create custom accountling abstraction
ANVIL__accountling_abstraction ANVIL__create__accountling_abstraction(ANVIL__parsling_statement header, ANVIL__list* predefined_variables, ANVIL__list* predefined_flags, ANVIL__list strings, ANVIL__list converted_strings, ANVIL__list inputs, ANVIL__list outputs, ANVIL__list variables, ANVIL__list offsets, ANVIL__list flags, ANVIL__list statements) {
    ANVIL__accountling_abstraction output;

    output.header = header;
    output.predefined_variables = predefined_variables;
    output.predefined_flags = predefined_flags;
    output.strings = strings;
    output.converted_strings = converted_strings;
    output.inputs = inputs;
    output.outputs = outputs;
    output.variables = variables;
    output.offsets = offsets;
    output.flags = flags;
    output.statements = statements;

    return output;
}

// create null accountling abstraction
ANVIL__accountling_abstraction ANVIL__create_null__accountling_abstraction() {
    return ANVIL__create__accountling_abstraction(ANVIL__create_null__parsling_statement(), ANVIL__define__null_address, ANVIL__define__null_address, ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list());
}

// append accountling statement
void ANVIL__append__accountling_statement(ANVIL__list* list, ANVIL__accountling_statement data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__accountling_statement), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__accountling_statement*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__accountling_statement);

    return;
}

// append accountling abstraction
void ANVIL__append__accountling_abstraction(ANVIL__list* list, ANVIL__accountling_abstraction data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__accountling_abstraction), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__accountling_abstraction*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__accountling_abstraction);

    return;
}

// close statements
void ANVIL__close__accountling_statements(ANVIL__list statements) {
    // check for empty statements
    if (ANVIL__check__empty_list(statements)) {
        // no need to free anything
        return;
    }

    // setup current
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&statements);

    // for each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // close statement
        ANVIL__close__accountling_statement(*(ANVIL__accountling_statement*)current_statement.start);

        // next statement
        current_statement.start += sizeof(ANVIL__accountling_statement);
    }

    // close lists
    ANVIL__close__list(statements);

    return;
}

// close accountling abstraction
void ANVIL__close__accountling_abstraction(ANVIL__accountling_abstraction abstraction) {
    // close lists
    if (ANVIL__check__empty_list(abstraction.inputs) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.inputs);
    }
    if (ANVIL__check__empty_list(abstraction.outputs) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.outputs);
    }
    if (ANVIL__check__empty_list(abstraction.variables) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.variables);
    }
    if (ANVIL__check__empty_list(abstraction.offsets) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.offsets);
    }
    if (ANVIL__check__empty_list(abstraction.flags) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.flags);
    }
    if (ANVIL__check__empty_list(abstraction.strings) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.strings);
    }
    if (ANVIL__check__empty_list(abstraction.converted_strings) == ANVIL__bt__false) {
        ANVIL__current current_buffer = ANVIL__calculate__current_from_list_filled_index(&abstraction.converted_strings);

        // for each buffer
        while (ANVIL__check__current_within_range(current_buffer)) {
            // free buffer
            ANVIL__close__buffer(*(ANVIL__buffer*)current_buffer.start);

            // next buffer
            current_buffer.start += sizeof(ANVIL__buffer);
        }

        // close list
        ANVIL__close__list(abstraction.converted_strings);
    }

    // close statements
    ANVIL__close__accountling_statements(abstraction.statements);

    return;
}

// accountling blueprint type
typedef enum ANVIL__abt {
    // start
    ANVIL__abt__START = ANVIL__act__END,

    // types
    ANVIL__abt__define_call = ANVIL__abt__START,
    ANVIL__abt__end_of_blueprint,

    // end
    ANVIL__abt__END,

    // count
    ANVIL__abt__COUNT = ANVIL__abt__END - ANVIL__abt__START,
} ANVIL__abt;

// blueprint name index type
typedef enum ANVIL__bnit {
    // names
    ANVIL__bnit__set,
    ANVIL__bnit__print__signed_integer,
    ANVIL__bnit__print__unsigned_integer,
    ANVIL__bnit__print__character,
    ANVIL__bnit__print__buffer_as_string,
    ANVIL__bnit__print__binary,
    ANVIL__bnit__io__cell_to_address,
    ANVIL__bnit__io__address_to_cell,
    ANVIL__bnit__io__file_to_buffer,
    ANVIL__bnit__io__buffer_to_file,
    ANVIL__bnit__copy__cell,
    ANVIL__bnit__copy__buffer,
    ANVIL__bnit__memory__request_memory,
    ANVIL__bnit__memory__return_memory,
    ANVIL__bnit__buffer__calculate_length,
    ANVIL__bnit__cast__cell_to_unsigned_integer_string,
    ANVIL__bnit__integer_add,
    ANVIL__bnit__integer_subtract,
    ANVIL__bnit__integer_multiply,
    ANVIL__bnit__integer_divide,
    ANVIL__bnit__integer_modulous,
    ANVIL__bnit__integer_within_range,
    ANVIL__bnit__binary__or,
    ANVIL__bnit__binary__invert,
    ANVIL__bnit__binary__and,
    ANVIL__bnit__binary__xor,
    ANVIL__bnit__binary__shift_higher,
    ANVIL__bnit__binary__shift_lower,
    ANVIL__bnit__binary__overwrite,
    ANVIL__bnit__flag__get,
    ANVIL__bnit__flag__set,
    ANVIL__bnit__flag__invert,
    ANVIL__bnit__flag__or,
    ANVIL__bnit__flag__and,
    ANVIL__bnit__flag__xor,
    ANVIL__bnit__jump,
    ANVIL__bnit__open_context,
    ANVIL__bnit__compile,
    ANVIL__bnit__run,
    ANVIL__bnit__reset_error_code,
    ANVIL__bnit__get_program_inputs,
    ANVIL__bnit__set_program_outputs,
    ANVIL__bnit__set_context_buffer_inputs,
    ANVIL__bnit__get_context_buffer_outputs,

    // stats
    ANVIL__bnit__COUNT,
} ANVIL__bnit;

// append accountling abstraction header
void ANVIL__append__accountling_abstraction_header(ANVIL__list* list, ANVIL__accountling_abstraction_header data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__accountling_abstraction_header), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__accountling_abstraction_header*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__accountling_abstraction_header);

    return;
}

// get and validate one header
ANVIL__accountling_abstraction_header ANVIL__account__get_abstraction_header(ANVIL__parsling_abstraction abstraction, ANVIL__call_index abstraction_call_ID, ANVIL__error* error) {
    ANVIL__accountling_abstraction_header output;

    output.header = abstraction.header;
    output.call_index = ANVIL__act__user_defined + abstraction_call_ID;

    // validate inputs
    for (ANVIL__length i = 0; i < output.header.input_count; i++) {
        // get argument
        ANVIL__parsling_argument argument = ((ANVIL__parsling_argument*)output.header.inputs.buffer.start)[i];

        // check argument validity
        if (argument.type != ANVIL__pat__variable) {
            *error = ANVIL__open__error("Accounting Error: An abstraction input header argument contains illegal argument type.", argument.text.lexling.location);

            return output;
        }
    }

    // validate outputs
    for (ANVIL__length i = 0; i < output.header.output_count; i++) {
        // get argument
        ANVIL__parsling_argument argument = ((ANVIL__parsling_argument*)output.header.outputs.buffer.start)[i];

        // check argument validity
        if (argument.type != ANVIL__pat__variable) {
            *error = ANVIL__open__error("Accounting Error: An abstraction output header argument contains illegal argument type.", argument.text.lexling.location);

            return output;
        }
    }

    return output;
}

// calculate blueprint list size
ANVIL__header_index ANVIL__calculate__call_blueprint_entry_count(ANVIL__list call_blueprint) {
    return call_blueprint.filled_index / sizeof(ANVIL__accountling_abstraction_header);
}

// check if argument is in variable category
ANVIL__bt ANVIL__check__argument_is_variable_type(ANVIL__pat argument_type) {
    return (argument_type == ANVIL__pat__variable) || (argument_type == ANVIL__pat__variable__body) || (argument_type == ANVIL__pat__variable__input) || (argument_type == ANVIL__pat__variable__output) || (argument_type == ANVIL__pat__variable__predefined);
}

// check if argument is in flag category
ANVIL__bt ANVIL__check__argument_is_flag_type(ANVIL__pat argument_type) {
    return (argument_type == ANVIL__pat__flag) || (argument_type == ANVIL__pat__flag__user_defined) || (argument_type == ANVIL__pat__flag__predefined);
}

// check if an accountling already exists
ANVIL__header_index ANVIL__find__accountling_header_index(ANVIL__list call_blueprint, ANVIL__parsling_statement searching_for) {
    // search the blueprint for the correct header
    // setup current
    ANVIL__current current_header = ANVIL__calculate__current_from_list_filled_index(&call_blueprint);
    ANVIL__header_index header_index = 0;

    // search for match
    while (ANVIL__check__current_within_range(current_header)) {
        // get current header
        ANVIL__accountling_abstraction_header header = *(ANVIL__accountling_abstraction_header*)current_header.start;

        // check if abstraction matches
        if(ANVIL__check__parsling_arguments_have_same_text(header.header.name, searching_for.name)) {
            // check if io is same size
            if ((ANVIL__calculate__lists_have_same_fill_size(&searching_for.inputs, &header.header.inputs) && ANVIL__calculate__lists_have_same_fill_size(&searching_for.outputs, &header.header.outputs)) == ANVIL__bt__false) {
                goto next_header;
            }

            // setup currents for input
            ANVIL__current current_statement_io = ANVIL__calculate__current_from_list_filled_index(&searching_for.inputs);
            ANVIL__current current_header_io = ANVIL__calculate__current_from_list_filled_index(&header.header.inputs);

            // if inputs are not empty
            if (ANVIL__check__empty_buffer(current_statement_io) == ANVIL__bt__false) {
                // check inputs
                while (ANVIL__check__current_within_range(current_statement_io) && ANVIL__check__current_within_range(current_header_io)) {
                    // get types
                    ANVIL__pat statement_io_type = (*(ANVIL__parsling_argument*)current_statement_io.start).type;
                    ANVIL__pat header_io_type = (*(ANVIL__parsling_argument*)current_header_io.start).type;

                    // check types
                    // if type is in variable category
                    if (ANVIL__check__argument_is_variable_type(statement_io_type) && ANVIL__check__argument_is_variable_type(header_io_type)) {
                        // next input
                        goto next_input;
                    } else if (ANVIL__check__argument_is_flag_type(statement_io_type) && ANVIL__check__argument_is_flag_type(header_io_type)) {
                        // next input
                        goto next_input;
                    } else if (statement_io_type != header_io_type) {
                        // not a match
                        goto next_header;
                    }

                    // next input
                    next_input:
                    current_statement_io.start += sizeof(ANVIL__parsling_argument);
                    current_header_io.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // re-setup currents
            current_statement_io = ANVIL__calculate__current_from_list_filled_index(&searching_for.outputs);
            current_header_io = ANVIL__calculate__current_from_list_filled_index(&header.header.outputs);

            // if outputs are not empty
            if (ANVIL__check__empty_buffer(current_statement_io) == ANVIL__bt__false) {
                // check outputs
                while (ANVIL__check__current_within_range(current_statement_io) && ANVIL__check__current_within_range(current_header_io)) {
                    // get types
                    ANVIL__pat statement_io_type = (*(ANVIL__parsling_argument*)current_statement_io.start).type;
                    ANVIL__pat header_io_type = (*(ANVIL__parsling_argument*)current_header_io.start).type;

                    // check types
                    // if type is in variable category
                    if (ANVIL__check__argument_is_variable_type(statement_io_type) && ANVIL__check__argument_is_variable_type(header_io_type)) {
                        // next output
                        goto next_output;
                    } else if (ANVIL__check__argument_is_flag_type(statement_io_type) && ANVIL__check__argument_is_flag_type(header_io_type)) {
                        // next output
                        goto next_output;
                    } else if (statement_io_type != header_io_type) {
                        // not a match
                        goto next_header;
                    }

                    // next output
                    next_output:
                    current_statement_io.start += sizeof(ANVIL__parsling_argument);
                    current_header_io.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // match!
            return header_index;
        }

        // next header
        next_header:
        current_header.start += sizeof(ANVIL__accountling_abstraction_header);
        header_index++;
    }

    // match not found
    return ANVIL__calculate__call_blueprint_entry_count(call_blueprint);
}

// create call blueprint
ANVIL__list ANVIL__generate__call_blueprint(ANVIL__list parsling_programs, ANVIL__error* error) {
    const ANVIL__blueprintling blueprint[] = {
        // sets
        ANVIL__abt__define_call,
            ANVIL__act__set__boolean,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__boolean,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__binary,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__binary,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__integer,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__integer,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__hexadecimal,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__hexadecimal,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__offset,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__offset,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__flag_ID,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__string,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__string,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        
        // io
        ANVIL__abt__define_call,
            ANVIL__act__io__cell_to_address,
            ANVIL__bnit__io__cell_to_address,
            3,
            ANVIL__pat__flag,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__io__address_to_cell,
            ANVIL__bnit__io__address_to_cell,
            3,
            ANVIL__pat__flag,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__io__file_to_buffer,
            ANVIL__bnit__io__file_to_buffer,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__io__buffer_to_file,
            ANVIL__bnit__io__buffer_to_file,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        
        // copies
        ANVIL__abt__define_call,
            ANVIL__act__copy__cell,
            ANVIL__bnit__copy__cell,
            1,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__copy__buffer,
            ANVIL__bnit__copy__buffer,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        
        // memory
        ANVIL__abt__define_call,
            ANVIL__act__memory__request_memory,
            ANVIL__bnit__memory__request_memory,
            1,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__memory__return_memory,
            ANVIL__bnit__memory__return_memory,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            0,
        
        // buffers
        ANVIL__abt__define_call,
            ANVIL__act__buffer__calculate_length,
            ANVIL__bnit__buffer__calculate_length,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        
        // casting
        ANVIL__abt__define_call,
            ANVIL__act__cast__cell_to_unsigned_integer_string,
            ANVIL__bnit__cast__cell_to_unsigned_integer_string,
            1,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        
        // printing
        ANVIL__abt__define_call,
            ANVIL__act__print__signed_integer,
            ANVIL__bnit__print__signed_integer,
            1,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__print__unsigned_integer,
            ANVIL__bnit__print__unsigned_integer,
            1,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__print__character,
            ANVIL__bnit__print__character,
            1,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__print__buffer_as_string,
            ANVIL__bnit__print__buffer_as_string,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__print__binary,
            ANVIL__bnit__print__binary,
            1,
            ANVIL__pat__variable,
            0,
        
        // integer operations
        ANVIL__abt__define_call,
            ANVIL__act__integer__add,
            ANVIL__bnit__integer_add,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__subtract,
            ANVIL__bnit__integer_subtract,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__multiply,
            ANVIL__bnit__integer_multiply,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__divide,
            ANVIL__bnit__integer_divide,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__modulous,
            ANVIL__bnit__integer_modulous,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__within_range,
            ANVIL__bnit__integer_within_range,
            4,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        
        // binary operations
        ANVIL__abt__define_call,
            ANVIL__act__binary__or,
            ANVIL__bnit__binary__or,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__invert,
            ANVIL__bnit__binary__invert,
            1,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__and,
            ANVIL__bnit__binary__and,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__xor,
            ANVIL__bnit__binary__xor,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__shift_higher,
            ANVIL__bnit__binary__shift_higher,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__shift_lower,
            ANVIL__bnit__binary__shift_lower,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__overwrite,
            ANVIL__bnit__binary__overwrite,
            3,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,

        // flags
        ANVIL__abt__define_call,
            ANVIL__act__flag__get,
            ANVIL__bnit__flag__get,
            1,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__flag__set,
            ANVIL__bnit__flag__set,
            1,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__flag,
        ANVIL__abt__define_call,
            ANVIL__act__flag__invert,
            ANVIL__bnit__flag__invert,
            1,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        ANVIL__abt__define_call,
            ANVIL__act__flag__or,
            ANVIL__bnit__flag__or,
            2,
            ANVIL__pat__flag,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        ANVIL__abt__define_call,
            ANVIL__act__flag__and,
            ANVIL__bnit__flag__and,
            2,
            ANVIL__pat__flag,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        ANVIL__abt__define_call,
            ANVIL__act__flag__xor,
            ANVIL__bnit__flag__xor,
            2,
            ANVIL__pat__flag,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        
        // jumps
        ANVIL__abt__define_call,
            ANVIL__act__jump,
            ANVIL__bnit__jump,
            2,
            ANVIL__pat__flag,
            ANVIL__pat__offset,
            0,

        // run time code generation
        ANVIL__abt__define_call,
            ANVIL__act__open_context,
            ANVIL__bnit__open_context,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__compile,
            ANVIL__bnit__compile,
            3,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            8,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__run,
            ANVIL__bnit__run,
            3,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            0,
        
        // etc
        ANVIL__abt__define_call,
            ANVIL__act__reset_error_code_cell,
            ANVIL__bnit__reset_error_code,
            0,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__get_program_inputs,
            ANVIL__bnit__get_program_inputs,
            0,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set_program_outputs,
            ANVIL__bnit__set_program_outputs,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__set_context_buffer_inputs,
            ANVIL__bnit__set_context_buffer_inputs,
            4,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__get_context_buffer_outputs,
            ANVIL__bnit__get_context_buffer_outputs,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        
        // end of blueprint
        ANVIL__abt__end_of_blueprint,
    };

    // open output
    ANVIL__list output = ANVIL__open__list_with_error(sizeof(ANVIL__accountling_abstraction_header) * 256, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // setup blueprint current
    ANVIL__current current_blueprintling = ANVIL__create__buffer((ANVIL__address)blueprint, (ANVIL__address)(blueprint + sizeof(blueprint) - 1));

    // create and add predefined headers to list
    while (ANVIL__check__current_within_range(current_blueprintling) && (*(ANVIL__blueprintling*)current_blueprintling.start) == ANVIL__abt__define_call) {
        // check for define call
        if ((*(ANVIL__blueprintling*)current_blueprintling.start) == ANVIL__abt__define_call) {
            // advance current
            current_blueprintling.start += sizeof(ANVIL__blueprintling);
        } else {
            *error = ANVIL__open__internal_memory_error();

            return output;
        }

        // open new header
        ANVIL__accountling_abstraction_header header;
        
        // set statement type
        header.header.type = ANVIL__stt__abstraction_header;

        // set call type
        header.call_index = (ANVIL__act)(*(ANVIL__blueprintling*)current_blueprintling.start);
        current_blueprintling.start += sizeof(ANVIL__blueprintling);

        // set name
        header.header.name = ANVIL__create__parsling_argument(ANVIL__pat__variable, ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__name, ANVIL__open__buffer_from_string((ANVIL__u8*)ANVIL__global__accountling_call_type_name_strings[*(ANVIL__blueprintling*)current_blueprintling.start], ANVIL__bt__false, ANVIL__bt__false), ANVIL__create_null__character_location())), 0);
        current_blueprintling.start += sizeof(ANVIL__blueprintling);

        // get input count
        header.header.input_count = (*(ANVIL__blueprintling*)current_blueprintling.start);
        current_blueprintling.start += sizeof(ANVIL__blueprintling);

        // open inputs
        header.header.inputs = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 8, error);

        // get inputs
        for (ANVIL__blueprintling i = 0; i < header.header.input_count; i++) {
            // add argument
            ANVIL__append__parsling_argument(&header.header.inputs, ANVIL__create__parsling_argument((ANVIL__pat)(*(ANVIL__blueprintling*)current_blueprintling.start), ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__name, ANVIL__open__buffer_from_string((u8*)"(internal)", ANVIL__bt__false, ANVIL__bt__false), ANVIL__create_null__character_location())), 0), error);

            // next blueprintling
            current_blueprintling.start += sizeof(ANVIL__blueprintling);
        }

        // get output count
        header.header.output_count = (*(ANVIL__blueprintling*)current_blueprintling.start);
        current_blueprintling.start += sizeof(ANVIL__blueprintling);

        // open outputs
        header.header.outputs = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 8, error);

        // get outputs
        for (ANVIL__blueprintling i = 0; i < header.header.output_count; i++) {
            // add argument
            ANVIL__append__parsling_argument(&header.header.outputs, ANVIL__create__parsling_argument((ANVIL__pat)(*(ANVIL__blueprintling*)current_blueprintling.start), ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__name, ANVIL__open__buffer_from_string((u8*)"(internal)", ANVIL__bt__false, ANVIL__bt__false), ANVIL__create_null__character_location())), 0), error);

            // next blueprintling
            current_blueprintling.start += sizeof(ANVIL__blueprintling);
        }

        // append header
        ANVIL__append__accountling_abstraction_header(&output, header, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
    }

    // get all user defined headers
    // setup current
    ANVIL__current current_file = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // setup abstraction call ID
    ANVIL__call_index abstraction_call_ID = 0;

    // across all files
    while (ANVIL__check__current_within_range(current_file)) {
        // setup current
        ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&(*(ANVIL__parsling_program*)current_file.start).abstractions);

        // get each abstraction header
        while (ANVIL__check__current_within_range(current_abstraction)) {
            // get abstraction header
            ANVIL__accountling_abstraction_header header = ANVIL__account__get_abstraction_header(*(ANVIL__parsling_abstraction*)current_abstraction.start, abstraction_call_ID, error);

            // next abstraction call ID
            abstraction_call_ID++;

            // check for error
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // check to see if header already exists
            if (ANVIL__find__accountling_header_index(output, header.header) < ANVIL__calculate__call_blueprint_entry_count(output)) {
                // setup error
                *error = ANVIL__open__error("Accounting Error: An abstraction is already defined.", header.header.name.text.lexling.location);

                return output;
            }

            // append header
            ANVIL__append__accountling_abstraction_header(&output, header, error);
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // next abstraction
            current_abstraction.start += sizeof(ANVIL__parsling_abstraction);
        }

        // next file
        current_file.start += sizeof(ANVIL__parsling_program);
    }

    return output;
}

// verify all statements across all files
void ANVIL__account__verify_all_calls(ANVIL__list parsling_programs, ANVIL__list call_blueprint, ANVIL__error* error) {
    // setup current file
    ANVIL__current current_file = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // across all files
    while (ANVIL__check__current_within_range(current_file)) {
        // setup current abstraction
        ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&(*(ANVIL__parsling_program*)current_file.start).abstractions);

        // across all abstractions
        while (ANVIL__check__current_within_range(current_abstraction)) {
            // setup current statment
            ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&(*(ANVIL__parsling_abstraction*)current_abstraction.start).statements);

            // across all statements
            while (ANVIL__check__current_within_range(current_statement)) {
                // setup header to search for
                ANVIL__parsling_statement searching_for = *(ANVIL__parsling_statement*)current_statement.start;

                // if the statement is a call
                if (searching_for.type == ANVIL__stt__abstraction_call) {
                    // verify header exists
                    if (ANVIL__find__accountling_header_index(call_blueprint, searching_for) >= ANVIL__calculate__call_blueprint_entry_count(call_blueprint)) {
                        // does not exist
                        *error = ANVIL__open__error("Accounting Error: A statement calls a non-existent abstraction / instruction.", searching_for.name.text.lexling.location);

                        return;
                    }
                }

                // next statement
                current_statement.start += sizeof(ANVIL__parsling_statement);
            }

            // next abstraction
            current_abstraction.start += sizeof(ANVIL__parsling_abstraction);
        }

        // next file
        current_file.start += sizeof(ANVIL__parsling_program);
    }

    // no errors, valid
    return;
}

// accountling program
typedef struct ANVIL__accountling_program {
    ANVIL__list call_blueprint; // ANVIL__accountling_abstraction_header
    ANVIL__list abstractions; // ANVIL__accountling_abstraction
    ANVIL__list predefined_variables; // ANVIL__parsling_argument
    ANVIL__list predefined_flags; // ANVIL__parsling_argument
    ANVIL__call_index main_abstraction_ID;
} ANVIL__accountling_program;

// create custom accountling program
ANVIL__accountling_program ANVIL__create__accountling_program(ANVIL__list call_blueprint, ANVIL__list abstractions, ANVIL__list predefined_variables, ANVIL__list predefined_flags, ANVIL__call_index main_abstraction_ID) {
    ANVIL__accountling_program output;

    output.call_blueprint = call_blueprint;
    output.abstractions = abstractions;
    output.predefined_variables = predefined_variables;
    output.predefined_flags = predefined_flags;
    output.main_abstraction_ID = main_abstraction_ID;

    return output;
}

// create null accountling program
ANVIL__accountling_program ANVIL__create_null__accountling_program() {
    return ANVIL__create__accountling_program(ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__define__null_call_ID);
}

// close accountling program
void ANVIL__close__accountling_program(ANVIL__accountling_program program) {
    // close predefined blueprints
    // get current
    ANVIL__current current_header = ANVIL__calculate__current_from_list_filled_index(&program.call_blueprint);

    // close all predefined blueprints
    while (ANVIL__check__current_within_range(current_header)) {
        // get header
        ANVIL__accountling_abstraction_header header = *(ANVIL__accountling_abstraction_header*)current_header.start;

        // if a predefined header
        if (header.call_index < ANVIL__act__user_defined) {
            // close header
            ANVIL__close__accountling_abstraction_header(header);
        }

        // next header
        current_header.start += sizeof(ANVIL__accountling_abstraction_header);
    }

    // close blueprint list
    ANVIL__close__list(program.call_blueprint);

    // close abstractions
    // check abstraction list exists
    if (ANVIL__check__empty_list(program.abstractions)) {
        return;
    }

    // get current
    ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&program.abstractions);

    // close each abstraction
    while (ANVIL__check__current_within_range(current_abstraction)) {
        ANVIL__close__accountling_abstraction(*(ANVIL__accountling_abstraction*)current_abstraction.start);

        // next abstraction
        current_abstraction.start += sizeof(ANVIL__accountling_abstraction);
    }

    // close abstraction list
    ANVIL__close__list(program.abstractions);

    // close predefineds
    ANVIL__close__list(program.predefined_variables);
    ANVIL__close__list(program.predefined_flags);

    return;
}

// convert string to converted string
ANVIL__buffer ANVIL__account__convert_string(ANVIL__parsling_argument string, ANVIL__error* error) {
    ANVIL__buffer output = ANVIL__create_null__buffer();
    ANVIL__length character_count = 0;

    // count characters
    for (ANVIL__character_index i = 1; i < ANVIL__calculate__buffer_length(string.text.lexling.value) - 1; i++) {
        // check for escape sequence
        if (((ANVIL__character*)string.text.lexling.value.start)[i] == '%') {
            // check for characters available
            if (i + 3 <= ANVIL__calculate__buffer_length(string.text.lexling.value) - 1) {
                // validate characters
                ANVIL__bt invalid_hexadecimal_character_1;
                ANVIL__bt invalid_hexadecimal_character_2;
                ANVIL__bt semi_colon_missing;
                ANVIL__translate__character_to_hexadecimal(((ANVIL__character*)string.text.lexling.value.start)[i + 1], &invalid_hexadecimal_character_1);
                ANVIL__translate__character_to_hexadecimal(((ANVIL__character*)string.text.lexling.value.start)[i + 2], &invalid_hexadecimal_character_2);
                semi_colon_missing = (ANVIL__bt)(((ANVIL__character*)string.text.lexling.value.start)[i + 3] != ';');

                // determine validity
                if (invalid_hexadecimal_character_1 != ANVIL__bt__false || invalid_hexadecimal_character_2 != ANVIL__bt__false || semi_colon_missing != ANVIL__bt__false) {
                    // invalid escape sequence
                    *error = ANVIL__open__error("Accounting Error: String literal has invalid escape sequences.", string.text.lexling.location);
                    
                    return output;
                }

                // skip past characters
                i += 3;
            // error
            } else {
                *error = ANVIL__open__error("Accounting Error: String literal has invalid escape sequences.", string.text.lexling.location);

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
        *error = ANVIL__open__internal_memory_error();

        return output;
    }

    // translate string
    character_count = 0;
    for (ANVIL__character_index i = 1; i < ANVIL__calculate__buffer_length(string.text.lexling.value) - 1; i++) {
        // check for escape sequence
        if (((ANVIL__character*)string.text.lexling.value.start)[i] == '%') {
            // validate characters
            ANVIL__bt invalid_hexadecimal_character_1;
            ANVIL__bt invalid_hexadecimal_character_2;
            ANVIL__character a = ANVIL__translate__character_to_hexadecimal(((ANVIL__character*)string.text.lexling.value.start)[i + 1], &invalid_hexadecimal_character_1);
            ANVIL__character b = ANVIL__translate__character_to_hexadecimal(((ANVIL__character*)string.text.lexling.value.start)[i + 2], &invalid_hexadecimal_character_2);

            // write character
            ((ANVIL__character*)output.start)[character_count] = b + (a << 4);
            
            // skip past characters
            i += 3;
        } else {
            // write character
            ((ANVIL__character*)output.start)[character_count] = ((ANVIL__character*)string.text.lexling.value.start)[i];
        }

        // next character
        character_count++;
    }

    return output;
}

// check to see if argument is in list
ANVIL__parsling_argument ANVIL__account__get_argument_in_list__by_text(ANVIL__list* arguments, ANVIL__parsling_argument searching_for, ANVIL__bt* found) {
    // setup current
    ANVIL__current current_argument = ANVIL__calculate__current_from_list_filled_index(arguments);

    // search for argument by name
    while (ANVIL__check__current_within_range(current_argument)) {
        // get current argument
        ANVIL__parsling_argument argument = *(ANVIL__parsling_argument*)current_argument.start;

        // if names are identical
        if (ANVIL__check__parsling_arguments_have_same_text(argument, searching_for)) {
            // found in list
            *found = ANVIL__bt__true;
            return argument;
        }

        // next argument
        current_argument.start += sizeof(ANVIL__parsling_argument);
    }

    // not in list
    *found = ANVIL__bt__false;
    return ANVIL__create_null__parsling_argument();;
}

// get accountling argument list
ANVIL__list ANVIL__account__accountling_argument_list(ANVIL__accountling_abstraction* abstraction, ANVIL__list parsling_arguments, ANVIL__error* error) {
    // open list
    ANVIL__list output = ANVIL__open__list_with_error(sizeof(ANVIL__accountling_argument) * 16, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // get arguments
    // setup current
    ANVIL__current current_argument = ANVIL__calculate__current_from_list_filled_index(&parsling_arguments);

    // get arguments
    while (ANVIL__check__current_within_range(current_argument)) {
        // get argument
        ANVIL__parsling_argument argument = *(ANVIL__parsling_argument*)current_argument.start;

        // create argument ID by type
        if (argument.type == ANVIL__pat__variable__input) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).inputs, argument), argument), error);
        } else if (argument.type == ANVIL__pat__variable__output) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).outputs, argument), argument), error);
        } else if (argument.type == ANVIL__pat__variable || argument.type == ANVIL__pat__variable__body) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).variables, argument), argument), error);
        } else if (argument.type == ANVIL__pat__literal__boolean || argument.type == ANVIL__pat__literal__binary || argument.type == ANVIL__pat__literal__integer || argument.type == ANVIL__pat__literal__hexadecimal) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, argument.value, argument), error);
        } else if (argument.type == ANVIL__pat__offset) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).offsets, argument), argument), error);
        } else if (argument.type == ANVIL__pat__variable__predefined) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name(*(*abstraction).predefined_variables, argument), argument), error);
        } else if (argument.type == ANVIL__pat__flag__predefined) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name(*(*abstraction).predefined_flags, argument), argument), error);
        } else if (argument.type == ANVIL__pat__flag__user_defined) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).flags, argument), argument), error);
        } else if (argument.type == ANVIL__pat__literal__string) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).strings, argument), argument), error);
        } else {
            // error
            *error = ANVIL__open__error("Internal Error: Unsupported argument type in accountling argument list.", argument.text.lexling.location);

            return output;
        }

        // check for error
        if (ANVIL__check__error_occured(error)) {
            break;
        }

        // next argument
        current_argument.start += sizeof(ANVIL__parsling_argument);
    }

    return output;
}

// account an abstraction
ANVIL__accountling_abstraction ANVIL__account__abstraction(ANVIL__list call_blueprint, ANVIL__list* predefined_variables, ANVIL__list* predefined_flags, ANVIL__parsling_abstraction parsling_abstraction, ANVIL__error* error) {
    ANVIL__accountling_abstraction output = ANVIL__create_null__accountling_abstraction();

    // get name
    output.header = parsling_abstraction.header;

    // get variables
    // validate that all variables are not used before declared
    {
        // get predefined variables
        output.predefined_variables = predefined_variables;

        // get abstraction inputs
        {
            // open inputs
            output.inputs = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // setup current
            ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.header.inputs);

            // get inputs
            while (ANVIL__check__current_within_range(current_input)) {
                // get argument to look for
                ANVIL__parsling_argument searching_for = *(ANVIL__parsling_argument*)current_input.start;

                // get found variable back
                ANVIL__bt found_predefined;
                ANVIL__bt found_variable;
                ANVIL__account__get_argument_in_list__by_text(predefined_variables, searching_for, &found_predefined);
                ANVIL__account__get_argument_in_list__by_text(&output.inputs, searching_for, &found_variable);

                // if variable already exists
                if (found_predefined == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: A predefined variable was illegally used as an input.", searching_for.text.lexling.location);

                    return output;
                }

                // modify variable type
                searching_for.type = ANVIL__pat__variable__input;

                // if variable already exists
                if (found_variable == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: A duplicate header input was detected.", searching_for.text.lexling.location);

                    return output;
                }

                // add variable
                ANVIL__append__parsling_argument(&output.inputs, searching_for, error);
                if (ANVIL__check__error_occured(error)) {
                    return output;
                }

                // next argument
                current_input.start += sizeof(ANVIL__parsling_argument);
            }
        }

        // get abstraction outputs
        {
            // open outputs
            output.outputs = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // setup current
            ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.header.outputs);

            // get outputs
            while (ANVIL__check__current_within_range(current_output)) {
                // get argument to look for
                ANVIL__parsling_argument searching_for = *(ANVIL__parsling_argument*)current_output.start;

                // get found variable back
                ANVIL__bt found_predefined;
                ANVIL__bt found_input;
                ANVIL__bt found_output;
                ANVIL__account__get_argument_in_list__by_text(predefined_variables, searching_for, &found_predefined);
                ANVIL__account__get_argument_in_list__by_text(&output.inputs, searching_for, &found_input);
                ANVIL__account__get_argument_in_list__by_text(&output.outputs, searching_for, &found_output);

                // if variable already exists
                if (found_predefined == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: A predefined variable was illegally used as an output.", searching_for.text.lexling.location);

                    return output;
                }

                // if variable already exists
                if (found_input == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: An input was illegally used as an output.", searching_for.text.lexling.location);

                    return output;
                }

                // modify variable type
                searching_for.type = ANVIL__pat__variable__output;

                // if variable already exists
                if (found_output == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: A duplicate header output was detected.", searching_for.text.lexling.location);

                    return output;
                }

                // add variable
                ANVIL__append__parsling_argument(&output.outputs, searching_for, error);
                if (ANVIL__check__error_occured(error)) {
                    return output;
                }

                // next argument
                current_output.start += sizeof(ANVIL__parsling_argument);
            }
        }

        // get abstraction body variables
        {
            // open variables
            output.variables = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // setup current
            ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

            // across all statements
            while (ANVIL__check__current_within_range(current_statement)) {
                // get one statement
                ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

                // if statement is abstraction call
                if (statement.type == ANVIL__stt__abstraction_call) {
                    // validate inputs
                    // setup current
                    ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                    // check all inputs
                    while (ANVIL__check__current_within_range(current_input)) {
                        // get input
                        ANVIL__parsling_argument argument = *(ANVIL__parsling_argument*)current_input.start;

                        // if current argument is a variable
                        if (argument.type == ANVIL__pat__variable) {
                            // assuming its not already defined
                            ANVIL__bt found_predefined;
                            ANVIL__account__get_argument_in_list__by_text(output.predefined_variables, argument, &found_predefined);
                            if (found_predefined == ANVIL__bt__true) {
                                goto next_input;
                            }

                            // check if argument already exists
                            ANVIL__bt found_input;
                            ANVIL__bt found_output;
                            ANVIL__bt found_body;
                            ANVIL__account__get_argument_in_list__by_text(&output.inputs, argument, &found_input);
                            ANVIL__account__get_argument_in_list__by_text(&output.outputs, argument, &found_output);
                            ANVIL__account__get_argument_in_list__by_text(&output.variables, argument, &found_body);
                            if ((found_input || found_output || found_body) == ANVIL__bt__false) {
                                *error = ANVIL__open__error("Accounting Error: A variable was used before it was declared.", argument.text.lexling.location);

                                return output;
                            }
                        }

                        // next input
                        next_input:
                        current_input.start += sizeof(ANVIL__parsling_argument);
                    }

                    // validate outputs
                    // setup current
                    ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&statement.outputs);

                    // check all outputs
                    while (ANVIL__check__current_within_range(current_output)) {
                        // get output
                        ANVIL__parsling_argument argument = *(ANVIL__parsling_argument*)current_output.start;
                    
                        // if current argument is a variable
                        if (argument.type == ANVIL__pat__variable) {
                            // assuming its not already defined
                            ANVIL__bt found_predefined;
                            ANVIL__account__get_argument_in_list__by_text(output.predefined_variables, argument, &found_predefined);
                            if (found_predefined == ANVIL__bt__true) {
                                *error = ANVIL__open__error("Accounting Error: Predefined variables cannot be written to.", argument.text.lexling.location);

                                return output;
                            }

                            // check if argument already exists
                            ANVIL__bt found_input;
                            ANVIL__bt found_output;
                            ANVIL__bt found_body;
                            ANVIL__account__get_argument_in_list__by_text(&output.inputs, argument, &found_input);
                            ANVIL__account__get_argument_in_list__by_text(&output.outputs, argument, &found_output);
                            ANVIL__account__get_argument_in_list__by_text(&output.variables, argument, &found_body);
                            if ((found_input || found_output || found_body) == ANVIL__bt__false) {
                                // setup argument type
                                argument.type = ANVIL__pat__variable__body;

                                // create variable
                                ANVIL__append__parsling_argument(&output.variables, argument, error);
                                if (ANVIL__check__error_occured(error)) {
                                    return output;
                                }
                            }
                        }

                        // next output
                        current_output.start += sizeof(ANVIL__parsling_argument);
                    }
                }

                // next statement
                current_statement.start += sizeof(ANVIL__parsling_statement);
            }
        }
    }

    // get offsets
    {
        // open list
        output.offsets = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // setup current
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if a statement offset
            if (statement.type == ANVIL__stt__offset) {
                // if offset doesnt already exist
                ANVIL__bt found_offset;
                ANVIL__account__get_argument_in_list__by_text(&output.offsets, statement.name, &found_offset);
                if (found_offset == ANVIL__bt__false) {
                    // append offset declaration
                    ANVIL__append__parsling_argument(&output.offsets, statement.name, error);
                    if (ANVIL__check__error_occured(error)) {
                        return output;
                    }
                // already defined, error
                } else {
                    *error = ANVIL__open__error("Accounting Error: Offset is declared more than once.", statement.name.text.lexling.location);

                    return output;
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }

        // verify all offsets used exist
        // setup current statement
        current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if statement is abstraction call
            if (statement.type == ANVIL__stt__abstraction_call) {
                // setup current input
                ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                // for each input
                while (ANVIL__check__current_within_range(current_input)) {
                    // get input
                    ANVIL__parsling_argument input_argument = *(ANVIL__parsling_argument*)current_input.start;

                    // if argument is offset
                    if (input_argument.type == ANVIL__pat__offset) {
                        // check if input exists
                        ANVIL__bt found_offset;
                        ANVIL__account__get_argument_in_list__by_text(&output.offsets, input_argument, &found_offset);
                        if (found_offset == ANVIL__bt__false) {
                            // offset nonexistent
                            *error = ANVIL__open__error("Accounting Error: Offset was used but never declared.", input_argument.text.lexling.location);

                            return output;
                        }
                    }

                    // next argument
                    current_input.start += sizeof(ANVIL__parsling_argument);
                }

                // setup current output
                ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&statement.outputs);

                // for each output
                while (ANVIL__check__current_within_range(current_output)) {
                    // get output
                    ANVIL__parsling_argument output_argument = *(ANVIL__parsling_argument*)current_output.start;

                    // if argument is offset
                    if (output_argument.type == ANVIL__pat__offset) {
                        // check if output exists
                        ANVIL__bt found_offset;
                        ANVIL__account__get_argument_in_list__by_text(&output.offsets, output_argument, &found_offset);
                        if (found_offset == ANVIL__bt__false) {
                            // offset nonexistent
                            *error = ANVIL__open__error("Accounting Error: Offset was used but never declared.", output_argument.text.lexling.location);

                            return output;
                        }
                    }

                    // next argument
                    current_output.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    // get flags
    {
        // setup predefined flags
        output.predefined_flags = predefined_flags;

        // open list
        output.flags = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // verify all flags used exist
        // setup current statement
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if statement is abstraction call
            if (statement.type == ANVIL__stt__abstraction_call) {
                // setup current input
                ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                // for each input
                while (ANVIL__check__current_within_range(current_input)) {
                    // get input
                    ANVIL__parsling_argument input_argument = *(ANVIL__parsling_argument*)current_input.start;

                    // if argument is flag
                    if (input_argument.type == ANVIL__pat__flag) {
                        // check if flag exists
                        ANVIL__bt found_predefined_flag;
                        ANVIL__bt found_user_defined_flag;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_flags, input_argument, &found_predefined_flag);
                        ANVIL__account__get_argument_in_list__by_text(&output.flags, input_argument, &found_user_defined_flag);
                        if (found_predefined_flag == ANVIL__bt__false && found_user_defined_flag == ANVIL__bt__false) {
                            // flag nonexistent
                            *error = ANVIL__open__error("Accounting Error: Flag was used but never declared.", input_argument.text.lexling.location);

                            return output;
                        }
                    }

                    // next argument
                    current_input.start += sizeof(ANVIL__parsling_argument);
                }

                // setup current output
                ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&statement.outputs);

                // for each output
                while (ANVIL__check__current_within_range(current_output)) {
                    // get output
                    ANVIL__parsling_argument output_argument = *(ANVIL__parsling_argument*)current_output.start;

                    // if argument is flag
                    if (output_argument.type == ANVIL__pat__flag) {
                        // check if flag exists
                        ANVIL__bt found_predefined_flag;
                        ANVIL__bt found_user_defined_flag;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_flags, output_argument, &found_predefined_flag);
                        ANVIL__account__get_argument_in_list__by_text(&output.flags, output_argument, &found_user_defined_flag);
                        if (found_predefined_flag == ANVIL__bt__true) {
                            // predefined flags cannot be set, error
                            *error = ANVIL__open__error("Accounting Error: Predefined flags cannot be written to.", output_argument.text.lexling.location);

                            return output;
                        }
                        if (found_user_defined_flag == ANVIL__bt__false) {
                            // declare new flag
                            ANVIL__append__parsling_argument(&output.flags, output_argument, error);
                            if (ANVIL__check__error_occured(error)) {
                                return output;
                            }
                        }
                    }

                    // next argument
                    current_output.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    // get strings
    {
        // open strings
        output.strings = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
        output.converted_strings = ANVIL__open__list_with_error(sizeof(ANVIL__buffer) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // get current statement
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if statement is call
            if (statement.type == ANVIL__stt__abstraction_call) {
                // get inputs
                ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                // for each input
                while (ANVIL__check__current_within_range(current_input)) {
                    // get input
                    ANVIL__parsling_argument* input = (ANVIL__parsling_argument*)current_input.start;

                    // if argument is string
                    if (input->type == ANVIL__pat__literal__string) {
                        // append string
                        ANVIL__append__parsling_argument(&output.strings, *input, error);
                        if (ANVIL__check__error_occured(error)) {
                            return output;
                        }

                        // translate string data
                        ANVIL__append__buffer_with_error(&output.converted_strings, ANVIL__account__convert_string(*input, error), error);
                        if (ANVIL__check__error_occured(error)) {
                            return output;
                        }
                    }

                    // next input
                    current_input.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    // modify parsling statements to reflect accurate variable types (predefined, input, output & body) & flag types (predefined & user defined)
    {
        // get current statement
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if statement is call
            if (statement.type == ANVIL__stt__abstraction_call) {
                // modify inputs
                // get inputs
                ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                // for each input
                while (ANVIL__check__current_within_range(current_input)) {
                    // get input
                    ANVIL__parsling_argument* input = (ANVIL__parsling_argument*)current_input.start;

                    // if argument is variable
                    if (input->type == ANVIL__pat__variable) {
                        // find variable by type
                        ANVIL__bt found_predefined;
                        ANVIL__bt found_input;
                        ANVIL__bt found_output;
                        ANVIL__bt found_body;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_variables, *input, &found_predefined);
                        ANVIL__account__get_argument_in_list__by_text(&output.inputs, *input, &found_input);
                        ANVIL__account__get_argument_in_list__by_text(&output.outputs, *input, &found_output);
                        ANVIL__account__get_argument_in_list__by_text(&output.variables, *input, &found_body);

                        // modify appropriately
                        if (found_predefined) {
                            input->type = ANVIL__pat__variable__predefined;
                        } else if (found_input) {
                            input->type = ANVIL__pat__variable__input;
                        } else if (found_output) {
                            input->type = ANVIL__pat__variable__output;
                        } else if (found_body) {
                            input->type = ANVIL__pat__variable__body;
                        } else {
                            // error
                            *error = ANVIL__open__error("Internal Error: Unrecognized variable type when modifying statement variable types, oops.", input->text.lexling.location);
                        }
                    } else if (input->type == ANVIL__pat__flag) {
                        // find variable by type
                        ANVIL__bt found_predefined;
                        ANVIL__bt found_user_defined;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_flags, *input, &found_predefined);
                        ANVIL__account__get_argument_in_list__by_text(&output.flags, *input, &found_user_defined);

                        // modify appropriately
                        if (found_predefined) {
                            input->type = ANVIL__pat__flag__predefined;
                        } else if (found_user_defined) {
                            input->type = ANVIL__pat__flag__user_defined;
                        } else {
                            // error
                            *error = ANVIL__open__error("Internal Error: Unrecognized variable type when modifying statement input flag types, oops.", input->text.lexling.location);
                        }
                    }

                    // next input
                    current_input.start += sizeof(ANVIL__parsling_argument);
                }
                
                // modify outputs
                // get outputs
                ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&statement.outputs);

                // for each output
                while (ANVIL__check__current_within_range(current_output)) {
                    // get output
                    ANVIL__parsling_argument* output_argument = (ANVIL__parsling_argument*)current_output.start;

                    // if argument is variable
                    if (output_argument->type == ANVIL__pat__variable) {
                        // find variable by type
                        ANVIL__bt found_predefined;
                        ANVIL__bt found_input;
                        ANVIL__bt found_output;
                        ANVIL__bt found_body;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_variables, *output_argument, &found_predefined);
                        ANVIL__account__get_argument_in_list__by_text(&output.inputs, *output_argument, &found_input);
                        ANVIL__account__get_argument_in_list__by_text(&output.outputs, *output_argument, &found_output);
                        ANVIL__account__get_argument_in_list__by_text(&output.variables, *output_argument, &found_body);

                        // modify appropriately
                        if (found_predefined) {
                            output_argument->type = ANVIL__pat__variable__predefined;
                        } else if (found_input) {
                            output_argument->type = ANVIL__pat__variable__input;
                        } else if (found_output) {
                            output_argument->type = ANVIL__pat__variable__output;
                        } else if (found_body) {
                            output_argument->type = ANVIL__pat__variable__body;
                        } else {
                            // error
                            *error = ANVIL__open__error("Internal Error: Unrecognized variable type when modifying statement variable types, oops.", output_argument->text.lexling.location);
                        }
                    } else if (output_argument->type == ANVIL__pat__flag) {
                        // find variable by type
                        ANVIL__bt found_predefined;
                        ANVIL__bt found_user_defined;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_flags, *output_argument, &found_predefined);
                        ANVIL__account__get_argument_in_list__by_text(&output.flags, *output_argument, &found_user_defined);

                        // modify appropriately
                        if (found_predefined) {
                            output_argument->type = ANVIL__pat__flag__predefined;
                        } else if (found_user_defined) {
                            output_argument->type = ANVIL__pat__flag__user_defined;
                        } else {
                            // error
                            *error = ANVIL__open__error("Internal Error: Unrecognized variable type when modifying statement output flag types, oops.", output_argument->text.lexling.location);
                        }
                    }

                    // next output
                    current_output.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    // get statements
    {
        // open list
        output.statements = ANVIL__open__list_with_error(sizeof(ANVIL__accountling_statement) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // setup offsets index
        ANVIL__offset_index offset_ID = 0;

        // convert statements
        // get current
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement parsling_statement = *(ANVIL__parsling_statement*)current_statement.start;

            // setup new statement
            ANVIL__accountling_statement accountling_statement;

            // translate statement
            // statement is a call
            if (parsling_statement.type == ANVIL__stt__abstraction_call) {
                // set type
                accountling_statement.type = parsling_statement.type;

                // get accountling header index
                ANVIL__header_index header_index = ANVIL__find__accountling_header_index(call_blueprint, parsling_statement);
                if (header_index > ANVIL__calculate__call_blueprint_entry_count(call_blueprint)) {
                    *error = ANVIL__open__error("Internal Error: Header was not found during lookup.\n", parsling_statement.name.text.lexling.location);

                    return output;
                }

                // get header
                accountling_statement.header = ((ANVIL__accountling_abstraction_header*)call_blueprint.buffer.start)[header_index];

                // get io
                accountling_statement.inputs = ANVIL__account__accountling_argument_list(&output, parsling_statement.inputs, error);
                if (ANVIL__check__error_occured(error)) {
                    return output;
                }
                accountling_statement.outputs = ANVIL__account__accountling_argument_list(&output, parsling_statement.outputs, error);
                if (ANVIL__check__error_occured(error)) {
                    return output;
                }
            // statement is an offset
            } else if (parsling_statement.type == ANVIL__stt__offset) {
                // set type
                accountling_statement.type = parsling_statement.type;

                // get offset index
                accountling_statement.offset_ID = offset_ID;

                // next offset
                offset_ID++;
            }

            // append statement
            ANVIL__append__accountling_statement(&output.statements, accountling_statement, error);

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    return output;
}

// print call header
void ANVIL__print__accounted_call_header(ANVIL__accountling_abstraction_header header) {
    // print ID
    printf("\t\tCall Index [%lu](user_defined=%lu) ", (ANVIL__u64)header.call_index, (ANVIL__u64)(header.call_index >= ANVIL__act__user_defined));

    // print statement
    ANVIL__print__parsed_statement(header.header);

    // new line
    printf("\n");

    return;
}

// print call blueprint
void ANVIL__print__call_blueprint(ANVIL__list blueprint) {
    // setup current
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&blueprint);

    // print all headers
    while (ANVIL__check__current_within_range(current)) {
        // print individual header
        ANVIL__print__accounted_call_header(*(ANVIL__accountling_abstraction_header*)current.start);

        // next header
        current.start += sizeof(ANVIL__accountling_abstraction_header);
    }

    return;
}

// append predefined variable
void ANVIL__generate_and_append__predefined_variable(ANVIL__list* list, const char* name, ANVIL__error* error) {
    // append
    ANVIL__append__parsling_argument(list, ANVIL__create__parsling_argument(ANVIL__pat__variable__predefined, ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__end_of_file, ANVIL__open__buffer_from_string((ANVIL__u8*)name, ANVIL__bt__false, ANVIL__bt__false), ANVIL__create__character_location(-1, -1, -1))), 0), error);

    return;
}

// generate predefined variables
ANVIL__list ANVIL__generate__predefined_variables(ANVIL__error* error) {
    ANVIL__list output;

    // open output
    output = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // append variables
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__error_code], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__character_byte_size], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__character_bit_size], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__bits_in_byte], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__cell_byte_size], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__cell_bit_size], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__true], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__false], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__0], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__1], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__2], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__4], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__8], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__16], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__24], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__32], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__40], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__48], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__56], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__64], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__input_start], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__input_end], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__output_start], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__output_end], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__stack_start], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__stack_current], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__stack_end], error);

    return output;
}

// append predefined flag
void ANVIL__generate_and_append__predefined_flag(ANVIL__list* list, const char* name, ANVIL__error* error) {
    // append
    ANVIL__append__parsling_argument(list, ANVIL__create__parsling_argument(ANVIL__pat__flag__predefined, ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__end_of_file, ANVIL__open__buffer_from_string((ANVIL__u8*)name, ANVIL__bt__false, ANVIL__bt__false), ANVIL__create__character_location(-1, -1, -1))), 0), error);

    return;
}

// generate predefined flags
ANVIL__list ANVIL__generate__predefined_flags(ANVIL__error* error) {
    ANVIL__list output;

    // open output
    output = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // append flags
    ANVIL__generate_and_append__predefined_flag(&output, ANVIL__global__predefined_flag_name_strings[ANVIL__pft__always_run], error);
    ANVIL__generate_and_append__predefined_flag(&output, ANVIL__global__predefined_flag_name_strings[ANVIL__pft__never_run], error);
    ANVIL__generate_and_append__predefined_flag(&output, ANVIL__global__predefined_flag_name_strings[ANVIL__pft__temporary], error);

    return output;
}

// account program
ANVIL__accountling_program ANVIL__account__program(ANVIL__list parsling_programs, ANVIL__error* error) {
    ANVIL__accountling_program output = ANVIL__create_null__accountling_program();

    // create headers
    output.call_blueprint = ANVIL__generate__call_blueprint(parsling_programs, error);
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // verify statments in all user defined abstractions
    ANVIL__account__verify_all_calls(parsling_programs, output.call_blueprint, error);
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // generate predefined variables
    output.predefined_variables = ANVIL__generate__predefined_variables(error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // generate predefined flags
    output.predefined_flags = ANVIL__generate__predefined_flags(error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // allocate accountling abstraction list
    output.abstractions = ANVIL__open__list_with_error(sizeof(ANVIL__accountling_abstraction) * 16, error);
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // setup current
    ANVIL__current current_parsling_program = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // account all programs
    while (ANVIL__check__current_within_range(current_parsling_program)) {
        // get program
        ANVIL__parsling_program parsling_program = *(ANVIL__parsling_program*)current_parsling_program.start;

        // setup current
        ANVIL__current current_parsling_abstraction = ANVIL__calculate__current_from_list_filled_index(&parsling_program.abstractions);

        // account all abstractions in program
        while (ANVIL__check__current_within_range(current_parsling_abstraction)) {
            // get parsling abstraction
            ANVIL__parsling_abstraction parsling_abstraction = *(ANVIL__parsling_abstraction*)current_parsling_abstraction.start;

            // account the parsling abstraction
            ANVIL__accountling_abstraction accountling_abstraction = ANVIL__account__abstraction(output.call_blueprint, &output.predefined_variables, &output.predefined_flags, parsling_abstraction, error);
            if (ANVIL__check__error_occured(error)) {
                // close abstraction
                ANVIL__close__accountling_abstraction(accountling_abstraction);

                // quit
                goto quit;
            }

            // append abstraction
            ANVIL__append__accountling_abstraction(&output.abstractions, accountling_abstraction, error);
            if (ANVIL__check__error_occured(error)) {
                goto quit;
            }

            // next abstraction
            current_parsling_abstraction.start += sizeof(ANVIL__parsling_abstraction);
        }

        // next program
        current_parsling_program.start += sizeof(ANVIL__parsling_program);
    }

    // search for main
    // setup current
    ANVIL__current current_header = ANVIL__calculate__current_from_list_filled_index(&output.call_blueprint);
    output.main_abstraction_ID = 0;

    // for each header
    while (ANVIL__check__current_within_range(current_header)) {
        // get header
        ANVIL__accountling_abstraction_header header = *(ANVIL__accountling_abstraction_header*)current_header.start;

        // check for correct title & io counts
        if (ANVIL__calculate__buffer_contents_equal(header.header.name.text.lexling.value, ANVIL__open__buffer_from_string((u8*)"main", ANVIL__bt__false, ANVIL__bt__false)) && header.header.input_count == 0 && header.header.output_count == 0) {
            break;
        }

        // next header
        output.main_abstraction_ID++;
        current_header.start += sizeof(ANVIL__accountling_abstraction_header);
    }

    // check to see if main was not found
    if (ANVIL__check__current_within_range(current_header) == ANVIL__bt__false) {
        // set error
        *error = ANVIL__open__error("Accounting Error: The entry point function main()() was not found.", ANVIL__create_null__character_location());

        goto quit;
    }

    // return
    quit:

    return output;
}

// print variable list
void ANVIL__print__accountling_variable_list(ANVIL__list* variables, ANVIL__tab_count tabs) {
    // setup current
    ANVIL__current current_variable = ANVIL__calculate__current_from_list_filled_index(variables);

    // print each argument
    while (ANVIL__check__current_within_range(current_variable)) {
        // print variable
        ANVIL__print__tabs(tabs);
        ANVIL__print__parsling_argument(*(ANVIL__parsling_argument*)current_variable.start);
        printf("\n");

        // next variable
        current_variable.start += sizeof(ANVIL__parsling_argument);
    }
    
    return;
}

// print accountling argument list
void ANVIL__print__accountling_arguments(ANVIL__list arguments) {
    // print opener
    printf("(");

    // setup current
    ANVIL__current current_argument = ANVIL__calculate__current_from_list_filled_index(&arguments);

    // for each argument
    while (ANVIL__check__current_within_range(current_argument)) {
        // get argument
        ANVIL__accountling_argument argument = *(ANVIL__accountling_argument*)current_argument.start;

        // print argument
        printf("[");
        ANVIL__print__buffer(ANVIL__convert__parsed_argument_type_to_string_buffer(argument.type));
        printf(":%lu]", argument.index);


        // next argument
        current_argument.start += sizeof(ANVIL__accountling_argument);
    }

    // print closer
    printf(")");

    return;
}

// print an accountling program
void ANVIL__print__accountling_program(ANVIL__accountling_program program) {
    // print section start
    printf("Accountlings:\n\tHeaders:\n");

    // print headers
    ANVIL__print__call_blueprint(program.call_blueprint);

    // print predefined variables
    printf("\tPredefined Variables:\n");

    // print variables
    ANVIL__print__accountling_variable_list(&program.predefined_variables, 2);

    // print predefined flags
    printf("\tPredefined Flags:\n");

    // print flags
    ANVIL__print__accountling_variable_list(&program.predefined_flags, 2);

    // print abstractions
    printf("\tAbstractions:\n");

    // setup current
    ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&program.abstractions);

    // print each abstraction
    while (ANVIL__check__current_within_range(current_abstraction)) {
        // get abstraction
        ANVIL__accountling_abstraction abstraction = *(ANVIL__accountling_abstraction*)current_abstraction.start;

        // print abstraction
        {
            // get call index
            ANVIL__call_index call_index = ANVIL__find__accountling_header_index(program.call_blueprint, abstraction.header);
            if (call_index >= ANVIL__calculate__call_blueprint_entry_count(program.call_blueprint)) {
                // call header not found, exit loop
                return;
            }

            // print header with call index
            printf("\t\t(%lu)", call_index);
            ANVIL__print__parsed_statement(abstraction.header);
            printf("\n");

            // if there are inputs
            if (abstraction.inputs.filled_index > 0) {
                // print inputs
                printf("\t\t\tInputs:\n");
                ANVIL__print__accountling_variable_list(&abstraction.inputs, 4);
            }

            // if there are outputs
            if (abstraction.outputs.filled_index > 0) {
                // print outputs
                printf("\t\t\tOutputs:\n");
                ANVIL__print__accountling_variable_list(&abstraction.outputs, 4);
            }

            // if there are variables
            if (abstraction.variables.filled_index > 0) {
                // print variables
                printf("\t\t\tVariables:\n");
                ANVIL__print__accountling_variable_list(&abstraction.variables, 4);
            }

            // if there are offsets
            if (abstraction.offsets.filled_index > 0) {
                // print offsets
                printf("\t\t\tOffsets:\n");
                ANVIL__print__accountling_variable_list(&abstraction.offsets, 4);
            }

            // if there are flags
            if (abstraction.flags.filled_index > 0) {
                // print flags
                printf("\t\t\tFlags:\n");
                ANVIL__print__accountling_variable_list(&abstraction.flags, 4);
            }

            // if there are statements
            if (abstraction.statements.filled_index > 0) {
                // print statements
                // print header
                printf("\t\t\tStatements:\n");

                // setup current
                ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&abstraction.statements);
                
                // for each statement
                while (ANVIL__check__current_within_range(current_statement)) {
                    // get statement
                    ANVIL__accountling_statement statement = *(ANVIL__accountling_statement*)current_statement.start;
                    
                    // determine statement type
                    // if abstraction call
                    if (statement.type == ANVIL__stt__abstraction_call) {
                        // print statement call index
                        printf("\t\t\t\t%lu = ", statement.header.call_index);

                        // print io
                        ANVIL__print__accountling_arguments(statement.inputs);
                        ANVIL__print__accountling_arguments(statement.outputs);

                        // print new line
                        printf("\n");
                    // if offset declaration
                    } else if (statement.type == ANVIL__stt__offset) {
                        // print index
                        printf("\t\t\t\t@%lu\n", statement.offset_ID);
                    }

                    // next statement
                    current_statement.start += sizeof(ANVIL__accountling_statement);
                }
            }
        }

        // next abstraction
        current_abstraction.start += sizeof(ANVIL__accountling_abstraction);
    }

    return;
}

#endif
