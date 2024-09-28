#ifndef DRAGON__compiler__generator
#define DRAGON__compiler__generator

/* Include */
#include "accounter.h"

/* Define */
// blank

/* Generation */
// one function
typedef struct COMPILER__generation_function {
    // offsets
    ANVIL__offset offset__function_start;
    ANVIL__offset offset__function_return;
    ANVIL__offset offset__function_data;
    ANVIL__counted_list statement_offsets;
    ANVIL__counted_list scope_offsets;
    ANVIL__counted_list data_offsets;

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
    ANVIL__counted_list user_defined_functions;
    //ANVIL__counted_list packer_functions; // TODO
} COMPILER__generation_workspace;

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

    // success
    return output;

    // failure to create, clean up object
    failure:
    return output;
}

// close generation workspace
void COMPILER__close__generation_workspace(COMPILER__generation_workspace workspace) {
    // close function bodies
    ANVIL__close__counted_list(workspace.user_defined_functions);

    return;
}

// generate user defined function statement
void COMPILER__generate__user_defined_function_scope(COMPILER__generation_workspace* workspace, COMPILER__accountling_function accountling_function, COMPILER__function_index user_defined_function_index, COMPILER__accountling_scope scope, COMPILER__error* error) {
    // setup helper variables
    COMPILER__generation_function* function = &((COMPILER__generation_function*)(*workspace).user_defined_functions.list.buffer.start)[user_defined_function_index];
    ANVIL__workspace* anvil = &(*workspace).workspace;

    // for each statement
    for (COMPILER__statement_index index = 0; index < scope.statements.count; index++) {
        // get statement
        COMPILER__accountling_statement statement = ((COMPILER__accountling_statement*)scope.statements.list.buffer.start)[index];

        // build statement
        // if statement is function call
        switch (statement.statement_type) {
        case COMPILER__ast__predefined__set__cell:
            // generate statement
            ANVIL__code__write_cell(anvil, (ANVIL__cell)statement.set_cell__raw_value, COMPILER__account__functions__get_variable_by_variable_argument(accountling_function.variables, statement.set_cell__variable_argument).cells.start);

            break;
        case COMPILER__ast__predefined__print__debug_cell:
            // generate statement
            ANVIL__code__debug__print_cell_as_decimal(anvil, COMPILER__account__functions__get_variable_by_variable_argument(accountling_function.variables, statement.print__variable_argument).cells.start);

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

    // setup function start offset
    (*function).offset__function_start = ANVIL__get__offset(anvil);

    // setup function prologue
    ANVIL__code__preserve_workspace(anvil, ANVIL__sft__always_run, (*function).cells__workspace.start, (*function).cells__workspace.end);

    // generate function body code
    COMPILER__generate__user_defined_function_scope(workspace, accountling_function, user_defined_function_index, accountling_function.scope, error);
    if (COMPILER__check__error_occured(error)) {
        return;
    }

    // setup function return offset
    (*function).offset__function_return = ANVIL__get__offset(anvil);

    // setup function epilogue
    ANVIL__code__restore_workspace(anvil, ANVIL__sft__always_run, (*function).cells__workspace.start, (*function).cells__workspace.end);

    // return to caller
    ANVIL__code__jump__explicit(anvil, ANVIL__sft__always_run, ANVIL__srt__return_address);

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
            // build abstraction
            COMPILER__generate__user_defined_function(&workspace, ((COMPILER__accountling_function*)program.functions.bodies.list.buffer.start)[index], index, error);
            if (COMPILER__check__error_occured(error)) {
                return;
            }
        }

        // build built in functions
        // TODO
    }

    // close workspace
    COMPILER__close__generation_workspace(workspace);
}

#endif
