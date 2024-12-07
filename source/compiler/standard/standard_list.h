#ifndef DRAGON__compiler__standard__list
#define DRAGON__compiler__standard__list

/* Include */
#include "standard_specifications.h"

/* List */
// cell types
typedef enum STANDARD__open_list {
	// preserve start
	STANDARD__open_list__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__open_list__list_allocation_start = STANDARD__open_list__preserve__START,
	STANDARD__open_list__list_allocation_end,
	STANDARD__open_list__list_content_length,
	STANDARD__open_list__list_increase,
	STANDARD__open_list__list_append_count,

	// preserve end
	STANDARD__open_list__preserve__END,

	// inputs
	STANDARD__open_list__input__increase = ANVIL__srt__start__function_io,

	// outputs
	STANDARD__open_list__output__list_allocation_start = ANVIL__srt__start__function_io,
	STANDARD__open_list__output__list_allocation_end,
	STANDARD__open_list__output__list_content_length,
	STANDARD__open_list__output__list_increase,
	STANDARD__open_list__output__list_append_count,
} STANDARD__open_list;

// call function
void STANDARD__code__call__open_list(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__increase, ANVIL__cell_ID output__list_allocation_start, ANVIL__cell_ID output__list_allocation_end, ANVIL__cell_ID output__list_content_length, ANVIL__cell_ID output__list_increase, ANVIL__cell_ID output__list_append_count) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__increase, STANDARD__open_list__input__increase);

	// call function
	ANVIL__code__call__static(workspace, flag, (*standard_offsets).offsets[STANDARD__sot__open_list__start]);

	// pass outputs
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__open_list__output__list_allocation_start, output__list_allocation_start);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__open_list__output__list_allocation_end, output__list_allocation_end);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__open_list__output__list_content_length, output__list_content_length);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__open_list__output__list_increase, output__list_increase);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__open_list__output__list_append_count, output__list_append_count);

	return;
}

// build function
void STANDARD__code__open_list(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets) {
	// setup function offset
	(*standard_offsets).offsets[STANDARD__sot__open_list__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__open_list__preserve__START, STANDARD__open_list__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__open_list__input__increase, STANDARD__open_list__list_increase);

	// open list
    ANVIL__code__request_memory(workspace, STANDARD__open_list__list_increase, STANDARD__open_list__list_allocation_start, STANDARD__open_list__list_allocation_end);
	ANVIL__code__write_cell(workspace, ANVIL__define__zero_length, STANDARD__open_list__list_content_length);
	ANVIL__code__write_cell(workspace, ANVIL__define__zero_length, STANDARD__open_list__list_append_count);

	// setup outputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__open_list__list_allocation_start, STANDARD__open_list__output__list_allocation_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__open_list__list_allocation_end, STANDARD__open_list__output__list_allocation_end);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__open_list__list_content_length, STANDARD__open_list__output__list_content_length);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__open_list__list_increase, STANDARD__open_list__output__list_increase);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__open_list__list_append_count, STANDARD__open_list__output__list_append_count);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__open_list__preserve__START, STANDARD__open_list__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// cell types
typedef enum STANDARD__close_list {
	// preserve start
	STANDARD__close_list__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__close_list__list_allocation_start = STANDARD__close_list__preserve__START,
	STANDARD__close_list__list_allocation_end,
	STANDARD__close_list__list_content_length,
	STANDARD__close_list__list_increase,
	STANDARD__close_list__list_append_count,

	// preserve end
	STANDARD__close_list__preserve__END,

	// inputs
	STANDARD__close_list__input__list_allocation_start = ANVIL__srt__start__function_io,
	STANDARD__close_list__input__list_allocation_end,
	STANDARD__close_list__input__list_content_length,
	STANDARD__close_list__input__list_increase,
	STANDARD__close_list__input__list_append_count,
} STANDARD__close_list;

// call function
void STANDARD__code__call__close_list(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__list_allocation_start, ANVIL__cell_ID input__list_allocation_end, ANVIL__cell_ID input__list_content_length, ANVIL__cell_ID input__list_increase, ANVIL__cell_ID input__list_append_count) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__list_allocation_start, STANDARD__close_list__input__list_allocation_start);
	ANVIL__code__cell_to_cell(workspace, flag, input__list_allocation_end, STANDARD__close_list__input__list_allocation_end);
	ANVIL__code__cell_to_cell(workspace, flag, input__list_content_length, STANDARD__close_list__input__list_content_length);
	ANVIL__code__cell_to_cell(workspace, flag, input__list_increase, STANDARD__close_list__input__list_increase);
	ANVIL__code__cell_to_cell(workspace, flag, input__list_append_count, STANDARD__close_list__input__list_append_count);

	// call function
	ANVIL__code__call__static(workspace, flag, (*standard_offsets).offsets[STANDARD__sot__close_list__start]);

	return;
}

// build function
void STANDARD__code__close_list(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets) {
	// setup function offset
	(*standard_offsets).offsets[STANDARD__sot__close_list__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__close_list__preserve__START, STANDARD__close_list__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__close_list__input__list_allocation_start, STANDARD__close_list__list_allocation_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__close_list__input__list_allocation_end, STANDARD__close_list__list_allocation_end);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__close_list__input__list_content_length, STANDARD__close_list__list_content_length);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__close_list__input__list_increase, STANDARD__close_list__list_increase);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__close_list__input__list_append_count, STANDARD__close_list__list_append_count);

	// close buffer
	ANVIL__code__return_memory(workspace, STANDARD__close_list__list_allocation_start, STANDARD__close_list__list_allocation_end);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__close_list__preserve__START, STANDARD__close_list__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// cell types
typedef enum STANDARD__append_buffer {
	// preserve start
	STANDARD__append_buffer__preserve__START = ANVIL__srt__start__workspace,

	// variables
	STANDARD__append_buffer__list_allocation_start = STANDARD__append_buffer__preserve__START,
	STANDARD__append_buffer__list_allocation_end,
	STANDARD__append_buffer__list_length,
	STANDARD__append_buffer__list_increase,
	STANDARD__append_buffer__list_append_count,
	STANDARD__append_buffer__data_start,
	STANDARD__append_buffer__data_end,
	STANDARD__append_buffer__data_length,
	STANDARD__append_buffer__old_allocation_length,
	STANDARD__append_buffer__required_length,
	STANDARD__append_buffer__chunk_count,
	STANDARD__append_buffer__new_allocation_length,
	STANDARD__append_buffer__new_allocation_start,
	STANDARD__append_buffer__new_allocation_end,
	STANDARD__append_buffer__old_content_end,
	STANDARD__append_buffer__new_content_end,
	STANDARD__append_buffer__destination_start,
	STANDARD__append_buffer__destination_end,

	// preserve end
	STANDARD__append_buffer__preserve__END,

	// inputs
	STANDARD__append_buffer__input__list_allocation_start = ANVIL__srt__start__function_io,
	STANDARD__append_buffer__input__list_allocation_end,
	STANDARD__append_buffer__input__list_length,
	STANDARD__append_buffer__input__list_increase,
	STANDARD__append_buffer__input__list_append_count,
	STANDARD__append_buffer__input__data_start,
	STANDARD__append_buffer__input__data_end,

	// outputs
	STANDARD__append_buffer__output__list_allocation_start = ANVIL__srt__start__function_io,
	STANDARD__append_buffer__output__list_allocation_end,
	STANDARD__append_buffer__output__list_length,
	STANDARD__append_buffer__output__list_increase,
	STANDARD__append_buffer__output__list_append_count,
} STANDARD__append_buffer;

// call function
void STANDARD__code__call__append_buffer(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__list_allocation_start, ANVIL__cell_ID input__list_allocation_end, ANVIL__cell_ID input__list_length, ANVIL__cell_ID input__list_increase, ANVIL__cell_ID input__list_append_count, ANVIL__cell_ID input__data_start, ANVIL__cell_ID input__data_end, ANVIL__cell_ID output__list_allocation_start, ANVIL__cell_ID output__list_allocation_end, ANVIL__cell_ID output__list_length, ANVIL__cell_ID output__list_increase, ANVIL__cell_ID output__list_append_count) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__list_allocation_start, STANDARD__append_buffer__input__list_allocation_start);
	ANVIL__code__cell_to_cell(workspace, flag, input__list_allocation_end, STANDARD__append_buffer__input__list_allocation_end);
	ANVIL__code__cell_to_cell(workspace, flag, input__list_length, STANDARD__append_buffer__input__list_length);
	ANVIL__code__cell_to_cell(workspace, flag, input__list_increase, STANDARD__append_buffer__input__list_increase);
	ANVIL__code__cell_to_cell(workspace, flag, input__list_append_count, STANDARD__append_buffer__input__list_append_count);
	ANVIL__code__cell_to_cell(workspace, flag, input__data_start, STANDARD__append_buffer__input__data_start);
	ANVIL__code__cell_to_cell(workspace, flag, input__data_end, STANDARD__append_buffer__input__data_end);

	// call function
	ANVIL__code__call__static(workspace, flag, (*standard_offsets).offsets[STANDARD__sot__append_buffer__start]);

	// pass outputs
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__append_buffer__output__list_allocation_start, output__list_allocation_start);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__append_buffer__output__list_allocation_end, output__list_allocation_end);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__append_buffer__output__list_length, output__list_length);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__append_buffer__output__list_increase, output__list_increase);
	ANVIL__code__cell_to_cell(workspace, flag, STANDARD__append_buffer__output__list_append_count, output__list_append_count);

	return;
}

// build function
void STANDARD__code__append_buffer(ANVIL__workspace* workspace, STANDARD__offsets* standard_offsets) {
	// setup function offset
	(*standard_offsets).offsets[STANDARD__sot__append_buffer__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__preserve__START, STANDARD__append_buffer__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__input__list_allocation_start, STANDARD__append_buffer__list_allocation_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__input__list_allocation_end, STANDARD__append_buffer__list_allocation_end);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__input__list_length, STANDARD__append_buffer__list_length);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__input__list_increase, STANDARD__append_buffer__list_increase);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__input__list_append_count, STANDARD__append_buffer__list_append_count);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__input__data_start, STANDARD__append_buffer__data_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__input__data_end, STANDARD__append_buffer__data_end);

	// calculate lengths
	ANVIL__code__calculate__buffer_length(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__data_start, STANDARD__append_buffer__data_end, STANDARD__append_buffer__data_length);
	ANVIL__code__calculate__buffer_length(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__list_allocation_start, STANDARD__append_buffer__list_allocation_end, STANDARD__append_buffer__old_allocation_length);
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__append_buffer__list_length, STANDARD__append_buffer__data_length, ANVIL__unused_cell_ID, STANDARD__append_buffer__required_length);

	// check to see if buffer needs to be reallocated and skip if not needed
	ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__0, STANDARD__append_buffer__required_length, STANDARD__append_buffer__old_allocation_length, ANVIL__sft__never_run, (*standard_offsets).offsets[STANDARD__sot__append_buffer__skip_reallocation]);

	// calculate required buffer size
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_division, STANDARD__append_buffer__required_length, STANDARD__append_buffer__list_increase, ANVIL__unused_cell_ID, STANDARD__append_buffer__chunk_count);
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__append_buffer__chunk_count, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, STANDARD__append_buffer__chunk_count);
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_multiply, STANDARD__append_buffer__chunk_count, STANDARD__append_buffer__list_increase, ANVIL__unused_cell_ID, STANDARD__append_buffer__new_allocation_length);

	// allocate
	ANVIL__code__request_memory(workspace, STANDARD__append_buffer__new_allocation_length, STANDARD__append_buffer__new_allocation_start, STANDARD__append_buffer__new_allocation_end);

	// calculate data copy buffers
	ANVIL__code__calculate__buffer_end_address(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__list_allocation_start, STANDARD__append_buffer__list_length, STANDARD__append_buffer__old_content_end);
	ANVIL__code__calculate__buffer_end_address(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__new_allocation_start, STANDARD__append_buffer__list_length, STANDARD__append_buffer__new_content_end);

	// copy old data
	ANVIL__code__buffer_to_buffer__low_to_high(workspace, STANDARD__append_buffer__list_allocation_start, STANDARD__append_buffer__old_content_end, STANDARD__append_buffer__new_allocation_start, STANDARD__append_buffer__new_content_end);

	// delete old allocation
	ANVIL__code__return_memory(workspace, STANDARD__append_buffer__list_allocation_start, STANDARD__append_buffer__list_allocation_end);

	// install new allocation
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__new_allocation_start, STANDARD__append_buffer__list_allocation_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__new_allocation_end, STANDARD__append_buffer__list_allocation_end);

	// setup skip reallocation
	(*standard_offsets).offsets[STANDARD__sot__append_buffer__skip_reallocation] = ANVIL__get__offset(workspace);

	// calculate appending addresses
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__append_buffer__list_allocation_start, STANDARD__append_buffer__list_length, ANVIL__unused_cell_ID, STANDARD__append_buffer__destination_start);
	ANVIL__code__calculate__buffer_end_address(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__destination_start, STANDARD__append_buffer__data_length, STANDARD__append_buffer__destination_end);

	// append data
	ANVIL__code__buffer_to_buffer__low_to_high(workspace, STANDARD__append_buffer__data_start, STANDARD__append_buffer__data_end, STANDARD__append_buffer__destination_start, STANDARD__append_buffer__destination_end);

	// update length
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__required_length, STANDARD__append_buffer__list_length);

	// update append count
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, STANDARD__append_buffer__list_append_count, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, STANDARD__append_buffer__list_append_count);

	// setup outputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__list_allocation_start, STANDARD__append_buffer__output__list_allocation_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__list_allocation_end, STANDARD__append_buffer__output__list_allocation_end);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__list_length, STANDARD__append_buffer__output__list_length);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__list_increase, STANDARD__append_buffer__output__list_increase);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__list_append_count, STANDARD__append_buffer__output__list_append_count);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, STANDARD__append_buffer__preserve__START, STANDARD__append_buffer__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

#endif
