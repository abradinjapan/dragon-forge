#ifndef DRAGON__compiler__accounter
#define DRAGON__compiler__accounter

/* Include */
#include "parser.h"

/* Account */
// one variable
typedef struct COMPILER__accountling_variable {
    COMPILER__variable_index variable_index;
    COMPILER__variable_type_index variable_type_index;
} COMPILER__accountling_variable;

// one structure
typedef struct COMPILER__accountling_structure {
    ANVIL__list names; // 
    ANVIL__list members; // COMPILER__accountling_variable
} COMPILER__accountling_structure;

// one function
typedef struct COMPILER__accountling_function {
    // each variable
    ANVIL__list variables;
} COMPILER__accountling_function;

// one program
typedef struct COMPILER__accountling_program {

} COMPILER__accountling_program;

// account all files into one accountling program
COMPILER__accountling_program COMPILER__account__program(ANVIL__list parsling_programs, COMPILER__error* error) {
    COMPILER__accountling_program output;

    // generate all predefined types
    // TODO

    // get all user defined types (structures)
    // TODO

    // generate predefined variables
    // TODO

    // generate predefined function headers
    // TODO

    return output;
}

#endif
