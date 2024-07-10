#ifndef DRAGON__compiler__expander
#define DRAGON__compiler__expander

/* Include */
// compiler
#include "parser.h"

/* Expander */
// one structure
typedef struct COMPILER__expandling_structure {
    COMPILER__parsling_argument name;
    ANVIL__list members; // COMPILER__parsling_argument
} COMPILER__expandling_structure;

// one statement
typedef struct COMPILER__expandling_statement {
    
} COMPILER__expandling_statement;

// one scope
typedef struct COMPILER__expandling_scope {
    ANVIL__list statements; // COMPILER__expandling_statement
} COMPILER__expandling_scope;

// one function
typedef struct COMPILER__expandling_function {
    COMPILER__parsling_argument name;
    ANVIL__list inputs; // COMPILER__parsling_argument
    ANVIL__list outputs; // COMPILER__parsling_argument
} COMPILER__expandling_function;

// one expandling program
typedef struct COMPILER__expandling_program {
    ANVIL__list functions; // COMPILER__expandling_function
    ANVIL__list structures; // COMPILER__expandling_structure
} COMPILER__expandling_program;

#endif
