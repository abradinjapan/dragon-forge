#ifndef DRAGON__compiler__accounter
#define DRAGON__compiler__accounter

/* Include */
#include "parser.h"

/* Account */
// account all files into one accountling program
COMPILER__accountling_program ANVIL__account__program(ANVIL__list parsling_programs, COMPILER__error* error) {
    // expand functions and structures (since functions & structures can have more than one type per definition, expanding out all definitions is necessary for semantic analysis)
}

#endif
