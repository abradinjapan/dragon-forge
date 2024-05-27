#ifndef DRAGON__compiler__lexer
#define DRAGON__compiler__lexer

/* Include */
#include "../anvil_coder.h"

/* Define */
// lexing types
typedef ANVIL__u8 ANVIL__lexling_type;
typedef ANVIL__address ANVIL__lexling_address;
typedef ANVIL__lexling_address ANVIL__lexling_start;
typedef ANVIL__lexling_address ANVIL__lexling_end;
typedef ANVIL__u64 ANVIL__lexling_index;
typedef ANVIL__u64 ANVIL__lexling_depth; // used for comments and strings

#endif
