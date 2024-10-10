#ifndef DRAGON__compiler__standard
#define DRAGON__compiler__standard

/* Include */
#include "standard_specifications.h"
#include "standard_printing.h"
#include "standard_context_related.h"
#include "standard_list.h"

/* Build */
// build package
void STANDARD__code__package(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets) {
    // write functions
    STANDARD__code__print_binary(workspace, standard_offsets);
    STANDARD__code__print_buffer_as_string(workspace, standard_offsets);
	STANDARD__code__calculate_decimal_digit_count(workspace, standard_offsets);
	STANDARD__code__cell_to_unsigned_integer_string(workspace, standard_offsets);
    STANDARD__code__set_inputs_in_context_buffer(workspace, standard_offsets);
    STANDARD__code__get_outputs_from_context_buffer(workspace, standard_offsets);
    STANDARD__code__open_list(workspace, standard_offsets);

    return;
}

#endif
