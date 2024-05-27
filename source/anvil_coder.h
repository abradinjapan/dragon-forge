#ifndef DRAGON__anvil_coder
#define DRAGON__anvil_coder

/* Include */
// C
#include "anvil_specifications.h"

/* Instruction Workspace */
// pass type
typedef enum ANVIL__pt {
    ANVIL__pt__get_offsets,
    ANVIL__pt__write_program,
    ANVIL__pt__COUNT,
} ANVIL__pt;

// offsets
typedef ANVIL__u64 ANVIL__offset;

// invalid offset placeholder
#define ANVIL__invalid_offset -1

// instruction creation container
typedef struct ANVIL__workspace {
    ANVIL__pt pass;
    ANVIL__u64 current_program_offset;
    ANVIL__address write_to;
    ANVIL__buffer* program_buffer;
} ANVIL__workspace;

// setup workspace
ANVIL__workspace ANVIL__setup__workspace(ANVIL__buffer* program_buffer_destination) {
    ANVIL__workspace output;

    // setup output
    output.pass = ANVIL__pt__get_offsets;
    output.current_program_offset = 0;
    output.write_to = (*program_buffer_destination).start;
    output.program_buffer = program_buffer_destination;

    return output;
}

// create an offset
ANVIL__offset ANVIL__get__offset(ANVIL__workspace* workspace) {
    // return current offset
    return (*workspace).current_program_offset;
}

// setup pass
void ANVIL__setup__pass(ANVIL__workspace* workspace, ANVIL__pt pass) {
    // setup pass in workspace
    (*workspace).pass = pass;

    // do stuff
    switch ((*workspace).pass) {
    case ANVIL__pt__get_offsets:
        (*workspace).current_program_offset = 0;
        
        break;
    case ANVIL__pt__write_program:
        // allocate program buffer
        (*(*workspace).program_buffer) = ANVIL__open__buffer((*workspace).current_program_offset);

        // setup pass
        (*workspace).current_program_offset = 0;
        (*workspace).write_to = (*(*workspace).program_buffer).start;

        break;
    default:
        break;
    }

    return;
}

/* Write Instruction Scraplets */
// write instruction ID
void ANVIL__write_next__instruction_ID(ANVIL__workspace* workspace, ANVIL__instruction_ID instruction_ID) {
    // write value
    if ((*workspace).pass == ANVIL__pt__write_program) {
        *((ANVIL__instruction_ID*)(*workspace).write_to) = instruction_ID;
    }

    // advance
    (*workspace).current_program_offset += sizeof(ANVIL__instruction_ID);
    (*workspace).write_to = (ANVIL__address)((u64)(*workspace).write_to + sizeof(ANVIL__instruction_ID));

    return;
}

// write flag ID
void ANVIL__write_next__flag_ID(ANVIL__workspace* workspace, ANVIL__flag_ID flag_ID) {
    // write value
    if ((*workspace).pass == ANVIL__pt__write_program) {
        *((ANVIL__flag_ID*)(*workspace).write_to) = flag_ID;
    }

    // advance
    (*workspace).current_program_offset += sizeof(ANVIL__flag_ID);
    (*workspace).write_to = (ANVIL__address)((u64)(*workspace).write_to + sizeof(ANVIL__flag_ID));

    return;
}

// write operation ID
void ANVIL__write_next__operation_ID(ANVIL__workspace* workspace, ANVIL__operation_ID operation_ID) {
    // write value
    if ((*workspace).pass == ANVIL__pt__write_program) {
        *((ANVIL__operation_ID*)(*workspace).write_to) = operation_ID;
    }

    // advance
    (*workspace).current_program_offset += sizeof(ANVIL__operation_ID);
    (*workspace).write_to = (ANVIL__address)((u64)(*workspace).write_to + sizeof(ANVIL__operation_ID));

    return;
}

// write cell ID
void ANVIL__write_next__cell_ID(ANVIL__workspace* workspace, ANVIL__cell_ID cell_ID) {
    // write value
    if ((*workspace).pass == ANVIL__pt__write_program) {
        *((ANVIL__cell_ID*)(*workspace).write_to) = cell_ID;
    }

    // advance
    (*workspace).current_program_offset += sizeof(ANVIL__cell_ID);
    (*workspace).write_to = (ANVIL__address)((u64)(*workspace).write_to + sizeof(ANVIL__cell_ID));

    return;
}

// write cell value
void ANVIL__write_next__cell(ANVIL__workspace* workspace, ANVIL__cell cell_value) {
    // write value
    if ((*workspace).pass == ANVIL__pt__write_program) {
        *((ANVIL__cell*)(*workspace).write_to) = cell_value;
    }

    // advance
    (*workspace).current_program_offset += sizeof(ANVIL__cell);
    (*workspace).write_to = (ANVIL__address)((u64)(*workspace).write_to + sizeof(ANVIL__cell));

    return;
}

// write buffer
void ANVIL__write_next__buffer(ANVIL__workspace* workspace, ANVIL__buffer buffer) {
    ANVIL__length buffer_length;

    // set buffer length
    buffer_length = ANVIL__calculate__buffer_length(buffer);

    // write buffer
    if ((*workspace).pass == ANVIL__pt__write_program) {
        // write buffer length
        ANVIL__write__buffer(buffer_length, sizeof(ANVIL__length), (*workspace).write_to);

        // copy buffer
        for (ANVIL__length byte_index = 0; byte_index < buffer_length; byte_index++) {
            // copy character
            ((ANVIL__u8*)(*workspace).write_to)[byte_index + sizeof(ANVIL__length)] = ((ANVIL__u8*)buffer.start)[byte_index];
        }
    }

    // advance
    (*workspace).current_program_offset += buffer_length + sizeof(ANVIL__length);
    (*workspace).write_to = (ANVIL__address)((u64)(*workspace).write_to + buffer_length + sizeof(ANVIL__length));

    return;
}

/* Write Instructions */
// write buffer data
void ANVIL__code__buffer(ANVIL__workspace* workspace, ANVIL__buffer buffer) {
    // write data
    ANVIL__write_next__buffer(workspace, buffer);

    return;
}

// write stop instruction
void ANVIL__code__stop(ANVIL__workspace* workspace) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__stop);

    return;
}

// write write cell instruction
void ANVIL__code__write_cell(ANVIL__workspace* workspace, ANVIL__cell value, ANVIL__cell_ID value_destination) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__write_cell);
    ANVIL__write_next__cell(workspace, value);
    ANVIL__write_next__cell_ID(workspace, value_destination);

    return;
}

// write operate instruction
void ANVIL__code__operate(ANVIL__workspace* workspace, ANVIL__flag_ID flag_ID, ANVIL__operation_ID operation_ID, ANVIL__cell_ID input_0, ANVIL__cell_ID input_1, ANVIL__cell_ID input_2, ANVIL__cell_ID output_0) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__operate);
    ANVIL__write_next__flag_ID(workspace, flag_ID);
    ANVIL__write_next__operation_ID(workspace, operation_ID);
    ANVIL__write_next__cell_ID(workspace, input_0);
    ANVIL__write_next__cell_ID(workspace, input_1);
    ANVIL__write_next__cell_ID(workspace, input_2);
    ANVIL__write_next__cell_ID(workspace, output_0);

    return;
}

// write request memory instruction
void ANVIL__code__request_memory(ANVIL__workspace* workspace, ANVIL__cell_ID allocation_size, ANVIL__cell_ID allocation_start, ANVIL__cell_ID allocation_end) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__request_memory);
    ANVIL__write_next__cell_ID(workspace, allocation_size);
    ANVIL__write_next__cell_ID(workspace, allocation_start);
    ANVIL__write_next__cell_ID(workspace, allocation_end);

    return;
}

// write return memory instruction
void ANVIL__code__return_memory(ANVIL__workspace* workspace, ANVIL__cell_ID allocation_start, ANVIL__cell_ID allocation_end) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__return_memory);
    ANVIL__write_next__cell_ID(workspace, allocation_start);
    ANVIL__write_next__cell_ID(workspace, allocation_end);

    return;
}

// write address to cell instruction
void ANVIL__code__address_to_cell(ANVIL__workspace* workspace, ANVIL__flag_ID flag_ID, ANVIL__cell_ID source_address, ANVIL__cell_ID bit_count, ANVIL__cell_ID destination_cell) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__address_to_cell);
    ANVIL__write_next__flag_ID(workspace, flag_ID);
    ANVIL__write_next__cell_ID(workspace, source_address);
    ANVIL__write_next__cell_ID(workspace, bit_count);
    ANVIL__write_next__cell_ID(workspace, destination_cell);

    return;
}

// write cell to address instruction
void ANVIL__code__cell_to_address(ANVIL__workspace* workspace, ANVIL__flag_ID flag_ID, ANVIL__cell_ID source_cell, ANVIL__cell_ID bit_count, ANVIL__cell_ID destination_address) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__cell_to_address);
    ANVIL__write_next__flag_ID(workspace, flag_ID);
    ANVIL__write_next__cell_ID(workspace, source_cell);
    ANVIL__write_next__cell_ID(workspace, bit_count);
    ANVIL__write_next__cell_ID(workspace, destination_address);

    return;
}

// write file to buffer instruction
void ANVIL__code__file_to_buffer(ANVIL__workspace* workspace, ANVIL__cell_ID file_name_start, ANVIL__cell_ID file_name_end, ANVIL__cell_ID file_data_start, ANVIL__cell_ID file_data_end) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__file_to_buffer);
    ANVIL__write_next__cell_ID(workspace, file_name_start);
    ANVIL__write_next__cell_ID(workspace, file_name_end);
    ANVIL__write_next__cell_ID(workspace, file_data_start);
    ANVIL__write_next__cell_ID(workspace, file_data_end);

    return;
}

// write buffer to file instruction
void ANVIL__code__buffer_to_file(ANVIL__workspace* workspace, ANVIL__cell_ID file_data_start, ANVIL__cell_ID file_data_end, ANVIL__cell_ID file_name_start, ANVIL__cell_ID file_name_end) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__buffer_to_file);
    ANVIL__write_next__cell_ID(workspace, file_data_start);
    ANVIL__write_next__cell_ID(workspace, file_data_end);
    ANVIL__write_next__cell_ID(workspace, file_name_start);
    ANVIL__write_next__cell_ID(workspace, file_name_end);

    return;
}

// write buffer to buffer instruction
void ANVIL__code__buffer_to_buffer(ANVIL__workspace* workspace, ANVIL__cell_ID source_start, ANVIL__cell_ID source_end, ANVIL__cell_ID destination_start, ANVIL__cell_ID destination_end) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__buffer_to_buffer);
    ANVIL__write_next__cell_ID(workspace, source_start);
    ANVIL__write_next__cell_ID(workspace, source_end);
    ANVIL__write_next__cell_ID(workspace, destination_start);
    ANVIL__write_next__cell_ID(workspace, destination_end);

    return;
}

// write compile instruction
void ANVIL__code__compile(ANVIL__workspace* workspace, ANVIL__cell_ID user_codes_buffers_buffer_start, ANVIL__cell_ID user_codes_buffers_buffer_end, ANVIL__cell_ID debug_enabled, ANVIL__cell_ID output_program_start, ANVIL__cell_ID output_program_end, ANVIL__cell_ID error__error_occured, ANVIL__cell_ID error__message_start, ANVIL__cell_ID error__message_end, ANVIL__cell_ID error__character_location__file_index, ANVIL__cell_ID error__character_location__line_number, ANVIL__cell_ID error__character_location__character_index) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__compile);
    ANVIL__write_next__cell_ID(workspace, user_codes_buffers_buffer_start);
    ANVIL__write_next__cell_ID(workspace, user_codes_buffers_buffer_end);
    ANVIL__write_next__cell_ID(workspace, debug_enabled);
    ANVIL__write_next__cell_ID(workspace, output_program_start);
    ANVIL__write_next__cell_ID(workspace, output_program_end);
    ANVIL__write_next__cell_ID(workspace, error__error_occured);
    ANVIL__write_next__cell_ID(workspace, error__message_start);
    ANVIL__write_next__cell_ID(workspace, error__message_end);
    ANVIL__write_next__cell_ID(workspace, error__character_location__file_index);
    ANVIL__write_next__cell_ID(workspace, error__character_location__line_number);
    ANVIL__write_next__cell_ID(workspace, error__character_location__character_index);

    return;
}

// write run instruction
void ANVIL__code__run(ANVIL__workspace* workspace, ANVIL__cell_ID context_buffer_start, ANVIL__cell_ID context_buffer_end, ANVIL__cell_ID instruction_count) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__run);
    ANVIL__write_next__cell_ID(workspace, context_buffer_start);
    ANVIL__write_next__cell_ID(workspace, context_buffer_end);
    ANVIL__write_next__cell_ID(workspace, instruction_count);

    return;
}

// write debug putchar instruction
void ANVIL__code__debug__putchar(ANVIL__workspace* workspace, ANVIL__cell_ID printing_cell_ID) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__debug__putchar);
    ANVIL__write_next__cell_ID(workspace, printing_cell_ID);

    return;
}

// write debug print cell as decimal instruction
void ANVIL__code__debug__print_cell_as_decimal(ANVIL__workspace* workspace, ANVIL__cell_ID printing_cell_ID) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__debug__print_cell_as_decimal);
    ANVIL__write_next__cell_ID(workspace, printing_cell_ID);

    return;
}

// write debug fgets instruction
void ANVIL__code__debug__fgets(ANVIL__workspace* workspace, ANVIL__cell_ID buffer_start_ID, ANVIL__cell_ID buffer_end_ID) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__debug__fgets);
    ANVIL__write_next__cell_ID(workspace, buffer_start_ID);
    ANVIL__write_next__cell_ID(workspace, buffer_end_ID);

    return;
}

// write debug mark data section instruction
void ANVIL__code__debug__mark_data_section(ANVIL__workspace* workspace, ANVIL__cell buffer_length) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__debug__mark_data_section);
    ANVIL__write_next__cell(workspace, buffer_length);

    return;
}

// write debug mark code section instruction
void ANVIL__code__debug__mark_code_section(ANVIL__workspace* workspace, ANVIL__cell code_buffer_length) {
    // write instruction
    ANVIL__write_next__instruction_ID(workspace, ANVIL__it__debug__mark_code_section);
    ANVIL__write_next__cell(workspace, code_buffer_length);

    return;
}

/* Stack ABI Defines */
// types
typedef u64 ANVIL__preserve;
typedef ANVIL__preserve ANVIL__preserve__start;
typedef ANVIL__preserve ANVIL__preserve__end;

// stack cell types
typedef enum ANVIL__srt {
    // start of cells
    ANVIL__srt__START = ANVIL__rt__END,

    // constant cells
    ANVIL__srt__constant__0 = ANVIL__srt__START,
    ANVIL__srt__constant__1,
    ANVIL__srt__constant__2,
    ANVIL__srt__constant__4,
    ANVIL__srt__constant__8,
    ANVIL__srt__constant__16,
    ANVIL__srt__constant__24,
    ANVIL__srt__constant__32,
    ANVIL__srt__constant__40,
    ANVIL__srt__constant__48,
    ANVIL__srt__constant__56,
    ANVIL__srt__constant__64,
    ANVIL__srt__constant__cell_byte_size,
    ANVIL__srt__constant__return_address_offset_creation_size,

    // context io cells
    ANVIL__srt__input_buffer_start,
    ANVIL__srt__input_buffer_end,
    ANVIL__srt__output_buffer_start,
    ANVIL__srt__output_buffer_end,

    // stack cells
    ANVIL__srt__stack__start_address,
    ANVIL__srt__stack__current_address,
    ANVIL__srt__stack__end_address,

    // control flow cells
    ANVIL__srt__return_address,

    // temporary cells
    ANVIL__srt__temp__write,
    ANVIL__srt__temp__offset,
    ANVIL__srt__temp__address,
    ANVIL__srt__temp__flag,
    ANVIL__srt__temp__flag_ID_0,
    ANVIL__srt__temp__flag_ID_1,
    ANVIL__srt__temp__flag_ID_2,
    ANVIL__srt__temp__bit_count,
    ANVIL__srt__temp__length,

    // end of cells
    ANVIL__srt__END,

    // aliases
    ANVIL__srt__constant__true = ANVIL__srt__constant__1,
    ANVIL__srt__constant__false = ANVIL__srt__constant__0,
    ANVIL__srt__constant__cell_byte_count = ANVIL__srt__constant__8,
    ANVIL__srt__constant__cell_bit_count = ANVIL__srt__constant__64,
    ANVIL__srt__constant__bits_in_byte = ANVIL__srt__constant__8,
    ANVIL__srt__constant__ascii_character_byte_size = ANVIL__srt__constant__1,
    ANVIL__srt__constant__ascii_character_bit_size = ANVIL__srt__constant__8,

    // locations
    ANVIL__srt__start__workspace = 8192,
    ANVIL__srt__start__function_io = ANVIL__rt__TOTAL_COUNT - 8192,

    // count
    ANVIL__srt__COUNT = ANVIL__srt__END - ANVIL__srt__START,
    ANVIL__srt__workspace__COUNT = ANVIL__srt__start__function_io - ANVIL__srt__start__workspace,
} ANVIL__srt;

// stack instruction types
typedef enum ANVIL__sit {
    // start
    ANVIL__sit__START = ANVIL__it__END,

    // instructions
    ANVIL__sit__cell_to_cell = ANVIL__sit__START,
    ANVIL__sit__push_cell,
    ANVIL__sit__pop_cell,
    ANVIL__sit__calculate_dynamically__offset_address,
    ANVIL__sit__calculate_statically__offset_address,
    ANVIL__sit__jump__explicit,
    ANVIL__sit__jump__static,
    ANVIL__sit__create_return_address__directly_after_jump,
    ANVIL__sit__call__explicit,
    ANVIL__sit__call__static,
    ANVIL__sit__start,
    ANVIL__sit__preserve_workspace,
    ANVIL__sit__restore_workspace,
    ANVIL__sit__operate__flag,
    ANVIL__sit__operate__jump__explicit,
    ANVIL__sit__operate__jump__dynamic,
    ANVIL__sit__operate__jump__static,
    ANVIL__sit__setup__context,

    // end
    ANVIL__sit__END,

    // counts
    ANVIL__sit__COUNT = ANVIL__sit__END - ANVIL__sit__START,
    ANVIL__sit__TOTAL_COUNT = ANVIL__sit__END - ANVIL__it__START,
} ANVIL__sit;

// stack instruction length types
typedef enum ANVIL__silt {
    ANVIL__silt__cell_to_cell = ANVIL__ilt__operate,
    ANVIL__silt__push_cell = ANVIL__ilt__cell_to_address + ANVIL__ilt__operate,
    ANVIL__silt__pop_cell = ANVIL__ilt__operate + ANVIL__ilt__address_to_cell,
    ANVIL__silt__calculate_dynamically__offset_address = ANVIL__ilt__operate,
    ANVIL__silt__calculate_statically__offset_address = ANVIL__ilt__write_cell + ANVIL__silt__calculate_dynamically__offset_address,
    ANVIL__silt__jump__explicit = ANVIL__silt__cell_to_cell,
    ANVIL__silt__jump__static = ANVIL__silt__calculate_statically__offset_address + ANVIL__silt__jump__explicit,
} ANVIL__silt;

// stack flag types
typedef enum ANVIL__sft {
    // start of flags
    ANVIL__sft__START = 0,

    // flags
    ANVIL__sft__always_run = ANVIL__sft__START,
    ANVIL__sft__never_run,
    ANVIL__sft__temp,

    // end of stack flags
    ANVIL__sft__END,
} ANVIL__sft;

// stack size
typedef u64 ANVIL__stack_size;

/* Context IO */
// pass input
void ANVIL__set__input(ANVIL__context* context, ANVIL__buffer input) {
    // write data to cells
    (*context).cells[ANVIL__srt__input_buffer_start] = (ANVIL__cell)input.start;
    (*context).cells[ANVIL__srt__input_buffer_end] = (ANVIL__cell)input.end;

    return;
}

/* Stack ABI Code */
// move one cell to the next
void ANVIL__code__cell_to_cell(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID source, ANVIL__cell_ID destination) {
    // write code
    ANVIL__code__operate(workspace, flag, ANVIL__ot__cell_to_cell, source, ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, destination);

    return;
}

// push a cell onto the stack
void ANVIL__code__push_cell(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID source_cell) {
    // write data to stack
    ANVIL__code__cell_to_address(workspace, flag, source_cell, ANVIL__srt__constant__64, ANVIL__srt__stack__current_address);

    // increase stack pointer
    ANVIL__code__operate(workspace, flag, ANVIL__ot__integer_add, ANVIL__srt__stack__current_address, ANVIL__srt__constant__cell_byte_size, ANVIL__unused_cell_ID, ANVIL__srt__stack__current_address);

    return;
}

// pop a cell from the stack
void ANVIL__code__pop_cell(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID destination_cell) {
    // decrease stack pointer
    ANVIL__code__operate(workspace, flag, ANVIL__ot__integer_subtract, ANVIL__srt__stack__current_address, ANVIL__srt__constant__cell_byte_size, ANVIL__unused_cell_ID, ANVIL__srt__stack__current_address);

    // read data from stack
    ANVIL__code__address_to_cell(workspace, flag, ANVIL__srt__stack__current_address, ANVIL__srt__constant__64, destination_cell);

    return;
}

// calculate an address from the program start and an offset cell
void ANVIL__code__calculate_dynamically__offset_address(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID offset_cell, ANVIL__cell_ID destination) {
    // calculate address
    ANVIL__code__operate(workspace, flag, ANVIL__ot__integer_add, ANVIL__rt__program_start_address, offset_cell, ANVIL__unused_cell_ID, destination);

    return;
}

// calculate an address from the program start and an offset
void ANVIL__code__calculate_statically__offset_address(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__offset offset, ANVIL__cell_ID destination) {
    // write temp
    ANVIL__code__write_cell(workspace, (ANVIL__cell)offset, ANVIL__srt__temp__offset);

    // calculate address
    ANVIL__code__calculate_dynamically__offset_address(workspace, flag, ANVIL__srt__temp__offset, destination);

    return;
}

// jump to a specific address
void ANVIL__code__jump__explicit(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID address) {
    // jump
    ANVIL__code__cell_to_cell(workspace, flag, address, ANVIL__rt__program_current_address);

    return;
}

// jump to an offset calculated address
void ANVIL__code__jump__static(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__offset offset) {
    // calculate offset
    ANVIL__code__calculate_statically__offset_address(workspace, flag, offset, ANVIL__srt__temp__address);

    // jump
    ANVIL__code__jump__explicit(workspace, flag, ANVIL__srt__temp__address);

    return;
}

// create return address
void ANVIL__code__create_return_address__directly_after_jump(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID destination) {
    // create offset
    ANVIL__code__operate(workspace, flag, ANVIL__ot__integer_add, ANVIL__rt__program_current_address, ANVIL__srt__constant__return_address_offset_creation_size, ANVIL__unused_cell_ID, destination);

    return;
}

// call function explicitly
void ANVIL__code__call__explicit(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID address) {
    // preserve return address
    ANVIL__code__push_cell(workspace, flag, ANVIL__srt__return_address);

    // setup new return address
    ANVIL__code__create_return_address__directly_after_jump(workspace, flag, ANVIL__srt__return_address);

    // jump
    ANVIL__code__jump__explicit(workspace, flag, address);

    // restore return address
    ANVIL__code__pop_cell(workspace, flag, ANVIL__srt__return_address);

    return;
}

// call function statically
void ANVIL__code__call__static(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__offset offset) {
    // calculate address
    ANVIL__code__calculate_statically__offset_address(workspace, flag, offset, ANVIL__srt__temp__address);

    // call function with offset set
    ANVIL__code__call__explicit(workspace, flag, ANVIL__srt__temp__address);

    return;
}

// kickstart program (assumes program variables are set!)
void ANVIL__code__start(ANVIL__workspace* workspace, ANVIL__stack_size stack_size, ANVIL__offset jump_to) {
    // setup code marker
    ANVIL__code__debug__mark_code_section(workspace, 0);

    // setup error cell
    ANVIL__code__write_cell(workspace, (ANVIL__cell)0, ANVIL__rt__error_code);

    // setup flag cells
    ANVIL__code__write_cell(workspace, (ANVIL__cell)1, ANVIL__rt__flags_0);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)0, ANVIL__rt__flags_1);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)0, ANVIL__rt__flags_2);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)0, ANVIL__rt__flags_3);

    // setup constants
    ANVIL__code__write_cell(workspace, (ANVIL__cell)0, ANVIL__srt__constant__0);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)1, ANVIL__srt__constant__1);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)2, ANVIL__srt__constant__2);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)4, ANVIL__srt__constant__4);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)8, ANVIL__srt__constant__8);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)16, ANVIL__srt__constant__16);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)24, ANVIL__srt__constant__24);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)32, ANVIL__srt__constant__32);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)40, ANVIL__srt__constant__40);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)48, ANVIL__srt__constant__48);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)56, ANVIL__srt__constant__56);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)64, ANVIL__srt__constant__64);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)sizeof(ANVIL__cell), ANVIL__srt__constant__cell_byte_size);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)ANVIL__silt__jump__explicit, ANVIL__srt__constant__return_address_offset_creation_size);

    // setup output
    ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__0, ANVIL__srt__output_buffer_start);
    ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__srt__constant__0, ANVIL__srt__output_buffer_end);

    // setup stack
    ANVIL__code__write_cell(workspace, (ANVIL__cell)stack_size, ANVIL__srt__temp__write);
    ANVIL__code__request_memory(workspace, ANVIL__srt__temp__write, ANVIL__srt__stack__start_address, ANVIL__srt__stack__end_address);
    ANVIL__code__cell_to_cell(workspace, (ANVIL__flag_ID)ANVIL__sft__always_run, ANVIL__srt__stack__start_address, ANVIL__srt__stack__current_address);

    // jump to main
    ANVIL__code__call__static(workspace, ANVIL__sft__always_run, jump_to);

    // deallocate stack
    ANVIL__code__return_memory(workspace, ANVIL__srt__stack__start_address, ANVIL__srt__stack__end_address);

    // quit program
    ANVIL__code__stop(workspace);

    return;
}

// preserve workspace
void ANVIL__code__preserve_workspace(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__preserve__start preserve_start, ANVIL__preserve__end preserve_end) {
    // preserve flags
    ANVIL__code__push_cell(workspace, flag, ANVIL__rt__flags_0);
    ANVIL__code__push_cell(workspace, flag, ANVIL__rt__flags_1);
    ANVIL__code__push_cell(workspace, flag, ANVIL__rt__flags_2);
    ANVIL__code__push_cell(workspace, flag, ANVIL__rt__flags_3);

    // preserve error code
    ANVIL__code__push_cell(workspace, flag, ANVIL__rt__error_code);

    // preserve workspace cells
    for (ANVIL__preserve i = preserve_start; i <= preserve_end; i++) {
        // preserve cell
        ANVIL__code__push_cell(workspace, flag, i);
    }

    return;
}

// restore workspace
void ANVIL__code__restore_workspace(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__preserve__start preserve_start, ANVIL__preserve__end preserve_end) {
    // restore workspace cells
    for (ANVIL__preserve i = preserve_end; i >= preserve_start; i--) {
        // restore cell
        ANVIL__code__pop_cell(workspace, flag, i);
    }

    // restore error code
    ANVIL__code__pop_cell(workspace, flag, ANVIL__rt__error_code);

    // restore flags
    ANVIL__code__pop_cell(workspace, flag, ANVIL__rt__flags_3);
    ANVIL__code__pop_cell(workspace, flag, ANVIL__rt__flags_2);
    ANVIL__code__pop_cell(workspace, flag, ANVIL__rt__flags_1);
    ANVIL__code__pop_cell(workspace, flag, ANVIL__rt__flags_0);

    return;
}

// operate flag
void ANVIL__code__operate__flag(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID lower_bound, ANVIL__cell_ID value, ANVIL__cell_ID upper_bound, ANVIL__flag_ID invert_result, ANVIL__cell_ID output_flag_ID) {
    // get comparison result set to variable
    ANVIL__code__operate(workspace, flag, ANVIL__ot__integer_within_range, lower_bound, value, upper_bound, ANVIL__srt__temp__flag);

    // write flag
    ANVIL__code__operate(workspace, flag, ANVIL__ot__flag_set, ANVIL__srt__temp__flag, ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, output_flag_ID);

    // invert flag
    ANVIL__code__operate(workspace, invert_result, ANVIL__ot__flag_invert, output_flag_ID, ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, output_flag_ID);

    return;
}

// operate jump explicitly
void ANVIL__code__operate__jump__explicit(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID lower_bound, ANVIL__cell_ID value, ANVIL__cell_ID upper_bound, ANVIL__flag_ID invert_result, ANVIL__cell_ID jump_address) {
    // setup flag temp
    ANVIL__code__write_cell(workspace, (ANVIL__cell)ANVIL__sft__temp, ANVIL__srt__temp__flag_ID_0);

    // perform comparison
    ANVIL__code__operate__flag(workspace, flag, lower_bound, value, upper_bound, invert_result, ANVIL__srt__temp__flag_ID_0);

    // attempt jump
    ANVIL__code__jump__explicit(workspace, ANVIL__sft__temp, jump_address);

    return;
}

// operate jump dynamically
void ANVIL__code__operate__jump__dynamic(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID lower_bound, ANVIL__cell_ID value, ANVIL__cell_ID upper_bound, ANVIL__flag_ID invert_result, ANVIL__cell_ID jump_offset_cell) {
    // calculate offset
    ANVIL__code__calculate_dynamically__offset_address(workspace, flag, jump_offset_cell, ANVIL__srt__temp__address);

    // attempt jump
    ANVIL__code__operate__jump__explicit(workspace, flag, lower_bound, value, upper_bound, invert_result, ANVIL__srt__temp__address);

    return;
}

// operate jump statically
void ANVIL__code__operate__jump__static(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID lower_bound, ANVIL__cell_ID value, ANVIL__cell_ID upper_bound, ANVIL__flag_ID invert_result, ANVIL__offset jump_offset) {
    // write offset to cell
    ANVIL__code__write_cell(workspace, (ANVIL__cell)jump_offset, ANVIL__srt__temp__offset);

    // attempt jump
    ANVIL__code__operate__jump__dynamic(workspace, flag, lower_bound, value, upper_bound, invert_result, ANVIL__srt__temp__offset);

    return;
}

// retrieve an embedded buffer // UNTESTED!
void ANVIL__code__retrieve_embedded_buffer(ANVIL__workspace* workspace, ANVIL__flag_ID flag, ANVIL__cell_ID program_offset, ANVIL__cell_ID buffer_start, ANVIL__cell_ID buffer_end) {
    // calculate buffer start
    ANVIL__code__calculate_dynamically__offset_address(workspace, flag, program_offset, ANVIL__srt__temp__address);

    // get buffer length
    ANVIL__code__write_cell(workspace, (ANVIL__cell)(ANVIL__define__bits_in_byte * sizeof(ANVIL__length)), ANVIL__srt__temp__bit_count);
    ANVIL__code__address_to_cell(workspace, flag, ANVIL__srt__temp__address, ANVIL__srt__temp__bit_count, ANVIL__srt__temp__length);

    // calculate buffer data start
    ANVIL__code__operate(workspace, flag, ANVIL__ot__integer_add, ANVIL__srt__temp__address, ANVIL__srt__constant__cell_byte_count, ANVIL__unused_cell_ID, buffer_start);

    // calculate buffer data end
    ANVIL__code__operate(workspace, flag, ANVIL__ot__integer_add, buffer_start, ANVIL__srt__temp__length, ANVIL__unused_cell_ID, ANVIL__srt__temp__address);
    ANVIL__code__operate(workspace, flag, ANVIL__ot__integer_subtract, ANVIL__srt__temp__address, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, buffer_end);

    return;
}

// setup context
void ANVIL__code__setup__context(ANVIL__workspace* workspace, ANVIL__cell_ID program_buffer_start, ANVIL__cell_ID program_buffer_end, ANVIL__cell_ID context_buffer_start, ANVIL__cell_ID context_buffer_end) {
    // setup allocation size
    ANVIL__code__write_cell(workspace, (ANVIL__cell)sizeof(ANVIL__context), ANVIL__srt__temp__write);

    // allocate context
    ANVIL__code__request_memory(workspace, ANVIL__srt__temp__write, context_buffer_start, context_buffer_end);

    // setup skeleton context
    // setup buffer start
    ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, context_buffer_start, ANVIL__srt__temp__address);
    ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, program_buffer_start, ANVIL__srt__constant__cell_bit_count, ANVIL__srt__temp__address);

    // setup current address
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__srt__temp__address, ANVIL__srt__constant__cell_byte_count, ANVIL__unused_cell_ID, ANVIL__srt__temp__address);
    ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, program_buffer_start, ANVIL__srt__constant__cell_bit_count, ANVIL__srt__temp__address);

    // setup end address
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__srt__temp__address, ANVIL__srt__constant__cell_byte_count, ANVIL__unused_cell_ID, ANVIL__srt__temp__address);
    ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, program_buffer_end, ANVIL__srt__constant__cell_bit_count, ANVIL__srt__temp__address);
    
    return;
}

/* Standard Library */
// standard offset type
typedef enum ANVIL__standard__sot {
    // offsets
    ANVIL__standard__sot__package_start,

    // translation functions
    ANVIL__standard__sot__calculate_decimal_digit_count__start,
    ANVIL__standard__sot__calculate_decimal_digit_count__loop_start,
    ANVIL__standard__sot__calculate_decimal_digit_count__return,
    ANVIL__standard__sot__cell_to_unsigned_integer_string__start,
    ANVIL__standard__sot__cell_to_unsigned_integer_string__loop_start,
    ANVIL__standard__sot__cell_to_unsigned_integer_string__return,

    // printing functions
    ANVIL__standard__sot__print_binary__start,
    ANVIL__standard__sot__print_buffer_as_string__start,
    ANVIL__standard__sot__print_buffer_as_string__loop__start,
    ANVIL__standard__sot__print_buffer_as_string__loop__end,

    // context functions
    ANVIL__standard__sot__set_inputs_in_context_buffer__start,
    ANVIL__standard__sot__get_outputs_from_context_buffer__start,

    // count
    ANVIL__standard__sot__COUNT,
} ANVIL__standard__sot;

// offsets
typedef struct ANVIL__standard__offsets {
    ANVIL__offset offsets[ANVIL__standard__sot__COUNT];
} ANVIL__standard__offsets;

// cell types
typedef enum ANVIL__standard__calculate_decimal_digit_count {
	// preserve start
	ANVIL__standard__calculate_decimal_digit_count__preserve__START = ANVIL__srt__start__workspace,

	// variables
	ANVIL__standard__calculate_decimal_digit_count__value = ANVIL__standard__calculate_decimal_digit_count__preserve__START,
	ANVIL__standard__calculate_decimal_digit_count__digit_count,
	ANVIL__standard__calculate_decimal_digit_count__zero,
	ANVIL__standard__calculate_decimal_digit_count__ten,

	// preserve end
	ANVIL__standard__calculate_decimal_digit_count__preserve__END,

	// inputs
	ANVIL__standard__calculate_decimal_digit_count__input__value = ANVIL__srt__start__function_io,

	// outputs
	ANVIL__standard__calculate_decimal_digit_count__output__digit_count = ANVIL__srt__start__function_io,
} ANVIL__standard__calculate_decimal_digit_count;

// call function
void ANVIL__standard__code__call__calculate_decimal_digit_count(ANVIL__workspace* workspace, ANVIL__standard__offsets* std_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__value, ANVIL__cell_ID output__digit_count) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__value, ANVIL__standard__calculate_decimal_digit_count__input__value);

	// call function
	ANVIL__code__call__static(workspace, flag, (*std_offsets).offsets[ANVIL__standard__sot__calculate_decimal_digit_count__start]);

	// pass outputs
	ANVIL__code__cell_to_cell(workspace, flag, ANVIL__standard__calculate_decimal_digit_count__output__digit_count, output__digit_count);

	return;
}

// build function
void ANVIL__standard__code__calculate_decimal_digit_count(ANVIL__workspace* workspace, ANVIL__standard__offsets* std_offsets) {
	// setup function offset
	(*std_offsets).offsets[ANVIL__standard__sot__calculate_decimal_digit_count__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__calculate_decimal_digit_count__preserve__START, ANVIL__standard__calculate_decimal_digit_count__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__calculate_decimal_digit_count__input__value, ANVIL__standard__calculate_decimal_digit_count__value);

    // check for zero
    ANVIL__code__write_cell(workspace, (ANVIL__cell)0, ANVIL__standard__calculate_decimal_digit_count__zero);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)1, ANVIL__standard__calculate_decimal_digit_count__digit_count);
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, ANVIL__standard__calculate_decimal_digit_count__zero, ANVIL__standard__calculate_decimal_digit_count__value, ANVIL__standard__calculate_decimal_digit_count__zero, ANVIL__sft__never_run, (*std_offsets).offsets[ANVIL__standard__sot__calculate_decimal_digit_count__return]);

    // not a zero, set to default
    ANVIL__code__write_cell(workspace, (ANVIL__cell)0, ANVIL__standard__calculate_decimal_digit_count__digit_count);

    // setup remaining variables
    ANVIL__code__write_cell(workspace, (ANVIL__cell)10, ANVIL__standard__calculate_decimal_digit_count__ten);

	// setup loop start
	(*std_offsets).offsets[ANVIL__standard__sot__calculate_decimal_digit_count__loop_start] = ANVIL__get__offset(workspace);

    // check for remaining digits
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, ANVIL__standard__calculate_decimal_digit_count__zero, ANVIL__standard__calculate_decimal_digit_count__value, ANVIL__standard__calculate_decimal_digit_count__zero, ANVIL__sft__never_run, (*std_offsets).offsets[ANVIL__standard__sot__calculate_decimal_digit_count__return]);

    // perform divide
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_division, ANVIL__standard__calculate_decimal_digit_count__value, ANVIL__standard__calculate_decimal_digit_count__ten, ANVIL__unused_cell_ID, ANVIL__standard__calculate_decimal_digit_count__value);

    // increment count
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__standard__calculate_decimal_digit_count__digit_count, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, ANVIL__standard__calculate_decimal_digit_count__digit_count);

    // jump to beginning
    ANVIL__code__jump__static(workspace, ANVIL__sft__always_run, (*std_offsets).offsets[ANVIL__standard__sot__calculate_decimal_digit_count__loop_start]);

	// setup loop return
	(*std_offsets).offsets[ANVIL__standard__sot__calculate_decimal_digit_count__return] = ANVIL__get__offset(workspace);

	// setup outputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__calculate_decimal_digit_count__digit_count, ANVIL__standard__calculate_decimal_digit_count__output__digit_count);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__calculate_decimal_digit_count__preserve__START, ANVIL__standard__calculate_decimal_digit_count__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// cell types
typedef enum ANVIL__standard__cell_to_unsigned_integer_string {
	// preserve start
	ANVIL__standard__cell_to_unsigned_integer_string__preserve__START = ANVIL__srt__start__workspace,

	// variables
	ANVIL__standard__cell_to_unsigned_integer_string__value = ANVIL__standard__cell_to_unsigned_integer_string__preserve__START,
	ANVIL__standard__cell_to_unsigned_integer_string__number_start,
	ANVIL__standard__cell_to_unsigned_integer_string__number_end,
	ANVIL__standard__cell_to_unsigned_integer_string__current,
	ANVIL__standard__cell_to_unsigned_integer_string__digit_count,
	ANVIL__standard__cell_to_unsigned_integer_string__character,
	ANVIL__standard__cell_to_unsigned_integer_string__ten,
	ANVIL__standard__cell_to_unsigned_integer_string__base_digit_character,

	// preserve end
	ANVIL__standard__cell_to_unsigned_integer_string__preserve__END,

	// inputs
	ANVIL__standard__cell_to_unsigned_integer_string__input__value = ANVIL__srt__start__function_io,

	// outputs
	ANVIL__standard__cell_to_unsigned_integer_string__output__number_start = ANVIL__srt__start__function_io,
	ANVIL__standard__cell_to_unsigned_integer_string__output__number_end,
} ANVIL__standard__cell_to_unsigned_integer_string;

// call function
void ANVIL__standard__code__call__cell_to_unsigned_integer_string(ANVIL__workspace* workspace, ANVIL__standard__offsets* std_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__value, ANVIL__cell_ID output__number_start, ANVIL__cell_ID output__number_end) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__value, ANVIL__standard__cell_to_unsigned_integer_string__input__value);

	// call function
	ANVIL__code__call__static(workspace, flag, (*std_offsets).offsets[ANVIL__standard__sot__cell_to_unsigned_integer_string__start]);

	// pass outputs
	ANVIL__code__cell_to_cell(workspace, flag, ANVIL__standard__cell_to_unsigned_integer_string__output__number_start, output__number_start);
	ANVIL__code__cell_to_cell(workspace, flag, ANVIL__standard__cell_to_unsigned_integer_string__output__number_end, output__number_end);

	return;
}

// build function
void ANVIL__standard__code__cell_to_unsigned_integer_string(ANVIL__workspace* workspace, ANVIL__standard__offsets* std_offsets) {
	// setup function offset
	(*std_offsets).offsets[ANVIL__standard__sot__cell_to_unsigned_integer_string__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__preserve__START, ANVIL__standard__cell_to_unsigned_integer_string__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__input__value, ANVIL__standard__cell_to_unsigned_integer_string__value);

	// setup variables
	ANVIL__code__write_cell(workspace, (ANVIL__cell)10, ANVIL__standard__cell_to_unsigned_integer_string__ten);
	ANVIL__code__write_cell(workspace, (ANVIL__cell)'0', ANVIL__standard__cell_to_unsigned_integer_string__base_digit_character);

	// calculate digit count
	ANVIL__standard__code__call__calculate_decimal_digit_count(workspace, std_offsets, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__value, ANVIL__standard__cell_to_unsigned_integer_string__digit_count);

	// allocate output
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_multiply, ANVIL__srt__constant__ascii_character_byte_size, ANVIL__standard__cell_to_unsigned_integer_string__digit_count, ANVIL__unused_cell_ID, ANVIL__srt__temp__length);
	ANVIL__code__request_memory(workspace, ANVIL__srt__temp__length, ANVIL__standard__cell_to_unsigned_integer_string__number_start, ANVIL__standard__cell_to_unsigned_integer_string__number_end);

	// init to zero
	ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__base_digit_character, ANVIL__srt__constant__ascii_character_bit_size, ANVIL__standard__cell_to_unsigned_integer_string__number_start);

	// convert to string
	// setup current
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__number_end, ANVIL__standard__cell_to_unsigned_integer_string__current);

	// setup loop start offset
	std_offsets->offsets[ANVIL__standard__sot__cell_to_unsigned_integer_string__loop_start] = ANVIL__get__offset(workspace);

	// check for loop end
	ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__number_start, ANVIL__standard__cell_to_unsigned_integer_string__current, ANVIL__standard__cell_to_unsigned_integer_string__number_end, ANVIL__sft__always_run, std_offsets->offsets[ANVIL__standard__sot__cell_to_unsigned_integer_string__return]);

	// calculate character
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_modulous, ANVIL__standard__cell_to_unsigned_integer_string__value, ANVIL__standard__cell_to_unsigned_integer_string__ten, ANVIL__unused_cell_ID, ANVIL__standard__cell_to_unsigned_integer_string__character);
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__standard__cell_to_unsigned_integer_string__base_digit_character, ANVIL__standard__cell_to_unsigned_integer_string__character, ANVIL__unused_cell_ID, ANVIL__standard__cell_to_unsigned_integer_string__character);

	// write character
	ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__character, ANVIL__srt__constant__ascii_character_bit_size, ANVIL__standard__cell_to_unsigned_integer_string__current);

	// next character
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_division, ANVIL__standard__cell_to_unsigned_integer_string__value, ANVIL__standard__cell_to_unsigned_integer_string__ten, ANVIL__unused_cell_ID, ANVIL__standard__cell_to_unsigned_integer_string__value);
	ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_subtract, ANVIL__standard__cell_to_unsigned_integer_string__current, ANVIL__srt__constant__ascii_character_byte_size, ANVIL__unused_cell_ID, ANVIL__standard__cell_to_unsigned_integer_string__current);
	ANVIL__code__jump__static(workspace, ANVIL__sft__always_run, std_offsets->offsets[ANVIL__standard__sot__cell_to_unsigned_integer_string__loop_start]);

	// setup return
	std_offsets->offsets[ANVIL__standard__sot__cell_to_unsigned_integer_string__return] = ANVIL__get__offset(workspace);

	// setup outputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__number_start, ANVIL__standard__cell_to_unsigned_integer_string__output__number_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__number_end, ANVIL__standard__cell_to_unsigned_integer_string__output__number_end);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__cell_to_unsigned_integer_string__preserve__START, ANVIL__standard__cell_to_unsigned_integer_string__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// cell types
typedef enum ANVIL__standard__print_binary {
	// preserve start
	ANVIL__standard__print_binary__preserve__START = ANVIL__srt__start__workspace,

	// variables
	ANVIL__standard__print_binary__value = ANVIL__standard__print_binary__preserve__START,
    ANVIL__standard__print_binary__bit,
    ANVIL__standard__print_binary__character,

	// preserve end
	ANVIL__standard__print_binary__preserve__END,

	// inputs
	ANVIL__standard__print_binary__input__value = ANVIL__srt__start__function_io,

	// outputs
} ANVIL__standard__print_binary;

// call function
void ANVIL__standard__code__call__print_binary(ANVIL__workspace* workspace, ANVIL__standard__offsets* standard_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__value) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__value, ANVIL__standard__print_binary__input__value);

	// call function
	ANVIL__code__call__static(workspace, flag, (*standard_offsets).offsets[ANVIL__standard__sot__print_binary__start]);

	return;
}

// build function
void ANVIL__standard__code__print_binary(ANVIL__workspace* workspace, ANVIL__standard__offsets* standard_offsets) {
	// setup function offset
	(*standard_offsets).offsets[ANVIL__standard__sot__print_binary__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__print_binary__preserve__START, ANVIL__standard__print_binary__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__print_binary__input__value, ANVIL__standard__print_binary__value);

	// print all bits
    // for each byte
    for (ANVIL__byte_count byte_index = 0; byte_index < sizeof(ANVIL__cell); byte_index++) {
        // for each bit
        for (ANVIL__bit_count bit_index = 0; bit_index < ANVIL__define__bits_in_byte; bit_index++) {
            // get bit
            ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__bits_and, ANVIL__standard__print_binary__value, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, ANVIL__standard__print_binary__bit);

            // get character
            ANVIL__code__write_cell(workspace, (ANVIL__cell)'0', ANVIL__standard__print_binary__character);
            ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__standard__print_binary__character, ANVIL__standard__print_binary__bit, ANVIL__unused_cell_ID, ANVIL__standard__print_binary__character);

            // print character
            ANVIL__code__debug__putchar(workspace, ANVIL__standard__print_binary__character);

            // next bit
            ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__bits_shift_lower, ANVIL__standard__print_binary__value, ANVIL__srt__constant__1, ANVIL__unused_cell_ID, ANVIL__standard__print_binary__value);
        }

        // print separator if not end of loop
        if (byte_index < sizeof(ANVIL__cell) - 1) {
            ANVIL__code__write_cell(workspace, (ANVIL__cell)'_', ANVIL__standard__print_binary__character);
            ANVIL__code__debug__putchar(workspace, ANVIL__standard__print_binary__character);
        }
    }

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__print_binary__preserve__START, ANVIL__standard__print_binary__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// cell types
typedef enum ANVIL__standard__print_buffer_as_string {
	// preserve start
	ANVIL__standard__print_buffer_as_string__preserve__START = ANVIL__srt__start__workspace,

	// variables
	ANVIL__standard__print_buffer_as_string__buffer_start = ANVIL__standard__print_buffer_as_string__preserve__START,
	ANVIL__standard__print_buffer_as_string__buffer_end,
	ANVIL__standard__print_buffer_as_string__current,
	ANVIL__standard__print_buffer_as_string__character,

	// preserve end
	ANVIL__standard__print_buffer_as_string__preserve__END,

	// inputs
	ANVIL__standard__print_buffer_as_string__input__buffer_start = ANVIL__srt__start__function_io,
	ANVIL__standard__print_buffer_as_string__input__buffer_end,

	// outputs
} ANVIL__standard__print_buffer_as_string;

// call function
void ANVIL__standard__code__call__print_buffer_as_string(ANVIL__workspace* workspace, ANVIL__standard__offsets* std_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__buffer_start, ANVIL__cell_ID input__buffer_end) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__buffer_start, ANVIL__standard__print_buffer_as_string__input__buffer_start);
	ANVIL__code__cell_to_cell(workspace, flag, input__buffer_end, ANVIL__standard__print_buffer_as_string__input__buffer_end);

	// call function
	ANVIL__code__call__static(workspace, flag, (*std_offsets).offsets[ANVIL__standard__sot__print_buffer_as_string__start]);

	return;
}

// build function
void ANVIL__standard__code__print_buffer_as_string(ANVIL__workspace* workspace, ANVIL__standard__offsets* std_offsets) {
	// setup function offset
	(*std_offsets).offsets[ANVIL__standard__sot__print_buffer_as_string__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__print_buffer_as_string__preserve__START, ANVIL__standard__print_buffer_as_string__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__print_buffer_as_string__input__buffer_start, ANVIL__standard__print_buffer_as_string__buffer_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__print_buffer_as_string__input__buffer_end, ANVIL__standard__print_buffer_as_string__buffer_end);

	// setup current
    ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__print_buffer_as_string__buffer_start, ANVIL__standard__print_buffer_as_string__current);

    // setup loop start offset
	(*std_offsets).offsets[ANVIL__standard__sot__print_buffer_as_string__loop__start] = ANVIL__get__offset(workspace);

    // check if loop range is still valid and exit if necessary
    ANVIL__code__write_cell(workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__sft__temp, ANVIL__srt__temp__flag_ID_0);
    ANVIL__code__operate__jump__static(workspace, ANVIL__sft__always_run, ANVIL__standard__print_buffer_as_string__buffer_start, ANVIL__standard__print_buffer_as_string__current, ANVIL__standard__print_buffer_as_string__buffer_end, ANVIL__sft__always_run, (*std_offsets).offsets[ANVIL__standard__sot__print_buffer_as_string__loop__end]);

    // read character
    ANVIL__code__address_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__print_buffer_as_string__current, ANVIL__srt__constant__ascii_character_bit_size, ANVIL__standard__print_buffer_as_string__character);

    // print character
    ANVIL__code__debug__putchar(workspace, ANVIL__standard__print_buffer_as_string__character);

    // next character
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__standard__print_buffer_as_string__current, ANVIL__srt__constant__ascii_character_byte_size, ANVIL__unused_cell_ID, ANVIL__standard__print_buffer_as_string__current);

    // jump to beginning of loop
    ANVIL__code__jump__static(workspace, ANVIL__sft__always_run, (*std_offsets).offsets[ANVIL__standard__sot__print_buffer_as_string__loop__start]);

    // setup loop end offset
	(*std_offsets).offsets[ANVIL__standard__sot__print_buffer_as_string__loop__end] = ANVIL__get__offset(workspace);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__print_buffer_as_string__preserve__START, ANVIL__standard__print_buffer_as_string__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// register types
typedef enum ANVIL__standard__set_inputs_in_context_buffer {
	// preserve start
	ANVIL__standard__set_inputs_in_context_buffer__preserve__START = ANVIL__srt__start__workspace,

	// variables
	ANVIL__standard__set_inputs_in_context_buffer__context_start = ANVIL__standard__set_inputs_in_context_buffer__preserve__START,
	ANVIL__standard__set_inputs_in_context_buffer__context_end,
	ANVIL__standard__set_inputs_in_context_buffer__input_start,
	ANVIL__standard__set_inputs_in_context_buffer__input_end,
	ANVIL__standard__set_inputs_in_context_buffer__input_start_offset,
    ANVIL__standard__set_inputs_in_context_buffer__input_end_offset,
	ANVIL__standard__set_inputs_in_context_buffer__input_start_address,
	ANVIL__standard__set_inputs_in_context_buffer__input_end_address,

	// preserve end
	ANVIL__standard__set_inputs_in_context_buffer__preserve__END,

	// inputs
	ANVIL__standard__set_inputs_in_context_buffer__input__context_start = ANVIL__srt__start__function_io,
	ANVIL__standard__set_inputs_in_context_buffer__input__context_end,
	ANVIL__standard__set_inputs_in_context_buffer__input__input_start,
	ANVIL__standard__set_inputs_in_context_buffer__input__input_end,
} ANVIL__standard__set_inputs_in_context_buffer;

// call function
void ANVIL__code__call__standard__set_inputs_in_context_buffer(ANVIL__workspace* workspace, ANVIL__standard__offsets* anvil_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__context_start, ANVIL__cell_ID input__context_end, ANVIL__cell_ID input__input_start, ANVIL__cell_ID input__input_end) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__context_start, ANVIL__standard__set_inputs_in_context_buffer__input__context_start);
	ANVIL__code__cell_to_cell(workspace, flag, input__context_end, ANVIL__standard__set_inputs_in_context_buffer__input__context_end);
	ANVIL__code__cell_to_cell(workspace, flag, input__input_start, ANVIL__standard__set_inputs_in_context_buffer__input__input_start);
	ANVIL__code__cell_to_cell(workspace, flag, input__input_end, ANVIL__standard__set_inputs_in_context_buffer__input__input_end);

	// call function
	ANVIL__code__call__static(workspace, flag, (*anvil_offsets).offsets[ANVIL__standard__sot__set_inputs_in_context_buffer__start]);

	return;
}

// build function
void ANVIL__code__standard__set_inputs_in_context_buffer(ANVIL__workspace* workspace, ANVIL__standard__offsets* anvil_offsets) {
	// setup function offset
	(*anvil_offsets).offsets[ANVIL__standard__sot__set_inputs_in_context_buffer__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__set_inputs_in_context_buffer__preserve__START, ANVIL__standard__set_inputs_in_context_buffer__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__set_inputs_in_context_buffer__input__context_start, ANVIL__standard__set_inputs_in_context_buffer__context_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__set_inputs_in_context_buffer__input__context_end, ANVIL__standard__set_inputs_in_context_buffer__context_end);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__set_inputs_in_context_buffer__input__input_start, ANVIL__standard__set_inputs_in_context_buffer__input_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__set_inputs_in_context_buffer__input__input_end, ANVIL__standard__set_inputs_in_context_buffer__input_end);

	// calculate variable addresses
    ANVIL__code__write_cell(workspace, (ANVIL__cell)(ANVIL__srt__input_buffer_start * sizeof(ANVIL__cell)), ANVIL__standard__set_inputs_in_context_buffer__input_start_offset);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)(ANVIL__srt__input_buffer_end * sizeof(ANVIL__cell)), ANVIL__standard__set_inputs_in_context_buffer__input_end_offset);
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__standard__set_inputs_in_context_buffer__context_start, ANVIL__standard__set_inputs_in_context_buffer__input_start_offset, ANVIL__unused_cell_ID, ANVIL__standard__set_inputs_in_context_buffer__input_start_address);
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__standard__set_inputs_in_context_buffer__context_start, ANVIL__standard__set_inputs_in_context_buffer__input_end_offset, ANVIL__unused_cell_ID, ANVIL__standard__set_inputs_in_context_buffer__input_end_address);

    // write data
    ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, ANVIL__standard__set_inputs_in_context_buffer__input_start, ANVIL__srt__constant__cell_bit_count, ANVIL__standard__set_inputs_in_context_buffer__input_start_address);
    ANVIL__code__cell_to_address(workspace, ANVIL__sft__always_run, ANVIL__standard__set_inputs_in_context_buffer__input_end, ANVIL__srt__constant__cell_bit_count, ANVIL__standard__set_inputs_in_context_buffer__input_end_address);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__set_inputs_in_context_buffer__preserve__START, ANVIL__standard__set_inputs_in_context_buffer__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// register types
typedef enum ANVIL__standard__get_outputs_from_context_buffer {
	// preserve start
	ANVIL__standard__get_outputs_from_context_buffer__preserve__START = ANVIL__srt__start__workspace,

	// variables
	ANVIL__standard__get_outputs_from_context_buffer__context_start = ANVIL__standard__get_outputs_from_context_buffer__preserve__START,
	ANVIL__standard__get_outputs_from_context_buffer__context_end,
	ANVIL__standard__get_outputs_from_context_buffer__output_start,
	ANVIL__standard__get_outputs_from_context_buffer__output_end,
	ANVIL__standard__get_outputs_from_context_buffer__output_start_read_offset,
	ANVIL__standard__get_outputs_from_context_buffer__output_end_read_offset,
	ANVIL__standard__get_outputs_from_context_buffer__output_start_read_address,
	ANVIL__standard__get_outputs_from_context_buffer__output_end_read_address,

	// preserve end
	ANVIL__standard__get_outputs_from_context_buffer__preserve__END,

	// inputs
	ANVIL__standard__get_outputs_from_context_buffer__input__context_start = ANVIL__srt__start__function_io,
	ANVIL__standard__get_outputs_from_context_buffer__input__context_end,

	// outputs
	ANVIL__standard__get_outputs_from_context_buffer__output__output_start = ANVIL__srt__start__function_io,
	ANVIL__standard__get_outputs_from_context_buffer__output__output_end,
} ANVIL__standard__get_outputs_from_context_buffer;

// call function
void ANVIL__code__call__standard__get_outputs_from_context_buffer(ANVIL__workspace* workspace, ANVIL__standard__offsets* anvil_offsets, ANVIL__flag_ID flag, ANVIL__cell_ID input__context_start, ANVIL__cell_ID input__context_end, ANVIL__cell_ID output__output_start, ANVIL__cell_ID output__output_end) {
	// pass inputs
	ANVIL__code__cell_to_cell(workspace, flag, input__context_start, ANVIL__standard__get_outputs_from_context_buffer__input__context_start);
	ANVIL__code__cell_to_cell(workspace, flag, input__context_end, ANVIL__standard__get_outputs_from_context_buffer__input__context_end);

	// call function
	ANVIL__code__call__static(workspace, flag, (*anvil_offsets).offsets[ANVIL__standard__sot__get_outputs_from_context_buffer__start]);

	// pass outputs
	ANVIL__code__cell_to_cell(workspace, flag, ANVIL__standard__get_outputs_from_context_buffer__output__output_start, output__output_start);
	ANVIL__code__cell_to_cell(workspace, flag, ANVIL__standard__get_outputs_from_context_buffer__output__output_end, output__output_end);

	return;
}

// build function
void ANVIL__code__standard__get_outputs_from_context_buffer(ANVIL__workspace* workspace, ANVIL__standard__offsets* anvil_offsets) {
	// setup function offset
	(*anvil_offsets).offsets[ANVIL__standard__sot__get_outputs_from_context_buffer__start] = ANVIL__get__offset(workspace);

	// setup function prologue
	ANVIL__code__preserve_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__get_outputs_from_context_buffer__preserve__START, ANVIL__standard__get_outputs_from_context_buffer__preserve__END);

	// get inputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__get_outputs_from_context_buffer__input__context_start, ANVIL__standard__get_outputs_from_context_buffer__context_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__get_outputs_from_context_buffer__input__context_end, ANVIL__standard__get_outputs_from_context_buffer__context_end);

	// setup addresses
    ANVIL__code__write_cell(workspace, (ANVIL__cell)(ANVIL__srt__output_buffer_start * sizeof(ANVIL__cell)), ANVIL__standard__get_outputs_from_context_buffer__output_start_read_offset);
    ANVIL__code__write_cell(workspace, (ANVIL__cell)(ANVIL__srt__output_buffer_end * sizeof(ANVIL__cell)), ANVIL__standard__get_outputs_from_context_buffer__output_end_read_offset);
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__standard__get_outputs_from_context_buffer__context_start, ANVIL__standard__get_outputs_from_context_buffer__output_start_read_offset, ANVIL__unused_cell_ID, ANVIL__standard__get_outputs_from_context_buffer__output_start_read_address);
    ANVIL__code__operate(workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__standard__get_outputs_from_context_buffer__context_start, ANVIL__standard__get_outputs_from_context_buffer__output_end_read_offset, ANVIL__unused_cell_ID, ANVIL__standard__get_outputs_from_context_buffer__output_end_read_address);

    // perform reads
    ANVIL__code__address_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__get_outputs_from_context_buffer__output_start_read_address, ANVIL__srt__constant__cell_bit_count, ANVIL__standard__get_outputs_from_context_buffer__output_start);
    ANVIL__code__address_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__get_outputs_from_context_buffer__output_end_read_address, ANVIL__srt__constant__cell_bit_count, ANVIL__standard__get_outputs_from_context_buffer__output_end);

	// setup outputs
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__get_outputs_from_context_buffer__output_start, ANVIL__standard__get_outputs_from_context_buffer__output__output_start);
	ANVIL__code__cell_to_cell(workspace, ANVIL__sft__always_run, ANVIL__standard__get_outputs_from_context_buffer__output_end, ANVIL__standard__get_outputs_from_context_buffer__output__output_end);

	// setup function epilogue
	ANVIL__code__restore_workspace(workspace, ANVIL__sft__always_run, ANVIL__standard__get_outputs_from_context_buffer__preserve__START, ANVIL__standard__get_outputs_from_context_buffer__preserve__END);

	// return to caller
	ANVIL__code__jump__explicit(workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

	return;
}

// build package
void ANVIL__standard__code__package(ANVIL__workspace* workspace, ANVIL__standard__offsets* standard_offsets) {
    // write functions
    ANVIL__standard__code__print_binary(workspace, standard_offsets);
    ANVIL__standard__code__print_buffer_as_string(workspace, standard_offsets);
	ANVIL__standard__code__calculate_decimal_digit_count(workspace, standard_offsets);
	ANVIL__standard__code__cell_to_unsigned_integer_string(workspace, standard_offsets);
    ANVIL__code__standard__set_inputs_in_context_buffer(workspace, standard_offsets);
    ANVIL__code__standard__get_outputs_from_context_buffer(workspace, standard_offsets);

    return;
}

#endif
