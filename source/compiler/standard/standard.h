#ifndef DRAGON__compiler__standard
#define DRAGON__compiler__standard

/* Include */
#include "standard_specifications.h"
#include "standard_context_related.h"
#include "standard_list.h"
//#include "standard_casting.h"

/* Build */
// build package
void STANDARD__code__package(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets) {
    // write functions
    // context functions
    STANDARD__code__set_inputs_in_context_buffer(workspace, standard_offsets);
    STANDARD__code__get_outputs_from_context_buffer(workspace, standard_offsets);

    // list functions
    STANDARD__code__open_list(workspace, standard_offsets);
    STANDARD__code__close_list(workspace, standard_offsets);
    STANDARD__code__append_buffer(workspace, standard_offsets);

    return;
}

#endif
