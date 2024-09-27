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
    COMPILER__cell_range cells__inputs; // inputs
    COMPILER__cell_range cells__outputs; // outputs
    COMPILER__cell_range cells__body; // all cells in the workspace of a function
    COMPILER__cell_range cells__workspace; // the input, output & body cells

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

// generate user defined function statement
void COMPILER__generate__user_defined_function_statement(COMPILER__generation_workspace* workspace, COMPILER__accountling_statement statement, COMPILER__error* error) {
    return;
}

// generate program


#endif
