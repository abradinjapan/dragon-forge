#ifndef DRAGON__compiler__generator
#define DRAGON__compiler__generator

/* Include */
#include "accounter.h"

/* Generator */
// add an offset to a list
void ANVIL__append__offset(ANVIL__list* list, ANVIL__offset data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__offset), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__offset*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__offset);

    return;
}

// generation offsets
typedef struct ANVIL__generation_offsets {
    ANVIL__offset function_start; // the first instruction in the function
    ANVIL__offset function_return; // the first instruction passing output
    ANVIL__offset function_data; // after the code of the function, the embedded data's place
    ANVIL__list statement_offsets; // ANVIL__offset the function specific statement offsets
    ANVIL__list strings_offsets; // ANVIL__offset the function specific string offsets
} ANVIL__generation_offsets;

// generation cell range
typedef struct ANVIL__generation_cell_range {
    ANVIL__cell_ID start;
    ANVIL__cell_ID end;
} ANVIL__generation_cell_range;

// create a cell range
ANVIL__generation_cell_range ANVIL__create__generation_cell_range(ANVIL__cell_ID start, ANVIL__cell_ID end) {
    ANVIL__generation_cell_range output;

    output.start = start;
    output.end = end;

    return output;
}

// calculate cell range length
ANVIL__cell_count ANVIL__calculate__generation_cell_length(ANVIL__generation_cell_range range) {
    return range.end - range.start + 1;
}

// calculate cell range from start and length
ANVIL__generation_cell_range ANVIL__calculate__generation_cell_range(ANVIL__cell_ID start, ANVIL__cell_count length) {
    return ANVIL__create__generation_cell_range(start, start + length - 1);
}

// generation cells
typedef struct ANVIL__generation_cells {
    ANVIL__generation_cell_range workspace_total_range;
    ANVIL__generation_cell_range workspace_input_range;
    ANVIL__generation_cell_range workspace_output_range;
    ANVIL__generation_cell_range workspace_body_range;
    ANVIL__generation_cell_range function_input_range;
    ANVIL__generation_cell_range function_output_range;
    ANVIL__cell_count input_count;
    ANVIL__cell_count output_count;
    ANVIL__cell_count variable_count;
    ANVIL__cell_count workspace_total_count;
} ANVIL__generation_cells;

// setup generation cells from io & variable counts
ANVIL__generation_cells ANVIL__setup__generation_cells(ANVIL__cell_count input_count, ANVIL__cell_count output_count, ANVIL__cell_count variable_count) {
    ANVIL__generation_cells output;

    // setup counts
    output.input_count = input_count;
    output.output_count = output_count;
    output.variable_count = variable_count;
    output.workspace_total_count = input_count + output_count + variable_count;

    // calculate ranges
    output.function_input_range = ANVIL__calculate__generation_cell_range(ANVIL__srt__start__function_io, input_count);
    output.function_output_range = ANVIL__calculate__generation_cell_range(ANVIL__srt__start__function_io, output_count);
    output.workspace_input_range = ANVIL__calculate__generation_cell_range(ANVIL__srt__start__workspace, input_count);
    output.workspace_output_range = ANVIL__calculate__generation_cell_range(output.workspace_input_range.end + 1, output_count);
    output.workspace_body_range = ANVIL__calculate__generation_cell_range(output.workspace_output_range.end + 1, variable_count);
    output.workspace_total_range = ANVIL__create__generation_cell_range(output.workspace_input_range.start, output.workspace_body_range.end);

    return output;
}

// generation abstraction
typedef struct ANVIL__generation_abstraction {
    ANVIL__generation_cells cells;
    ANVIL__generation_offsets offsets;
    ANVIL__list converted_strings; // ANVIL__buffer (copied from accountlings! do not free!)
} ANVIL__generation_abstraction;

// translate accountling abstraction to generation abstraction
ANVIL__generation_abstraction ANVIL__open__generation_abstraction(ANVIL__accountling_abstraction accountlings, ANVIL__error* error) {
    ANVIL__generation_abstraction output;

    // translate registers
    output.cells = ANVIL__setup__generation_cells(ANVIL__calculate__list_content_count(accountlings.inputs, sizeof(ANVIL__parsling_argument)), ANVIL__calculate__list_content_count(accountlings.outputs, sizeof(ANVIL__parsling_argument)), ANVIL__calculate__list_content_count(accountlings.variables, sizeof(ANVIL__parsling_argument)));

    // open offsets
    output.offsets.function_start = ANVIL__define__null_offset_ID;
    output.offsets.function_return = ANVIL__define__null_offset_ID;
    output.offsets.function_data = ANVIL__define__null_offset_ID;
    output.offsets.statement_offsets = ANVIL__open__list_with_error(sizeof(ANVIL__offset) * 16, error);
    output.offsets.strings_offsets = ANVIL__open__list_with_error(sizeof(ANVIL__offset) * 16, error);

    // append statement offsets
    for (ANVIL__offset_index i = 0; i < ANVIL__calculate__list_content_count(accountlings.offsets, sizeof(ANVIL__parsling_argument)); i++) {
        // append blank offset
        ANVIL__append__offset(&output.offsets.statement_offsets, ANVIL__define__null_offset_ID, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
    }

    // append string offsets
    for (ANVIL__offset_index i = 0; i < ANVIL__calculate__list_content_count(accountlings.strings, sizeof(ANVIL__parsling_argument)); i++) {
        // append blank offset
        ANVIL__append__offset(&output.offsets.strings_offsets, ANVIL__define__null_offset_ID, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
    }

    // open strings
    output.converted_strings = accountlings.converted_strings;

    return output;
}

// close generation abstraction
void ANVIL__close__generation_abstraction(ANVIL__generation_abstraction abstraction) {
    // close offsets
    ANVIL__close__list(abstraction.offsets.statement_offsets);
    ANVIL__close__list(abstraction.offsets.strings_offsets);

    return;
}

// append generation abstraction
void ANVIL__append__generation_abstraction(ANVIL__list* list, ANVIL__generation_abstraction data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__generation_abstraction), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__generation_abstraction*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__generation_abstraction);

    return;
}

// generation workspace
typedef struct ANVIL__generation_workspace {
    ANVIL__workspace anvil;
    ANVIL__workspace* workspace;
    ANVIL__list abstractions; // ANVIL__generation_abstraction
    ANVIL__abstraction_index entry_point;
    ANVIL__abstraction_index function_count;
    ANVIL__standard__offsets standard_offsets;
} ANVIL__generation_workspace;

// open workspace
void ANVIL__open__generation_workspace(ANVIL__buffer* program_buffer, ANVIL__accountling_program accountlings, ANVIL__generation_workspace* output, ANVIL__error* error) {
    // setup anvil workspace
    output->anvil = ANVIL__setup__workspace(program_buffer);
    output->workspace = &(output->anvil);

    // setup entry point
    output->entry_point = ANVIL__calculate__abstraction_index_from_call_index(accountlings.main_abstraction_ID);

    // setup generation abstractions from accountling abstractions
    // open generation abstractions
    output->abstractions = ANVIL__open__list_with_error(sizeof(ANVIL__generation_abstraction) * 16, error);
    output->function_count = 0;

    // setup current
    ANVIL__current current_accountling_abstraction = ANVIL__calculate__current_from_list_filled_index(&accountlings.abstractions);
    
    // for each accountling abstraction
    while (ANVIL__check__current_within_range(current_accountling_abstraction)) {
        // get accountling abstraction
        ANVIL__accountling_abstraction accountling_abstraction = *(ANVIL__accountling_abstraction*)current_accountling_abstraction.start;

        // setup generation abstraction
        ANVIL__generation_abstraction generation_abstraction = ANVIL__open__generation_abstraction(accountling_abstraction, error);
        if (ANVIL__check__error_occured(error)) {
            return;
        }

        // append abstraction
        ANVIL__append__generation_abstraction(&output->abstractions, generation_abstraction, error);
        if (ANVIL__check__error_occured(error)) {
            return;
        }

        // next abstraction
        output->function_count++;
        current_accountling_abstraction.start += sizeof(ANVIL__accountling_abstraction);
    }

    return;
}

// close workspace
void ANVIL__close__generation_workspace(ANVIL__generation_workspace workspace) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&workspace.abstractions);

    // close all abstractions
    while (ANVIL__check__current_within_range(current)) {
        // close abstraction
        ANVIL__close__generation_abstraction(*(ANVIL__generation_abstraction*)current.start);

        // next abstraction
        current.start += sizeof(ANVIL__generation_abstraction);
    }

    // close abstractions list
    ANVIL__close__list(workspace.abstractions);

    return;
}

// get abstraction from workspace
ANVIL__generation_abstraction ANVIL__find__generation_abstraction_by_index(ANVIL__generation_workspace* workspace, ANVIL__abstraction_index index) {
    return ((ANVIL__generation_abstraction*)(*workspace).abstractions.buffer.start)[index];
}

// get abstraction from abstraction list
ANVIL__accountling_abstraction ANVIL__find__accountling_abstraction_by_index(ANVIL__accountling_program program, ANVIL__abstraction_index index) {
    return ((ANVIL__accountling_abstraction*)program.abstractions.buffer.start)[index];
}

// convert variable index to cell ID
ANVIL__cell_ID ANVIL__translate__accountling_variable_index_to_cell_ID(ANVIL__generation_abstraction* generation_abstraction, ANVIL__accountling_argument argument, ANVIL__error* error) {
    ANVIL__pvt predefineds[] = {
        ANVIL__rt__error_code,
        ANVIL__srt__constant__1,
        ANVIL__srt__constant__8,
        ANVIL__srt__constant__bits_in_byte,
        ANVIL__srt__constant__cell_byte_count,
        ANVIL__srt__constant__cell_bit_count,
        ANVIL__srt__constant__true,
        ANVIL__srt__constant__false,
        ANVIL__srt__constant__0,
        ANVIL__srt__constant__1,
        ANVIL__srt__constant__2,
        ANVIL__srt__constant__4,
        ANVIL__srt__constant__8,
        ANVIL__srt__constant__16,
        ANVIL__srt__constant__24,
        ANVIL__srt__constant__32,
        ANVIL__srt__constant__40,
        ANVIL__srt__constant__48,
        ANVIL__srt__constant__56,
        ANVIL__srt__constant__64,
        ANVIL__srt__input_buffer_start,
        ANVIL__srt__input_buffer_end,
        ANVIL__srt__output_buffer_start,
        ANVIL__srt__output_buffer_end,
        ANVIL__srt__stack__start_address,
        ANVIL__srt__stack__current_address,
        ANVIL__srt__stack__end_address,
    };

    // convert based on type
    if (argument.type == ANVIL__pat__variable || argument.type == ANVIL__pat__variable__body) {
        // return cell ID
        return generation_abstraction->cells.workspace_body_range.start + argument.index;
    } else if (argument.type == ANVIL__pat__variable__input) {
        // return cell ID
        return generation_abstraction->cells.workspace_input_range.start + argument.index;
    } else if (argument.type == ANVIL__pat__variable__output) {
        // return cell ID
        return generation_abstraction->cells.workspace_output_range.start + argument.index;
    } else if (argument.type == ANVIL__pat__variable__predefined) {
        // return cell ID from list
        return predefineds[argument.index];
    }
    
    // error
    *error = ANVIL__open__error("Internal Error: Unsupported variable index type.", ANVIL__create_null__character_location());

    return ANVIL__define__null_offset_ID;
}

// convert flag index to flag ID
ANVIL__flag_ID ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__accountling_argument argument, ANVIL__error* error) {
    if (argument.type == ANVIL__pat__flag__predefined) {
        return argument.index;
    } else if (argument.type == ANVIL__pat__flag__user_defined) {
        return ANVIL__pft__COUNT + argument.index;
    }

    // error
    *error = ANVIL__open__error("Internal Error: Flag is not a usable type (aka, not a predefined flag or user defined flag), oops.", ANVIL__create_null__character_location());

    return ANVIL__define__null_flag;
}

// generate function
void ANVIL__forge__anvil_abstraction(ANVIL__generation_workspace* workspace, ANVIL__generation_abstraction* generation_abstraction, ANVIL__accountling_abstraction accountling_abstraction, ANVIL__error* error) {
    ANVIL__string_index current_string_ID = 0;

    // setup offset
    (*generation_abstraction).offsets.function_start = ANVIL__get__offset((*workspace).workspace);

    // setup function prologue
    ANVIL__code__preserve_workspace((*workspace).workspace, ANVIL__sft__always_run, generation_abstraction->cells.workspace_total_range.start, generation_abstraction->cells.workspace_total_range.end);

    // get inputs
    for (ANVIL__input_count i = 0; i < generation_abstraction->cells.input_count; i++) {
        // pass input
        ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, generation_abstraction->cells.function_input_range.start + i, generation_abstraction->cells.workspace_input_range.start + i);
    }

    // translate statements
    // setup current
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&accountling_abstraction.statements);

    // for each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // get statement
        ANVIL__accountling_statement statement = *(ANVIL__accountling_statement*)current_statement.start;

        // build statement
        // if statement is call
        if (statement.type == ANVIL__stt__abstraction_call) {
            // if statement is instruction
            if (statement.header.call_index < ANVIL__act__user_defined) {
                // determine instruction type & write instruction
                switch ((ANVIL__act)statement.header.call_index) {
                case ANVIL__act__set__boolean:
                case ANVIL__act__set__binary:
                case ANVIL__act__set__integer:
                case ANVIL__act__set__hexadecimal:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0).index, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__set__offset:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(((ANVIL__offset*)generation_abstraction->offsets.statement_offsets.buffer.start)[ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0).index]), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__set__flag_ID:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__set__string:
                    // write string data
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)((ANVIL__offset*)generation_abstraction->offsets.strings_offsets.buffer.start)[current_string_ID], ANVIL__srt__temp__offset);
                    ANVIL__code__retrieve_embedded_buffer(workspace->workspace, ANVIL__sft__always_run, ANVIL__srt__temp__offset, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    // next string ID
                    current_string_ID++;

                    break;
                case ANVIL__act__io__cell_to_address:
                    ANVIL__code__cell_to_address(workspace->workspace, ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__io__address_to_cell:
                    ANVIL__code__address_to_cell(workspace->workspace, ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__io__file_to_buffer:
                    ANVIL__code__file_to_buffer(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__io__buffer_to_file:
                    ANVIL__code__buffer_to_file(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__copy__cell:
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__copy__buffer:
                    ANVIL__code__buffer_to_buffer(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__memory__request_memory:
                    ANVIL__code__request_memory(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__memory__return_memory:
                    ANVIL__code__return_memory(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error));

                    break;
                case ANVIL__act__buffer__calculate_length:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_subtract, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__constant__1, ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__cast__cell_to_unsigned_integer_string:
                    ANVIL__standard__code__call__cell_to_unsigned_integer_string(workspace->workspace, &workspace->standard_offsets, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__print__unsigned_integer:
                    ANVIL__code__debug__print_cell_as_decimal(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error));

                    break;
                case ANVIL__act__print__character:
                    ANVIL__code__debug__putchar(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error));

                    break;
                case ANVIL__act__print__buffer_as_string:
                    ANVIL__standard__code__call__print_buffer_as_string(workspace->workspace, &workspace->standard_offsets, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error));

                    break;
                case ANVIL__act__print__binary:
                    ANVIL__standard__code__call__print_binary(workspace->workspace, &workspace->standard_offsets, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error));

                    break;
                case ANVIL__act__integer__add:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__subtract:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_subtract, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__multiply:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_multiply, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__divide:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_division, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__modulous:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_modulous, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__within_range:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__operate__flag(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error), ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 3), error), ANVIL__srt__temp__flag_ID_0);

                    break;
                case ANVIL__act__binary__or:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_or, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__invert:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_invert, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__and:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_and, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__xor:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_xor, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__shift_higher:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_shift_higher, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__shift_lower:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_shift_lower, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__overwrite:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_overwrite, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__flag__get:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_get, ANVIL__srt__temp__flag_ID_0, ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__flag__set:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_set, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_0);

                    break;
                case ANVIL__act__flag__invert:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_1);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_invert, ANVIL__srt__temp__flag_ID_0, ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_1);

                    break;
                case ANVIL__act__flag__or:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__srt__temp__flag_ID_1);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_2);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_or, ANVIL__srt__temp__flag_ID_0, ANVIL__srt__temp__flag_ID_1, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_2);

                    break;
                case ANVIL__act__flag__and:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__srt__temp__flag_ID_1);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_2);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_and, ANVIL__srt__temp__flag_ID_0, ANVIL__srt__temp__flag_ID_1, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_2);

                    break;
                case ANVIL__act__flag__xor:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__srt__temp__flag_ID_1);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_2);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_xor, ANVIL__srt__temp__flag_ID_0, ANVIL__srt__temp__flag_ID_1, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_2);

                    break;
                case ANVIL__act__jump:
                    ANVIL__code__jump__static(workspace->workspace, ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), (((ANVIL__offset*)generation_abstraction->offsets.statement_offsets.buffer.start)[ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1).index]));

                    break;
                case ANVIL__act__open_context:
                    ANVIL__code__setup__context(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__compile:
                    ANVIL__code__compile(
                        workspace->workspace,
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 2), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 3), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 4), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 5), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 6), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 7), error)
                    );

                    break;
                case ANVIL__act__run:
                    ANVIL__code__run(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error));

                    break;
                case ANVIL__act__reset_error_code_cell:
                    ANVIL__code__write_cell(workspace->workspace, ANVIL__et__no_error, ANVIL__rt__error_code);

                    break;
                case ANVIL__act__get_program_inputs:
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__srt__input_buffer_start, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__srt__input_buffer_end, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__set_program_outputs:
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__output_buffer_start);
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__srt__output_buffer_end);

                    break;
                case ANVIL__act__set_context_buffer_inputs:
                    ANVIL__code__call__standard__set_inputs_in_context_buffer(workspace->workspace, &workspace->standard_offsets, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 3), error));

                    break;
                case ANVIL__act__get_context_buffer_outputs:
                    ANVIL__code__call__standard__get_outputs_from_context_buffer(workspace->workspace, &workspace->standard_offsets, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                default:
                    // should not be reachable
                    break;
                }
            // if statement is abstraction call
            } else {
                // pass inputs
                for (ANVIL__input_count i = 0; i < ANVIL__calculate__list_content_count(statement.inputs, sizeof(ANVIL__accountling_argument)); i++) {
                    // pass input
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, i), error), generation_abstraction->cells.function_input_range.start + i);
                }

                // call function
                ANVIL__code__call__static(workspace->workspace, ANVIL__sft__always_run, ((ANVIL__generation_abstraction*)workspace->abstractions.buffer.start)[ANVIL__calculate__abstraction_index_from_call_index(statement.header.call_index)].offsets.function_start);

                // pass outputs
                for (ANVIL__output_count i = 0; i < ANVIL__calculate__list_content_count(statement.outputs, sizeof(ANVIL__accountling_argument)); i++) {
                    // pass output
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, generation_abstraction->cells.function_output_range.start + i, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, i), error));
                }
            }
        // statement is offset
        } else if (statement.type == ANVIL__stt__offset) {
            // declare offset
            ((ANVIL__offset*)generation_abstraction->offsets.statement_offsets.buffer.start)[statement.offset_ID] = ANVIL__get__offset(workspace->workspace);
        // unrecognized statement type, error
        } else {
            *error = ANVIL__open__error("Internal Error: In code generator, invalid statement type, oops.", statement.header.header.name.text.lexling.location);

            return;
        }

        // check for error
        if (ANVIL__check__error_occured(error)) {
            return;
        }

        // next statement
        current_statement.start += sizeof(ANVIL__accountling_statement);
    }

    // setup function return offset
    generation_abstraction->offsets.function_return = ANVIL__get__offset(workspace->workspace);

    // pass outputs
    for (ANVIL__output_count i = 0; i < generation_abstraction->cells.output_count; i++) {
        // pass output
        ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, generation_abstraction->cells.workspace_output_range.start + i, generation_abstraction->cells.function_output_range.start + i);
    }

    // setup function epilogue
    ANVIL__code__restore_workspace(workspace->workspace, ANVIL__sft__always_run, generation_abstraction->cells.workspace_total_range.start, generation_abstraction->cells.workspace_total_range.end);

    // return to caller
    ANVIL__code__jump__explicit(workspace->workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

    // setup data offset
    generation_abstraction->offsets.function_data = ANVIL__get__offset(workspace->workspace);

    // setup strings
    for (ANVIL__string_index i = 0; i < ANVIL__calculate__list_content_count(generation_abstraction->converted_strings, sizeof(ANVIL__buffer)); i++) {
        // setup offset
        ((ANVIL__offset*)generation_abstraction->offsets.strings_offsets.buffer.start)[i] = ANVIL__get__offset(workspace->workspace);

        // embed string
        ANVIL__code__buffer(workspace->workspace, ((ANVIL__buffer*)generation_abstraction->converted_strings.buffer.start)[i]);
    }

    return;
}

// generate program
void ANVIL__forge__anvil_program(ANVIL__buffer* final_program, ANVIL__accountling_program accountlings, ANVIL__stack_size stack_size, ANVIL__error* error) {
    ANVIL__generation_workspace workspace;
    
    // setup generation workspace
    ANVIL__open__generation_workspace(final_program, accountlings, &workspace, error);
    if (ANVIL__check__error_occured(error)) {
        return;
    }

    // forge program
    for (ANVIL__pt pass = ANVIL__pt__get_offsets; pass < ANVIL__pt__COUNT; pass++) {
        // setup pass
        ANVIL__setup__pass(workspace.workspace, pass);

        // build program
        // setup kickstarter
        ANVIL__code__start(&workspace.anvil, stack_size, ANVIL__find__generation_abstraction_by_index(&workspace, workspace.entry_point).offsets.function_start);

        // weave user defined abstractions
        // setup current
        ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&workspace.abstractions);
        ANVIL__abstraction_index index = 0;

        // for each abstraction
        while (ANVIL__check__current_within_range(current_abstraction)) {
            // weave abstraction
            ANVIL__forge__anvil_abstraction(&workspace, (ANVIL__generation_abstraction*)current_abstraction.start, ANVIL__find__accountling_abstraction_by_index(accountlings, index), error);

            // next abstraction
            current_abstraction.start += sizeof(ANVIL__generation_abstraction);
            index++;
        }

        // forge built in functions
        ANVIL__standard__code__package(workspace.workspace, &workspace.standard_offsets);
    }

    // close workspace
    ANVIL__close__generation_workspace(workspace);

    return;
}

#endif
