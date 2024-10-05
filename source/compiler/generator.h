#ifndef DRAGON__compiler__generator
#define DRAGON__compiler__generator

/* Include */
#include "accounter.h"
#include "standard/standard.h"

/* Define */
// blank

/* Generation */
// one function
typedef struct COMPILER__generation_function {
    // offsets
    ANVIL__offset offset__function_start;
    ANVIL__offset offset__function_return;
    ANVIL__offset offset__function_data;
    ANVIL__counted_list statement_offsets; // ANVIL__offset
    ANVIL__counted_list scope_offsets; // ANVIL__offset
    ANVIL__counted_list data_offsets; // ANVIL__offset

    // cell ranges
    COMPILER__cell_range cells__inputs; // function io inputs
    COMPILER__cell_range cells__outputs; // function io outputs
    COMPILER__cell_range cells__workspace; // all workspace cells

    // data
    ANVIL__counted_list data__user_defined_strings; // copied from accounting, DO NOT FREE!
} COMPILER__generation_function;

// one program's workspace
// (program buffer is not in workspace as it needs to be freed separately)
typedef struct COMPILER__generation_workspace {
    ANVIL__workspace workspace;

    // functions
    ANVIL__counted_list user_defined_functions; // COMPILER__generation_function
    //ANVIL__counted_list packer_functions; // TODO
    STANDARD__offsets standard_offsets;
} COMPILER__generation_workspace;

// open generation function
COMPILER__generation_function COMPILER__open__generation_function(COMPILER__accountling_function accountlings, COMPILER__error* error) {
    COMPILER__generation_function output;

    // open offsets
    output.data_offsets = COMPILER__open__counted_list_with_error(sizeof(ANVIL__offset) * accountlings.strings.count, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }
    for (COMPILER__offset_index index = 0; index < accountlings.strings.count; index++) {
        // append blank offset
        ((ANVIL__offset*)output.data_offsets.list.buffer.start)[index] = ANVIL__define__null_offset_ID;
    }

    // setup cells
    output.cells__workspace.start = (ANVIL__u64)ANVIL__srt__start__workspace;
    output.cells__workspace.end = accountlings.next_available_workspace_cell - 1;

    return output;
}

// close generation fnuction
void COMPILER__close__generation_function(COMPILER__generation_function function) {
    // close lists
    ANVIL__close__counted_list(function.data_offsets);

    return;
}

// open generation workspace
COMPILER__generation_workspace COMPILER__open__generation_workspace(ANVIL__buffer* program_buffer, COMPILER__accountling_program accountlings, COMPILER__error* error) {
    COMPILER__generation_workspace output;

    // open anvil workspace
    output.workspace = ANVIL__setup__workspace(program_buffer);

    // open user defined functions
    output.user_defined_functions = COMPILER__open__counted_list_with_error(sizeof(COMPILER__generation_function) * accountlings.functions.headers.category[COMPILER__afht__user_defined].count, error);
    if (COMPILER__check__error_occured(error)) {
        goto failure;
    }
    output.user_defined_functions.count = accountlings.functions.bodies.count;
    for (COMPILER__function_index index = 0; index < accountlings.functions.headers.category[COMPILER__afht__user_defined].count; index++) {
        // open one function
        ((COMPILER__generation_function*)output.user_defined_functions.list.buffer.start)[index] = COMPILER__open__generation_function(((COMPILER__accountling_function*)accountlings.functions.bodies.list.buffer.start)[index], error);
        if (COMPILER__check__error_occured(error)) {
            goto failure;
        }
    }

    // success
    return output;

    // failure to create, clean up object
    failure:
    return output;
}

// close generation workspace
void COMPILER__close__generation_workspace(COMPILER__generation_workspace workspace) {
    // close function bodies
    for (COMPILER__function_index index = 0; index < workspace.user_defined_functions.count; index++) {
        // close function
        COMPILER__close__generation_function(((COMPILER__generation_function*)workspace.user_defined_functions.list.buffer.start)[index]);
    }
    ANVIL__close__counted_list(workspace.user_defined_functions);

    return;
}

// variable macro
#define COMPILER__generate__use_variable(value) COMPILER__account__functions__get_variable_by_variable_argument(accountling_function.variables, statement.value)

// generate user defined function statement
void COMPILER__generate__user_defined_function_scope(COMPILER__generation_workspace* workspace, COMPILER__accountling_function accountling_function, COMPILER__function_index user_defined_function_index, COMPILER__accountling_scope scope, COMPILER__error* error) {
    // setup helper variables
    COMPILER__generation_function* function = &((COMPILER__generation_function*)(*workspace).user_defined_functions.list.buffer.start)[user_defined_function_index];
    ANVIL__workspace* anvil = &(*workspace).workspace;

    // for each statement
    for (COMPILER__statement_index index = 0; index < scope.statements.count; index++) {
        // define variables
        ANVIL__cell_ID pack__output_to;
        ANVIL__cell_ID user_defined_function_call__current_function_io_register;

        // get statement
        COMPILER__accountling_statement statement = ((COMPILER__accountling_statement*)scope.statements.list.buffer.start)[index];

        // build statement
        // if statement is function call
        switch (statement.statement_type) {
        case COMPILER__ast__predefined__set__cell:
            ANVIL__code__write_cell(anvil, (ANVIL__cell)statement.set_cell__raw_value, COMPILER__generate__use_variable(set_cell__variable_argument).cells.start);

            break;
        case COMPILER__ast__predefined__set__string:
            ANVIL__code__write_cell(anvil, (ANVIL__cell)((ANVIL__offset*)(*function).data_offsets.list.buffer.start)[statement.set_string__string_value_index], ANVIL__srt__temp__offset);
            ANVIL__code__retrieve_embedded_buffer(anvil, ANVIL__sft__always_run, ANVIL__srt__temp__offset, COMPILER__generate__use_variable(set_string__variable_argument).cells.start, COMPILER__generate__use_variable(set_string__variable_argument).cells.end);

            break;
        case COMPILER__ast__predefined__print__debug_cell:
            ANVIL__code__debug__print_cell_as_decimal(anvil, COMPILER__generate__use_variable(print__variable_argument).cells.start);

            break;
        case COMPILER__ast__predefined__print__buffer_as_string:
            STANDARD__code__call__print_buffer_as_string(anvil, &(*workspace).standard_offsets, ANVIL__sft__always_run, COMPILER__generate__use_variable(print__variable_argument).cells.start, COMPILER__generate__use_variable(print__variable_argument).cells.end);

            break;
        case COMPILER__ast__predefined__pack__anything:
            // setup variables
            pack__output_to = COMPILER__generate__use_variable(pack__output).cells.start;

            // pack variables
            for (COMPILER__variable_index variable_index = 0; variable_index < statement.pack__inputs.count; variable_index++) {
                // get one input variable
                COMPILER__accountling_variable packer_input = COMPILER__account__functions__get_variable_by_variable_argument(accountling_function.variables, ((COMPILER__accountling_variable_argument*)statement.pack__inputs.list.buffer.start)[variable_index]);
                
                // pass variables for one structure
                for (ANVIL__cell_index cell_index = packer_input.cells.start; cell_index <= packer_input.cells.end; cell_index++) {
                    // pass one cell
                    ANVIL__code__cell_to_cell(anvil, ANVIL__sft__always_run, cell_index, pack__output_to);

                    // next output cell
                    pack__output_to++;
                }
            }

            break;
        case COMPILER__ast__user_defined_function_call:
            // setup counter
            user_defined_function_call__current_function_io_register = ANVIL__srt__start__function_io;

            // pass inputs
            for (COMPILER__variable_index input_variables = 0; input_variables < statement.function_call__inputs.count; input_variables++) {
                // get variable
                COMPILER__accountling_variable variable = COMPILER__account__functions__get_variable_by_variable_argument(accountling_function.variables, ((COMPILER__accountling_variable_argument*)statement.function_call__inputs.list.buffer.start)[input_variables]);

                // pass one variable
                for (ANVIL__cell_index input_cell = variable.cells.start; input_cell <= variable.cells.end; input_cell++) {
                    // code cell passing
                    ANVIL__code__cell_to_cell(anvil, ANVIL__sft__always_run, input_cell, user_defined_function_call__current_function_io_register);

                    // next cell
                    user_defined_function_call__current_function_io_register++;
                }
            }

            // call function
            ANVIL__code__call__static(anvil, ANVIL__sft__always_run, ((COMPILER__generation_function*)(*workspace).user_defined_functions.list.buffer.start)[statement.function_call__calling_function_header_index].offset__function_start);

            // setup counter
            user_defined_function_call__current_function_io_register = ANVIL__srt__start__function_io;

            // get outputs
            for (COMPILER__variable_index output_variables = 0; output_variables < statement.function_call__outputs.count; output_variables++) {
                // get variable
                COMPILER__accountling_variable variable = COMPILER__account__functions__get_variable_by_variable_argument(accountling_function.variables, ((COMPILER__accountling_variable_argument*)statement.function_call__outputs.list.buffer.start)[output_variables]);

                // pass one variable
                for (ANVIL__cell_index output_cell = variable.cells.start; output_cell <= variable.cells.end; output_cell++) {
                    // code cell passing
                    ANVIL__code__cell_to_cell(anvil, ANVIL__sft__always_run, user_defined_function_call__current_function_io_register, output_cell);

                    // next cell
                    user_defined_function_call__current_function_io_register++;
                }
            }

            break;
        default:
            break;
        }
    }

    return;
}

// generate user defined function
void COMPILER__generate__user_defined_function(COMPILER__generation_workspace* workspace, COMPILER__accountling_function accountling_function, COMPILER__function_index user_defined_function_index, COMPILER__error* error) {
    // setup helper variables
    COMPILER__generation_function* function = &((COMPILER__generation_function*)(*workspace).user_defined_functions.list.buffer.start)[user_defined_function_index];
    ANVIL__workspace* anvil = &(*workspace).workspace;
    ANVIL__cell_ID current_function_io_register;

    // setup function start offset
    (*function).offset__function_start = ANVIL__get__offset(anvil);

    // setup function prologue
    ANVIL__code__preserve_workspace(anvil, ANVIL__sft__always_run, (*function).cells__workspace.start, (*function).cells__workspace.end);

    // setup function io index
    current_function_io_register = ANVIL__srt__start__function_io;

    // get inputs
    for (COMPILER__variable_index input_variables = 0; input_variables < accountling_function.function_inputs.count; input_variables++) {
        // get variable
        COMPILER__accountling_variable variable = COMPILER__account__functions__get_variable_by_variable_argument(accountling_function.variables, ((COMPILER__accountling_variable_argument*)accountling_function.function_inputs.list.buffer.start)[input_variables]);

        // pass one variable
        for (ANVIL__cell_index input_cell = variable.cells.start; input_cell <= variable.cells.end; input_cell++) {
            // code cell passing
            ANVIL__code__cell_to_cell(anvil, ANVIL__sft__always_run, current_function_io_register, input_cell);

            // next cell
            current_function_io_register++;
        }
    }

    // generate function body code
    COMPILER__generate__user_defined_function_scope(workspace, accountling_function, user_defined_function_index, accountling_function.scope, error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // setup function return offset
    (*function).offset__function_return = ANVIL__get__offset(anvil);

    // setup function io index
    current_function_io_register = ANVIL__srt__start__function_io;

    // pass outputs
    for (COMPILER__variable_index output_variables = 0; output_variables < accountling_function.function_outputs.count; output_variables++) {
        // get variable
        COMPILER__accountling_variable variable = COMPILER__account__functions__get_variable_by_variable_argument(accountling_function.variables, ((COMPILER__accountling_variable_argument*)accountling_function.function_outputs.list.buffer.start)[output_variables]);

        // pass one variable
        for (ANVIL__cell_index output_cell = variable.cells.start; output_cell <= variable.cells.end; output_cell++) {
            // code cell passing
            ANVIL__code__cell_to_cell(anvil, ANVIL__sft__always_run, output_cell, current_function_io_register);

            // next cell
            current_function_io_register++;
        }
    }

    // setup function epilogue
    ANVIL__code__restore_workspace(anvil, ANVIL__sft__always_run, (*function).cells__workspace.start, (*function).cells__workspace.end);

    // return to caller
    ANVIL__code__jump__explicit(anvil, ANVIL__sft__always_run, ANVIL__srt__return_address);

    // setup data offset
    (*function).offset__function_data = ANVIL__get__offset(anvil);

    // write strings
    for (COMPILER__string_index index = 0; index < accountling_function.strings.count; index++) {
        // mark data section
        ANVIL__code__debug__mark_data_section(anvil, (ANVIL__cell)ANVIL__calculate__buffer_length(((ANVIL__buffer*)accountling_function.strings.list.buffer.start)[index]));

        // setup offset
        ((ANVIL__offset*)(*function).data_offsets.list.buffer.start)[index] = ANVIL__get__offset(anvil);

        // embed string
        ANVIL__code__buffer(anvil, ((ANVIL__buffer*)accountling_function.strings.list.buffer.start)[index]);
    }

    return;
}

// generate program
void COMPILER__generate__program(ANVIL__buffer* final_program, COMPILER__accountling_program program, ANVIL__stack_size stack_size, COMPILER__error* error) {
    COMPILER__generation_workspace workspace;

    // open generation workspace
    workspace = COMPILER__open__generation_workspace(final_program, program, error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // forge program in two passes
    for (ANVIL__pt pass = ANVIL__pt__get_offsets; pass < ANVIL__pt__COUNT; pass++) {
        // setup pass
        ANVIL__setup__pass(&workspace.workspace, pass);

        // build program
        // setup kickstarter
        ANVIL__code__start(&workspace.workspace, stack_size, ((COMPILER__generation_function*)workspace.user_defined_functions.list.buffer.start)[program.entry_point].offset__function_start);

        // build each user defined abstraction
        for (COMPILER__function_index index = 0; index < program.functions.headers.category[COMPILER__afht__user_defined].count; index++) {
            // build function
            COMPILER__generate__user_defined_function(&workspace, ((COMPILER__accountling_function*)program.functions.bodies.list.buffer.start)[index], index, error);
            if (COMPILER__check__error_occured(error)) {
                return;
            }
        }

        // build built in functions
        // build standard library
        STANDARD__code__package(&workspace.workspace, &workspace.standard_offsets);
    }

    // close workspace
    COMPILER__close__generation_workspace(workspace);

    return;
}

#endif
