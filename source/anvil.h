#ifndef DRAGON__anvil
#define DRAGON__anvil

/* Include */
// C
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

/* Define */
// types
typedef uint8_t ANVIL__u8;
typedef uint16_t ANVIL__u16;
typedef uint32_t ANVIL__u32;
typedef uint64_t ANVIL__u64;
typedef int8_t ANVIL__s8;
typedef int16_t ANVIL__s16;
typedef int32_t ANVIL__s32;
typedef int64_t ANVIL__s64;

// aliases
typedef ANVIL__u8 u8;
typedef ANVIL__u16 u16;
typedef ANVIL__u32 u32;
typedef ANVIL__u64 u64;
typedef ANVIL__s8 s8;
typedef ANVIL__s16 s16;
typedef ANVIL__s32 s32;
typedef ANVIL__s64 s64;
// (ANVIL__current defined later as it requires buffer type)

// pointers
typedef void* ANVIL__address;
#define ANVIL__define__null_address 0
#define ANVIL__define__null_flag 0

// general
typedef ANVIL__u8 ANVIL__character;
typedef ANVIL__u64 ANVIL__file_index;
typedef ANVIL__u64 ANVIL__line_number;
typedef ANVIL__u64 ANVIL__character_index;
typedef ANVIL__u64 ANVIL__tab_count;
typedef ANVIL__u64 ANVIL__length;
typedef ANVIL__u64 ANVIL__bit_count;
typedef ANVIL__u64 ANVIL__byte_count;
#define ANVIL__define__bits_in_byte 8

// cells
typedef ANVIL__u64 ANVIL__cell_count;
typedef ANVIL__cell_count ANVIL__cell_index;
typedef ANVIL__u64 ANVIL__cell_integer_value;
#define ANVIL__unused_cell_ID 0

// instructions
typedef ANVIL__u64 ANVIL__instruction_count;
#define ANVIL__define__run_forever (ANVIL__u64)-1
#define ANVIL__define__input_string_max_length 256 // is part of fgets instruction and nothing else
typedef ANVIL__address ANVIL__cell;
typedef ANVIL__u8 ANVIL__instruction_ID;
typedef ANVIL__u8 ANVIL__flag_ID;
typedef ANVIL__u8 ANVIL__operation_ID;
typedef ANVIL__u16 ANVIL__cell_ID;

// invalids
#define ANVIL__define__null_offset_ID -1
#define ANVIL__define__null_call_ID -1
#define ANVIL__define__invalid_abstraction_call_ID -1

// lexing types
typedef ANVIL__u8 ANVIL__lexling_type;
typedef ANVIL__address ANVIL__lexling_address;
typedef ANVIL__lexling_address ANVIL__lexling_start;
typedef ANVIL__lexling_address ANVIL__lexling_end;
typedef ANVIL__u64 ANVIL__lexling_index;
typedef ANVIL__u64 ANVIL__lexling_depth; // used for comments and strings

// other compiler types
typedef ANVIL__u64 ANVIL__argument_index;
typedef ANVIL__u64 ANVIL__io_count;
typedef ANVIL__io_count ANVIL__input_count;
typedef ANVIL__io_count ANVIL__output_count;
typedef ANVIL__u64 ANVIL__accountling_index;
typedef ANVIL__accountling_index ANVIL__variable_index;
typedef ANVIL__accountling_index ANVIL__call_index;
typedef ANVIL__accountling_index ANVIL__offset_index;
typedef ANVIL__accountling_index ANVIL__flag_index;
typedef ANVIL__accountling_index ANVIL__sttatement_index;
typedef ANVIL__accountling_index ANVIL__header_index;
typedef ANVIL__accountling_index ANVIL__string_index;
typedef ANVIL__u64 ANVIL__abstraction_index;

// strings
char* ANVIL__global__predefined_cell_name_strings[] = {
    "forge.error_code",
    "forge.constant.character_byte_size",
    "forge.constant.character_bit_size",
    "forge.constant.bits_in_byte",
    "forge.constant.cell_byte_size",
    "forge.constant.cell_bit_size",
    "forge.constant.true",
    "forge.constant.false",
    "forge.constant.0",
    "forge.constant.1",
    "forge.constant.2",
    "forge.constant.4",
    "forge.constant.8",
    "forge.constant.16",
    "forge.constant.24",
    "forge.constant.32",
    "forge.constant.40",
    "forge.constant.48",
    "forge.constant.56",
    "forge.constant.64",
    "forge.constant.program_input.start",
    "forge.constant.program_input.end",
    "forge.constant.program_output.start",
    "forge.constant.program_output.end",
    "forge.stack.start",
    "forge.stack.current",
    "forge.stack.end",
};
char* ANVIL__global__predefined_flag_name_strings[] = {
    "forge.always",
    "forge.never",
    "forge.temporary",
};
char* ANVIL__global__argument_type_name_strings[] = {
    "invalid",
    "variable",
    "input_variable",
    "output_variable",
    "body_variable",
    "predefined_variable",
    "offset",
    "flag",
    "flag_user_defined",
    "flag_predefined",
    "boolean",
    "binary",
    "integer",
    "hexadecimal",
    "string",
};
char* ANVIL__global__accountling_call_type_name_strings[] = {
    "forge.set",
    "forge.print.integer.signed",
    "forge.print.integer.unsigned",
    "forge.print.character",
    "forge.print.buffer_as_string",
    "forge.print.binary",
    "forge.io.cell_to_address",
    "forge.io.address_to_cell",
    "forge.io.file_to_buffer",
    "forge.io.buffer_to_file",
    "forge.copy",
    "forge.copy.buffer",
    "forge.memory.request",
    "forge.memory.return",
    "forge.buffer.calculate_length",
    "forge.cast.cell_to_unsigned_integer_string",
    "forge.integer.add",
    "forge.integer.subtract",
    "forge.integer.multiply",
    "forge.integer.divide",
    "forge.integer.modulous",
    "forge.integer.within_range",
    "forge.binary.or",
    "forge.binary.invert",
    "forge.binary.and",
    "forge.binary.xor",
    "forge.binary.shift_higher",
    "forge.binary.shift_lower",
    "forge.binary.overwrite",
    "forge.flag.get",
    "forge.flag.set",
    "forge.flag.invert",
    "forge.flag.or",
    "forge.flag.and",
    "forge.flag.xor",
    "forge.jump",
    "forge.open.context",
    "forge.compile",
    "forge.run",
    "forge.reset.error_code",
    "forge.set.program_outputs",
};

// program stage type
typedef enum ANVIL__pst {
    // stages
    ANVIL__pst__invalid, // invalid
    ANVIL__pst__lexing, // lexing files
    ANVIL__pst__parsing, // parsing files
    ANVIL__pst__accounting, // semantics for all files
    ANVIL__pst__generating, // building program
    ANVIL__pst__running, // running program

    // count
    ANVIL__pst__COUNT,
} ANVIL__pst;

// boolean type
typedef enum ANVIL__bt {
	ANVIL__bt__false = 0,
	ANVIL__bt__true = 1,
	ANVIL__bt__COUNT = 2,
} ANVIL__bt;

/* Allocation */
// return memory to OS
void ANVIL__close__allocation(ANVIL__address start, ANVIL__address end) {
	// do useless operation to quiet compiler
	end = end;

	// return memory
	free(start);

	return;
}

// ask OS for memory
ANVIL__address ANVIL__open__allocation(ANVIL__length length) {
	// return allocation
	return (ANVIL__address)malloc(length);
}

/* Buffer */
// buffer
typedef struct ANVIL__buffer {
	ANVIL__address start;
	ANVIL__address end;
} ANVIL__buffer;

// close buffer
void ANVIL__close__buffer(ANVIL__buffer buffer) {
	// close allocation
	ANVIL__close__allocation(buffer.start, buffer.end);

	return;
}

// create buffer to have specific exact contents
ANVIL__buffer ANVIL__create__buffer(ANVIL__address start, ANVIL__address end) {
	ANVIL__buffer output;

	// setup output
	output.start = start;
	output.end = end;

	return output;
}

// aliase defines
typedef ANVIL__buffer ANVIL__current;

// create buffer in it's standard null setup
ANVIL__buffer ANVIL__create_null__buffer() {
	// return standard null buffer
	return ANVIL__create__buffer(ANVIL__define__null_address, ANVIL__define__null_address);
}

// calculate buffer length
ANVIL__length ANVIL__calculate__buffer_length(ANVIL__buffer buffer) {
    return (ANVIL__length)((u8*)buffer.end - (u8*)buffer.start) + 1;
}

// check to see if the pointers in the buffers are equal
ANVIL__bt ANVIL__calculate__buffer_addresses_equal(ANVIL__buffer a, ANVIL__buffer b) {
    return (ANVIL__bt)((a.start == b.start) && (a.end == b.end));
}

// check to see if the contents in the buffers are identical
ANVIL__bt ANVIL__calculate__buffer_contents_equal(ANVIL__buffer a, ANVIL__buffer b) {
    // check if names are same length
    if (ANVIL__calculate__buffer_length(a) != ANVIL__calculate__buffer_length(b)) {
        // not same length so not identical
        return ANVIL__bt__false;
    }

    // get pointers
    ANVIL__address a_current = a.start;
    ANVIL__address b_current = b.start;

    // check each character
    while (a_current <= a.end) {
        // check character
        if (*(ANVIL__character*)a_current != *(ANVIL__character*)b_current) {
            // character not identical, string not identical
            return ANVIL__bt__false;
        }

        // next characters
        a_current += sizeof(ANVIL__character);
        b_current += sizeof(ANVIL__character);
    }

    // no issues found, buffers are identical
    return ANVIL__bt__true;
}

ANVIL__bt ANVIL__calculate__buffer_starts_with_buffer(ANVIL__buffer searching_in, ANVIL__buffer searching_for) {
    // ensure search is larger than the buffer
    if (ANVIL__calculate__buffer_length(searching_in) < ANVIL__calculate__buffer_length(searching_for)) {
        // not bigger or equal, thus impossible to contain
        return ANVIL__bt__false;
    }

    return ANVIL__calculate__buffer_contents_equal(searching_for, ANVIL__create__buffer(searching_in.start, searching_in.start + ANVIL__calculate__buffer_length(searching_for) - 1));
}

// calculate buffer contains range
ANVIL__bt ANVIL__calculate__buffer_range_in_buffer_range_inclusive(ANVIL__buffer outside, ANVIL__buffer inside) {
    // return calculation
    return (outside.start <= inside.start) && (outside.end >= inside.end);
}

// check to see if it is an empty buffer
ANVIL__bt ANVIL__check__empty_buffer(ANVIL__buffer buffer) {
    // return calculation
    return (ANVIL__bt)(buffer.start == ANVIL__define__null_address);
}

// open buffer
ANVIL__buffer ANVIL__open__buffer(ANVIL__length length) {
	ANVIL__buffer output;

	// attempt allocation
	output.start = ANVIL__open__allocation(length);

	// set end of buffer according to allocation success
	if (output.start != ANVIL__define__null_address) {
		output.end = (ANVIL__address)((((ANVIL__u64)output.start) + length) - 1);
	} else {
		output.end = ANVIL__define__null_address;
	}

	return output;
}

// check if buffers are the same size
ANVIL__bt ANVIL__calculate__are_buffers_same_size(ANVIL__buffer a, ANVIL__buffer b) {
    // return calculation
    return (ANVIL__bt)((a.end - a.start) == (b.end - b.start));
}

// copy buffer
void ANVIL__copy__buffer(ANVIL__buffer source, ANVIL__buffer destination, ANVIL__bt* error) {
    // check for invalid buffer
    if (ANVIL__calculate__are_buffers_same_size(source, destination) == ANVIL__bt__true) {
        // copy buffer
        for (ANVIL__length byte_index = 0; byte_index < (ANVIL__length)(destination.end - destination.start) + 1; byte_index++) {
            // copy byte
            ((ANVIL__u8*)destination.start)[byte_index] = ((ANVIL__u8*)source.start)[byte_index];
        }
    } else {
        // set error
        *error = ANVIL__bt__true;
    }

    return;
}

// create or open a buffer from a string literal (can either duplicate buffer or simply reference original) (can opt out of null termination)
ANVIL__buffer ANVIL__open__buffer_from_string(u8* string, ANVIL__bt duplicate, ANVIL__bt null_terminate) {
    ANVIL__buffer output;
    ANVIL__length length;

    // setup length
    length = 0;

    // get buffer length
    while (string[length] != 0) {
        length++;
    }

    // optionally append null termination
    if (null_terminate == ANVIL__bt__true) {
        length++;
    }

    // reference or duplicate
    if (duplicate == ANVIL__bt__true) {
        // attempt allocation
        output = ANVIL__open__buffer(length);

        // check for null allocation
        if (output.start == ANVIL__define__null_address) {
            // return empty buffer
            return output;
        }

        // copy buffer byte by byte
        for (ANVIL__length byte_index = 0; byte_index < length; byte_index++) {
            // copy byte
            ((ANVIL__u8*)output.start)[byte_index] = string[byte_index];
        }
    } else {
        // setup duplicate output
        output.start = string;
        output.end = string + length - 1;
    }

    return output;
}

// read buffer
ANVIL__u64 ANVIL__read__buffer(ANVIL__address source, ANVIL__length byte_amount) {
	ANVIL__u64 output;

	// setup output
	output = 0;

	// read buffer
	for (ANVIL__u64 byte_index = 0; byte_index < byte_amount; byte_index += 1) {
		// get byte
		((ANVIL__u8*)&output)[byte_index] = ((ANVIL__u8*)source)[byte_index];
	}

	// return output
	return output;
}

// write buffer
void ANVIL__write__buffer(ANVIL__u64 source, ANVIL__length byte_amount, ANVIL__address destination) {
	// write data to buffer
	for (ANVIL__length byte_index = 0; byte_index < byte_amount; byte_index += 1) {
		// write byte
		((ANVIL__u8*)destination)[byte_index] = ((ANVIL__u8*)&source)[byte_index];
	}
	
	return;
}

// append null termination
ANVIL__buffer ANVIL__add__null_termination_to_file_path(ANVIL__buffer file_path, ANVIL__bt* error_occured) {
    ANVIL__buffer output;

    // allocate buffer
    output = ANVIL__open__buffer(ANVIL__calculate__buffer_length(file_path) + 1);

    // copy buffer
    ANVIL__copy__buffer(file_path, ANVIL__create__buffer(output.start, output.end - 1), error_occured);

    // append null termination
    *((ANVIL__character*)output.end) = 0;

    return output;
}

// create buffer from file
ANVIL__buffer ANVIL__move__file_to_buffer(ANVIL__buffer file_path) {
	ANVIL__buffer output;
	FILE* file_handle;
	ANVIL__u64 file_size;
    ANVIL__buffer null_terminated_file_path = file_path;
    ANVIL__bt error_occured = ANVIL__bt__false;

    // check for null termination
    if (*(ANVIL__character*)file_path.end != 0) {
        // setup null termination
        null_terminated_file_path = ANVIL__add__null_termination_to_file_path(file_path, &error_occured);
    }

	// open file
	file_handle = fopen((const char*)null_terminated_file_path.start, "rb");

	// check if the file opened
	if (file_handle == 0) {
		// if not, return empty buffer
		return ANVIL__create_null__buffer();
	}

	// get file size
	fseek(file_handle, 0, SEEK_END);
	file_size = ftell(file_handle);
	fseek(file_handle, 0, SEEK_SET);

	// allocate buffer
	output = ANVIL__open__buffer(file_size);

	// check if buffer allocated
	if (output.start == ANVIL__define__null_address) {
		// close file handle
		fclose(file_handle);

		// return empty buffer
		return output;
	}

	// read file into buffer
	fread(output.start, file_size, 1, file_handle);

	// close file handle
	fclose(file_handle);

    // close null file path if necessary
    if (*(ANVIL__character*)file_path.end != 0) {
        ANVIL__close__buffer(null_terminated_file_path);
    }

	// return buffer
	return output;
}

// create file from buffer
void ANVIL__move__buffer_to_file(ANVIL__bt* error, ANVIL__buffer file_path, ANVIL__buffer data) {
	FILE* file_handle;
    ANVIL__buffer null_terminated_file_path = file_path;

    // check for null termination
    if (*(ANVIL__character*)file_path.end != 0) {
        // setup null termination
        null_terminated_file_path = ANVIL__add__null_termination_to_file_path(file_path, error);
    }

    // setup error to no error to start
    *error = ANVIL__bt__false;

	// open file
	file_handle = fopen((const char*)null_terminated_file_path.start, "w+b");

	// check if the file opened
	if (file_handle == 0) {
		// if not, return error
        *error = ANVIL__bt__true;

		return;
	}

	// write buffer to file
	fwrite(data.start, ANVIL__calculate__buffer_length(data), 1, file_handle);

	// close file handle
	fclose(file_handle);

    // close null file path if necessary
    if (*(ANVIL__character*)file_path.end != 0) {
        ANVIL__close__buffer(null_terminated_file_path);
    }

	// return
	return;
}

// print buffer
void ANVIL__print__buffer(ANVIL__buffer buffer) {
    // print character by character
    for (ANVIL__address character = buffer.start; character <= buffer.end; character += sizeof(ANVIL__character)) {
        // print character
        putchar(*(ANVIL__character*)character);
    }

    return;
}

/* List */
// list types
typedef ANVIL__u64 ANVIL__list_filled_index;
typedef ANVIL__u64 ANVIL__list_increase;

// list object
typedef struct ANVIL__list {
    ANVIL__buffer buffer;
    ANVIL__list_filled_index filled_index;
    ANVIL__list_increase increase;
} ANVIL__list;

// create a list
ANVIL__list ANVIL__create__list(ANVIL__buffer buffer, ANVIL__list_filled_index filled_index, ANVIL__list_increase increase) {
    ANVIL__list output;

    // setup output
    output.buffer = buffer;
    output.filled_index = filled_index;
    output.increase = increase;

    return output;
}

// create a null list
ANVIL__list ANVIL__create_null__list() {
    // return empty list
    return ANVIL__create__list(ANVIL__create_null__buffer(), 0, 0);
}

// open a list
ANVIL__list ANVIL__open__list(ANVIL__list_increase increase, ANVIL__bt* error_occured) {
    ANVIL__list output;
    ANVIL__buffer allocation;

    // allocate list
    allocation = ANVIL__open__buffer(increase);

    // check list validity
    if (allocation.start == ANVIL__define__null_address) {
        // set error
        *error_occured = ANVIL__bt__true;

        // return empty
        return ANVIL__create_null__list();
    // list is valid
    } else {
        // set error to false
        *error_occured = ANVIL__bt__false;
    }

    // setup output
    output.buffer = allocation;
    output.filled_index = 0;
    output.increase = increase;

    return output;
}

// destroy a list
void ANVIL__close__list(ANVIL__list list) {
    // free buffer
    ANVIL__close__buffer(ANVIL__create__buffer(list.buffer.start, list.buffer.end));

    return;
}

// expand a list
void ANVIL__list__expand(ANVIL__list* list, ANVIL__bt* error_occured) {
    ANVIL__list_filled_index new_size;
    ANVIL__buffer new_allocation;

    // calculate new buffer size
    new_size = ((ANVIL__u64)(*list).buffer.end - (ANVIL__u64)(*list).buffer.start + 1) + (*list).increase;

    // request new memory
    new_allocation = ANVIL__open__buffer(new_size);

    // check for failure
    if (new_allocation.start == ANVIL__define__null_address) {
        // set error
        *error_occured = ANVIL__bt__true;

        // return unmodified list
        return;
    }

    // copy old data to new list
    for (ANVIL__list_filled_index i = 0; i < (*list).filled_index; i++) {
        // copy one byte
        ((ANVIL__u8*)new_allocation.start)[i] = ((ANVIL__u8*)(*list).buffer.start)[i];
    }

    // free old buffer
    ANVIL__close__buffer((*list).buffer);

    // setup new list allocation
    (*list).buffer = new_allocation;

    return;
}

// request space for the list
void ANVIL__list__request__space(ANVIL__list* list, ANVIL__byte_count byte_count, ANVIL__bt* error_occured) {
    // expand the list until there is enough space
    while (((ANVIL__u64)(*list).buffer.end - (ANVIL__u64)(*list).buffer.start + 1) < ((*list).filled_index + byte_count)) {
        // expand the list
        ANVIL__list__expand(list, error_occured);

        // check for error
        if (*error_occured == ANVIL__bt__true) {
            // return last modified list
            return;
        }
    }

    return;
}

// add index to address
ANVIL__address ANVIL__calculate__address_from_buffer_index(ANVIL__address start, ANVIL__list_filled_index index) {
    return start + index;
}

// calculate the tip of the list
ANVIL__address ANVIL__calculate__list_current_address(ANVIL__list* list) {
    return ANVIL__calculate__address_from_buffer_index((*list).buffer.start, (*list).filled_index);
}

// calculate the current buffer
ANVIL__buffer ANVIL__calculate__list_current_buffer(ANVIL__list* list) {
    return ANVIL__create__buffer(((*list).buffer.start), ANVIL__calculate__list_current_address(list) - 1);
}

// add a buffer to a list
void ANVIL__list__append__buffer(ANVIL__list* list, ANVIL__buffer buffer, ANVIL__bt* memory_error_occured) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__buffer), memory_error_occured);

    // append data
    (*(ANVIL__buffer*)ANVIL__calculate__list_current_address(list)) = buffer;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__buffer);

    return;
}

// add a buffer's data to a list
void ANVIL__list__append__buffer_data(ANVIL__list* list, ANVIL__buffer buffer, ANVIL__bt* memory_error_occured) {
    ANVIL__length buffer_length;
    ANVIL__address buffer_old_end;

    // calculate buffer length
    buffer_length = ANVIL__calculate__buffer_length(buffer);

    // calculate old buffer end
    buffer_old_end = (*list).buffer.start + (*list).filled_index - 1;

    // request space
    ANVIL__list__request__space(list, buffer_length, memory_error_occured);

    // append data
    ANVIL__copy__buffer(buffer, ANVIL__create__buffer(buffer_old_end + 1, buffer_old_end + buffer_length), memory_error_occured);

    // increase fill
    (*list).filled_index += buffer_length;

    return;
}

// add a list to a list
void ANVIL__list__append__list(ANVIL__list* list, ANVIL__list data, ANVIL__bt* memory_error_occured) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__list), memory_error_occured);

    // append data
    (*(ANVIL__list*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__list);

    return;
}

// remove a slice of data from a list
void ANVIL__list__erase__space(ANVIL__list* list, ANVIL__list_filled_index range_start_index, ANVIL__list_filled_index range_end_index) {
    ANVIL__buffer old_right;
    ANVIL__buffer new_right;
    ANVIL__bt error;

    // get new right buffer
    old_right = ANVIL__create__buffer(ANVIL__calculate__address_from_buffer_index((*list).buffer.start, range_end_index), ANVIL__calculate__list_current_address(list));
    new_right = ANVIL__create__buffer(old_right.start - (range_end_index - range_start_index + 1), old_right.end - (range_end_index - range_start_index + 1));

    // move data from left to right filling in the gap
    ANVIL__copy__buffer(old_right, new_right, &error);

    // should not happen but handled anyways
    if (error == ANVIL__bt__true) {
        // tell user
        printf("Internal Error: Buffer could not erase data.\n");
    // buffer was clipped, change filled index
    } else {
        // change filled index
        (*list).filled_index -= range_end_index - range_start_index + 1;
    }

    return;
}

// check if two lists are filled up to the same amount
ANVIL__bt ANVIL__calculate__lists_have_same_fill_size(ANVIL__list* a, ANVIL__list* b) {
    return (ANVIL__bt)((*a).filled_index == (*b).filled_index);
}

// take a list and make a standalone buffer
ANVIL__buffer ANVIL__list__open_buffer_from_list(ANVIL__list* list, ANVIL__bt* memory_error_occured) {
    ANVIL__buffer output;

    // allocate output
    output = ANVIL__open__buffer((*list).filled_index);

    // if buffer did not open
    if (ANVIL__check__empty_buffer(output) == ANVIL__bt__true) {
        // set error
        *memory_error_occured = ANVIL__bt__true;
    // if buffer opened
    } else {
        // copy data from list to buffer
        ANVIL__copy__buffer(ANVIL__calculate__list_current_buffer(list), output, memory_error_occured);
    }

    return output;
}

// check if a list is uninitialized
ANVIL__bt ANVIL__check__empty_list(ANVIL__list list) {
    return ANVIL__check__empty_buffer(list.buffer);
}

/* Translation */
// calculate the amount of digits in a decimal number
ANVIL__length ANVIL__calculate__decimals_digit_count_in_number(ANVIL__u64 number) {
    ANVIL__length output = 0;

    // check for zero
    if (number == 0) {
        return 1;
    }

    // while there are digits left
    while (number > 0) {
        // divide by ten
        number /= 10;

        // increment digit count
        output++;
    }

    return output;
}

// create buffer string from number
ANVIL__buffer ANVIL__translate__integer_value_to_string(ANVIL__u64 number) {
    ANVIL__buffer output;
    ANVIL__length digit_count;

    // count digits
    digit_count = ANVIL__calculate__decimals_digit_count_in_number(number);
    
    // allocate output
    output = ANVIL__open__buffer(sizeof(ANVIL__character) * digit_count);

    // if allocation succeded
    if (ANVIL__check__empty_buffer(output) == ANVIL__bt__false) {
        // calculate characters
        for (ANVIL__u64 i = digit_count; i > 0; i--) {
            // create character
            ((ANVIL__character*)output.start)[i - 1] = '0' + (number % 10);

            // next character
            number /= 10;
        }
    }

    return output;
}

/* Character Locations */
// parsing character location
typedef struct ANVIL__character_location {
    ANVIL__file_index file_index;
    ANVIL__line_number line_number;
    ANVIL__character_index character_index;
} ANVIL__character_location;

// create custom character marker location
ANVIL__character_location ANVIL__create__character_location(ANVIL__file_index file_index, ANVIL__line_number line_number, ANVIL__character_index character_index) {
    ANVIL__character_location output;

    output.file_index = file_index;
    output.line_number = line_number;
    output.character_index = character_index;

    return output;
}

// create null character location
ANVIL__character_location ANVIL__create_null__character_location() {
    return ANVIL__create__character_location(-1, -1, -1);
}

/* Errors */
// error information
typedef struct ANVIL__error {
    ANVIL__bt occured;
    ANVIL__buffer message;
    ANVIL__character_location location;
    ANVIL__bt memory_error_occured;
} ANVIL__error;

// create custom error
ANVIL__error ANVIL__create__error(ANVIL__bt occured, ANVIL__buffer message, ANVIL__character_location location, ANVIL__bt memory_error_occured) {
    ANVIL__error output;

    // setup output
    output.occured = occured;
    output.message = message;
    output.location = location;
    output.memory_error_occured = memory_error_occured;

    return output;
}

// create null error
ANVIL__error ANVIL__create_null__error() {
    // return empty
    return ANVIL__create__error(ANVIL__bt__false, ANVIL__create_null__buffer(), ANVIL__create_null__character_location(), ANVIL__bt__false);
}

// open a specific error
ANVIL__error ANVIL__open__error(const char* message, ANVIL__character_location location) {
    return ANVIL__create__error(ANVIL__bt__true, ANVIL__open__buffer_from_string((u8*)message, ANVIL__bt__true, ANVIL__bt__false), location, ANVIL__bt__false);
}

// open a generic memory allocation error
ANVIL__error ANVIL__open__internal_memory_error() {
    return ANVIL__create__error(ANVIL__bt__true, ANVIL__open__buffer_from_string((u8*)"Internal memory error.", ANVIL__bt__true, ANVIL__bt__false), ANVIL__create_null__character_location(), ANVIL__bt__true);
}

// create an error report in json
ANVIL__buffer ANVIL__serialize__error_json(ANVIL__error error, ANVIL__bt* error_occured) {
    ANVIL__buffer output;
    ANVIL__list json;
    ANVIL__buffer temp_buffer;

    // initialize json string builder
    json = ANVIL__open__list(sizeof(ANVIL__character) * 2048, error_occured);

    // generate json
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"{\n\t\"message\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    ANVIL__list__append__buffer_data(&json, error.message, error_occured);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\",\n\t\"file_index\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    temp_buffer = ANVIL__translate__integer_value_to_string(error.location.file_index);
    ANVIL__list__append__buffer_data(&json, temp_buffer, error_occured);
    ANVIL__close__buffer(temp_buffer);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\",\n\t\"line_number\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    temp_buffer = ANVIL__translate__integer_value_to_string(error.location.line_number);
    ANVIL__list__append__buffer_data(&json, temp_buffer, error_occured);
    ANVIL__close__buffer(temp_buffer);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\",\n\t\"character_index\": \"", ANVIL__bt__false, ANVIL__bt__false), error_occured);
    temp_buffer = ANVIL__translate__integer_value_to_string(error.location.character_index);
    ANVIL__list__append__buffer_data(&json, temp_buffer, error_occured);
    ANVIL__close__buffer(temp_buffer);
    ANVIL__list__append__buffer_data(&json, ANVIL__open__buffer_from_string((u8*)"\"\n}\n", ANVIL__bt__false, ANVIL__bt__false), error_occured);

    // create buffer from list
    output = ANVIL__list__open_buffer_from_list(&json, error_occured);

    // clean up list
    ANVIL__close__list(json);

    return output;
}

// check to see if an error occured
ANVIL__bt ANVIL__check__error_occured(ANVIL__error* error) {
    // check for memory error
    if ((*error).memory_error_occured == ANVIL__bt__true) {
        // set error
        *error = ANVIL__open__internal_memory_error();
    }

    return (*error).occured;
}

// close an error
void ANVIL__close__error(ANVIL__error error) {
    // clean up buffers
    ANVIL__close__buffer(error.message);
    
    return;
}

/* List Functions With Errors */
// open a list but the error is a compiler error
ANVIL__list ANVIL__open__list_with_error(ANVIL__list_increase list_increase, ANVIL__error* error) {
    // open with error
    return ANVIL__open__list(list_increase, &((*error).memory_error_occured));
}

// append a buffer but the error is compiler
void ANVIL__append__buffer_with_error(ANVIL__list* list, ANVIL__buffer buffer, ANVIL__error* error) {
    ANVIL__list__append__buffer(list, buffer, &((*error).memory_error_occured));

    return;
}

/* Essentials */
ANVIL__u64 ANVIL__calculate__exponent(ANVIL__u64 base, ANVIL__u64 exponent) {
    ANVIL__u64 output = 1;

    // if zero
    if (exponent == 0) {
        return output;
    }

    // calculate number
    while (exponent > 0) {
        // multiply
        output *= base;

        // next exponent
        exponent--;
    }

    return output;
}

// print tabs
void ANVIL__print__tabs(ANVIL__tab_count tab_count) {
    // print tabs
    while (tab_count > 0) {
        // print tab
        printf("\t");

        // next tab
        tab_count--;
    }

    return;
}

/* Machine Specifications */
// scraplet types
typedef enum ANVIL__st {
    // scraplet types
    ANVIL__st__instruction_ID,
    ANVIL__st__flag_ID,
    ANVIL__st__operation_ID,
    ANVIL__st__cell_ID,
    ANVIL__st__cell,

    // count
    ANVIL__st__COUNT,
} ANVIL__st;

// should the next instruction process
// next instruction type
typedef enum ANVIL__nit {
    // next instruction types
    ANVIL__nit__return_context,
    ANVIL__nit__next_instruction,

    // count
    ANVIL__nit__COUNT,
} ANVIL__nit;

/* Current */
// check if a current buffer is still valid
ANVIL__bt ANVIL__check__current_within_range(ANVIL__buffer current) {
    return (current.start <= current.end);
}

// calculate a current buffer from a list // NOTE: buffer cannot be null or calculation fails!
ANVIL__current ANVIL__calculate__current_from_list_filled_index(ANVIL__list* list) {
    return ANVIL__create__buffer((*list).buffer.start, (*list).buffer.start + (*list).filled_index - 1);
}

// check for a character at a current
ANVIL__bt ANVIL__check__character_range_at_current(ANVIL__buffer current, ANVIL__character range_start, ANVIL__character range_end) {
    return ((*(ANVIL__character*)current.start) >= range_start) && ((*(ANVIL__character*)current.start) <= range_end);
}

// calculate the amounnt of items in one list (assumes all items are same size!)
ANVIL__list_filled_index ANVIL__calculate__list_content_count(ANVIL__list list, size_t item_size) {
    return list.filled_index / item_size;
}

/* Allocations */
// allocations
typedef struct ANVIL__allocations {
    ANVIL__list buffers;
} ANVIL__allocations;

// find an allocation
ANVIL__list_filled_index ANVIL__find__allocation(ANVIL__allocations* allocations, ANVIL__buffer allocation, ANVIL__bt* found) {
    ANVIL__list_filled_index output = 0;
    ANVIL__buffer current;
    
    // setup current
    current = ANVIL__calculate__list_current_buffer(&((*allocations).buffers));

    // check for valid allocation
    while (current.start <= current.end) {
        // check one allocation
        if (ANVIL__calculate__buffer_addresses_equal(*(ANVIL__buffer*)current.start, allocation)) {
            // allocation is valid
            *found = ANVIL__bt__true;

            return output;
        }

        // next allocation
        current.start += sizeof(ANVIL__buffer);
        output += sizeof(ANVIL__buffer);
    }

    // not valid
    *found = ANVIL__bt__false;

    return output;
}

// add a buffer
void ANVIL__remember__allocation(ANVIL__allocations* allocations, ANVIL__buffer allocation, ANVIL__bt* list_error_occured) {
    // append allocation
    ANVIL__list__append__buffer(&((*allocations).buffers), allocation, list_error_occured);

    return;
}

// remove a buffer
void ANVIL__forget__allocation(ANVIL__allocations* allocations, ANVIL__buffer allocation, ANVIL__bt* buffer_did_not_exist) {
    ANVIL__bt found = ANVIL__bt__false;

    // find allocation index
    ANVIL__list_filled_index start_index = ANVIL__find__allocation(allocations, allocation, &found);

    // if found
    if (found == ANVIL__bt__true) {
        // erase from list
        ANVIL__list__erase__space(&((*allocations).buffers), start_index, start_index + sizeof(ANVIL__buffer) - 1);
    // not found, so not removed
    } else {
        // set error
        *buffer_did_not_exist = ANVIL__bt__true;
    }

    return;
}

// check to see if an address is valid
ANVIL__bt ANVIL__check__valid_address_range_in_allocations(ANVIL__allocations* allocations, ANVIL__buffer range) {
    ANVIL__buffer current;
    
    // setup current
    current = ANVIL__calculate__list_current_buffer(&allocations->buffers); //ANVIL__create__buffer((*allocations).buffers.buffer.start, ANVIL__calculate__list_current_address(&((*allocations).buffers)) - 1);

    // check for valid allocation range
    while (current.start <= current.end) {
        // check one allocation
        if (ANVIL__calculate__buffer_range_in_buffer_range_inclusive(*(ANVIL__buffer*)current.start, range)) {
            // allocation is valid
            return ANVIL__bt__true;
        }
        
        // next allocation
        current.start += sizeof(ANVIL__buffer);
    }

    return ANVIL__bt__false;
}

// open allocations
ANVIL__allocations ANVIL__open__allocations(ANVIL__bt* memory_error_occured) {
    ANVIL__allocations output;

    // setup output
    output.buffers = ANVIL__open__list(sizeof(ANVIL__buffer) * 256, memory_error_occured);

    return output;
}

// close allocations (does NOT clear actual allocations)
void ANVIL__close__allocations(ANVIL__allocations* allocations) {
    // clean up
    ANVIL__close__list((*allocations).buffers);

    return;
}

// print allocations
void ANVIL__print__allocations(ANVIL__allocations allocations) {
    // print header
    printf("Allocations:\n");

    // setup current
    ANVIL__current current_allocation = ANVIL__calculate__current_from_list_filled_index(&allocations.buffers);

    // for each allocation
    while (ANVIL__check__current_within_range(current_allocation)) {
        // get allocation
        ANVIL__buffer allocation = *(ANVIL__buffer*)current_allocation.start;

        // print allocation
        printf("\t[ %lu %lu ] (%li)\n", (ANVIL__cell_integer_value)allocation.start, (ANVIL__cell_integer_value)allocation.end, (ANVIL__cell_integer_value)allocation.end - (ANVIL__cell_integer_value)allocation.start + 1);

        // next allocation
        current_allocation.start += sizeof(ANVIL__buffer);
    }

    return;
}

/* Compiler Predefinitions */
void ANVIL__compile__files(ANVIL__buffer user_codes, ANVIL__bt print_debug, ANVIL__buffer* final_program, ANVIL__error* error);

/* Alloy Specification */
// context cell organization defines
// cell type
typedef enum ANVIL__rt {
    // start of defined values
    ANVIL__rt__START = 0,

    // basic cells
    ANVIL__rt__program_start_address = ANVIL__rt__START,
    ANVIL__rt__program_current_address,
    ANVIL__rt__program_end_address,
    ANVIL__rt__error_code,
    ANVIL__rt__flags_0,
    ANVIL__rt__flags_1,
    ANVIL__rt__flags_2,
    ANVIL__rt__flags_3,

    // end of defined cells
    ANVIL__rt__END,

    // count
    ANVIL__rt__RESERVED_COUNT = ANVIL__rt__END - ANVIL__rt__START,

    // statistics
    ANVIL__rt__FIRST_ID = ANVIL__rt__START,
    ANVIL__rt__LAST_ID = 65535, // NUMBER DOES NOT CHANGE!
    ANVIL__rt__TOTAL_COUNT = ANVIL__rt__LAST_ID + 1,
} ANVIL__rt;

// context
typedef struct ANVIL__context {
    ANVIL__cell cells[ANVIL__rt__TOTAL_COUNT];
} ANVIL__context;

// instruction type and instruction ID defines (type number is also ID)
typedef enum ANVIL__it {
    // start of defined instructions
    ANVIL__it__START = 0,

    // defined instructions
    ANVIL__it__stop = ANVIL__it__START, // returns context to caller
    ANVIL__it__write_cell, // overwrites entire cell with hard coded value
    ANVIL__it__operate, // inter-cell operations
    ANVIL__it__request_memory, // allocate request
    ANVIL__it__return_memory, // deallocate request
    ANVIL__it__address_to_cell, // read memory into cell
    ANVIL__it__cell_to_address, // write cell to memory
    ANVIL__it__file_to_buffer, // get file into buffer
    ANVIL__it__buffer_to_file, // write buffer to disk
    ANVIL__it__buffer_to_buffer, // copy data from one buffer to another of the same size
    ANVIL__it__compile, // compile one or more source files to an anvil program
    ANVIL__it__run, // run context (both until finished and run one instruction)

    // extra defined instructions
    ANVIL__it__debug__putchar, // print one character to stdout
    ANVIL__it__debug__print_cell_as_decimal, // print an entire cell as a decimal number
    ANVIL__it__debug__fgets, // read one line from stdin
    ANVIL__it__debug__mark_data_section, // mark a section of data (NOP)
    ANVIL__it__debug__mark_code_section, // mark a section of code (NOP)

    // end of defined instruction types
    ANVIL__it__END,

    // count
    ANVIL__it__COUNT = ANVIL__it__END - ANVIL__it__START,
} ANVIL__it;

// instruction length types
typedef enum ANVIL__ilt {
    ANVIL__ilt__stop = sizeof(ANVIL__instruction_ID),
    ANVIL__ilt__write_cell = sizeof(ANVIL__instruction_ID) + sizeof(ANVIL__cell) + sizeof(ANVIL__cell_ID),
    ANVIL__ilt__operate = sizeof(ANVIL__instruction_ID) + sizeof(ANVIL__flag_ID) + sizeof(ANVIL__operation_ID) + (sizeof(ANVIL__cell_ID) * 4),
    ANVIL__ilt__request_memory = sizeof(ANVIL__instruction_ID) + (sizeof(ANVIL__cell_ID) * 3),
    ANVIL__ilt__return_memory = sizeof(ANVIL__instruction_ID) + (sizeof(ANVIL__cell_ID) * 2),
    ANVIL__ilt__address_to_cell = sizeof(ANVIL__instruction_ID) + sizeof(ANVIL__flag_ID) + (sizeof(ANVIL__cell_ID) * 3),
    ANVIL__ilt__cell_to_address = sizeof(ANVIL__instruction_ID) + sizeof(ANVIL__flag_ID) + (sizeof(ANVIL__cell_ID) * 3),
    ANVIL__ilt__file_to_buffer = sizeof(ANVIL__instruction_ID) + (sizeof(ANVIL__cell_ID) * 4),
    ANVIL__ilt__buffer_to_file = sizeof(ANVIL__instruction_ID) + (sizeof(ANVIL__cell_ID) * 4),
    ANVIL__ilt__buffer_to_buffer = sizeof(ANVIL__instruction_ID) + (sizeof(ANVIL__cell_ID) * 4),
    ANVIL__ilt__compile = sizeof(ANVIL__instruction_ID) + (sizeof(ANVIL__cell_ID) * 11),
    ANVIL__ilt__run = sizeof(ANVIL__instruction_ID) + (sizeof(ANVIL__cell_ID) * 3),
    ANVIL__ilt__debug__putchar = sizeof(ANVIL__instruction_ID) + sizeof(ANVIL__cell_ID),
    ANVIL__ilt__debug__print_cell_as_decimal = sizeof(ANVIL__instruction_ID) + sizeof(ANVIL__cell_ID),
    ANVIL__ilt__debug__fgets = sizeof(ANVIL__instruction_ID) + (sizeof(ANVIL__cell_ID) * 2),
    ANVIL__ilt__debug__mark_data_section = sizeof(ANVIL__instruction_ID) + sizeof(ANVIL__cell),
    ANVIL__ilt__debug__mark_code_section = sizeof(ANVIL__instruction_ID) + sizeof(ANVIL__cell),
} ANVIL__ilt;

// error codes
typedef enum ANVIL__et {
    // error codes
    ANVIL__et__no_error = 0,
    ANVIL__et__invalid_instruction_ID,
    ANVIL__et__divide_by_zero,
    ANVIL__et__modulous_by_zero,
    ANVIL__et__file_not_found,
    ANVIL__et__file_not_created,
    ANVIL__et__invalid_address__address_to_cell,
    ANVIL__et__invalid_address__cell_to_address,
    ANVIL__et__internal_allocation_tracking_error__could_not_record_buffer,
    ANVIL__et__invalid_allocation__allocation_does_not_exist,
    ANVIL__et__invalid_address_range,
    ANVIL__et__buffer_to_buffer__buffers_are_different_sizes,
    ANVIL__et__compile__compilation_error,
    ANVIL__et__program_ran_out_of_instructions,

    // count
    ANVIL__et__COUNT,
} ANVIL__et;

// operation types
typedef enum ANVIL__ot {
    // copy
    ANVIL__ot__cell_to_cell, // copies one cell to another without transformation

    // binary operations
    ANVIL__ot__bits_or,
    ANVIL__ot__bits_invert,
    ANVIL__ot__bits_and,
    ANVIL__ot__bits_xor,
    ANVIL__ot__bits_shift_higher,
    ANVIL__ot__bits_shift_lower,
    ANVIL__ot__bits_overwrite,

    // arithmetic operations
    ANVIL__ot__integer_add,
    ANVIL__ot__integer_subtract,
    ANVIL__ot__integer_multiply,
    ANVIL__ot__integer_division,
    ANVIL__ot__integer_modulous,

    // comparison operations
    ANVIL__ot__integer_within_range, // equivalent to (range_start <= integer_n && integer_n <= range_end) -> boolean

    // flag operations
    ANVIL__ot__flag_or,
    ANVIL__ot__flag_invert,
    ANVIL__ot__flag_and,
    ANVIL__ot__flag_xor,
    ANVIL__ot__flag_get,
    ANVIL__ot__flag_set,

    // count
    ANVIL__ot__COUNT,
} ANVIL__ot;

/* Helper Functions */
// convert instruction type to instruction length type
ANVIL__ilt ANVIL__convert__it_to_ilt(ANVIL__it instruction) {
    ANVIL__ilt lengths[] = {
        ANVIL__ilt__stop,
        ANVIL__ilt__write_cell,
        ANVIL__ilt__operate,
        ANVIL__ilt__request_memory,
        ANVIL__ilt__return_memory,
        ANVIL__ilt__address_to_cell,
        ANVIL__ilt__cell_to_address,
        ANVIL__ilt__file_to_buffer,
        ANVIL__ilt__buffer_to_file,
        ANVIL__ilt__buffer_to_buffer,
        ANVIL__ilt__compile,
        ANVIL__ilt__run,
        ANVIL__ilt__debug__putchar,
        ANVIL__ilt__debug__print_cell_as_decimal,
        ANVIL__ilt__debug__fgets,
        ANVIL__ilt__debug__mark_data_section,
        ANVIL__ilt__debug__mark_code_section,
    };

    return lengths[instruction];
}

// get a pointer from a context to a cell inside that context
ANVIL__cell* ANVIL__get__cell_address_from_context(ANVIL__context* context, ANVIL__cell_ID cell_ID) {
    // return data
    return &((*context).cells[cell_ID]);
}

// get cell value from context
ANVIL__cell ANVIL__get__cell_from_context(ANVIL__context* context, ANVIL__cell_ID cell_ID) {
    // return data
    return *ANVIL__get__cell_address_from_context(context, cell_ID);
}

// set cell value by pointer
void ANVIL__set__cell_by_address(ANVIL__cell* destination_cell, ANVIL__cell value) {
    // set value
    *destination_cell = value;

    return;
}

// set value in error code cell
void ANVIL__set__error_code_cell(ANVIL__context* context, ANVIL__et error_code) {
    // set error
    ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, ANVIL__rt__error_code), (ANVIL__cell)error_code);

    return;
}

// get flag (value)
ANVIL__bt ANVIL__get__flag_from_context(ANVIL__context* context, ANVIL__flag_ID flag_ID) {
    ANVIL__u8 masks[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

    // return bit
    return (ANVIL__bt)((((ANVIL__u8*)&((*context).cells[ANVIL__rt__flags_0]))[flag_ID / 8] & masks[flag_ID % 8]) > 0);
}

// set flag (value)
void ANVIL__set__flag_in_context(ANVIL__context* context, ANVIL__flag_ID flag_ID, ANVIL__bt value) {
    ANVIL__u8 masks[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

    // clear bit
    ((((ANVIL__u8*)&((*context).cells[ANVIL__rt__flags_0]))[flag_ID / 8] &= (~masks[flag_ID % 8])));

    // set bit
    ((ANVIL__u8*)&((*context).cells[ANVIL__rt__flags_0]))[flag_ID / 8] |= (value << (flag_ID % 8));

    return;
}

// read item from buffer and advance read address
ANVIL__u64 ANVIL__read_next__buffer_item(ANVIL__cell* address, ANVIL__byte_count byte_count) {
    ANVIL__u64 output;

    // read data from buffer
    output = ANVIL__read__buffer(*address, byte_count);

    // advance pointer
    *address = (ANVIL__address)(*((u8**)address) + byte_count);

    // return data
    return output;
}

// get cell value and advance
ANVIL__cell ANVIL__read_next__cell(ANVIL__cell* address) {
    // read data
    return (ANVIL__cell)ANVIL__read_next__buffer_item(address, sizeof(ANVIL__cell));
}

// get instruction ID and advance
ANVIL__instruction_ID ANVIL__read_next__instruction_ID(ANVIL__cell* address) {
    // read data
    return (ANVIL__instruction_ID)ANVIL__read_next__buffer_item(address, sizeof(ANVIL__instruction_ID));
}

// get flag ID and advance
ANVIL__flag_ID ANVIL__read_next__flag_ID(ANVIL__cell* address) {
    // read data
    return (ANVIL__flag_ID)ANVIL__read_next__buffer_item(address, sizeof(ANVIL__flag_ID));
}

// get operation ID and advance
ANVIL__operation_ID ANVIL__read_next__operation_ID(ANVIL__cell* address) {
    // read data
    return (ANVIL__operation_ID)ANVIL__read_next__buffer_item(address, sizeof(ANVIL__operation_ID));
}

// get cell ID and advance
ANVIL__cell_ID ANVIL__read_next__cell_ID(ANVIL__cell* address) {
    // read data
    return (ANVIL__cell_ID)ANVIL__read_next__buffer_item(address, sizeof(ANVIL__cell_ID));
}

/* Setup Alloy Code */
// create a skeleton context
ANVIL__context ANVIL__setup__context(ANVIL__buffer program) {
    ANVIL__context output;

    // setup program and execution cells
    output.cells[ANVIL__rt__program_start_address] = (ANVIL__cell)program.start;
    output.cells[ANVIL__rt__program_end_address] = (ANVIL__cell)program.end;
    output.cells[ANVIL__rt__program_current_address] = output.cells[ANVIL__rt__program_start_address];

    return output;
}

/* Run Alloy Code */
void ANVIL__run__context(ANVIL__allocations* allocations, ANVIL__context* context, ANVIL__instruction_count instruction_count);

// process operation (assumes flag was checked)
ANVIL__nit ANVIL__run__operation(ANVIL__context* context, ANVIL__ot operation_type, ANVIL__cell_ID input_0, ANVIL__cell_ID input_1, ANVIL__cell_ID input_2, ANVIL__cell_ID output_0) {
    ANVIL__cell_integer_value temp_input_0;
    ANVIL__cell_integer_value temp_input_1;
    ANVIL__cell_integer_value temp_input_2;
    ANVIL__cell_integer_value temp_result;

    // do operation based on type
    switch (operation_type) {
    // cell to cell
    case ANVIL__ot__cell_to_cell:
        // set value
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), ANVIL__get__cell_from_context(context, input_0));

        break;
    // binary or
    case ANVIL__ot__bits_or:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // perform operation
        temp_result = temp_input_0 | temp_input_1;

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // binary invert
    case ANVIL__ot__bits_invert:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);

        // perform operation
        temp_result = ~temp_input_0;

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // binary and
    case ANVIL__ot__bits_and:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // perform operation
        temp_result = temp_input_0 & temp_input_1;

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // binary xor
    case ANVIL__ot__bits_xor:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // perform operation
        temp_result = temp_input_0 ^ temp_input_1;

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // binary bit shift higher
    case ANVIL__ot__bits_shift_higher:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // perform operation
        temp_result = temp_input_0 << temp_input_1;

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // binary bit shift lower
    case ANVIL__ot__bits_shift_lower:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // perform operation
        temp_result = temp_input_0 >> temp_input_1;

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // binary overwrite bits
    case ANVIL__ot__bits_overwrite:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0); // mask (positive bits are the ones being overwritten!)
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1); // old bits
        temp_input_2 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_2); // new bits

        // perform operation
        temp_result = (~temp_input_0) & temp_input_1;
        temp_result = temp_result | (temp_input_2 & temp_input_0);

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // integer addition
    case ANVIL__ot__integer_add:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // perform operation
        temp_result = temp_input_0 + temp_input_1;

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // integer subtraction
    case ANVIL__ot__integer_subtract:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // perform operation
        temp_result = temp_input_0 - temp_input_1;

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // integer multiplication
    case ANVIL__ot__integer_multiply:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // perform operation
        temp_result = temp_input_0 * temp_input_1;

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // integer division
    case ANVIL__ot__integer_division:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // check if division is possible
        if (temp_input_1 != 0) {
            // compute result
            temp_result = temp_input_0 / temp_input_1;
        } else {
            // set error
            ANVIL__set__error_code_cell(context, ANVIL__et__divide_by_zero);

            // set blank output
            temp_result = 0;
        }

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // integer modulous
    case ANVIL__ot__integer_modulous:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0);
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1);

        // check if modulous is possible
        if (temp_input_1 != 0) {
            // compute result
            temp_result = temp_input_0 % temp_input_1;
        } else {
            // set error
            ANVIL__set__error_code_cell(context, ANVIL__et__modulous_by_zero);

            // set blank output
            temp_result = 0;
        }

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // integer range check
    case ANVIL__ot__integer_within_range:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0); // range start
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1); // value to be checked
        temp_input_2 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_2); // range end

        // check range
        temp_result = (ANVIL__cell_integer_value)((temp_input_0 <= temp_input_1) && (temp_input_1 <= temp_input_2));

        // set result cell
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)temp_result);

        break;
    // flag or
    case ANVIL__ot__flag_or:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0); // first flag
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1); // second flag
        temp_result = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, output_0); // output flag

        // 'or' flags into new flag
        ANVIL__set__flag_in_context(context, temp_result, (ANVIL__bt)(ANVIL__get__flag_from_context(context, (ANVIL__flag_ID)temp_input_0) | ANVIL__get__flag_from_context(context, (ANVIL__flag_ID)temp_input_1)));

        break;
    // flag invert
    case ANVIL__ot__flag_invert:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0); // first flag
        temp_result = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, output_0); // output flag

        // invert flag
        ANVIL__set__flag_in_context(context, temp_result, (ANVIL__bt)!(ANVIL__get__flag_from_context(context, (ANVIL__flag_ID)temp_input_0)));

        break;
    // flag and
    case ANVIL__ot__flag_and:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0); // first flag
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1); // second flag
        temp_result = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, output_0); // output flag

        // 'and' flags into new flag
        ANVIL__set__flag_in_context(context, temp_result, (ANVIL__bt)(ANVIL__get__flag_from_context(context, (ANVIL__flag_ID)temp_input_0) & ANVIL__get__flag_from_context(context, (ANVIL__flag_ID)temp_input_1)));

        break;
    // flag xor
    case ANVIL__ot__flag_xor:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0); // first flag
        temp_input_1 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_1); // second flag
        temp_result = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, output_0); // output flag

        // 'xor' flags into new flag
        ANVIL__set__flag_in_context(context, temp_result, (ANVIL__bt)(ANVIL__get__flag_from_context(context, (ANVIL__flag_ID)temp_input_0) != ANVIL__get__flag_from_context(context, (ANVIL__flag_ID)temp_input_1)));

        break;
    // flag get
    case ANVIL__ot__flag_get:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0); // flag address

        // get flag
        ANVIL__set__cell_by_address(ANVIL__get__cell_address_from_context(context, output_0), (ANVIL__cell)(ANVIL__u64)(ANVIL__get__flag_from_context(context, (ANVIL__flag_ID)temp_input_0) > 0));

        break;
    // flag set
    case ANVIL__ot__flag_set:
        // get data
        temp_input_0 = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, input_0); // flag value
        temp_result = (ANVIL__cell_integer_value)ANVIL__get__cell_from_context(context, output_0); // destination flag ID

        // set flag
        ANVIL__set__flag_in_context(context, (ANVIL__flag_ID)temp_result, (ANVIL__bt)temp_input_0);

        break;
    // operation ID was invalid
    default:
        // return failed instruction
        return ANVIL__nit__return_context;
    }

    return ANVIL__nit__next_instruction;
}

// process instruction
ANVIL__nit ANVIL__run__instruction(ANVIL__allocations* allocations, ANVIL__context* context) {
    // execution current read address
    ANVIL__cell* execution_read_address;

    // instruction ID
    ANVIL__it instruction_ID;

    // write cell temps
    ANVIL__cell write_cell__cell;
    ANVIL__cell_ID write_cell__cell_ID;

    // operate temps
    ANVIL__flag_ID operate__flag_ID;
    ANVIL__operation_ID operate__operation_ID;
    ANVIL__cell_ID operate__input_cell_ID_0;
    ANVIL__cell_ID operate__input_cell_ID_1;
    ANVIL__cell_ID operate__input_cell_ID_2;
    ANVIL__cell_ID operate__output_cell_ID_0;

    // request memory temps
    ANVIL__cell_ID request_memory__allocation_size;
    ANVIL__cell_ID request_memory__allocation_start;
    ANVIL__cell_ID request_memory__allocation_end;
    ANVIL__buffer request_memory__allocation;
    ANVIL__bt request_memory__buffer_appending_error = ANVIL__bt__false;

    // return memory temps
    ANVIL__cell_ID return_memory__allocation_start;
    ANVIL__cell_ID return_memory__allocation_end;
    ANVIL__buffer return_memory__allocation;
    ANVIL__bt return_memory__allocation_does_not_exist = ANVIL__bt__false;

    // address to cell temps
    ANVIL__flag_ID address_to_cell__flag_ID;
    ANVIL__length address_to_cell__source_cell_ID;
    ANVIL__length address_to_cell__bit_count_cell_ID;
    ANVIL__length address_to_cell__destination_cell_ID;

    // cell to address temps
    ANVIL__flag_ID cell_to_address__flag_ID;
    ANVIL__length cell_to_address__source_cell_ID;
    ANVIL__length cell_to_address__bit_count_cell_ID;
    ANVIL__length cell_to_address__destination_cell_ID;

    // file to buffer temps
    ANVIL__cell_ID file_to_buffer__file_name_start;
    ANVIL__cell_ID file_to_buffer__file_name_end;
    ANVIL__cell_ID file_to_buffer__file_data_start;
    ANVIL__cell_ID file_to_buffer__file_data_end;
    ANVIL__buffer file_to_buffer__file_name;
    ANVIL__buffer file_to_buffer__file_data;
    ANVIL__bt file_to_buffer__buffer_appending_error = ANVIL__bt__false;

    // buffer to file temps
    ANVIL__cell_ID buffer_to_file__file_data_start;
    ANVIL__cell_ID buffer_to_file__file_data_end;
    ANVIL__cell_ID buffer_to_file__file_name_start;
    ANVIL__cell_ID buffer_to_file__file_name_end;
    ANVIL__buffer buffer_to_file__file_data;
    ANVIL__buffer buffer_to_file__file_name;
    ANVIL__bt buffer_to_file__error = ANVIL__bt__false;

    // buffer to buffer temps
    ANVIL__cell_ID buffer_to_buffer__source_start;
    ANVIL__cell_ID buffer_to_buffer__source_end;
    ANVIL__cell_ID buffer_to_buffer__destination_start;
    ANVIL__cell_ID buffer_to_buffer__destination_end;
    ANVIL__buffer buffer_to_buffer__source;
    ANVIL__buffer buffer_to_buffer__destination;
    ANVIL__bt buffer_to_buffer__error = ANVIL__bt__false;

    // compile temps
    ANVIL__cell_ID compile__user_code_buffers_buffer_start;
    ANVIL__cell_ID compile__user_code_buffers_buffer_end;
    ANVIL__cell_ID compile__debug_enabled;
    ANVIL__cell_ID compile__output_start;
    ANVIL__cell_ID compile__output_end;
    ANVIL__cell_ID compile__error__occured;
    ANVIL__cell_ID compile__error__message_start;
    ANVIL__cell_ID compile__error__message_end;
    ANVIL__cell_ID compile__error__character_location__file_index;
    ANVIL__cell_ID compile__error__character_location__line_number;
    ANVIL__cell_ID compile__error__character_location__character_index;
    ANVIL__buffer compile__output_program;
    ANVIL__buffer compile__output_error_message;
    ANVIL__error compile__error;
    ANVIL__bt compile__buffer_appending_error = ANVIL__bt__false;

    // run temps
    ANVIL__cell_ID run__context_buffer_start;
    ANVIL__cell_ID run__context_buffer_end;
    ANVIL__cell_ID run__instruction_count;

    // debug putchar temps
    ANVIL__cell_ID debug__putchar__printing_cell_ID;

    // debug print cell as decimal temps
    ANVIL__cell_ID debug__print_cell_as_decimal__printing_cell_ID;

    // debug fgets temps
    ANVIL__cell_ID debug__fgets__buffer_address_start;
    ANVIL__cell_ID debug__fgets__buffer_address_end;
    ANVIL__u8 debug__fgets__temporary_string[ANVIL__define__input_string_max_length];
    ANVIL__buffer debug__fgets__buffer;
    ANVIL__length debug__fgets__buffer_length;
    ANVIL__bt debug__fgets__buffer_appending_error = ANVIL__bt__false;

    // debug mark data section temps
    ANVIL__cell debug__mark_data_section__section_length;

    // debug mark code section temps
    ANVIL__cell debug__mark_code_section__section_length;

    // setup execution read address
    execution_read_address = ANVIL__get__cell_address_from_context(context, ANVIL__rt__program_current_address);

    // get instruction ID from program
    instruction_ID = (ANVIL__it)ANVIL__read_next__instruction_ID(execution_read_address);

    // DEBUG
    // printf("[%lu]: instruction_ID: %lu\n", (ANVIL__u64)(*execution_read_address) - 1, (ANVIL__u64)instruction_ID);

    // check for valid instruction ID
    if (instruction_ID < ANVIL__it__COUNT) {
        // check for valid allocation
        if (ANVIL__check__valid_address_range_in_allocations(allocations, ANVIL__create__buffer((*context).cells[ANVIL__rt__program_current_address], (*context).cells[ANVIL__rt__program_current_address] + ANVIL__convert__it_to_ilt(instruction_ID) - 2)) == ANVIL__bt__false) {
            // set error code
            ANVIL__set__error_code_cell(context, ANVIL__et__program_ran_out_of_instructions);

            /*// DEBUG
            printf("Ran out of instructions!\n");
            
            // print allocations
            ANVIL__print__allocations(*allocations);

            // print allocation
            printf("\t[ %lu %lu ] (%li)\n", (ANVIL__cell_integer_value)(*context).cells[ANVIL__rt__program_current_address], (ANVIL__cell_integer_value)(*context).cells[ANVIL__rt__program_current_address] + ANVIL__convert__it_to_ilt(instruction_ID) - 2, (ANVIL__cell_integer_value)(*context).cells[ANVIL__rt__program_current_address] + ANVIL__convert__it_to_ilt(instruction_ID) - 2 - (ANVIL__cell_integer_value)((ANVIL__cell_integer_value)(*context).cells[ANVIL__rt__program_current_address]));*/

            // allocation does not exist, quit
            return ANVIL__nit__return_context;
        }
    // invalid instruction ID
    } else {
        // set error
        ANVIL__set__error_code_cell(context, ANVIL__et__invalid_instruction_ID);

        // return exit context
        return ANVIL__nit__return_context;
    }

    // process instruction accordingly
    switch (instruction_ID) {
    // if context should stop
    case ANVIL__it__stop:
        // return exit context
        return ANVIL__nit__return_context;
    // overwrite cell value
    case ANVIL__it__write_cell:
        // get parameters
        write_cell__cell = ANVIL__read_next__cell(execution_read_address);
        write_cell__cell_ID = ANVIL__read_next__cell_ID(execution_read_address);

        // do action
        (*context).cells[write_cell__cell_ID] = write_cell__cell;

        break;
    // operate between cells
    case ANVIL__it__operate:
        // get parameters
        operate__flag_ID = ANVIL__read_next__flag_ID(execution_read_address);
        operate__operation_ID = ANVIL__read_next__operation_ID(execution_read_address);
        operate__input_cell_ID_0 = ANVIL__read_next__cell_ID(execution_read_address);
        operate__input_cell_ID_1 = ANVIL__read_next__cell_ID(execution_read_address);
        operate__input_cell_ID_2 = ANVIL__read_next__cell_ID(execution_read_address);
        operate__output_cell_ID_0 = ANVIL__read_next__cell_ID(execution_read_address);

        // if flag enabled
        if (ANVIL__get__flag_from_context(context, operate__flag_ID) == ANVIL__bt__true) {
            // perform operation
            return ANVIL__run__operation(context, (ANVIL__ot)operate__operation_ID, operate__input_cell_ID_0, operate__input_cell_ID_1, operate__input_cell_ID_2, operate__output_cell_ID_0);
        }

        break;
    // ask os for new buffer
    case ANVIL__it__request_memory:
        // get parameters
        request_memory__allocation_size = ANVIL__read_next__cell_ID(execution_read_address);
        request_memory__allocation_start = ANVIL__read_next__cell_ID(execution_read_address);
        request_memory__allocation_end = ANVIL__read_next__cell_ID(execution_read_address);

        // do action
        request_memory__allocation = ANVIL__open__buffer((ANVIL__length)(*context).cells[request_memory__allocation_size]);

        // add allocation if successful
        if (ANVIL__check__empty_buffer(request_memory__allocation) == ANVIL__bt__false) {
            // remember allocation
            ANVIL__remember__allocation(allocations, request_memory__allocation, &request_memory__buffer_appending_error);

            // if buffer could not be added
            if (request_memory__buffer_appending_error == ANVIL__bt__true) {
                // set internal error
                ANVIL__set__error_code_cell(context, ANVIL__et__internal_allocation_tracking_error__could_not_record_buffer);

                // free buffer since it could not be safely added and is thus useless
                ANVIL__close__buffer(request_memory__allocation);

                // setup empty buffer
                request_memory__allocation = ANVIL__create_null__buffer();
            }
        }

        // set cell data
        (*context).cells[request_memory__allocation_start] = request_memory__allocation.start;
        (*context).cells[request_memory__allocation_end] = request_memory__allocation.end;

        break;
    // return buffer to OS
    case ANVIL__it__return_memory:
        // get parameters
        return_memory__allocation_start = ANVIL__read_next__cell_ID(execution_read_address);
        return_memory__allocation_end = ANVIL__read_next__cell_ID(execution_read_address);

        // get parameters
        return_memory__allocation.start = (*context).cells[return_memory__allocation_start];
        return_memory__allocation.end = (*context).cells[return_memory__allocation_end];

        // remove buffer from valid allocation list
        ANVIL__forget__allocation(allocations, return_memory__allocation, &return_memory__allocation_does_not_exist);

        // if allocation existed
        if (return_memory__allocation_does_not_exist == ANVIL__bt__false) {
            // deallocate
            ANVIL__close__buffer(return_memory__allocation);
        // allocation did not exist
        } else {
            // set error
            ANVIL__set__error_code_cell(context, ANVIL__et__invalid_allocation__allocation_does_not_exist);
        }

        break;
    // take data from an address and put it into a cell
    case ANVIL__it__address_to_cell:
        // get parameters
        address_to_cell__flag_ID = ANVIL__read_next__flag_ID(execution_read_address);
        address_to_cell__source_cell_ID = ANVIL__read_next__cell_ID(execution_read_address);
        address_to_cell__bit_count_cell_ID = ANVIL__read_next__cell_ID(execution_read_address);
        address_to_cell__destination_cell_ID = ANVIL__read_next__cell_ID(execution_read_address);

        // if valid address range
        if (ANVIL__check__valid_address_range_in_allocations(allocations, ANVIL__create__buffer((ANVIL__address)(*context).cells[address_to_cell__source_cell_ID], (ANVIL__address)(*context).cells[address_to_cell__source_cell_ID] + (((ANVIL__length)(*context).cells[address_to_cell__bit_count_cell_ID]) / ANVIL__define__bits_in_byte) - 1))) {
            // if flag enabled
            if (ANVIL__get__flag_from_context(context, address_to_cell__flag_ID) == ANVIL__bt__true) {
                // read data into cell
                (*context).cells[address_to_cell__destination_cell_ID] = (ANVIL__cell)ANVIL__read__buffer((ANVIL__address)(*context).cells[address_to_cell__source_cell_ID], ((ANVIL__length)(*context).cells[address_to_cell__bit_count_cell_ID]) / ANVIL__define__bits_in_byte);
            }
        // invalid address range
        } else {
            // set error
            ANVIL__set__error_code_cell(context, ANVIL__et__invalid_address_range);

            // setup blank value
            (*context).cells[address_to_cell__destination_cell_ID] = ANVIL__define__null_address;
        }

        break;
    // take data from a cell and put it at an address
    case ANVIL__it__cell_to_address:
        // get parameters
        cell_to_address__flag_ID = ANVIL__read_next__flag_ID(execution_read_address);
        cell_to_address__source_cell_ID = ANVIL__read_next__cell_ID(execution_read_address);
        cell_to_address__bit_count_cell_ID = ANVIL__read_next__cell_ID(execution_read_address);
        cell_to_address__destination_cell_ID = ANVIL__read_next__cell_ID(execution_read_address);

        // if valid address range
        if (ANVIL__check__valid_address_range_in_allocations(allocations, ANVIL__create__buffer((*context).cells[cell_to_address__destination_cell_ID], (*context).cells[cell_to_address__destination_cell_ID] + (((ANVIL__length)(*context).cells[cell_to_address__bit_count_cell_ID]) / ANVIL__define__bits_in_byte) - 1))) {
            // if flag enabled
            if (ANVIL__get__flag_from_context(context, cell_to_address__flag_ID) == ANVIL__bt__true) {
                // write data to an address
                ANVIL__write__buffer((ANVIL__u64)(*context).cells[cell_to_address__source_cell_ID], ((ANVIL__length)(*context).cells[cell_to_address__bit_count_cell_ID]) / ANVIL__define__bits_in_byte, (*context).cells[cell_to_address__destination_cell_ID]);
            }
        // invalid address range
        } else {
            // set error
            ANVIL__set__error_code_cell(context, ANVIL__et__invalid_address_range);
        }

        break;
    // take data from a file and create a buffer with it
    case ANVIL__it__file_to_buffer:
        // get parameters
        file_to_buffer__file_name_start = ANVIL__read_next__cell_ID(execution_read_address);
        file_to_buffer__file_name_end = ANVIL__read_next__cell_ID(execution_read_address);
        file_to_buffer__file_data_start = ANVIL__read_next__cell_ID(execution_read_address);
        file_to_buffer__file_data_end = ANVIL__read_next__cell_ID(execution_read_address);

        // setup temps
        file_to_buffer__file_name.start = (*context).cells[file_to_buffer__file_name_start];
        file_to_buffer__file_name.end = (*context).cells[file_to_buffer__file_name_end];

        // get data from file
        file_to_buffer__file_data = ANVIL__move__file_to_buffer(file_to_buffer__file_name);

        // check for errors
        if (file_to_buffer__file_data.start == ANVIL__define__null_address) {
            // set error
            ANVIL__set__error_code_cell(context, ANVIL__et__file_not_found);
        // append buffer as valid allocation
        } else {
            // remember allocation
            ANVIL__remember__allocation(allocations, file_to_buffer__file_data, &file_to_buffer__buffer_appending_error);

            // if buffer could not be added
            if (file_to_buffer__buffer_appending_error == ANVIL__bt__true) {
                // set internal error
                ANVIL__set__error_code_cell(context, ANVIL__et__internal_allocation_tracking_error__could_not_record_buffer);

                // free buffer since it could not be safely added and is thus useless
                ANVIL__close__buffer(file_to_buffer__file_data);

                // setup empty buffer
                file_to_buffer__file_data = ANVIL__create_null__buffer();
            }
        }

        // write data to cells
        (*context).cells[file_to_buffer__file_data_start] = file_to_buffer__file_data.start;
        (*context).cells[file_to_buffer__file_data_end] = file_to_buffer__file_data.end;

        break;
    // take a buffer and overwrite a file with it
    case ANVIL__it__buffer_to_file:
        // get parameters
        buffer_to_file__file_data_start = ANVIL__read_next__cell_ID(execution_read_address);
        buffer_to_file__file_data_end = ANVIL__read_next__cell_ID(execution_read_address);
        buffer_to_file__file_name_start = ANVIL__read_next__cell_ID(execution_read_address);
        buffer_to_file__file_name_end = ANVIL__read_next__cell_ID(execution_read_address);

        // setup temps
        buffer_to_file__file_data.start = (*context).cells[buffer_to_file__file_data_start];
        buffer_to_file__file_data.end = (*context).cells[buffer_to_file__file_data_end];
        buffer_to_file__file_name.start = (*context).cells[buffer_to_file__file_name_start];
        buffer_to_file__file_name.end = (*context).cells[buffer_to_file__file_name_end];

        // if source allocations exists
        if (ANVIL__check__valid_address_range_in_allocations(allocations, buffer_to_file__file_data) && ANVIL__check__valid_address_range_in_allocations(allocations, buffer_to_file__file_name)) {
            // create file
            ANVIL__move__buffer_to_file(&buffer_to_file__error, buffer_to_file__file_name, buffer_to_file__file_data);

            // check for errors
            if (buffer_to_file__error == ANVIL__bt__true) {
                // set error
                ANVIL__set__error_code_cell(context, ANVIL__et__file_not_created);
            }
        // if any allocations do not exist
        } else {
            // set error
            ANVIL__set__error_code_cell(context, ANVIL__et__invalid_allocation__allocation_does_not_exist);
        }

        break;
    // copy one buffer of data from one area to another of equal size
    case ANVIL__it__buffer_to_buffer:
        // get parameters
        buffer_to_buffer__source_start = ANVIL__read_next__cell_ID(execution_read_address);
        buffer_to_buffer__source_end = ANVIL__read_next__cell_ID(execution_read_address);
        buffer_to_buffer__destination_start = ANVIL__read_next__cell_ID(execution_read_address);
        buffer_to_buffer__destination_end = ANVIL__read_next__cell_ID(execution_read_address);

        // setup temps
        buffer_to_buffer__source.start = (*context).cells[buffer_to_buffer__source_start];
        buffer_to_buffer__source.end = (*context).cells[buffer_to_buffer__source_end];
        buffer_to_buffer__destination.start = (*context).cells[buffer_to_buffer__destination_start];
        buffer_to_buffer__destination.end = (*context).cells[buffer_to_buffer__destination_end];

        // if both allocations exist
        if (ANVIL__check__valid_address_range_in_allocations(allocations, buffer_to_buffer__source) && ANVIL__check__valid_address_range_in_allocations(allocations, buffer_to_buffer__destination)) {
            // perform copy
            ANVIL__copy__buffer(buffer_to_buffer__source, buffer_to_buffer__destination, &buffer_to_buffer__error);

            // check for error
            if (buffer_to_buffer__error == ANVIL__bt__true) {
                ANVIL__set__error_code_cell(context, ANVIL__et__buffer_to_buffer__buffers_are_different_sizes);
            }
        // if one allocation does not exist
        } else {
            // set error
            ANVIL__set__error_code_cell(context, ANVIL__et__invalid_allocation__allocation_does_not_exist);
        }

        break;
    // compile program
    case ANVIL__it__compile:
        // get parameters
        compile__user_code_buffers_buffer_start = ANVIL__read_next__cell_ID(execution_read_address);
        compile__user_code_buffers_buffer_end = ANVIL__read_next__cell_ID(execution_read_address);
        compile__debug_enabled = ANVIL__read_next__cell_ID(execution_read_address);
        compile__output_start = ANVIL__read_next__cell_ID(execution_read_address);
        compile__output_end = ANVIL__read_next__cell_ID(execution_read_address);
        compile__error__occured = ANVIL__read_next__cell_ID(execution_read_address);
        compile__error__message_start = ANVIL__read_next__cell_ID(execution_read_address);
        compile__error__message_end = ANVIL__read_next__cell_ID(execution_read_address);
        compile__error__character_location__file_index = ANVIL__read_next__cell_ID(execution_read_address);
        compile__error__character_location__line_number = ANVIL__read_next__cell_ID(execution_read_address);
        compile__error__character_location__character_index = ANVIL__read_next__cell_ID(execution_read_address);

        // run compiler (WARNING, buffers are NOT checked for vality!)
        ANVIL__compile__files(ANVIL__create__buffer((*context).cells[compile__user_code_buffers_buffer_start], (*context).cells[compile__user_code_buffers_buffer_end]), (ANVIL__bt)(ANVIL__cell_integer_value)(*context).cells[compile__debug_enabled], &compile__output_program, &compile__error);

        // get temps
        compile__output_error_message = compile__error.message;

        // check if program was created
        if (ANVIL__check__error_occured(&compile__error) == ANVIL__bt__false) {
            // add new buffer to allocations
            ANVIL__remember__allocation(allocations, compile__output_program, &compile__buffer_appending_error);

            // if buffer could not be added
            if (compile__buffer_appending_error == ANVIL__bt__true) {
                // set internal error
                ANVIL__set__error_code_cell(context, ANVIL__et__internal_allocation_tracking_error__could_not_record_buffer);

                // free buffer since it could not be safely added and is thus useless
                ANVIL__close__buffer(compile__output_program);

                // setup empty buffer
                compile__output_program = ANVIL__create_null__buffer();
            }
        // program was not created
        } else {
            // setup empty buffer
            compile__output_program = ANVIL__create_null__buffer();

            // set error code
            ANVIL__set__error_code_cell(context, ANVIL__et__compile__compilation_error);

            // add error message to allocations
            ANVIL__remember__allocation(allocations, compile__output_error_message, &compile__buffer_appending_error);

            // if buffer could not be added
            if (compile__buffer_appending_error == ANVIL__bt__true) {
                // set internal error
                ANVIL__set__error_code_cell(context, ANVIL__et__internal_allocation_tracking_error__could_not_record_buffer);

                // free buffer since it could not be safely added and is thus useless
                ANVIL__close__buffer(compile__output_error_message);

                // setup empty buffer
                compile__output_error_message = ANVIL__create_null__buffer();
            }
        }

        // setup outputs
        (*context).cells[compile__output_start] = compile__output_program.start;
        (*context).cells[compile__output_end] = compile__output_program.end;
        (*context).cells[compile__error__occured] = (ANVIL__cell)(ANVIL__cell_integer_value)compile__error.occured;
        (*context).cells[compile__error__message_start] = compile__output_error_message.start;
        (*context).cells[compile__error__message_end] = compile__output_error_message.end;
        (*context).cells[compile__error__character_location__file_index] = (ANVIL__cell)compile__error.location.file_index;
        (*context).cells[compile__error__character_location__line_number] = (ANVIL__cell)compile__error.location.line_number;
        (*context).cells[compile__error__character_location__character_index] = (ANVIL__cell)compile__error.location.character_index;

        break;
    // run a context like a program
    case ANVIL__it__run:
        // get parameters
        run__context_buffer_start = ANVIL__read_next__cell_ID(execution_read_address);
        run__context_buffer_end = ANVIL__read_next__cell_ID(execution_read_address);
        run__instruction_count = ANVIL__read_next__cell_ID(execution_read_address);

        // useless operation to quiet compiler
        run__context_buffer_end = run__context_buffer_end;

        // if context buffer is valid
        if (ANVIL__check__valid_address_range_in_allocations(allocations, ANVIL__create__buffer((ANVIL__context*)(*context).cells[run__context_buffer_start], (ANVIL__context*)(*context).cells[run__context_buffer_end]))) {
            // run context
            ANVIL__run__context(allocations, (ANVIL__context*)(*context).cells[run__context_buffer_start], (u64)(*context).cells[run__instruction_count]);
        // error
        } else {
            // set error
            ANVIL__set__error_code_cell(context, ANVIL__et__invalid_address_range);
        }

        break;
    // print one char to stdout
    case ANVIL__it__debug__putchar:
        // get parameters
        debug__putchar__printing_cell_ID = ANVIL__read_next__cell_ID(execution_read_address);

        // print
        putchar((u8)(u64)((*context).cells[debug__putchar__printing_cell_ID]));

        // flush stream for full update
        fflush(stdout);

        break;
    // print one cell as a decimal number
    case ANVIL__it__debug__print_cell_as_decimal:
        // get parameters
        debug__print_cell_as_decimal__printing_cell_ID = ANVIL__read_next__cell_ID(execution_read_address);

        // print
        printf("%lu", (u64)(*context).cells[debug__print_cell_as_decimal__printing_cell_ID]);

        break;
    // read one string from stdin
    case ANVIL__it__debug__fgets:
        // get parameters
        debug__fgets__buffer_address_start = ANVIL__read_next__cell_ID(execution_read_address);
        debug__fgets__buffer_address_end = ANVIL__read_next__cell_ID(execution_read_address);

        // zero out temporaries
        for (ANVIL__length i = 0; i < ANVIL__define__input_string_max_length; i++) {
            // zero out character
            debug__fgets__temporary_string[i] = 0;
        }
        debug__fgets__buffer_length = 0;

        // read string
        fgets((char*)debug__fgets__temporary_string, ANVIL__define__input_string_max_length, stdin);

        // read buffer for string size
        while (debug__fgets__buffer_length < ANVIL__define__input_string_max_length && debug__fgets__temporary_string[debug__fgets__buffer_length] != 0) {
            // increment length
            debug__fgets__buffer_length++;
        }

        // create buffer based on string length
        debug__fgets__buffer = ANVIL__open__buffer(debug__fgets__buffer_length);

        // if buffer is not empty
        if (ANVIL__check__empty_buffer(debug__fgets__buffer) == ANVIL__bt__false) {
            // copy data into buffer
            for (ANVIL__u64 i = 0; i < debug__fgets__buffer_length; i++) {
                // write character
                ANVIL__write__buffer((u8)debug__fgets__temporary_string[i], sizeof(ANVIL__u8), (ANVIL__u8*)debug__fgets__buffer.start + (i * sizeof(ANVIL__u8)));
            }

            // remember allocation
            ANVIL__remember__allocation(allocations, debug__fgets__buffer, &debug__fgets__buffer_appending_error);

            // if buffer could not be added
            if (debug__fgets__buffer_appending_error == ANVIL__bt__true) {
                // set internal error
                ANVIL__set__error_code_cell(context, ANVIL__et__internal_allocation_tracking_error__could_not_record_buffer);

                // free buffer since it could not be safely added and is thus useless
                ANVIL__close__buffer(debug__fgets__buffer);

                // setup empty buffer
                debug__fgets__buffer = ANVIL__create_null__buffer();
            }
        }

        // setup cells
        (*context).cells[debug__fgets__buffer_address_start] = debug__fgets__buffer.start;
        (*context).cells[debug__fgets__buffer_address_end] = debug__fgets__buffer.end;

        break;
    // mark section of data
    case ANVIL__it__debug__mark_data_section:
        // get parameters
        debug__mark_data_section__section_length = ANVIL__read_next__cell(execution_read_address);

        // skip over data section
        (*context).cells[ANVIL__rt__program_current_address] = (ANVIL__address)((u64)(*context).cells[ANVIL__rt__program_current_address] + (u64)debug__mark_data_section__section_length);

        break;
    // mark section of code
    case ANVIL__it__debug__mark_code_section:
        // instruction does nothing but mark code space, so get length and do nothing
        debug__mark_code_section__section_length = ANVIL__read_next__cell(execution_read_address);

        // useless operation to quiet compiler
        debug__mark_code_section__section_length = debug__mark_code_section__section_length;

        break;
    // in case instruction ID was invalid
    default:
        // set error
        ANVIL__set__error_code_cell(context, ANVIL__et__invalid_instruction_ID);

        // return exit context
        return ANVIL__nit__return_context;
    }

    // return next instruction by default
    return ANVIL__nit__next_instruction;
}

// run context
void ANVIL__run__context(ANVIL__allocations* allocations, ANVIL__context* context, ANVIL__instruction_count instruction_count) {
    ANVIL__nit next_instruction_action;

    // if an infinite amount of instructions can execute
    if (instruction_count == ANVIL__define__run_forever) {
        // run instructions
        while (1) {
            // run instruction
            next_instruction_action = ANVIL__run__instruction(allocations, context);

            // if quit
            if (next_instruction_action == ANVIL__nit__return_context) {
                return;
            }

            // assuming ANVIL__nit__next_instruction
            continue;
        }
    // if a finite amount of instructions can execute
    } else {
        for (ANVIL__instruction_count i = 0; i < instruction_count; i++) {
            // run instruction
            next_instruction_action = ANVIL__run__instruction(allocations, context);

            // check for early quitting
            if (next_instruction_action == ANVIL__nit__return_context) {
                return;
            }
        }
    }

    return;
}

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
    ANVIL__srt__start__workspace = 4096,
    ANVIL__srt__start__function_io = ANVIL__rt__TOTAL_COUNT - 4096,

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

// build package
void ANVIL__standard__code__package(ANVIL__workspace* workspace, ANVIL__standard__offsets* standard_offsets) {
    // write functions
    ANVIL__standard__code__print_binary(workspace, standard_offsets);
    ANVIL__standard__code__print_buffer_as_string(workspace, standard_offsets);
	ANVIL__standard__code__calculate_decimal_digit_count(workspace, standard_offsets);
	ANVIL__standard__code__cell_to_unsigned_integer_string(workspace, standard_offsets);

    return;
}

/* Lexer */
// lexling types
typedef enum ANVIL__lt {
    ANVIL__lt__invalid,
    ANVIL__lt__left_parenthesis,
    ANVIL__lt__right_parenthesis,
    ANVIL__lt__left_curly_bracket,
    ANVIL__lt__right_curly_bracket,
    ANVIL__lt__name,
    ANVIL__lt__period,
    ANVIL__lt__at,
    ANVIL__lt__hashtag,
    ANVIL__lt__equals,
    ANVIL__lt__string_literal,
    ANVIL__lt__end_of_file,
    ANVIL__lt__COUNT,
} ANVIL__lt;

// lexling type
typedef struct ANVIL__lexling {
    ANVIL__lexling_type type;
    ANVIL__buffer value;
    ANVIL__character_location location;
} ANVIL__lexling;

// create custom lexling
ANVIL__lexling ANVIL__create__lexling(ANVIL__lexling_type type, ANVIL__buffer value, ANVIL__character_location location) {
    ANVIL__lexling output;

    // setup output
    output.type = type;
    output.value = value;
    output.location = location;

    return output;
}

// create null lexling
ANVIL__lexling ANVIL__create_null__lexling() {
    // return empty
    return ANVIL__create__lexling(ANVIL__lt__invalid, ANVIL__create_null__buffer(), ANVIL__create_null__character_location());
}

// lexlings
typedef struct ANVIL__lexlings {
    ANVIL__list data;
} ANVIL__lexlings;

// create custom lexlings
ANVIL__lexlings ANVIL__create__lexlings(ANVIL__list list) {
    ANVIL__lexlings output;

    // setup output
    output.data = list;

    return output;
}

// create null lexlings
ANVIL__lexlings ANVIL__create_null__lexlings() {
    // return empty
    return ANVIL__create__lexlings(ANVIL__create_null__list());
}

// close lexlings
void ANVIL__close__lexlings(ANVIL__lexlings lexlings) {
    // close buffer
    ANVIL__close__list(lexlings.data);

    return;
}

// append a lexling to the list
void ANVIL__append__lexling(ANVIL__lexlings* lexlings, ANVIL__lexling lexling, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(&((*lexlings).data), sizeof(ANVIL__lexling), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__lexling*)ANVIL__calculate__list_current_address(&((*lexlings).data))) = lexling;

    // increase fill
    (*lexlings).data.filled_index += sizeof(ANVIL__lexling);

    return;
}

// append a lexlings to the list
void ANVIL__append__lexlings(ANVIL__list* list, ANVIL__lexlings lexlings, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__lexlings), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__lexlings*)ANVIL__calculate__list_current_address(list)) = lexlings;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__lexlings);

    return;
}

// read a lexling from an address
ANVIL__lexling ANVIL__read__lexling_from_current(ANVIL__current current) {
    // return struct
    return *(ANVIL__lexling*)current.start;
}

// check one character
ANVIL__bt ANVIL__calculate__valid_character_range(ANVIL__current current, ANVIL__character start, ANVIL__character end) {
    // perform calculation
    return (ANVIL__bt)((*(ANVIL__character*)(current.start) >= start) && (*(ANVIL__character*)(current.start) <= end));
}

// check one character for a name character
ANVIL__bt ANVIL__calculate__valid_name_character(ANVIL__current current) {
    return (ANVIL__calculate__valid_character_range(current, 'a', 'z') || ANVIL__calculate__valid_character_range(current, 'A', 'Z') || ANVIL__calculate__valid_character_range(current, '0', '9') || ANVIL__calculate__valid_character_range(current, '_', '_'));
}

// calculate character index
ANVIL__character_index ANVIL__calculate__character_index(ANVIL__buffer main_buffer, ANVIL__buffer current) {
    return (ANVIL__character_index)(current.start - main_buffer.start);
}

// lex a program
ANVIL__lexlings ANVIL__compile__lex(ANVIL__buffer user_code, ANVIL__file_index file_index, ANVIL__error* error) {
    ANVIL__lexlings output;
    ANVIL__current current;
    ANVIL__line_number current_line_number;
    ANVIL__lexling_start temp_start;
    ANVIL__lexling_end temp_end;

    // setup output
    output.data = ANVIL__open__list_with_error(sizeof(ANVIL__lexling) * 64, error);

    // check for error
    if (ANVIL__check__error_occured(error)) {
        // return empty
        return ANVIL__create_null__lexlings();
    }

    // setup current & locations
    current = user_code;
    current_line_number = 1;

    // lex program
    while (ANVIL__check__current_within_range(current)) {
        // skip comments and whitespace
        while (ANVIL__check__current_within_range(current) && (ANVIL__calculate__valid_character_range(current, 0, 32) || ANVIL__calculate__valid_character_range(current, '[', '['))) {
            // skip whitespace
            while (ANVIL__check__current_within_range(current) && ANVIL__calculate__valid_character_range(current, 0, 32)) {
                // check for new line
                if (ANVIL__calculate__valid_character_range(current, '\n', '\n') || ANVIL__calculate__valid_character_range(current, '\r', '\r')) {
                    // next line
                    current_line_number++;
                }

                // next character
                current.start += sizeof(ANVIL__character);
            }

            // skip comments
            if (ANVIL__check__current_within_range(current) && ANVIL__calculate__valid_character_range(current, '[', '[')) {
                ANVIL__lexling_depth depth = 1;

                // next character
                current.start += sizeof(ANVIL__character);

                // skip past characters
                while (ANVIL__check__current_within_range(current) && depth > 0) {
                    // check for new line
                    if (ANVIL__calculate__valid_character_range(current, '\n', '\n') || ANVIL__calculate__valid_character_range(current, '\r', '\r')) {
                        // next line
                        current_line_number++;
                    }
                    // check for opening comment
                    if (ANVIL__calculate__valid_character_range(current, '[', '[')) {
                        // increase depth
                        depth++;
                    }
                    // check for closing comment
                    if (ANVIL__calculate__valid_character_range(current, ']', ']')) {
                        // decrease depth
                        depth--;
                    }

                    // next character
                    current.start += sizeof(ANVIL__character);
                }

                // check for unfinished comment
                if (depth > 0) {
                    // set error
                    *error = ANVIL__open__error("Lexing Error: Comment ended with end of file instead of proper closing.", ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current)));

                    goto quit;
                }
            }
        }

        // check for out of range
        if (ANVIL__check__current_within_range(current) == ANVIL__bt__false) {
            goto quit;
        }

        // check for lexlings
        if (ANVIL__calculate__valid_character_range(current, '(', '(')) {
            // add lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__left_parenthesis, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, ')', ')')) {
            // add lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__right_parenthesis, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '{', '{')) {
            // add lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__left_curly_bracket, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '}', '}')) {
            // add lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__right_curly_bracket, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_name_character(current)) {
            // get lexling start and setup temp end
            temp_start = current.start;
            temp_end = temp_start - 1;

            // get lexling size
            while (ANVIL__check__current_within_range(current) && ANVIL__calculate__valid_name_character(current)) {
                // next character
                current.start += sizeof(ANVIL__character);
                temp_end += sizeof(ANVIL__character);
            }

            // record lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__name, ANVIL__create__buffer(temp_start, temp_end), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, ANVIL__create__buffer(temp_start, temp_end)))), error);
        } else if (ANVIL__calculate__valid_character_range(current, '.', '.')) {
            // add lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__period, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '@', '@')) {
            // add lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__at, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '#', '#')) {
            // add lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__hashtag, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '=', '=')) {
            // add lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__equals, ANVIL__create__buffer(current.start, current.start), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        } else if (ANVIL__calculate__valid_character_range(current, '"', '"')) {
            ANVIL__buffer data;

            // get string start
            data.start = current.start;

            // advance current
            current.start += sizeof(ANVIL__character);

            // search for string end
            while (ANVIL__check__current_within_range(current) && ANVIL__calculate__valid_character_range(current, '\"', '\"') == ANVIL__bt__false) {
                // next character
                current.start += sizeof(ANVIL__character);
            }

            // check for end of file
            if (ANVIL__check__current_within_range(current) == ANVIL__bt__false) {
                // string ended abruptly
                *error = ANVIL__open__error("Lexical Error: String ended at the end of a file and not with a (\").", ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current)));

                goto quit;
            }

            // finish string data
            data.end = current.start;

            // append lexling
            ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__string_literal, data, ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, data))), error);

            // next character
            current.start += sizeof(ANVIL__character);
        // no lexling found
        } else {
            // open error
            *error = ANVIL__open__error("Lexical Error: Invalid character.", ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current)));

            // quit
            goto quit;
        }

        // check for error
        if (ANVIL__check__error_occured(error)) {
            // return lexlings as they are
            goto quit;
        }
    }

    // quit
    quit:

    // append eof lexling
    ANVIL__append__lexling(&output, ANVIL__create__lexling(ANVIL__lt__end_of_file, ANVIL__open__buffer_from_string((u8*)"[EOF]", ANVIL__bt__false, ANVIL__bt__false), ANVIL__create__character_location(file_index, current_line_number, ANVIL__calculate__character_index(user_code, current))), error);

    return output;
}

// print lexlings
void ANVIL__debug__print_lexlings(ANVIL__lexlings lexlings) {
    ANVIL__current current;
    ANVIL__lexling temp;

    // setup current
    current = lexlings.data.buffer;

    // print header
    printf("Lexlings:\n");

    // print each lexling
    while (current.start < lexlings.data.buffer.start + lexlings.data.filled_index) {
        // get lexling
        temp = ANVIL__read__lexling_from_current(current);

        // next lexling
        current.start += sizeof(ANVIL__lexling);

        // print lexling type
        printf("\t%lu [ %lu, %lu ] [ file_index: %lu, line_number: %lu, character_index: %lu ] : ", (ANVIL__u64)temp.type, (ANVIL__u64)temp.value.start, (ANVIL__u64)temp.value.end, temp.location.file_index, (ANVIL__u64)temp.location.line_number, (ANVIL__u64)temp.location.character_index);
        fflush(stdout);

        // print lexling string
        ANVIL__print__buffer(temp.value);

        // print new line
        printf("\n");
    }

    return;
}

/* Names */
// name struct
typedef struct ANVIL__name {
    ANVIL__lexling lexling;
} ANVIL__name;

// create name
ANVIL__name ANVIL__create__name(ANVIL__lexling lexling) {
    ANVIL__name output;

    output.lexling = lexling;

    return output;
}

// create null name
ANVIL__name ANVIL__create_null__name() {
    return ANVIL__create__name(ANVIL__create_null__lexling());
}

// append name
void ANVIL__append__name(ANVIL__list* list, ANVIL__name data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__name), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__name*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__name);

    return;
}

/* Parser */
// parsing argument type
typedef enum ANVIL__pat {
    ANVIL__pat__invalid,
    ANVIL__pat__variable,
    ANVIL__pat__variable__input,
    ANVIL__pat__variable__output,
    ANVIL__pat__variable__body,
    ANVIL__pat__variable__predefined,
    ANVIL__pat__offset,
    ANVIL__pat__flag,
    ANVIL__pat__flag__user_defined,
    ANVIL__pat__flag__predefined,
    ANVIL__pat__literal__boolean,
    ANVIL__pat__literal__binary,
    ANVIL__pat__literal__integer,
    ANVIL__pat__literal__hexadecimal,
    ANVIL__pat__literal__string,

    // count
    ANVIL__pat__COUNT,
} ANVIL__pat;

// parsling argument
typedef struct ANVIL__parsling_argument {
    ANVIL__pat type;
    ANVIL__name text;
    ANVIL__cell_integer_value value;
} ANVIL__parsling_argument;

// create a custom argument
ANVIL__parsling_argument ANVIL__create__parsling_argument(ANVIL__pat type, ANVIL__name text, ANVIL__cell_integer_value value) {
    ANVIL__parsling_argument output;

    // setup output
    output.type = type;
    output.text = text;
    output.value = value;

    return output;
}

// check if arguments have the same text
ANVIL__bt ANVIL__check__parsling_arguments_have_same_text(ANVIL__parsling_argument a, ANVIL__parsling_argument b) {
    return ANVIL__calculate__buffer_contents_equal(a.text.lexling.value, b.text.lexling.value);
}

// find parsling argument index in list
ANVIL__argument_index ANVIL__find__parsling_argument_index__by_name(ANVIL__list list, ANVIL__parsling_argument argument) {
    ANVIL__argument_index output = 0;

    // setup current
    ANVIL__current current_argument = ANVIL__calculate__current_from_list_filled_index(&list);

    // find argument
    while (ANVIL__check__current_within_range(current_argument)) {
        // check arg
        if (ANVIL__check__parsling_arguments_have_same_text(*(ANVIL__parsling_argument*)current_argument.start, argument)) {
            // found
            return output;
        }

        // next arg
        output++;
        current_argument.start += sizeof(ANVIL__parsling_argument);
    }

    // not found
    return output;
}

// translate string to boolean
ANVIL__bt ANVIL__translate__string_to_boolean(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    // check possible values
    if (ANVIL__calculate__buffer_contents_equal(string, ANVIL__open__buffer_from_string((u8*)"forge.boolean.false", ANVIL__bt__false, ANVIL__bt__false)) == ANVIL__bt__true) {
        *value = (ANVIL__cell_integer_value)(ANVIL__bt__false);

        return ANVIL__bt__true;
    }
    if (ANVIL__calculate__buffer_contents_equal(string, ANVIL__open__buffer_from_string((u8*)"forge.boolean.true", ANVIL__bt__false, ANVIL__bt__false)) == ANVIL__bt__true) {
        *value = (ANVIL__cell_integer_value)(ANVIL__bt__true);

        return ANVIL__bt__true;
    }

    return ANVIL__bt__false;
}

// translate string to binary
ANVIL__bt ANVIL__translate__string_to_binary(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    ANVIL__buffer prefix = ANVIL__open__buffer_from_string((u8*)"forge.binary.", ANVIL__bt__false, ANVIL__bt__false);
    ANVIL__buffer current;
    ANVIL__u64 character_count_limit = sizeof(ANVIL__u64) * ANVIL__define__bits_in_byte;
    ANVIL__u64 character_count = 0;

    // check for prefix
    if (ANVIL__calculate__buffer_starts_with_buffer(string, prefix) == ANVIL__bt__false) {
        // not a binary literal
        *value = ANVIL__define__null_address;

        return ANVIL__bt__false;
    }

    // setup current
    current = ANVIL__create__buffer(string.start + ANVIL__calculate__buffer_length(prefix), string.end);

    // pre check for all valid characters
    while (ANVIL__check__current_within_range(current)) {
        // check character
        if ((ANVIL__check__character_range_at_current(current, '0', '1') || ANVIL__check__character_range_at_current(current, '_', '_')) == ANVIL__bt__false) {
            // not a binary literal
            *value = ANVIL__define__null_address;

            return ANVIL__bt__false;
        }

        // count binary character
        if (ANVIL__check__character_range_at_current(current, '0', '1') == ANVIL__bt__true) {
            character_count++;
        }

        // advance current
        current.start += sizeof(ANVIL__character);
    }

    // check for sane character limit
    if (character_count > character_count_limit) {
        // binary literal to large, conversion failed
        *value = ANVIL__define__null_address;

        return ANVIL__bt__false;
    }

    // setup bit index
    ANVIL__bit_count bit_index = 0;

    // reset current
    current = ANVIL__create__buffer(string.start + ANVIL__calculate__buffer_length(prefix), string.end);

    // convert binary string to binary number
    while (ANVIL__check__current_within_range(current)) {
        if (ANVIL__check__character_range_at_current(current, '_', '_') == ANVIL__bt__false) {
            // append value
            *value += (((*(ANVIL__character*)current.start) - '0') << bit_index);

            // next bit index
            bit_index++;
        }

        // next character
        current.start += sizeof(ANVIL__character);
    }

    return ANVIL__bt__true;
}

// translate string to integer
ANVIL__bt ANVIL__translate__string_to_integer(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    ANVIL__buffer prefix = ANVIL__open__buffer_from_string((u8*)"forge.integer.", ANVIL__bt__false, ANVIL__bt__false);
    ANVIL__buffer suffix;
    ANVIL__u64 digit = 0;

    // check for prefix
    if (ANVIL__calculate__buffer_starts_with_buffer(string, prefix) == ANVIL__bt__false) {
        // not an integer literal
        *value = ANVIL__define__null_address;

        return ANVIL__bt__false;
    }

    // create suffix
    suffix = ANVIL__create__buffer(string.start + ANVIL__calculate__buffer_length(prefix), string.end);

    // translate number
    // if number is negative
    if (*(ANVIL__character*)suffix.start == (ANVIL__character)'n') {
        // for each character
        for (ANVIL__character_index i = ANVIL__calculate__buffer_length(suffix); i > 1; i--) {
            // check for valid character
            if (((((ANVIL__character*)suffix.start)[i - 1] >= '0' && ((ANVIL__character*)suffix.start)[i - 1] <= '9') || ((ANVIL__character*)suffix.start)[i - 1] == '_') == ANVIL__bt__false) {
                // invalid character
                *value = ANVIL__define__null_address;

                return ANVIL__bt__false;
            }

            // if calculable character
            if (((ANVIL__character*)suffix.start)[i - 1] != '_') {
                // add value
                *value += ANVIL__calculate__exponent(10, digit) * (((ANVIL__character*)suffix.start)[i - 1] - '0');

                // next digit power
                digit++;
            }
        }

        // turn number negative using twos compliment
        *value = ~(*value);
        *value = (*value) + 1;
    // if number is positive
    } else {
        // for each character
        for (ANVIL__character_index i = ANVIL__calculate__buffer_length(suffix); i > 0; i--) {
            // check for valid character
            if (((((ANVIL__character*)suffix.start)[i - 1] >= '0' && ((ANVIL__character*)suffix.start)[i - 1] <= '9') || ((ANVIL__character*)suffix.start)[i - 1] == '_') == ANVIL__bt__false) {
                // invalid character
                *value = ANVIL__define__null_address;

                return ANVIL__bt__false;
            }

            // if calculable character
            if (((ANVIL__character*)suffix.start)[i - 1] != '_') {
                // add value
                *value += ANVIL__calculate__exponent(10, digit) * (((ANVIL__character*)suffix.start)[i - 1] - '0');

                // next digit power
                digit++;
            }
        }
    }

    return ANVIL__bt__true;
}

// translate character to hexadecimal
ANVIL__cell_integer_value ANVIL__translate__character_to_hexadecimal(ANVIL__character character, ANVIL__bt* invalid_character) {
    // set character as valid
    *invalid_character = ANVIL__bt__false;

    // translate character
    if (character >= '0' && character <= '9') {
        return character - '0';
    } else if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    } else if (character >= 'A' && character <= 'F') {
        return character - 'A' + 10;
    } else {
        // invalid character
        *invalid_character = ANVIL__bt__true;
    }

    return ANVIL__define__null_address;
}

// translate string to hexedecimal
ANVIL__bt ANVIL__translate__string_to_hexedecimal(ANVIL__buffer string, ANVIL__cell_integer_value* value) {
    ANVIL__buffer prefix = ANVIL__open__buffer_from_string((u8*)"forge.hexadecimal.", ANVIL__bt__false, ANVIL__bt__false);
    ANVIL__buffer suffix;
    ANVIL__buffer current;
    ANVIL__bt invalid_character;
    ANVIL__cell_integer_value hex_digit;

    // check for prefix
    if (ANVIL__calculate__buffer_starts_with_buffer(string, prefix) == ANVIL__bt__false) {
        // not a hexadecimal literal
        *value = ANVIL__define__null_address;

        return ANVIL__bt__false;
    }

    // create suffix
    suffix = ANVIL__create__buffer(string.start + ANVIL__calculate__buffer_length(prefix), string.end);

    // create current
    current = suffix;

    // setup value
    *value = ANVIL__define__null_address;

    // translate number
    while (ANVIL__check__current_within_range(current)) {
        // check separator
        if (*(ANVIL__character*)current.start == '_') {
            // skip
            current.start += sizeof(ANVIL__character);

            continue;
        }
        
        // check digit
        hex_digit = ANVIL__translate__character_to_hexadecimal(*(ANVIL__character*)current.start, &invalid_character);
        if (invalid_character == ANVIL__bt__true) {
            // invalid digit, invalid hex string
            *value = ANVIL__define__null_address;

            return ANVIL__bt__false;
        }

        // append digit
        *value = (*value) << 4;
        *value = (*value) & (~15);
        *value = (*value) + hex_digit;

        // next character
        current.start += sizeof(ANVIL__character);
    }

    return ANVIL__bt__true;
}

// setup null argument
ANVIL__parsling_argument ANVIL__create_null__parsling_argument() {
    return ANVIL__create__parsling_argument(ANVIL__pat__invalid, ANVIL__create_null__name(), 0);
}

// statement type
typedef enum ANVIL__stt {
    ANVIL__stt__invalid,
    ANVIL__stt__abstraction_call,
    ANVIL__stt__abstraction_header,
    ANVIL__stt__offset,

    // count
    ANVIL__stt__COUNT,
} ANVIL__stt;

// one statement
typedef struct ANVIL__parsling_statement {
    ANVIL__stt type;

    // name
    ANVIL__parsling_argument name;

    // abstraction call data
    ANVIL__list inputs; // ANVIL__parsling_argument
    ANVIL__list outputs; // ANVIL__parsling_argument

    // metadata
    ANVIL__input_count input_count;
    ANVIL__output_count output_count;
} ANVIL__parsling_statement;

// create a custom statement
ANVIL__parsling_statement ANVIL__create__parsling_statement(ANVIL__stt type, ANVIL__parsling_argument name, ANVIL__list inputs, ANVIL__list outputs, ANVIL__input_count input_count, ANVIL__output_count output_count) {
    ANVIL__parsling_statement output;

    // setup output
    output.type = type;
    output.name = name;
    output.inputs = inputs;
    output.outputs = outputs;
    output.input_count = input_count;
    output.output_count = output_count;

    return output;
}

// create a null statement
ANVIL__parsling_statement ANVIL__create_null__parsling_statement() {
    // return empty
    return ANVIL__create__parsling_statement(ANVIL__stt__invalid, ANVIL__create_null__parsling_argument(), ANVIL__create_null__list(), ANVIL__create_null__list(), 0, 0);
}

// one abstraction
typedef struct ANVIL__parsling_abstraction {
    ANVIL__parsling_statement header;
    ANVIL__list statements; // ANVIL__parsling_statement
} ANVIL__parsling_abstraction;

// create a custom abstraction
ANVIL__parsling_abstraction ANVIL__create__parsling_abstraction(ANVIL__parsling_statement header, ANVIL__list statements) {
    ANVIL__parsling_abstraction output;

    // setup output
    output.header = header;
    output.statements = statements;

    return output;
}

// create a null abstraction
ANVIL__parsling_abstraction ANVIL__create_null__parsling_abstraction() {
    // return empty
    return ANVIL__create__parsling_abstraction(ANVIL__create_null__parsling_statement(), ANVIL__create_null__list());
}

// one program
typedef struct ANVIL__parsling_program {
    ANVIL__list abstractions; // ANVIL__parsling_abstraction
} ANVIL__parsling_program;

// create a custom program
ANVIL__parsling_program ANVIL__create__parsling_program(ANVIL__list abstractions) {
    ANVIL__parsling_program output;

    // setup output
    output.abstractions = abstractions;

    return output;
}

// create a null program
ANVIL__parsling_program ANVIL__create_null__parsling_program() {
    // return empty
    return ANVIL__create__parsling_program(ANVIL__create_null__list());
}

// append parsling argument
void ANVIL__append__parsling_argument(ANVIL__list* list, ANVIL__parsling_argument data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_argument), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_argument*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_argument);

    return;
}

// append parsling statement
void ANVIL__append__parsling_statement(ANVIL__list* list, ANVIL__parsling_statement data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_statement), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_statement*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_statement);

    return;
}

// append parsling abstraction
void ANVIL__append__parsling_abstraction(ANVIL__list* list, ANVIL__parsling_abstraction data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_abstraction), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_abstraction*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_abstraction);

    return;
}

// append parsling program
void ANVIL__append__parsling_program(ANVIL__list* list, ANVIL__parsling_program data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_program), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_program*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_program);

    return;
}

// close statement
void ANVIL__close__parsling_statement(ANVIL__parsling_statement statement) {
    // close io
    if (ANVIL__check__empty_list(statement.inputs) == ANVIL__bt__false) {
        ANVIL__close__list(statement.inputs);
    }
    if (ANVIL__check__empty_list(statement.outputs) == ANVIL__bt__false) {
        ANVIL__close__list(statement.outputs);
    }

    return;
}

// close statements
void ANVIL__close__parsling_statements(ANVIL__list list) {
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&list);

    // clean up each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // clean up statement
        ANVIL__close__parsling_statement(*(ANVIL__parsling_statement*)current_statement.start);

        // next statement
        current_statement.start += sizeof(ANVIL__parsling_statement);
    }

    // clean up statements buffer
    ANVIL__close__list(list);

    return;
}

// close abstraction
void ANVIL__close__parsling_abstraction(ANVIL__parsling_abstraction abstraction) {
    // close header
    ANVIL__close__parsling_statement(abstraction.header);
    
    // close statements
    if (ANVIL__check__empty_list(abstraction.statements) == ANVIL__bt__false) {
        ANVIL__close__parsling_statements(abstraction.statements);
    }

    return;
}

// close parsling program
void ANVIL__close__parsling_program(ANVIL__parsling_program program) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&(program.abstractions));

    // free each abstraction
    while (ANVIL__check__current_within_range(current)) {
        // free abstraction
        ANVIL__close__parsling_abstraction(*(ANVIL__parsling_abstraction*)current.start);

        // next abstraction
        current.start += sizeof(ANVIL__parsling_abstraction);
    }

    // free the abstraction list
    ANVIL__close__list(program.abstractions);

    return;
}

// create a name from a lexing
ANVIL__name ANVIL__create__name_from_lexling_current(ANVIL__current lexling_current) {
    ANVIL__name output;

    // setup
    output.lexling = ANVIL__read__lexling_from_current(lexling_current);

    return output;
}

// advance the current buffer by N number of lexlings
void ANVIL__advance__lexling_current(ANVIL__current* current, ANVIL__lexling_index lexling_count) {
    // advance the pointer
    (*current).start += sizeof(ANVIL__lexling) * lexling_count;

    return;
}

// revert lexling current by N number of lexlings
void ANVIL__revert__lexling_current(ANVIL__current* current, ANVIL__lexling_index lexling_count) {
    (*current).start -= sizeof(ANVIL__lexling) * lexling_count;

    return;
}

// parse arguments
ANVIL__list ANVIL__parse__arguments(ANVIL__current* current, ANVIL__io_count* count, ANVIL__bt is_header, ANVIL__error* error) {
    ANVIL__list output;
    ANVIL__cell_integer_value value = 0;

    // init count
    *count = 0;

    // open names list
    output = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 8, error);

    // check for error
    if (ANVIL__check__error_occured(error)) {
        return output;
    }
    
    // check for opening parenthesis
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__left_parenthesis) {
        // next lexling
        ANVIL__advance__lexling_current(current, 1);
    // not found, error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: Arguments is missing opening parenthesis.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    // get arguments
    while (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type != ANVIL__lt__right_parenthesis) {
        ANVIL__parsling_argument argument = ANVIL__create_null__parsling_argument();

        // check type
        // is variable / literal
        if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
            // determine value / is variable
            if (ANVIL__translate__string_to_boolean(ANVIL__read__lexling_from_current(*current).value, &value)) {
                argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__boolean, ANVIL__create__name_from_lexling_current(*current), value);
            } else if (ANVIL__translate__string_to_binary(ANVIL__read__lexling_from_current(*current).value, &value)) {
                argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__binary, ANVIL__create__name_from_lexling_current(*current), value);
            } else if (ANVIL__translate__string_to_integer(ANVIL__read__lexling_from_current(*current).value, &value)) {
                argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__integer, ANVIL__create__name_from_lexling_current(*current), value);
            } else if (ANVIL__translate__string_to_hexedecimal(ANVIL__read__lexling_from_current(*current).value, &value)) {
                argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__hexadecimal, ANVIL__create__name_from_lexling_current(*current), value);
            } else {
                // must be a variable
                argument = ANVIL__create__parsling_argument(ANVIL__pat__variable, ANVIL__create__name_from_lexling_current(*current), 0);
            }
        // offset
        } else if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__at) {
            // advance current past at
            ANVIL__advance__lexling_current(current, 1);

            // if correct type
            if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
                // get name
                argument = ANVIL__create__parsling_argument(ANVIL__pat__offset, ANVIL__create__name_from_lexling_current(*current), 0);
            // error
            } else {
                *error = ANVIL__open__error("Parse Error: Offset is missing name.", ANVIL__read__lexling_from_current(*current).location);

                return output;
            }
        // flag
        } else if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__hashtag) {
            // advance current past hashtag
            ANVIL__advance__lexling_current(current, 1);

            // if correct type
            if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
                // get name
                argument = ANVIL__create__parsling_argument(ANVIL__pat__flag, ANVIL__create__name_from_lexling_current(*current), 0);
            // error
            } else {
                *error = ANVIL__open__error("Parse Error: Flag is missing name.", ANVIL__read__lexling_from_current(*current).location);

                return output;
            }
        // string literal
        } else if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__string_literal) {
            // get argument
            argument = ANVIL__create__parsling_argument(ANVIL__pat__literal__string, ANVIL__create__name_from_lexling_current(*current), 0);
        // error
        } else {
            *error = ANVIL__open__error("Parse Error: Unrecognized argument type.", ANVIL__read__lexling_from_current(*current).location);

            return output;
        }

        // check argument for variable only
        if (is_header == ANVIL__bt__true && argument.type != ANVIL__pat__variable) {
            // set error
            *error = ANVIL__open__error("Parse Error: A non-variable argument was detected in a header.", ANVIL__read__lexling_from_current(*current).location);

            return output;
        }

        // append argument
        ANVIL__append__parsling_argument(&output, argument, error);
        
        // increment count
        *count = *count + 1;

        // check for error
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // next current
        ANVIL__advance__lexling_current(current, 1);
    }

    // check for closing parenthesis
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__right_parenthesis) {
        // next lexling
        ANVIL__advance__lexling_current(current, 1);
    // not found, error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: Arguments is missing closing parenthesis.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    return output;
}

// parse one statement
ANVIL__parsling_statement ANVIL__parse__statement(ANVIL__current* current, ANVIL__bt is_header, ANVIL__error* error) {
    ANVIL__parsling_statement output = ANVIL__create_null__parsling_statement();
    ANVIL__input_count input_count;
    ANVIL__output_count output_count;

    // check for offset
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__at) {
        // advance current
        ANVIL__advance__lexling_current(current, 1);

        // check for offset name
        if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
            // set name
            output.name = ANVIL__create__parsling_argument(ANVIL__pat__offset, ANVIL__create__name_from_lexling_current(*current), 0);

            // advance current
            ANVIL__advance__lexling_current(current, 1);
        // invalid syntax
        } else {
            // set error
            *error = ANVIL__open__error("Parse Error: Offset statement name is an invalid lexling.", ANVIL__read__lexling_from_current(*current).location);

            return output;
        }

        // null initialize unused data
        output.inputs = ANVIL__create_null__list();
        output.outputs = ANVIL__create_null__list();

        // setup type
        output.type = ANVIL__stt__offset;
    // is an abstraction call
    } else if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
        // get name
        output.name = ANVIL__create__parsling_argument(ANVIL__pat__offset, ANVIL__create__name_from_lexling_current(*current), 0);

        // advance current
        ANVIL__advance__lexling_current(current, 1);

        // get inputs
        output.inputs = ANVIL__parse__arguments(current, &input_count, is_header, error);
        output.input_count = input_count;

        // check for error
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // get outputs
        output.outputs = ANVIL__parse__arguments(current, &output_count, is_header, error);
        output.output_count = output_count;

        // check for error
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // set type
        if (is_header == ANVIL__bt__true) {
            output.type = ANVIL__stt__abstraction_header;
        } else {
            output.type = ANVIL__stt__abstraction_call;
        }
    // error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: Unrecognized statement type.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    return output;
}

// parse an abstraction
ANVIL__parsling_abstraction ANVIL__parse__abstraction(ANVIL__current* current, ANVIL__error* error) {
    ANVIL__parsling_abstraction output = ANVIL__create_null__parsling_abstraction();

    // check for eof
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__end_of_file) {
        return output;
    }

    // parse header
    output.header = ANVIL__parse__statement(current, ANVIL__bt__true, error);
    
    // check for error
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // check for equals sign
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__equals) {
        // no saving data necessary, next lexling
        ANVIL__advance__lexling_current(current, 1);
    // error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: An abstraction definition has an equals sign missing.", ANVIL__read__lexling_from_current(*current).location);

        // quit
        return output;
    }

    // check for scope opener
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__left_curly_bracket) {
        // advance current
        ANVIL__advance__lexling_current(current, 1);
    // scope opener not found, error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: Scope is missing left curly bracket.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    // parse statements
    // open statements list
    output.statements = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_statement) * 16, error);
    
    // get statements
    while (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type != ANVIL__lt__right_curly_bracket) {
        // parse statement
        ANVIL__parsling_statement statement = ANVIL__parse__statement(current, ANVIL__bt__false, error);

        // add statement
        ANVIL__append__parsling_statement(&output.statements, statement, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
    }

    // check for scope closer
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__right_curly_bracket) {
        // advance current
        ANVIL__advance__lexling_current(current, 1);
    // scope opener not found, error
    } else {
        // setup error
        *error = ANVIL__open__error("Parse Error: Scope is missing right curly bracket.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    return output;
}

// parse a file (program)
ANVIL__parsling_program ANVIL__parse__program(ANVIL__lexlings lexlings, ANVIL__error* error) {
    ANVIL__parsling_program output;
    ANVIL__parsling_abstraction temp;
    ANVIL__current current;

    // setup current
    current = ANVIL__calculate__current_from_list_filled_index(&lexlings.data);

    // open the abstraction list
    output.abstractions = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_abstraction) * 64, error);

    // check for memory error
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // parse abstractions
    while (ANVIL__check__current_within_range(current)) {
        // if end of file
        if (ANVIL__read__lexling_from_current(current).type == ANVIL__lt__end_of_file) {
            // finished parsing
            goto quit;
        }
        
        // parse abstraction
        temp = ANVIL__parse__abstraction(&current, error);

        // append abstraction
        ANVIL__append__parsling_abstraction(&(output.abstractions), temp, error);

        // check for error
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            goto quit;
        }
    }

    // quit
    quit:

    return output;
}

ANVIL__buffer ANVIL__convert__parsed_argument_type_to_string_buffer(ANVIL__pat argument_type) {
    return ANVIL__open__buffer_from_string((u8*)(ANVIL__global__argument_type_name_strings[argument_type]), ANVIL__bt__false, ANVIL__bt__false);
}

// print argument
void ANVIL__print__parsling_argument(ANVIL__parsling_argument argument) {
    // print type
    printf("[");
    ANVIL__print__buffer(ANVIL__convert__parsed_argument_type_to_string_buffer(argument.type));
    printf("]");

    // print data
    if (argument.type == ANVIL__pat__variable || ANVIL__pat__variable__input || ANVIL__pat__variable__output || ANVIL__pat__variable__body || ANVIL__pat__variable__predefined || ANVIL__pat__offset || ANVIL__pat__flag || ANVIL__pat__literal__string) {
        ANVIL__print__buffer(argument.text.lexling.value);
    } else if (argument.type == ANVIL__pat__literal__boolean || ANVIL__pat__literal__binary || ANVIL__pat__literal__hexadecimal) {
        ANVIL__print__buffer(argument.text.lexling.value);
        printf("[%lu]", argument.value);
    } else if (argument.type == ANVIL__pat__literal__integer) {
        ANVIL__print__buffer(argument.text.lexling.value);
        printf("[%lu, %li]", argument.value, argument.value);
    }

    return;
}

// print arguments
void ANVIL__print__parsling_arguments(ANVIL__list* arguments) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(arguments);

    // print opener
    printf("(");

    // for each name
    while (ANVIL__check__current_within_range(current)) {
        // print separator
        if (current.start != (*arguments).buffer.start) {
            // print
            printf(" ");
        }

        // print argument
        ANVIL__print__parsling_argument(*(ANVIL__parsling_argument*)current.start);

        // next buffer
        current.start += sizeof(ANVIL__parsling_argument);
    }

    // print closer
    printf(")");

    return;
}

// print statement
void ANVIL__print__parsed_statement(ANVIL__parsling_statement statement) {
    // print statement
    if (statement.type == ANVIL__stt__offset) {
        // print offset information
        printf("@");
        ANVIL__print__buffer(statement.name.text.lexling.value);
    } else if (statement.type == ANVIL__stt__abstraction_call || statement.type == ANVIL__stt__abstraction_header) {
        // print statement
        ANVIL__print__buffer(statement.name.text.lexling.value);

        // print inputs
        ANVIL__print__parsling_arguments(&statement.inputs);

        // print outputs
        ANVIL__print__parsling_arguments(&statement.outputs);
    }

    return;
}

// print an abstraction
void ANVIL__print__parsed_abstraction(ANVIL__parsling_abstraction abstraction) {
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&abstraction.statements);

    // print header
    printf("\tAbstraction: ");
    if (abstraction.header.type != ANVIL__stt__invalid) {
        ANVIL__print__parsed_statement(abstraction.header);
    }

    // new line for statements
    printf("\n");

    // print statements
    if (ANVIL__check__empty_list(abstraction.statements) == ANVIL__bt__false) {
        // print each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // print formatting
            printf("\t\t");

            // print statement
            ANVIL__print__parsed_statement(*(ANVIL__parsling_statement*)current_statement.start);

            // print new line
            printf("\n");

            // advance current
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    return;
}

// print a program
void ANVIL__print__parsed_program(ANVIL__parsling_program program) {
    ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&(program.abstractions));
    
    // print header
    printf("Parsed Program:\n");

    // print each function
    while (ANVIL__check__current_within_range(current_abstraction)) {
        // print function
        ANVIL__print__parsed_abstraction(*(ANVIL__parsling_abstraction*)current_abstraction.start);

        // advance current
        current_abstraction.start += sizeof(ANVIL__parsling_abstraction);
    }

    return;
}

/* Accounting */
// predefined variable type
typedef enum ANVIL__pvt {
    // variables
    ANVIL__pvt__error_code,
    ANVIL__pvt__constant__character_byte_size,
    ANVIL__pvt__constant__character_bit_size,
    ANVIL__pvt__constant__bits_in_byte,
    ANVIL__pvt__constant__cell_byte_size,
    ANVIL__pvt__constant__cell_bit_size,
    ANVIL__pvt__constant__true,
    ANVIL__pvt__constant__false,
    ANVIL__pvt__constant__0,
    ANVIL__pvt__constant__1,
    ANVIL__pvt__constant__2,
    ANVIL__pvt__constant__4,
    ANVIL__pvt__constant__8,
    ANVIL__pvt__constant__16,
    ANVIL__pvt__constant__24,
    ANVIL__pvt__constant__32,
    ANVIL__pvt__constant__40,
    ANVIL__pvt__constant__48,
    ANVIL__pvt__constant__56,
    ANVIL__pvt__constant__64,
    ANVIL__pvt__constant__input_start,
    ANVIL__pvt__constant__input_end,
    ANVIL__pvt__constant__output_start,
    ANVIL__pvt__constant__output_end,
    ANVIL__pvt__stack_start,
    ANVIL__pvt__stack_current,
    ANVIL__pvt__stack_end,

    // count
    ANVIL__pvt__COUNT,
} ANVIL__pvt;

// predefined flag type
typedef enum ANVIL__pft {
    // flags
    ANVIL__pft__always_run,
    ANVIL__pft__never_run,
    ANVIL__pft__temporary,

    // count
    ANVIL__pft__COUNT,
} ANVIL__pft;

// accountling argument
typedef struct ANVIL__accountling_argument {
    ANVIL__pat type;
    ANVIL__accountling_index index;
    ANVIL__parsling_argument value;
} ANVIL__accountling_argument;

// create accountling argument
ANVIL__accountling_argument ANVIL__create__accountling_argument(ANVIL__pat type, ANVIL__accountling_index index, ANVIL__parsling_argument value) {
    ANVIL__accountling_argument output;

    output.type = type;
    output.index = index;
    output.value = value;

    return output;
}

// create null accountling argument
ANVIL__accountling_argument ANVIL__create_null__accountling_argument() {
    return ANVIL__create__accountling_argument(ANVIL__pat__invalid, ANVIL__define__null_call_ID, ANVIL__create_null__parsling_argument());
}

// append accountling argument
void ANVIL__append__accountling_argument(ANVIL__list* list, ANVIL__accountling_argument data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__accountling_argument), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__accountling_argument*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__accountling_argument);

    return;
}

// blueprintling
typedef ANVIL__u64 ANVIL__blueprintling;

// accountling call type
typedef enum ANVIL__act {
    // start
    ANVIL__act__START = 0,

    // acts
    // sets
    ANVIL__act__set__boolean = ANVIL__act__START,
    ANVIL__act__set__binary,
    ANVIL__act__set__integer,
    ANVIL__act__set__hexadecimal,
    ANVIL__act__set__offset,
    ANVIL__act__set__flag_ID,
    ANVIL__act__set__string,

    // io
    ANVIL__act__io__cell_to_address,
    ANVIL__act__io__address_to_cell,
    ANVIL__act__io__file_to_buffer,
    ANVIL__act__io__buffer_to_file,

    // copy
    ANVIL__act__copy__cell,
    ANVIL__act__copy__buffer,

    // memory
    ANVIL__act__memory__request_memory,
    ANVIL__act__memory__return_memory,

    // buffers
    ANVIL__act__buffer__calculate_length,

    // casts
    ANVIL__act__cast__cell_to_unsigned_integer_string,

    // prints
    ANVIL__act__print__binary,
    ANVIL__act__print__signed_integer,
    ANVIL__act__print__unsigned_integer,
    ANVIL__act__print__character,
    ANVIL__act__print__buffer_as_string,

    // integers
    ANVIL__act__integer__add,
    ANVIL__act__integer__subtract,
    ANVIL__act__integer__multiply,
    ANVIL__act__integer__divide,
    ANVIL__act__integer__modulous,
    ANVIL__act__integer__within_range,

    // binary
    ANVIL__act__binary__or,
    ANVIL__act__binary__invert,
    ANVIL__act__binary__and,
    ANVIL__act__binary__xor,
    ANVIL__act__binary__shift_higher,
    ANVIL__act__binary__shift_lower,
    ANVIL__act__binary__overwrite,

    // flags
    ANVIL__act__flag__get,
    ANVIL__act__flag__set,
    ANVIL__act__flag__invert,
    ANVIL__act__flag__or,
    ANVIL__act__flag__and,
    ANVIL__act__flag__xor,

    // jumps
    ANVIL__act__jump,

    // rtcg
    ANVIL__act__open_context,
    ANVIL__act__compile,
    ANVIL__act__run,

    // etc
    ANVIL__act__reset_error_code_cell,
    ANVIL__act__set_program_outputs,

    // end
    ANVIL__act__END,

    // user defined
    ANVIL__act__user_defined = ANVIL__act__END,

    // invalid
    ANVIL__act__invalid,

    // count
    ANVIL__act__COUNT = ANVIL__act__END - ANVIL__act__START,
} ANVIL__act;

// calculate user defined call ID
ANVIL__abstraction_index ANVIL__calculate__abstraction_index_from_call_index(ANVIL__call_index call_index) {
    return call_index - ANVIL__act__user_defined;
}

// accountling variable type (all predefined variables)
typedef enum ANVIL__avt {
    // start
    ANVIL__avt__START = 0,

    // avts
    ANVIL__avt__invalid = ANVIL__avt__START,
    ANVIL__avt__error_code,

    // end
    ANVIL__avt__END,

    // user defined
    ANVIL__avt__user_defined = ANVIL__avt__END,

    // count
    ANVIL__avt__COUNT = ANVIL__avt__END - ANVIL__avt__START,
} ANVIL__avt;

// accountling header
typedef struct ANVIL__accountling_abstraction_header {
    ANVIL__parsling_statement header;
    ANVIL__call_index call_index;
} ANVIL__accountling_abstraction_header;

// create custom accountling abstraction header
ANVIL__accountling_abstraction_header ANVIL__create__accountling_abstraction_header(ANVIL__parsling_statement header, ANVIL__call_index call_index) {
    ANVIL__accountling_abstraction_header output;

    output.header = header;
    output.call_index = call_index;

    return output;
}

// create null accountling abstraction header
ANVIL__accountling_abstraction_header ANVIL__create_null__accountling_abstraction_header() {
    return ANVIL__create__accountling_abstraction_header(ANVIL__create_null__parsling_statement(), ANVIL__act__invalid);
}

// close accountling abstraction header
void ANVIL__close__accountling_abstraction_header(ANVIL__accountling_abstraction_header header) {
    // close header
    ANVIL__close__parsling_statement(header.header);

    return;
}

// accountling statement
typedef struct ANVIL__accountling_statement {
    // type
    ANVIL__stt type;

    // call
    ANVIL__accountling_abstraction_header header;
    ANVIL__list inputs; // ANVIL__accountling_argument
    ANVIL__list outputs; // ANVIL__accountling_argument

    // offset
    ANVIL__offset_index offset_ID;
} ANVIL__accountling_statement;

// setup custom accountling statement
ANVIL__accountling_statement ANVIL__create__accountling_statement(ANVIL__stt type, ANVIL__accountling_abstraction_header header, ANVIL__list inputs, ANVIL__list outputs, ANVIL__offset_index offset_ID) {
    ANVIL__accountling_statement output;

    output.type = type;
    output.header = header;
    output.inputs = inputs;
    output.outputs = outputs;
    output.offset_ID = offset_ID;

    return output;
}

// create null accountling statement
ANVIL__accountling_statement ANVIL__create_null__accountling_statement() {
    return ANVIL__create__accountling_statement(ANVIL__stt__invalid, ANVIL__create_null__accountling_abstraction_header(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__define__null_offset_ID);
}

// close statement
void ANVIL__close__accountling_statement(ANVIL__accountling_statement statement) {
    // close io
    if (statement.type == ANVIL__stt__abstraction_call) {
        ANVIL__close__list(statement.inputs);
        ANVIL__close__list(statement.outputs);
    }

    return;
}

// get statement input by index
ANVIL__accountling_argument ANVIL__get__abstractling_statement_argument_by_index(ANVIL__list list, ANVIL__argument_index index) {
    return ((ANVIL__accountling_argument*)list.buffer.start)[index];
}

// accountling abstraction
typedef struct ANVIL__accountling_abstraction {
    ANVIL__parsling_statement header;
    ANVIL__list* predefined_variables; // ANVIL__parsling_argument
    ANVIL__list* predefined_flags; // ANVIL__parsling_argument
    ANVIL__list strings; // ANVIL__parsling_argument
    ANVIL__list converted_strings; // ANVIL__buffer
    ANVIL__list inputs; // ANVIL__parsling_argument
    ANVIL__list outputs; // ANVIL__parsling_argument
    ANVIL__list variables; // ANVIL__parsling_argument
    ANVIL__list offsets; // ANVIL__parsling_argument
    ANVIL__list flags; // ANVIL__parsling_argument
    ANVIL__list statements; // ANVIL__accountling_statement
} ANVIL__accountling_abstraction;

// create custom accountling abstraction
ANVIL__accountling_abstraction ANVIL__create__accountling_abstraction(ANVIL__parsling_statement header, ANVIL__list* predefined_variables, ANVIL__list* predefined_flags, ANVIL__list strings, ANVIL__list converted_strings, ANVIL__list inputs, ANVIL__list outputs, ANVIL__list variables, ANVIL__list offsets, ANVIL__list flags, ANVIL__list statements) {
    ANVIL__accountling_abstraction output;

    output.header = header;
    output.predefined_variables = predefined_variables;
    output.predefined_flags = predefined_flags;
    output.strings = strings;
    output.converted_strings = converted_strings;
    output.inputs = inputs;
    output.outputs = outputs;
    output.variables = variables;
    output.offsets = offsets;
    output.flags = flags;
    output.statements = statements;

    return output;
}

// create null accountling abstraction
ANVIL__accountling_abstraction ANVIL__create_null__accountling_abstraction() {
    return ANVIL__create__accountling_abstraction(ANVIL__create_null__parsling_statement(), ANVIL__define__null_address, ANVIL__define__null_address, ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list());
}

// append accountling statement
void ANVIL__append__accountling_statement(ANVIL__list* list, ANVIL__accountling_statement data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__accountling_statement), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__accountling_statement*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__accountling_statement);

    return;
}

// append accountling abstraction
void ANVIL__append__accountling_abstraction(ANVIL__list* list, ANVIL__accountling_abstraction data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__accountling_abstraction), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__accountling_abstraction*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__accountling_abstraction);

    return;
}

// close statements
void ANVIL__close__accountling_statements(ANVIL__list statements) {
    // check for empty statements
    if (ANVIL__check__empty_list(statements)) {
        // no need to free anything
        return;
    }

    // setup current
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&statements);

    // for each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // close statement
        ANVIL__close__accountling_statement(*(ANVIL__accountling_statement*)current_statement.start);

        // next statement
        current_statement.start += sizeof(ANVIL__accountling_statement);
    }

    // close lists
    ANVIL__close__list(statements);

    return;
}

// close accountling abstraction
void ANVIL__close__accountling_abstraction(ANVIL__accountling_abstraction abstraction) {
    // close lists
    if (ANVIL__check__empty_list(abstraction.inputs) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.inputs);
    }
    if (ANVIL__check__empty_list(abstraction.outputs) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.outputs);
    }
    if (ANVIL__check__empty_list(abstraction.variables) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.variables);
    }
    if (ANVIL__check__empty_list(abstraction.offsets) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.offsets);
    }
    if (ANVIL__check__empty_list(abstraction.flags) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.flags);
    }
    if (ANVIL__check__empty_list(abstraction.strings) == ANVIL__bt__false) {
        ANVIL__close__list(abstraction.strings);
    }
    if (ANVIL__check__empty_list(abstraction.converted_strings) == ANVIL__bt__false) {
        ANVIL__current current_buffer = ANVIL__calculate__current_from_list_filled_index(&abstraction.converted_strings);

        // for each buffer
        while (ANVIL__check__current_within_range(current_buffer)) {
            // free buffer
            ANVIL__close__buffer(*(ANVIL__buffer*)current_buffer.start);

            // next buffer
            current_buffer.start += sizeof(ANVIL__buffer);
        }

        // close list
        ANVIL__close__list(abstraction.converted_strings);
    }

    // close statements
    ANVIL__close__accountling_statements(abstraction.statements);

    return;
}

// accountling blueprint type
typedef enum ANVIL__abt {
    // start
    ANVIL__abt__START = ANVIL__act__END,

    // types
    ANVIL__abt__define_call = ANVIL__abt__START,
    ANVIL__abt__end_of_blueprint,

    // end
    ANVIL__abt__END,

    // count
    ANVIL__abt__COUNT = ANVIL__abt__END - ANVIL__abt__START,
} ANVIL__abt;

// blueprint name index type
typedef enum ANVIL__bnit {
    // names
    ANVIL__bnit__set,
    ANVIL__bnit__print__signed_integer,
    ANVIL__bnit__print__unsigned_integer,
    ANVIL__bnit__print__character,
    ANVIL__bnit__print__buffer_as_string,
    ANVIL__bnit__print__binary,
    ANVIL__bnit__io__cell_to_address,
    ANVIL__bnit__io__address_to_cell,
    ANVIL__bnit__io__file_to_buffer,
    ANVIL__bnit__io__buffer_to_file,
    ANVIL__bnit__copy__cell,
    ANVIL__bnit__copy__buffer,
    ANVIL__bnit__memory__request_memory,
    ANVIL__bnit__memory__return_memory,
    ANVIL__bnit__buffer__calculate_length,
    ANVIL__bnit__cast__cell_to_unsigned_integer_string,
    ANVIL__bnit__integer_add,
    ANVIL__bnit__integer_subtract,
    ANVIL__bnit__integer_multiply,
    ANVIL__bnit__integer_divide,
    ANVIL__bnit__integer_modulous,
    ANVIL__bnit__integer_within_range,
    ANVIL__bnit__binary__or,
    ANVIL__bnit__binary__invert,
    ANVIL__bnit__binary__and,
    ANVIL__bnit__binary__xor,
    ANVIL__bnit__binary__shift_higher,
    ANVIL__bnit__binary__shift_lower,
    ANVIL__bnit__binary__overwrite,
    ANVIL__bnit__flag__get,
    ANVIL__bnit__flag__set,
    ANVIL__bnit__flag__invert,
    ANVIL__bnit__flag__or,
    ANVIL__bnit__flag__and,
    ANVIL__bnit__flag__xor,
    ANVIL__bnit__jump,
    ANVIL__bnit__open_context,
    ANVIL__bnit__compile,
    ANVIL__bnit__run,
    ANVIL__bnit__reset_error_code,
    ANVIL__bnit__set_program_outputs,

    // stats
    ANVIL__bnit__COUNT,
} ANVIL__bnit;

// append accountling abstraction header
void ANVIL__append__accountling_abstraction_header(ANVIL__list* list, ANVIL__accountling_abstraction_header data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__accountling_abstraction_header), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__accountling_abstraction_header*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__accountling_abstraction_header);

    return;
}

// get and validate one header
ANVIL__accountling_abstraction_header ANVIL__account__get_abstraction_header(ANVIL__parsling_abstraction abstraction, ANVIL__call_index abstraction_call_ID, ANVIL__error* error) {
    ANVIL__accountling_abstraction_header output;

    output.header = abstraction.header;
    output.call_index = ANVIL__act__user_defined + abstraction_call_ID;

    // validate inputs
    for (ANVIL__length i = 0; i < output.header.input_count; i++) {
        // get argument
        ANVIL__parsling_argument argument = ((ANVIL__parsling_argument*)output.header.inputs.buffer.start)[i];

        // check argument validity
        if (argument.type != ANVIL__pat__variable) {
            *error = ANVIL__open__error("Accounting Error: An abstraction input header argument contains illegal argument type.", argument.text.lexling.location);

            return output;
        }
    }

    // validate outputs
    for (ANVIL__length i = 0; i < output.header.output_count; i++) {
        // get argument
        ANVIL__parsling_argument argument = ((ANVIL__parsling_argument*)output.header.outputs.buffer.start)[i];

        // check argument validity
        if (argument.type != ANVIL__pat__variable) {
            *error = ANVIL__open__error("Accounting Error: An abstraction output header argument contains illegal argument type.", argument.text.lexling.location);

            return output;
        }
    }

    return output;
}

// calculate blueprint list size
ANVIL__header_index ANVIL__calculate__call_blueprint_entry_count(ANVIL__list call_blueprint) {
    return call_blueprint.filled_index / sizeof(ANVIL__accountling_abstraction_header);
}

// check if argument is in variable category
ANVIL__bt ANVIL__check__argument_is_variable_type(ANVIL__pat argument_type) {
    return (argument_type == ANVIL__pat__variable) || (argument_type == ANVIL__pat__variable__body) || (argument_type == ANVIL__pat__variable__input) || (argument_type == ANVIL__pat__variable__output) || (argument_type == ANVIL__pat__variable__predefined);
}

// check if argument is in flag category
ANVIL__bt ANVIL__check__argument_is_flag_type(ANVIL__pat argument_type) {
    return (argument_type == ANVIL__pat__flag) || (argument_type == ANVIL__pat__flag__user_defined) || (argument_type == ANVIL__pat__flag__predefined);
}

// check if an accountling already exists
ANVIL__header_index ANVIL__find__accountling_header_index(ANVIL__list call_blueprint, ANVIL__parsling_statement searching_for) {
    // search the blueprint for the correct header
    // setup current
    ANVIL__current current_header = ANVIL__calculate__current_from_list_filled_index(&call_blueprint);
    ANVIL__header_index header_index = 0;

    // search for match
    while (ANVIL__check__current_within_range(current_header)) {
        // get current header
        ANVIL__accountling_abstraction_header header = *(ANVIL__accountling_abstraction_header*)current_header.start;

        // check if abstraction matches
        if(ANVIL__check__parsling_arguments_have_same_text(header.header.name, searching_for.name)) {
            // check if io is same size
            if ((ANVIL__calculate__lists_have_same_fill_size(&searching_for.inputs, &header.header.inputs) && ANVIL__calculate__lists_have_same_fill_size(&searching_for.outputs, &header.header.outputs)) == ANVIL__bt__false) {
                goto next_header;
            }

            // setup currents for input
            ANVIL__current current_statement_io = ANVIL__calculate__current_from_list_filled_index(&searching_for.inputs);
            ANVIL__current current_header_io = ANVIL__calculate__current_from_list_filled_index(&header.header.inputs);

            // if inputs are not empty
            if (ANVIL__check__empty_buffer(current_statement_io) == ANVIL__bt__false) {
                // check inputs
                while (ANVIL__check__current_within_range(current_statement_io) && ANVIL__check__current_within_range(current_header_io)) {
                    // get types
                    ANVIL__pat statement_io_type = (*(ANVIL__parsling_argument*)current_statement_io.start).type;
                    ANVIL__pat header_io_type = (*(ANVIL__parsling_argument*)current_header_io.start).type;

                    // check types
                    // if type is in variable category
                    if (ANVIL__check__argument_is_variable_type(statement_io_type) && ANVIL__check__argument_is_variable_type(header_io_type)) {
                        // next input
                        goto next_input;
                    } else if (ANVIL__check__argument_is_flag_type(statement_io_type) && ANVIL__check__argument_is_flag_type(header_io_type)) {
                        // next input
                        goto next_input;
                    } else if (statement_io_type != header_io_type) {
                        // not a match
                        goto next_header;
                    }

                    // next input
                    next_input:
                    current_statement_io.start += sizeof(ANVIL__parsling_argument);
                    current_header_io.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // re-setup currents
            current_statement_io = ANVIL__calculate__current_from_list_filled_index(&searching_for.outputs);
            current_header_io = ANVIL__calculate__current_from_list_filled_index(&header.header.outputs);

            // if outputs are not empty
            if (ANVIL__check__empty_buffer(current_statement_io) == ANVIL__bt__false) {
                // check outputs
                while (ANVIL__check__current_within_range(current_statement_io) && ANVIL__check__current_within_range(current_header_io)) {
                    // get types
                    ANVIL__pat statement_io_type = (*(ANVIL__parsling_argument*)current_statement_io.start).type;
                    ANVIL__pat header_io_type = (*(ANVIL__parsling_argument*)current_header_io.start).type;

                    // check types
                    // if type is in variable category
                    if (ANVIL__check__argument_is_variable_type(statement_io_type) && ANVIL__check__argument_is_variable_type(header_io_type)) {
                        // next output
                        goto next_output;
                    } else if (ANVIL__check__argument_is_flag_type(statement_io_type) && ANVIL__check__argument_is_flag_type(header_io_type)) {
                        // next output
                        goto next_output;
                    } else if (statement_io_type != header_io_type) {
                        // not a match
                        goto next_header;
                    }

                    // next output
                    next_output:
                    current_statement_io.start += sizeof(ANVIL__parsling_argument);
                    current_header_io.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // match!
            return header_index;
        }

        // next header
        next_header:
        current_header.start += sizeof(ANVIL__accountling_abstraction_header);
        header_index++;
    }

    // match not found
    return ANVIL__calculate__call_blueprint_entry_count(call_blueprint);
}

// create call blueprint
ANVIL__list ANVIL__generate__call_blueprint(ANVIL__list parsling_programs, ANVIL__error* error) {
    const ANVIL__blueprintling blueprint[] = {
        // sets
        ANVIL__abt__define_call,
            ANVIL__act__set__boolean,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__boolean,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__binary,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__binary,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__integer,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__integer,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__hexadecimal,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__hexadecimal,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__offset,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__offset,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__flag_ID,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__set__string,
            ANVIL__bnit__set,
            1,
            ANVIL__pat__literal__string,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        
        // io
        ANVIL__abt__define_call,
            ANVIL__act__io__cell_to_address,
            ANVIL__bnit__io__cell_to_address,
            3,
            ANVIL__pat__flag,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__io__address_to_cell,
            ANVIL__bnit__io__address_to_cell,
            3,
            ANVIL__pat__flag,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__io__file_to_buffer,
            ANVIL__bnit__io__file_to_buffer,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__io__buffer_to_file,
            ANVIL__bnit__io__buffer_to_file,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        
        // copies
        ANVIL__abt__define_call,
            ANVIL__act__copy__cell,
            ANVIL__bnit__copy__cell,
            1,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__copy__buffer,
            ANVIL__bnit__copy__buffer,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        
        // memory
        ANVIL__abt__define_call,
            ANVIL__act__memory__request_memory,
            ANVIL__bnit__memory__request_memory,
            1,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__memory__return_memory,
            ANVIL__bnit__memory__return_memory,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            0,
        
        // buffers
        ANVIL__abt__define_call,
            ANVIL__act__buffer__calculate_length,
            ANVIL__bnit__buffer__calculate_length,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        
        // casting
        ANVIL__abt__define_call,
            ANVIL__act__cast__cell_to_unsigned_integer_string,
            ANVIL__bnit__cast__cell_to_unsigned_integer_string,
            1,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        
        // printing
        ANVIL__abt__define_call,
            ANVIL__act__print__signed_integer,
            ANVIL__bnit__print__signed_integer,
            1,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__print__unsigned_integer,
            ANVIL__bnit__print__unsigned_integer,
            1,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__print__character,
            ANVIL__bnit__print__character,
            1,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__print__buffer_as_string,
            ANVIL__bnit__print__buffer_as_string,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__print__binary,
            ANVIL__bnit__print__binary,
            1,
            ANVIL__pat__variable,
            0,
        
        // integer operations
        ANVIL__abt__define_call,
            ANVIL__act__integer__add,
            ANVIL__bnit__integer_add,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__subtract,
            ANVIL__bnit__integer_subtract,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__multiply,
            ANVIL__bnit__integer_multiply,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__divide,
            ANVIL__bnit__integer_divide,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__modulous,
            ANVIL__bnit__integer_modulous,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__integer__within_range,
            ANVIL__bnit__integer_within_range,
            4,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        
        // binary operations
        ANVIL__abt__define_call,
            ANVIL__act__binary__or,
            ANVIL__bnit__binary__or,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__invert,
            ANVIL__bnit__binary__invert,
            1,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__and,
            ANVIL__bnit__binary__and,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__xor,
            ANVIL__bnit__binary__xor,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__shift_higher,
            ANVIL__bnit__binary__shift_higher,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__shift_lower,
            ANVIL__bnit__binary__shift_lower,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__binary__overwrite,
            ANVIL__bnit__binary__overwrite,
            3,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__variable,

        // flags
        ANVIL__abt__define_call,
            ANVIL__act__flag__get,
            ANVIL__bnit__flag__get,
            1,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__flag__set,
            ANVIL__bnit__flag__set,
            1,
            ANVIL__pat__variable,
            1,
            ANVIL__pat__flag,
        ANVIL__abt__define_call,
            ANVIL__act__flag__invert,
            ANVIL__bnit__flag__invert,
            1,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        ANVIL__abt__define_call,
            ANVIL__act__flag__or,
            ANVIL__bnit__flag__or,
            2,
            ANVIL__pat__flag,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        ANVIL__abt__define_call,
            ANVIL__act__flag__and,
            ANVIL__bnit__flag__and,
            2,
            ANVIL__pat__flag,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        ANVIL__abt__define_call,
            ANVIL__act__flag__xor,
            ANVIL__bnit__flag__xor,
            2,
            ANVIL__pat__flag,
            ANVIL__pat__flag,
            1,
            ANVIL__pat__flag,
        
        // jumps
        ANVIL__abt__define_call,
            ANVIL__act__jump,
            ANVIL__bnit__jump,
            2,
            ANVIL__pat__flag,
            ANVIL__pat__offset,
            0,

        // run time code generation
        ANVIL__abt__define_call,
            ANVIL__act__open_context,
            ANVIL__bnit__open_context,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__compile,
            ANVIL__bnit__compile,
            3,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            8,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
        ANVIL__abt__define_call,
            ANVIL__act__run,
            ANVIL__bnit__run,
            3,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            0,
        
        // etc
        ANVIL__abt__define_call,
            ANVIL__act__reset_error_code_cell,
            ANVIL__bnit__reset_error_code,
            0,
            0,
        ANVIL__abt__define_call,
            ANVIL__act__set_program_outputs,
            ANVIL__bnit__set_program_outputs,
            2,
            ANVIL__pat__variable,
            ANVIL__pat__variable,
            0,
        
        // end of blueprint
        ANVIL__abt__end_of_blueprint,
    };

    // open output
    ANVIL__list output = ANVIL__open__list_with_error(sizeof(ANVIL__accountling_abstraction_header) * 256, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // setup blueprint current
    ANVIL__current current_blueprintling = ANVIL__create__buffer((ANVIL__address)blueprint, (ANVIL__address)(blueprint + sizeof(blueprint) - 1));

    // create and add predefined headers to list
    while (ANVIL__check__current_within_range(current_blueprintling) && (*(ANVIL__blueprintling*)current_blueprintling.start) == ANVIL__abt__define_call) {
        // check for define call
        if ((*(ANVIL__blueprintling*)current_blueprintling.start) == ANVIL__abt__define_call) {
            // advance current
            current_blueprintling.start += sizeof(ANVIL__blueprintling);
        } else {
            *error = ANVIL__open__internal_memory_error();

            return output;
        }

        // open new header
        ANVIL__accountling_abstraction_header header;
        
        // set statement type
        header.header.type = ANVIL__stt__abstraction_header;

        // set call type
        header.call_index = (ANVIL__act)(*(ANVIL__blueprintling*)current_blueprintling.start);
        current_blueprintling.start += sizeof(ANVIL__blueprintling);

        // set name
        header.header.name = ANVIL__create__parsling_argument(ANVIL__pat__variable, ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__name, ANVIL__open__buffer_from_string((ANVIL__u8*)ANVIL__global__accountling_call_type_name_strings[*(ANVIL__blueprintling*)current_blueprintling.start], ANVIL__bt__false, ANVIL__bt__false), ANVIL__create_null__character_location())), 0);
        current_blueprintling.start += sizeof(ANVIL__blueprintling);

        // get input count
        header.header.input_count = (*(ANVIL__blueprintling*)current_blueprintling.start);
        current_blueprintling.start += sizeof(ANVIL__blueprintling);

        // open inputs
        header.header.inputs = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 8, error);

        // get inputs
        for (ANVIL__blueprintling i = 0; i < header.header.input_count; i++) {
            // add argument
            ANVIL__append__parsling_argument(&header.header.inputs, ANVIL__create__parsling_argument((ANVIL__pat)(*(ANVIL__blueprintling*)current_blueprintling.start), ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__name, ANVIL__open__buffer_from_string((u8*)"(internal)", ANVIL__bt__false, ANVIL__bt__false), ANVIL__create_null__character_location())), 0), error);

            // next blueprintling
            current_blueprintling.start += sizeof(ANVIL__blueprintling);
        }

        // get output count
        header.header.output_count = (*(ANVIL__blueprintling*)current_blueprintling.start);
        current_blueprintling.start += sizeof(ANVIL__blueprintling);

        // open outputs
        header.header.outputs = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 8, error);

        // get outputs
        for (ANVIL__blueprintling i = 0; i < header.header.output_count; i++) {
            // add argument
            ANVIL__append__parsling_argument(&header.header.outputs, ANVIL__create__parsling_argument((ANVIL__pat)(*(ANVIL__blueprintling*)current_blueprintling.start), ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__name, ANVIL__open__buffer_from_string((u8*)"(internal)", ANVIL__bt__false, ANVIL__bt__false), ANVIL__create_null__character_location())), 0), error);

            // next blueprintling
            current_blueprintling.start += sizeof(ANVIL__blueprintling);
        }

        // append header
        ANVIL__append__accountling_abstraction_header(&output, header, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
    }

    // get all user defined headers
    // setup current
    ANVIL__current current_file = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // setup abstraction call ID
    ANVIL__call_index abstraction_call_ID = 0;

    // across all files
    while (ANVIL__check__current_within_range(current_file)) {
        // setup current
        ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&(*(ANVIL__parsling_program*)current_file.start).abstractions);

        // get each abstraction header
        while (ANVIL__check__current_within_range(current_abstraction)) {
            // get abstraction header
            ANVIL__accountling_abstraction_header header = ANVIL__account__get_abstraction_header(*(ANVIL__parsling_abstraction*)current_abstraction.start, abstraction_call_ID, error);

            // next abstraction call ID
            abstraction_call_ID++;

            // check for error
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // check to see if header already exists
            if (ANVIL__find__accountling_header_index(output, header.header) < ANVIL__calculate__call_blueprint_entry_count(output)) {
                // setup error
                *error = ANVIL__open__error("Accounting Error: An abstraction is already defined.", header.header.name.text.lexling.location);

                return output;
            }

            // append header
            ANVIL__append__accountling_abstraction_header(&output, header, error);
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // next abstraction
            current_abstraction.start += sizeof(ANVIL__parsling_abstraction);
        }

        // next file
        current_file.start += sizeof(ANVIL__parsling_program);
    }

    return output;
}

// verify all statements across all files
void ANVIL__account__verify_all_calls(ANVIL__list parsling_programs, ANVIL__list call_blueprint, ANVIL__error* error) {
    // setup current file
    ANVIL__current current_file = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // across all files
    while (ANVIL__check__current_within_range(current_file)) {
        // setup current abstraction
        ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&(*(ANVIL__parsling_program*)current_file.start).abstractions);

        // across all abstractions
        while (ANVIL__check__current_within_range(current_abstraction)) {
            // setup current statment
            ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&(*(ANVIL__parsling_abstraction*)current_abstraction.start).statements);

            // across all statements
            while (ANVIL__check__current_within_range(current_statement)) {
                // setup header to search for
                ANVIL__parsling_statement searching_for = *(ANVIL__parsling_statement*)current_statement.start;

                // if the statement is a call
                if (searching_for.type == ANVIL__stt__abstraction_call) {
                    // verify header exists
                    if (ANVIL__find__accountling_header_index(call_blueprint, searching_for) >= ANVIL__calculate__call_blueprint_entry_count(call_blueprint)) {
                        // does not exist
                        *error = ANVIL__open__error("Accounting Error: A statement calls a non-existent abstraction / instruction.", searching_for.name.text.lexling.location);

                        return;
                    }
                }

                // next statement
                current_statement.start += sizeof(ANVIL__parsling_statement);
            }

            // next abstraction
            current_abstraction.start += sizeof(ANVIL__parsling_abstraction);
        }

        // next file
        current_file.start += sizeof(ANVIL__parsling_program);
    }

    // no errors, valid
    return;
}

// accountling program
typedef struct ANVIL__accountling_program {
    ANVIL__list call_blueprint; // ANVIL__accountling_abstraction_header
    ANVIL__list abstractions; // ANVIL__accountling_abstraction
    ANVIL__list predefined_variables; // ANVIL__parsling_argument
    ANVIL__list predefined_flags; // ANVIL__parsling_argument
    ANVIL__call_index main_abstraction_ID;
} ANVIL__accountling_program;

// create custom accountling program
ANVIL__accountling_program ANVIL__create__accountling_program(ANVIL__list call_blueprint, ANVIL__list abstractions, ANVIL__list predefined_variables, ANVIL__list predefined_flags, ANVIL__call_index main_abstraction_ID) {
    ANVIL__accountling_program output;

    output.call_blueprint = call_blueprint;
    output.abstractions = abstractions;
    output.predefined_variables = predefined_variables;
    output.predefined_flags = predefined_flags;
    output.main_abstraction_ID = main_abstraction_ID;

    return output;
}

// create null accountling program
ANVIL__accountling_program ANVIL__create_null__accountling_program() {
    return ANVIL__create__accountling_program(ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__create_null__list(), ANVIL__define__null_call_ID);
}

// close accountling program
void ANVIL__close__accountling_program(ANVIL__accountling_program program) {
    // close predefined blueprints
    // get current
    ANVIL__current current_header = ANVIL__calculate__current_from_list_filled_index(&program.call_blueprint);

    // close all predefined blueprints
    while (ANVIL__check__current_within_range(current_header)) {
        // get header
        ANVIL__accountling_abstraction_header header = *(ANVIL__accountling_abstraction_header*)current_header.start;

        // if a predefined header
        if (header.call_index < ANVIL__act__user_defined) {
            // close header
            ANVIL__close__accountling_abstraction_header(header);
        }

        // next header
        current_header.start += sizeof(ANVIL__accountling_abstraction_header);
    }

    // close blueprint list
    ANVIL__close__list(program.call_blueprint);

    // close abstractions
    // check abstraction list exists
    if (ANVIL__check__empty_list(program.abstractions)) {
        return;
    }

    // get current
    ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&program.abstractions);

    // close each abstraction
    while (ANVIL__check__current_within_range(current_abstraction)) {
        ANVIL__close__accountling_abstraction(*(ANVIL__accountling_abstraction*)current_abstraction.start);

        // next abstraction
        current_abstraction.start += sizeof(ANVIL__accountling_abstraction);
    }

    // close abstraction list
    ANVIL__close__list(program.abstractions);

    // close predefineds
    ANVIL__close__list(program.predefined_variables);
    ANVIL__close__list(program.predefined_flags);

    return;
}

// convert string to converted string
ANVIL__buffer ANVIL__account__convert_string(ANVIL__parsling_argument string, ANVIL__error* error) {
    ANVIL__buffer output = ANVIL__create_null__buffer();
    ANVIL__length character_count = 0;

    // count characters
    for (ANVIL__character_index i = 1; i < ANVIL__calculate__buffer_length(string.text.lexling.value) - 1; i++) {
        // check for escape sequence
        if (((ANVIL__character*)string.text.lexling.value.start)[i] == '%') {
            // check for characters available
            if (i + 3 <= ANVIL__calculate__buffer_length(string.text.lexling.value) - 1) {
                // validate characters
                ANVIL__bt invalid_hexadecimal_character_1;
                ANVIL__bt invalid_hexadecimal_character_2;
                ANVIL__bt semi_colon_missing;
                ANVIL__translate__character_to_hexadecimal(((ANVIL__character*)string.text.lexling.value.start)[i + 1], &invalid_hexadecimal_character_1);
                ANVIL__translate__character_to_hexadecimal(((ANVIL__character*)string.text.lexling.value.start)[i + 2], &invalid_hexadecimal_character_2);
                semi_colon_missing = (ANVIL__bt)(((ANVIL__character*)string.text.lexling.value.start)[i + 3] != ';');

                // determine validity
                if (invalid_hexadecimal_character_1 != ANVIL__bt__false || invalid_hexadecimal_character_2 != ANVIL__bt__false || semi_colon_missing != ANVIL__bt__false) {
                    // invalid escape sequence
                    *error = ANVIL__open__error("Accounting Error: String literal has invalid escape sequences.", string.text.lexling.location);
                    
                    return output;
                }

                // skip past characters
                i += 3;
            // error
            } else {
                *error = ANVIL__open__error("Accounting Error: String literal has invalid escape sequences.", string.text.lexling.location);

                return output;
            }
        }

        // next character
        character_count++;
    }

    // check for empty string
    if (character_count == 0) {
        // return empty string
        return output;
    }

    // allocate string
    output = ANVIL__open__buffer(character_count);
    if (ANVIL__check__empty_buffer(output)) {
        *error = ANVIL__open__internal_memory_error();

        return output;
    }

    // translate string
    character_count = 0;
    for (ANVIL__character_index i = 1; i < ANVIL__calculate__buffer_length(string.text.lexling.value) - 1; i++) {
        // check for escape sequence
        if (((ANVIL__character*)string.text.lexling.value.start)[i] == '%') {
            // validate characters
            ANVIL__bt invalid_hexadecimal_character_1;
            ANVIL__bt invalid_hexadecimal_character_2;
            ANVIL__character a = ANVIL__translate__character_to_hexadecimal(((ANVIL__character*)string.text.lexling.value.start)[i + 1], &invalid_hexadecimal_character_1);
            ANVIL__character b = ANVIL__translate__character_to_hexadecimal(((ANVIL__character*)string.text.lexling.value.start)[i + 2], &invalid_hexadecimal_character_2);

            // write character
            ((ANVIL__character*)output.start)[character_count] = b + (a << 4);
            
            // skip past characters
            i += 3;
        } else {
            // write character
            ((ANVIL__character*)output.start)[character_count] = ((ANVIL__character*)string.text.lexling.value.start)[i];
        }

        // next character
        character_count++;
    }

    return output;
}

// check to see if argument is in list
ANVIL__parsling_argument ANVIL__account__get_argument_in_list__by_text(ANVIL__list* arguments, ANVIL__parsling_argument searching_for, ANVIL__bt* found) {
    // setup current
    ANVIL__current current_argument = ANVIL__calculate__current_from_list_filled_index(arguments);

    // search for argument by name
    while (ANVIL__check__current_within_range(current_argument)) {
        // get current argument
        ANVIL__parsling_argument argument = *(ANVIL__parsling_argument*)current_argument.start;

        // if names are identical
        if (ANVIL__check__parsling_arguments_have_same_text(argument, searching_for)) {
            // found in list
            *found = ANVIL__bt__true;
            return argument;
        }

        // next argument
        current_argument.start += sizeof(ANVIL__parsling_argument);
    }

    // not in list
    *found = ANVIL__bt__false;
    return ANVIL__create_null__parsling_argument();;
}

// get accountling argument list
ANVIL__list ANVIL__account__accountling_argument_list(ANVIL__accountling_abstraction* abstraction, ANVIL__list parsling_arguments, ANVIL__error* error) {
    // open list
    ANVIL__list output = ANVIL__open__list_with_error(sizeof(ANVIL__accountling_argument) * 16, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // get arguments
    // setup current
    ANVIL__current current_argument = ANVIL__calculate__current_from_list_filled_index(&parsling_arguments);

    // get arguments
    while (ANVIL__check__current_within_range(current_argument)) {
        // get argument
        ANVIL__parsling_argument argument = *(ANVIL__parsling_argument*)current_argument.start;

        // create argument ID by type
        if (argument.type == ANVIL__pat__variable__input) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).inputs, argument), argument), error);
        } else if (argument.type == ANVIL__pat__variable__output) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).outputs, argument), argument), error);
        } else if (argument.type == ANVIL__pat__variable || argument.type == ANVIL__pat__variable__body) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).variables, argument), argument), error);
        } else if (argument.type == ANVIL__pat__literal__boolean || argument.type == ANVIL__pat__literal__binary || argument.type == ANVIL__pat__literal__integer || argument.type == ANVIL__pat__literal__hexadecimal) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, argument.value, argument), error);
        } else if (argument.type == ANVIL__pat__offset) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).offsets, argument), argument), error);
        } else if (argument.type == ANVIL__pat__variable__predefined) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name(*(*abstraction).predefined_variables, argument), argument), error);
        } else if (argument.type == ANVIL__pat__flag__predefined) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name(*(*abstraction).predefined_flags, argument), argument), error);
        } else if (argument.type == ANVIL__pat__flag__user_defined) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).flags, argument), argument), error);
        } else if (argument.type == ANVIL__pat__literal__string) {
            ANVIL__append__accountling_argument(&output, ANVIL__create__accountling_argument(argument.type, ANVIL__find__parsling_argument_index__by_name((*abstraction).strings, argument), argument), error);
        } else {
            // error
            *error = ANVIL__open__error("Internal Error: Unsupported argument type in accountling argument list.", argument.text.lexling.location);

            return output;
        }

        // check for error
        if (ANVIL__check__error_occured(error)) {
            break;
        }

        // next argument
        current_argument.start += sizeof(ANVIL__parsling_argument);
    }

    return output;
}

// account an abstraction
ANVIL__accountling_abstraction ANVIL__account__abstraction(ANVIL__list call_blueprint, ANVIL__list* predefined_variables, ANVIL__list* predefined_flags, ANVIL__parsling_abstraction parsling_abstraction, ANVIL__error* error) {
    ANVIL__accountling_abstraction output = ANVIL__create_null__accountling_abstraction();

    // get name
    output.header = parsling_abstraction.header;

    // get variables
    // validate that all variables are not used before declared
    {
        // get predefined variables
        output.predefined_variables = predefined_variables;

        // get abstraction inputs
        {
            // open inputs
            output.inputs = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // setup current
            ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.header.inputs);

            // get inputs
            while (ANVIL__check__current_within_range(current_input)) {
                // get argument to look for
                ANVIL__parsling_argument searching_for = *(ANVIL__parsling_argument*)current_input.start;

                // get found variable back
                ANVIL__bt found_predefined;
                ANVIL__bt found_variable;
                ANVIL__account__get_argument_in_list__by_text(predefined_variables, searching_for, &found_predefined);
                ANVIL__account__get_argument_in_list__by_text(&output.inputs, searching_for, &found_variable);

                // if variable already exists
                if (found_predefined == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: A predefined variable was illegally used as an input.", searching_for.text.lexling.location);

                    return output;
                }

                // modify variable type
                searching_for.type = ANVIL__pat__variable__input;

                // if variable already exists
                if (found_variable == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: A duplicate header input was detected.", searching_for.text.lexling.location);

                    return output;
                }

                // add variable
                ANVIL__append__parsling_argument(&output.inputs, searching_for, error);
                if (ANVIL__check__error_occured(error)) {
                    return output;
                }

                // next argument
                current_input.start += sizeof(ANVIL__parsling_argument);
            }
        }

        // get abstraction outputs
        {
            // open outputs
            output.outputs = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // setup current
            ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.header.outputs);

            // get outputs
            while (ANVIL__check__current_within_range(current_output)) {
                // get argument to look for
                ANVIL__parsling_argument searching_for = *(ANVIL__parsling_argument*)current_output.start;

                // get found variable back
                ANVIL__bt found_predefined;
                ANVIL__bt found_input;
                ANVIL__bt found_output;
                ANVIL__account__get_argument_in_list__by_text(predefined_variables, searching_for, &found_predefined);
                ANVIL__account__get_argument_in_list__by_text(&output.inputs, searching_for, &found_input);
                ANVIL__account__get_argument_in_list__by_text(&output.outputs, searching_for, &found_output);

                // if variable already exists
                if (found_predefined == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: A predefined variable was illegally used as an output.", searching_for.text.lexling.location);

                    return output;
                }

                // if variable already exists
                if (found_input == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: An input was illegally used as an output.", searching_for.text.lexling.location);

                    return output;
                }

                // modify variable type
                searching_for.type = ANVIL__pat__variable__output;

                // if variable already exists
                if (found_output == ANVIL__bt__true) {
                    // variable declared twice
                    *error = ANVIL__open__error("Accounting Error: A duplicate header output was detected.", searching_for.text.lexling.location);

                    return output;
                }

                // add variable
                ANVIL__append__parsling_argument(&output.outputs, searching_for, error);
                if (ANVIL__check__error_occured(error)) {
                    return output;
                }

                // next argument
                current_output.start += sizeof(ANVIL__parsling_argument);
            }
        }

        // get abstraction body variables
        {
            // open variables
            output.variables = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
            if (ANVIL__check__error_occured(error)) {
                return output;
            }

            // setup current
            ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

            // across all statements
            while (ANVIL__check__current_within_range(current_statement)) {
                // get one statement
                ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

                // if statement is abstraction call
                if (statement.type == ANVIL__stt__abstraction_call) {
                    // validate inputs
                    // setup current
                    ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                    // check all inputs
                    while (ANVIL__check__current_within_range(current_input)) {
                        // get input
                        ANVIL__parsling_argument argument = *(ANVIL__parsling_argument*)current_input.start;

                        // if current argument is a variable
                        if (argument.type == ANVIL__pat__variable) {
                            // assuming its not already defined
                            ANVIL__bt found_predefined;
                            ANVIL__account__get_argument_in_list__by_text(output.predefined_variables, argument, &found_predefined);
                            if (found_predefined == ANVIL__bt__true) {
                                goto next_input;
                            }

                            // check if argument already exists
                            ANVIL__bt found_input;
                            ANVIL__bt found_output;
                            ANVIL__bt found_body;
                            ANVIL__account__get_argument_in_list__by_text(&output.inputs, argument, &found_input);
                            ANVIL__account__get_argument_in_list__by_text(&output.outputs, argument, &found_output);
                            ANVIL__account__get_argument_in_list__by_text(&output.variables, argument, &found_body);
                            if ((found_input || found_output || found_body) == ANVIL__bt__false) {
                                *error = ANVIL__open__error("Accounting Error: A variable was used before it was declared.", argument.text.lexling.location);

                                return output;
                            }
                        }

                        // next input
                        next_input:
                        current_input.start += sizeof(ANVIL__parsling_argument);
                    }

                    // validate outputs
                    // setup current
                    ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&statement.outputs);

                    // check all outputs
                    while (ANVIL__check__current_within_range(current_output)) {
                        // get output
                        ANVIL__parsling_argument argument = *(ANVIL__parsling_argument*)current_output.start;
                    
                        // if current argument is a variable
                        if (argument.type == ANVIL__pat__variable) {
                            // assuming its not already defined
                            ANVIL__bt found_predefined;
                            ANVIL__account__get_argument_in_list__by_text(output.predefined_variables, argument, &found_predefined);
                            if (found_predefined == ANVIL__bt__true) {
                                *error = ANVIL__open__error("Accounting Error: Predefined variables cannot be written to.", argument.text.lexling.location);

                                return output;
                            }

                            // check if argument already exists
                            ANVIL__bt found_input;
                            ANVIL__bt found_output;
                            ANVIL__bt found_body;
                            ANVIL__account__get_argument_in_list__by_text(&output.inputs, argument, &found_input);
                            ANVIL__account__get_argument_in_list__by_text(&output.outputs, argument, &found_output);
                            ANVIL__account__get_argument_in_list__by_text(&output.variables, argument, &found_body);
                            if ((found_input || found_output || found_body) == ANVIL__bt__false) {
                                // setup argument type
                                argument.type = ANVIL__pat__variable__body;

                                // create variable
                                ANVIL__append__parsling_argument(&output.variables, argument, error);
                                if (ANVIL__check__error_occured(error)) {
                                    return output;
                                }
                            }
                        }

                        // next output
                        current_output.start += sizeof(ANVIL__parsling_argument);
                    }
                }

                // next statement
                current_statement.start += sizeof(ANVIL__parsling_statement);
            }
        }
    }

    // get offsets
    {
        // open list
        output.offsets = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // setup current
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if a statement offset
            if (statement.type == ANVIL__stt__offset) {
                // if offset doesnt already exist
                ANVIL__bt found_offset;
                ANVIL__account__get_argument_in_list__by_text(&output.offsets, statement.name, &found_offset);
                if (found_offset == ANVIL__bt__false) {
                    // append offset declaration
                    ANVIL__append__parsling_argument(&output.offsets, statement.name, error);
                    if (ANVIL__check__error_occured(error)) {
                        return output;
                    }
                // already defined, error
                } else {
                    *error = ANVIL__open__error("Accounting Error: Offset is declared more than once.", statement.name.text.lexling.location);

                    return output;
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }

        // verify all offsets used exist
        // setup current statement
        current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if statement is abstraction call
            if (statement.type == ANVIL__stt__abstraction_call) {
                // setup current input
                ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                // for each input
                while (ANVIL__check__current_within_range(current_input)) {
                    // get input
                    ANVIL__parsling_argument input_argument = *(ANVIL__parsling_argument*)current_input.start;

                    // if argument is offset
                    if (input_argument.type == ANVIL__pat__offset) {
                        // check if input exists
                        ANVIL__bt found_offset;
                        ANVIL__account__get_argument_in_list__by_text(&output.offsets, input_argument, &found_offset);
                        if (found_offset == ANVIL__bt__false) {
                            // offset nonexistent
                            *error = ANVIL__open__error("Accounting Error: Offset was used but never declared.", input_argument.text.lexling.location);

                            return output;
                        }
                    }

                    // next argument
                    current_input.start += sizeof(ANVIL__parsling_argument);
                }

                // setup current output
                ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&statement.outputs);

                // for each output
                while (ANVIL__check__current_within_range(current_output)) {
                    // get output
                    ANVIL__parsling_argument output_argument = *(ANVIL__parsling_argument*)current_output.start;

                    // if argument is offset
                    if (output_argument.type == ANVIL__pat__offset) {
                        // check if output exists
                        ANVIL__bt found_offset;
                        ANVIL__account__get_argument_in_list__by_text(&output.offsets, output_argument, &found_offset);
                        if (found_offset == ANVIL__bt__false) {
                            // offset nonexistent
                            *error = ANVIL__open__error("Accounting Error: Offset was used but never declared.", output_argument.text.lexling.location);

                            return output;
                        }
                    }

                    // next argument
                    current_output.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    // get flags
    {
        // setup predefined flags
        output.predefined_flags = predefined_flags;

        // open list
        output.flags = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // verify all flags used exist
        // setup current statement
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if statement is abstraction call
            if (statement.type == ANVIL__stt__abstraction_call) {
                // setup current input
                ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                // for each input
                while (ANVIL__check__current_within_range(current_input)) {
                    // get input
                    ANVIL__parsling_argument input_argument = *(ANVIL__parsling_argument*)current_input.start;

                    // if argument is flag
                    if (input_argument.type == ANVIL__pat__flag) {
                        // check if flag exists
                        ANVIL__bt found_predefined_flag;
                        ANVIL__bt found_user_defined_flag;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_flags, input_argument, &found_predefined_flag);
                        ANVIL__account__get_argument_in_list__by_text(&output.flags, input_argument, &found_user_defined_flag);
                        if (found_predefined_flag == ANVIL__bt__false && found_user_defined_flag == ANVIL__bt__false) {
                            // flag nonexistent
                            *error = ANVIL__open__error("Accounting Error: Flag was used but never declared.", input_argument.text.lexling.location);

                            return output;
                        }
                    }

                    // next argument
                    current_input.start += sizeof(ANVIL__parsling_argument);
                }

                // setup current output
                ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&statement.outputs);

                // for each output
                while (ANVIL__check__current_within_range(current_output)) {
                    // get output
                    ANVIL__parsling_argument output_argument = *(ANVIL__parsling_argument*)current_output.start;

                    // if argument is flag
                    if (output_argument.type == ANVIL__pat__flag) {
                        // check if flag exists
                        ANVIL__bt found_predefined_flag;
                        ANVIL__bt found_user_defined_flag;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_flags, output_argument, &found_predefined_flag);
                        ANVIL__account__get_argument_in_list__by_text(&output.flags, output_argument, &found_user_defined_flag);
                        if (found_predefined_flag == ANVIL__bt__true) {
                            // predefined flags cannot be set, error
                            *error = ANVIL__open__error("Accounting Error: Predefined flags cannot be written to.", output_argument.text.lexling.location);

                            return output;
                        }
                        if (found_user_defined_flag == ANVIL__bt__false) {
                            // declare new flag
                            ANVIL__append__parsling_argument(&output.flags, output_argument, error);
                            if (ANVIL__check__error_occured(error)) {
                                return output;
                            }
                        }
                    }

                    // next argument
                    current_output.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    // get strings
    {
        // open strings
        output.strings = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
        output.converted_strings = ANVIL__open__list_with_error(sizeof(ANVIL__buffer) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // get current statement
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if statement is call
            if (statement.type == ANVIL__stt__abstraction_call) {
                // get inputs
                ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                // for each input
                while (ANVIL__check__current_within_range(current_input)) {
                    // get input
                    ANVIL__parsling_argument* input = (ANVIL__parsling_argument*)current_input.start;

                    // if argument is string
                    if (input->type == ANVIL__pat__literal__string) {
                        // append string
                        ANVIL__append__parsling_argument(&output.strings, *input, error);
                        if (ANVIL__check__error_occured(error)) {
                            return output;
                        }

                        // translate string data
                        ANVIL__append__buffer_with_error(&output.converted_strings, ANVIL__account__convert_string(*input, error), error);
                        if (ANVIL__check__error_occured(error)) {
                            return output;
                        }
                    }

                    // next input
                    current_input.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    // modify parsling statements to reflect accurate variable types (predefined, input, output & body) & flag types (predefined & user defined)
    {
        // get current statement
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement statement = *(ANVIL__parsling_statement*)current_statement.start;

            // if statement is call
            if (statement.type == ANVIL__stt__abstraction_call) {
                // modify inputs
                // get inputs
                ANVIL__current current_input = ANVIL__calculate__current_from_list_filled_index(&statement.inputs);

                // for each input
                while (ANVIL__check__current_within_range(current_input)) {
                    // get input
                    ANVIL__parsling_argument* input = (ANVIL__parsling_argument*)current_input.start;

                    // if argument is variable
                    if (input->type == ANVIL__pat__variable) {
                        // find variable by type
                        ANVIL__bt found_predefined;
                        ANVIL__bt found_input;
                        ANVIL__bt found_output;
                        ANVIL__bt found_body;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_variables, *input, &found_predefined);
                        ANVIL__account__get_argument_in_list__by_text(&output.inputs, *input, &found_input);
                        ANVIL__account__get_argument_in_list__by_text(&output.outputs, *input, &found_output);
                        ANVIL__account__get_argument_in_list__by_text(&output.variables, *input, &found_body);

                        // modify appropriately
                        if (found_predefined) {
                            input->type = ANVIL__pat__variable__predefined;
                        } else if (found_input) {
                            input->type = ANVIL__pat__variable__input;
                        } else if (found_output) {
                            input->type = ANVIL__pat__variable__output;
                        } else if (found_body) {
                            input->type = ANVIL__pat__variable__body;
                        } else {
                            // error
                            *error = ANVIL__open__error("Internal Error: Unrecognized variable type when modifying statement variable types, oops.", input->text.lexling.location);
                        }
                    } else if (input->type == ANVIL__pat__flag) {
                        // find variable by type
                        ANVIL__bt found_predefined;
                        ANVIL__bt found_user_defined;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_flags, *input, &found_predefined);
                        ANVIL__account__get_argument_in_list__by_text(&output.flags, *input, &found_user_defined);

                        // modify appropriately
                        if (found_predefined) {
                            input->type = ANVIL__pat__flag__predefined;
                        } else if (found_user_defined) {
                            input->type = ANVIL__pat__flag__user_defined;
                        } else {
                            // error
                            *error = ANVIL__open__error("Internal Error: Unrecognized variable type when modifying statement input flag types, oops.", input->text.lexling.location);
                        }
                    }

                    // next input
                    current_input.start += sizeof(ANVIL__parsling_argument);
                }
                
                // modify outputs
                // get outputs
                ANVIL__current current_output = ANVIL__calculate__current_from_list_filled_index(&statement.outputs);

                // for each output
                while (ANVIL__check__current_within_range(current_output)) {
                    // get output
                    ANVIL__parsling_argument* output_argument = (ANVIL__parsling_argument*)current_output.start;

                    // if argument is variable
                    if (output_argument->type == ANVIL__pat__variable) {
                        // find variable by type
                        ANVIL__bt found_predefined;
                        ANVIL__bt found_input;
                        ANVIL__bt found_output;
                        ANVIL__bt found_body;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_variables, *output_argument, &found_predefined);
                        ANVIL__account__get_argument_in_list__by_text(&output.inputs, *output_argument, &found_input);
                        ANVIL__account__get_argument_in_list__by_text(&output.outputs, *output_argument, &found_output);
                        ANVIL__account__get_argument_in_list__by_text(&output.variables, *output_argument, &found_body);

                        // modify appropriately
                        if (found_predefined) {
                            output_argument->type = ANVIL__pat__variable__predefined;
                        } else if (found_input) {
                            output_argument->type = ANVIL__pat__variable__input;
                        } else if (found_output) {
                            output_argument->type = ANVIL__pat__variable__output;
                        } else if (found_body) {
                            output_argument->type = ANVIL__pat__variable__body;
                        } else {
                            // error
                            *error = ANVIL__open__error("Internal Error: Unrecognized variable type when modifying statement variable types, oops.", output_argument->text.lexling.location);
                        }
                    } else if (output_argument->type == ANVIL__pat__flag) {
                        // find variable by type
                        ANVIL__bt found_predefined;
                        ANVIL__bt found_user_defined;
                        ANVIL__account__get_argument_in_list__by_text(output.predefined_flags, *output_argument, &found_predefined);
                        ANVIL__account__get_argument_in_list__by_text(&output.flags, *output_argument, &found_user_defined);

                        // modify appropriately
                        if (found_predefined) {
                            output_argument->type = ANVIL__pat__flag__predefined;
                        } else if (found_user_defined) {
                            output_argument->type = ANVIL__pat__flag__user_defined;
                        } else {
                            // error
                            *error = ANVIL__open__error("Internal Error: Unrecognized variable type when modifying statement output flag types, oops.", output_argument->text.lexling.location);
                        }
                    }

                    // next output
                    current_output.start += sizeof(ANVIL__parsling_argument);
                }
            }

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    // get statements
    {
        // open list
        output.statements = ANVIL__open__list_with_error(sizeof(ANVIL__accountling_statement) * 16, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // setup offsets index
        ANVIL__offset_index offset_ID = 0;

        // convert statements
        // get current
        ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&parsling_abstraction.statements);

        // for each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // get statement
            ANVIL__parsling_statement parsling_statement = *(ANVIL__parsling_statement*)current_statement.start;

            // setup new statement
            ANVIL__accountling_statement accountling_statement;

            // translate statement
            // statement is a call
            if (parsling_statement.type == ANVIL__stt__abstraction_call) {
                // set type
                accountling_statement.type = parsling_statement.type;

                // get accountling header index
                ANVIL__header_index header_index = ANVIL__find__accountling_header_index(call_blueprint, parsling_statement);
                if (header_index > ANVIL__calculate__call_blueprint_entry_count(call_blueprint)) {
                    *error = ANVIL__open__error("Internal Error: Header was not found during lookup.\n", parsling_statement.name.text.lexling.location);

                    return output;
                }

                // get header
                accountling_statement.header = ((ANVIL__accountling_abstraction_header*)call_blueprint.buffer.start)[header_index];

                // get io
                accountling_statement.inputs = ANVIL__account__accountling_argument_list(&output, parsling_statement.inputs, error);
                if (ANVIL__check__error_occured(error)) {
                    return output;
                }
                accountling_statement.outputs = ANVIL__account__accountling_argument_list(&output, parsling_statement.outputs, error);
                if (ANVIL__check__error_occured(error)) {
                    return output;
                }
            // statement is an offset
            } else if (parsling_statement.type == ANVIL__stt__offset) {
                // set type
                accountling_statement.type = parsling_statement.type;

                // get offset index
                accountling_statement.offset_ID = offset_ID;

                // next offset
                offset_ID++;
            }

            // append statement
            ANVIL__append__accountling_statement(&output.statements, accountling_statement, error);

            // next statement
            current_statement.start += sizeof(ANVIL__parsling_statement);
        }
    }

    return output;
}

// print call header
void ANVIL__print__accounted_call_header(ANVIL__accountling_abstraction_header header) {
    // print ID
    printf("\t\tCall Index [%lu](user_defined=%lu) ", (ANVIL__u64)header.call_index, (ANVIL__u64)(header.call_index >= ANVIL__act__user_defined));

    // print statement
    ANVIL__print__parsed_statement(header.header);

    // new line
    printf("\n");

    return;
}

// print call blueprint
void ANVIL__print__call_blueprint(ANVIL__list blueprint) {
    // setup current
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&blueprint);

    // print all headers
    while (ANVIL__check__current_within_range(current)) {
        // print individual header
        ANVIL__print__accounted_call_header(*(ANVIL__accountling_abstraction_header*)current.start);

        // next header
        current.start += sizeof(ANVIL__accountling_abstraction_header);
    }

    return;
}

// append predefined variable
void ANVIL__generate_and_append__predefined_variable(ANVIL__list* list, const char* name, ANVIL__error* error) {
    // append
    ANVIL__append__parsling_argument(list, ANVIL__create__parsling_argument(ANVIL__pat__variable__predefined, ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__end_of_file, ANVIL__open__buffer_from_string((ANVIL__u8*)name, ANVIL__bt__false, ANVIL__bt__false), ANVIL__create__character_location(-1, -1, -1))), 0), error);

    return;
}

// generate predefined variables
ANVIL__list ANVIL__generate__predefined_variables(ANVIL__error* error) {
    ANVIL__list output;

    // open output
    output = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // append variables
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__error_code], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__character_byte_size], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__character_bit_size], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__bits_in_byte], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__cell_byte_size], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__cell_bit_size], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__true], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__false], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__0], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__1], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__2], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__4], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__8], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__16], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__24], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__32], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__40], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__48], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__56], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__64], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__input_start], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__input_end], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__output_start], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__constant__output_end], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__stack_start], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__stack_current], error);
    ANVIL__generate_and_append__predefined_variable(&output, ANVIL__global__predefined_cell_name_strings[ANVIL__pvt__stack_end], error);

    return output;
}

// append predefined flag
void ANVIL__generate_and_append__predefined_flag(ANVIL__list* list, const char* name, ANVIL__error* error) {
    // append
    ANVIL__append__parsling_argument(list, ANVIL__create__parsling_argument(ANVIL__pat__flag__predefined, ANVIL__create__name(ANVIL__create__lexling(ANVIL__lt__end_of_file, ANVIL__open__buffer_from_string((ANVIL__u8*)name, ANVIL__bt__false, ANVIL__bt__false), ANVIL__create__character_location(-1, -1, -1))), 0), error);

    return;
}

// generate predefined flags
ANVIL__list ANVIL__generate__predefined_flags(ANVIL__error* error) {
    ANVIL__list output;

    // open output
    output = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 16, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // append flags
    ANVIL__generate_and_append__predefined_flag(&output, ANVIL__global__predefined_flag_name_strings[ANVIL__pft__always_run], error);
    ANVIL__generate_and_append__predefined_flag(&output, ANVIL__global__predefined_flag_name_strings[ANVIL__pft__never_run], error);
    ANVIL__generate_and_append__predefined_flag(&output, ANVIL__global__predefined_flag_name_strings[ANVIL__pft__temporary], error);

    return output;
}

// account program
ANVIL__accountling_program ANVIL__account__program(ANVIL__list parsling_programs, ANVIL__error* error) {
    ANVIL__accountling_program output = ANVIL__create_null__accountling_program();

    // create headers
    output.call_blueprint = ANVIL__generate__call_blueprint(parsling_programs, error);
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // verify statments in all user defined abstractions
    ANVIL__account__verify_all_calls(parsling_programs, output.call_blueprint, error);
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // generate predefined variables
    output.predefined_variables = ANVIL__generate__predefined_variables(error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // generate predefined flags
    output.predefined_flags = ANVIL__generate__predefined_flags(error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // allocate accountling abstraction list
    output.abstractions = ANVIL__open__list_with_error(sizeof(ANVIL__accountling_abstraction) * 16, error);
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // setup current
    ANVIL__current current_parsling_program = ANVIL__calculate__current_from_list_filled_index(&parsling_programs);

    // account all programs
    while (ANVIL__check__current_within_range(current_parsling_program)) {
        // get program
        ANVIL__parsling_program parsling_program = *(ANVIL__parsling_program*)current_parsling_program.start;

        // setup current
        ANVIL__current current_parsling_abstraction = ANVIL__calculate__current_from_list_filled_index(&parsling_program.abstractions);

        // account all abstractions in program
        while (ANVIL__check__current_within_range(current_parsling_abstraction)) {
            // get parsling abstraction
            ANVIL__parsling_abstraction parsling_abstraction = *(ANVIL__parsling_abstraction*)current_parsling_abstraction.start;

            // account the parsling abstraction
            ANVIL__accountling_abstraction accountling_abstraction = ANVIL__account__abstraction(output.call_blueprint, &output.predefined_variables, &output.predefined_flags, parsling_abstraction, error);
            if (ANVIL__check__error_occured(error)) {
                // close abstraction
                ANVIL__close__accountling_abstraction(accountling_abstraction);

                // quit
                goto quit;
            }

            // append abstraction
            ANVIL__append__accountling_abstraction(&output.abstractions, accountling_abstraction, error);
            if (ANVIL__check__error_occured(error)) {
                goto quit;
            }

            // next abstraction
            current_parsling_abstraction.start += sizeof(ANVIL__parsling_abstraction);
        }

        // next program
        current_parsling_program.start += sizeof(ANVIL__parsling_program);
    }

    // search for main
    // setup current
    ANVIL__current current_header = ANVIL__calculate__current_from_list_filled_index(&output.call_blueprint);
    output.main_abstraction_ID = 0;

    // for each header
    while (ANVIL__check__current_within_range(current_header)) {
        // get header
        ANVIL__accountling_abstraction_header header = *(ANVIL__accountling_abstraction_header*)current_header.start;

        // check for correct title & io counts
        if (ANVIL__calculate__buffer_contents_equal(header.header.name.text.lexling.value, ANVIL__open__buffer_from_string((u8*)"main", ANVIL__bt__false, ANVIL__bt__false)) && header.header.input_count == 0 && header.header.output_count == 0) {
            break;
        }

        // next header
        output.main_abstraction_ID++;
        current_header.start += sizeof(ANVIL__accountling_abstraction_header);
    }

    // check to see if main was not found
    if (ANVIL__check__current_within_range(current_header) == ANVIL__bt__false) {
        // set error
        *error = ANVIL__open__error("Accounting Error: The entry point function main()() was not found.", ANVIL__create_null__character_location());

        goto quit;
    }

    // return
    quit:

    return output;
}

// print variable list
void ANVIL__print__accountling_variable_list(ANVIL__list* variables, ANVIL__tab_count tabs) {
    // setup current
    ANVIL__current current_variable = ANVIL__calculate__current_from_list_filled_index(variables);

    // print each argument
    while (ANVIL__check__current_within_range(current_variable)) {
        // print variable
        ANVIL__print__tabs(tabs);
        ANVIL__print__parsling_argument(*(ANVIL__parsling_argument*)current_variable.start);
        printf("\n");

        // next variable
        current_variable.start += sizeof(ANVIL__parsling_argument);
    }
    
    return;
}

// print accountling argument list
void ANVIL__print__accountling_arguments(ANVIL__list arguments) {
    // print opener
    printf("(");

    // setup current
    ANVIL__current current_argument = ANVIL__calculate__current_from_list_filled_index(&arguments);

    // for each argument
    while (ANVIL__check__current_within_range(current_argument)) {
        // get argument
        ANVIL__accountling_argument argument = *(ANVIL__accountling_argument*)current_argument.start;

        // print argument
        printf("[");
        ANVIL__print__buffer(ANVIL__convert__parsed_argument_type_to_string_buffer(argument.type));
        printf(":%lu]", argument.index);


        // next argument
        current_argument.start += sizeof(ANVIL__accountling_argument);
    }

    // print closer
    printf(")");

    return;
}

// print an accountling program
void ANVIL__print__accountling_program(ANVIL__accountling_program program) {
    // print section start
    printf("Accountlings:\n\tHeaders:\n");

    // print headers
    ANVIL__print__call_blueprint(program.call_blueprint);

    // print predefined variables
    printf("\tPredefined Variables:\n");

    // print variables
    ANVIL__print__accountling_variable_list(&program.predefined_variables, 2);

    // print predefined flags
    printf("\tPredefined Flags:\n");

    // print flags
    ANVIL__print__accountling_variable_list(&program.predefined_flags, 2);

    // print abstractions
    printf("\tAbstractions:\n");

    // setup current
    ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&program.abstractions);

    // print each abstraction
    while (ANVIL__check__current_within_range(current_abstraction)) {
        // get abstraction
        ANVIL__accountling_abstraction abstraction = *(ANVIL__accountling_abstraction*)current_abstraction.start;

        // print abstraction
        {
            // get call index
            ANVIL__call_index call_index = ANVIL__find__accountling_header_index(program.call_blueprint, abstraction.header);
            if (call_index >= ANVIL__calculate__call_blueprint_entry_count(program.call_blueprint)) {
                // call header not found, exit loop
                return;
            }

            // print header with call index
            printf("\t\t(%lu)", call_index);
            ANVIL__print__parsed_statement(abstraction.header);
            printf("\n");

            // if there are inputs
            if (abstraction.inputs.filled_index > 0) {
                // print inputs
                printf("\t\t\tInputs:\n");
                ANVIL__print__accountling_variable_list(&abstraction.inputs, 4);
            }

            // if there are outputs
            if (abstraction.outputs.filled_index > 0) {
                // print outputs
                printf("\t\t\tOutputs:\n");
                ANVIL__print__accountling_variable_list(&abstraction.outputs, 4);
            }

            // if there are variables
            if (abstraction.variables.filled_index > 0) {
                // print variables
                printf("\t\t\tVariables:\n");
                ANVIL__print__accountling_variable_list(&abstraction.variables, 4);
            }

            // if there are offsets
            if (abstraction.offsets.filled_index > 0) {
                // print offsets
                printf("\t\t\tOffsets:\n");
                ANVIL__print__accountling_variable_list(&abstraction.offsets, 4);
            }

            // if there are flags
            if (abstraction.flags.filled_index > 0) {
                // print flags
                printf("\t\t\tFlags:\n");
                ANVIL__print__accountling_variable_list(&abstraction.flags, 4);
            }

            // if there are statements
            if (abstraction.statements.filled_index > 0) {
                // print statements
                // print header
                printf("\t\t\tStatements:\n");

                // setup current
                ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&abstraction.statements);
                
                // for each statement
                while (ANVIL__check__current_within_range(current_statement)) {
                    // get statement
                    ANVIL__accountling_statement statement = *(ANVIL__accountling_statement*)current_statement.start;
                    
                    // determine statement type
                    // if abstraction call
                    if (statement.type == ANVIL__stt__abstraction_call) {
                        // print statement call index
                        printf("\t\t\t\t%lu = ", statement.header.call_index);

                        // print io
                        ANVIL__print__accountling_arguments(statement.inputs);
                        ANVIL__print__accountling_arguments(statement.outputs);

                        // print new line
                        printf("\n");
                    // if offset declaration
                    } else if (statement.type == ANVIL__stt__offset) {
                        // print index
                        printf("\t\t\t\t@%lu\n", statement.offset_ID);
                    }

                    // next statement
                    current_statement.start += sizeof(ANVIL__accountling_statement);
                }
            }
        }

        // next abstraction
        current_abstraction.start += sizeof(ANVIL__accountling_abstraction);
    }

    return;
}

/* Generation */
// add an offset to a list
void ANVIL__append__offset(ANVIL__list* list, ANVIL__offset data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__offset), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__offset*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__offset);

    return;
}

// generation offsets
typedef struct ANVIL__generation_offsets {
    ANVIL__offset function_start; // the first instruction in the function
    ANVIL__offset function_return; // the first instruction passing output
    ANVIL__offset function_data; // after the code of the function, the embedded data's place
    ANVIL__list statement_offsets; // ANVIL__offset the function specific statement offsets
    ANVIL__list strings_offsets; // ANVIL__offset the function specific string offsets
} ANVIL__generation_offsets;

// generation cell range
typedef struct ANVIL__generation_cell_range {
    ANVIL__cell_ID start;
    ANVIL__cell_ID end;
} ANVIL__generation_cell_range;

// create a cell range
ANVIL__generation_cell_range ANVIL__create__generation_cell_range(ANVIL__cell_ID start, ANVIL__cell_ID end) {
    ANVIL__generation_cell_range output;

    output.start = start;
    output.end = end;

    return output;
}

// calculate cell range length
ANVIL__cell_count ANVIL__calculate__generation_cell_length(ANVIL__generation_cell_range range) {
    return range.end - range.start + 1;
}

// calculate cell range from start and length
ANVIL__generation_cell_range ANVIL__calculate__generation_cell_range(ANVIL__cell_ID start, ANVIL__cell_count length) {
    return ANVIL__create__generation_cell_range(start, start + length - 1);
}

// generation cells
typedef struct ANVIL__generation_cells {
    ANVIL__generation_cell_range workspace_total_range;
    ANVIL__generation_cell_range workspace_input_range;
    ANVIL__generation_cell_range workspace_output_range;
    ANVIL__generation_cell_range workspace_body_range;
    ANVIL__generation_cell_range function_input_range;
    ANVIL__generation_cell_range function_output_range;
    ANVIL__cell_count input_count;
    ANVIL__cell_count output_count;
    ANVIL__cell_count variable_count;
    ANVIL__cell_count workspace_total_count;
} ANVIL__generation_cells;

// setup generation cells from io & variable counts
ANVIL__generation_cells ANVIL__setup__generation_cells(ANVIL__cell_count input_count, ANVIL__cell_count output_count, ANVIL__cell_count variable_count) {
    ANVIL__generation_cells output;

    // setup counts
    output.input_count = input_count;
    output.output_count = output_count;
    output.variable_count = variable_count;
    output.workspace_total_count = input_count + output_count + variable_count;

    // calculate ranges
    output.function_input_range = ANVIL__calculate__generation_cell_range(ANVIL__srt__start__function_io, input_count);
    output.function_output_range = ANVIL__calculate__generation_cell_range(ANVIL__srt__start__function_io, output_count);
    output.workspace_input_range = ANVIL__calculate__generation_cell_range(ANVIL__srt__start__workspace, input_count);
    output.workspace_output_range = ANVIL__calculate__generation_cell_range(output.workspace_input_range.end + 1, output_count);
    output.workspace_body_range = ANVIL__calculate__generation_cell_range(output.workspace_output_range.end + 1, variable_count);
    output.workspace_total_range = ANVIL__create__generation_cell_range(output.workspace_input_range.start, output.workspace_body_range.end);

    return output;
}

// generation abstraction
typedef struct ANVIL__generation_abstraction {
    ANVIL__generation_cells cells;
    ANVIL__generation_offsets offsets;
    ANVIL__list converted_strings; // ANVIL__buffer (copied from accountlings! do not free!)
} ANVIL__generation_abstraction;

// translate accountling abstraction to generation abstraction
ANVIL__generation_abstraction ANVIL__open__generation_abstraction(ANVIL__accountling_abstraction accountlings, ANVIL__error* error) {
    ANVIL__generation_abstraction output;

    // translate registers
    output.cells = ANVIL__setup__generation_cells(ANVIL__calculate__list_content_count(accountlings.inputs, sizeof(ANVIL__parsling_argument)), ANVIL__calculate__list_content_count(accountlings.outputs, sizeof(ANVIL__parsling_argument)), ANVIL__calculate__list_content_count(accountlings.variables, sizeof(ANVIL__parsling_argument)));

    // open offsets
    output.offsets.function_start = ANVIL__define__null_offset_ID;
    output.offsets.function_return = ANVIL__define__null_offset_ID;
    output.offsets.function_data = ANVIL__define__null_offset_ID;
    output.offsets.statement_offsets = ANVIL__open__list_with_error(sizeof(ANVIL__offset) * 16, error);
    output.offsets.strings_offsets = ANVIL__open__list_with_error(sizeof(ANVIL__offset) * 16, error);

    // append statement offsets
    for (ANVIL__offset_index i = 0; i < ANVIL__calculate__list_content_count(accountlings.offsets, sizeof(ANVIL__parsling_argument)); i++) {
        // append blank offset
        ANVIL__append__offset(&output.offsets.statement_offsets, ANVIL__define__null_offset_ID, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
    }

    // append string offsets
    for (ANVIL__offset_index i = 0; i < ANVIL__calculate__list_content_count(accountlings.strings, sizeof(ANVIL__parsling_argument)); i++) {
        // append blank offset
        ANVIL__append__offset(&output.offsets.strings_offsets, ANVIL__define__null_offset_ID, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }
    }

    // open strings
    output.converted_strings = accountlings.converted_strings;

    return output;
}

// close generation abstraction
void ANVIL__close__generation_abstraction(ANVIL__generation_abstraction abstraction) {
    // close offsets
    ANVIL__close__list(abstraction.offsets.statement_offsets);
    ANVIL__close__list(abstraction.offsets.strings_offsets);

    return;
}

// append generation abstraction
void ANVIL__append__generation_abstraction(ANVIL__list* list, ANVIL__generation_abstraction data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__generation_abstraction), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__generation_abstraction*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__generation_abstraction);

    return;
}

// generation workspace
typedef struct ANVIL__generation_workspace {
    ANVIL__workspace anvil;
    ANVIL__workspace* workspace;
    ANVIL__list abstractions; // ANVIL__generation_abstraction
    ANVIL__abstraction_index entry_point;
    ANVIL__abstraction_index function_count;
    ANVIL__standard__offsets standard_offsets;
} ANVIL__generation_workspace;

// open workspace
void ANVIL__open__generation_workspace(ANVIL__buffer* program_buffer, ANVIL__accountling_program accountlings, ANVIL__generation_workspace* output, ANVIL__error* error) {
    // setup anvil workspace
    output->anvil = ANVIL__setup__workspace(program_buffer);
    output->workspace = &(output->anvil);

    // setup entry point
    output->entry_point = ANVIL__calculate__abstraction_index_from_call_index(accountlings.main_abstraction_ID);

    // setup generation abstractions from accountling abstractions
    // open generation abstractions
    output->abstractions = ANVIL__open__list_with_error(sizeof(ANVIL__generation_abstraction) * 16, error);
    output->function_count = 0;

    // setup current
    ANVIL__current current_accountling_abstraction = ANVIL__calculate__current_from_list_filled_index(&accountlings.abstractions);
    
    // for each accountling abstraction
    while (ANVIL__check__current_within_range(current_accountling_abstraction)) {
        // get accountling abstraction
        ANVIL__accountling_abstraction accountling_abstraction = *(ANVIL__accountling_abstraction*)current_accountling_abstraction.start;

        // setup generation abstraction
        ANVIL__generation_abstraction generation_abstraction = ANVIL__open__generation_abstraction(accountling_abstraction, error);
        if (ANVIL__check__error_occured(error)) {
            return;
        }

        // append abstraction
        ANVIL__append__generation_abstraction(&output->abstractions, generation_abstraction, error);
        if (ANVIL__check__error_occured(error)) {
            return;
        }

        // next abstraction
        output->function_count++;
        current_accountling_abstraction.start += sizeof(ANVIL__accountling_abstraction);
    }

    return;
}

// close workspace
void ANVIL__close__generation_workspace(ANVIL__generation_workspace workspace) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&workspace.abstractions);

    // close all abstractions
    while (ANVIL__check__current_within_range(current)) {
        // close abstraction
        ANVIL__close__generation_abstraction(*(ANVIL__generation_abstraction*)current.start);

        // next abstraction
        current.start += sizeof(ANVIL__generation_abstraction);
    }

    // close abstractions list
    ANVIL__close__list(workspace.abstractions);

    return;
}

// get abstraction from workspace
ANVIL__generation_abstraction ANVIL__find__generation_abstraction_by_index(ANVIL__generation_workspace* workspace, ANVIL__abstraction_index index) {
    return ((ANVIL__generation_abstraction*)(*workspace).abstractions.buffer.start)[index];
}

// get abstraction from abstraction list
ANVIL__accountling_abstraction ANVIL__find__accountling_abstraction_by_index(ANVIL__accountling_program program, ANVIL__abstraction_index index) {
    return ((ANVIL__accountling_abstraction*)program.abstractions.buffer.start)[index];
}

// convert variable index to cell ID
ANVIL__cell_ID ANVIL__translate__accountling_variable_index_to_cell_ID(ANVIL__generation_abstraction* generation_abstraction, ANVIL__accountling_argument argument, ANVIL__error* error) {
    ANVIL__pvt predefineds[] = {
        ANVIL__rt__error_code,
        ANVIL__srt__constant__1,
        ANVIL__srt__constant__8,
        ANVIL__srt__constant__bits_in_byte,
        ANVIL__srt__constant__cell_byte_count,
        ANVIL__srt__constant__cell_bit_count,
        ANVIL__srt__constant__true,
        ANVIL__srt__constant__false,
        ANVIL__srt__constant__0,
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
        ANVIL__srt__input_buffer_start,
        ANVIL__srt__input_buffer_end,
        ANVIL__srt__output_buffer_start,
        ANVIL__srt__output_buffer_end,
        ANVIL__srt__stack__start_address,
        ANVIL__srt__stack__current_address,
        ANVIL__srt__stack__end_address,
    };

    // convert based on type
    if (argument.type == ANVIL__pat__variable || argument.type == ANVIL__pat__variable__body) {
        // return cell ID
        return generation_abstraction->cells.workspace_body_range.start + argument.index;
    } else if (argument.type == ANVIL__pat__variable__input) {
        // return cell ID
        return generation_abstraction->cells.workspace_input_range.start + argument.index;
    } else if (argument.type == ANVIL__pat__variable__output) {
        // return cell ID
        return generation_abstraction->cells.workspace_output_range.start + argument.index;
    } else if (argument.type == ANVIL__pat__variable__predefined) {
        // return cell ID from list
        return predefineds[argument.index];
    }
    
    // error
    *error = ANVIL__open__error("Internal Error: Unsupported variable index type.", ANVIL__create_null__character_location());

    return ANVIL__define__null_offset_ID;
}

// convert flag index to flag ID
ANVIL__flag_ID ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__accountling_argument argument, ANVIL__error* error) {
    if (argument.type == ANVIL__pat__flag__predefined) {
        return argument.index;
    } else if (argument.type == ANVIL__pat__flag__user_defined) {
        return ANVIL__pft__COUNT + argument.index;
    }

    // error
    *error = ANVIL__open__error("Internal Error: Flag is not a usable type (aka, not a predefined flag or user defined flag), oops.", ANVIL__create_null__character_location());

    return ANVIL__define__null_flag;
}

// generate function
void ANVIL__forge__anvil_abstraction(ANVIL__generation_workspace* workspace, ANVIL__generation_abstraction* generation_abstraction, ANVIL__accountling_abstraction accountling_abstraction, ANVIL__error* error) {
    ANVIL__string_index current_string_ID = 0;

    // setup offset
    (*generation_abstraction).offsets.function_start = ANVIL__get__offset((*workspace).workspace);

    // setup function prologue
    ANVIL__code__preserve_workspace((*workspace).workspace, ANVIL__sft__always_run, generation_abstraction->cells.workspace_total_range.start, generation_abstraction->cells.workspace_total_range.end);

    // get inputs
    for (ANVIL__input_count i = 0; i < generation_abstraction->cells.input_count; i++) {
        // pass input
        ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, generation_abstraction->cells.function_input_range.start + i, generation_abstraction->cells.workspace_input_range.start + i);
    }

    // translate statements
    // setup current
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&accountling_abstraction.statements);

    // for each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // get statement
        ANVIL__accountling_statement statement = *(ANVIL__accountling_statement*)current_statement.start;

        // build statement
        // if statement is call
        if (statement.type == ANVIL__stt__abstraction_call) {
            // if statement is instruction
            if (statement.header.call_index < ANVIL__act__user_defined) {
                // determine instruction type & write instruction
                switch ((ANVIL__act)statement.header.call_index) {
                case ANVIL__act__set__boolean:
                case ANVIL__act__set__binary:
                case ANVIL__act__set__integer:
                case ANVIL__act__set__hexadecimal:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0).index, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__set__offset:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(((ANVIL__offset*)generation_abstraction->offsets.statement_offsets.buffer.start)[ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0).index]), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__set__flag_ID:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__set__string:
                    // write string data
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)((ANVIL__offset*)generation_abstraction->offsets.strings_offsets.buffer.start)[current_string_ID], ANVIL__srt__temp__offset);
                    ANVIL__code__retrieve_embedded_buffer(workspace->workspace, ANVIL__sft__always_run, ANVIL__srt__temp__offset, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    // next string ID
                    current_string_ID++;

                    break;
                case ANVIL__act__io__cell_to_address:
                    ANVIL__code__cell_to_address(workspace->workspace, ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__io__address_to_cell:
                    ANVIL__code__address_to_cell(workspace->workspace, ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__io__file_to_buffer:
                    ANVIL__code__file_to_buffer(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__io__buffer_to_file:
                    ANVIL__code__buffer_to_file(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__copy__cell:
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__copy__buffer:
                    ANVIL__code__buffer_to_buffer(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__memory__request_memory:
                    ANVIL__code__request_memory(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__memory__return_memory:
                    ANVIL__code__return_memory(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error));

                    break;
                case ANVIL__act__buffer__calculate_length:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_subtract, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__constant__1, ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__cast__cell_to_unsigned_integer_string:
                    ANVIL__standard__code__call__cell_to_unsigned_integer_string(workspace->workspace, &workspace->standard_offsets, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__print__unsigned_integer:
                    ANVIL__code__debug__print_cell_as_decimal(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error));

                    break;
                case ANVIL__act__print__character:
                    ANVIL__code__debug__putchar(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error));

                    break;
                case ANVIL__act__print__buffer_as_string:
                    ANVIL__standard__code__call__print_buffer_as_string(workspace->workspace, &workspace->standard_offsets, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error));

                    break;
                case ANVIL__act__print__binary:
                    ANVIL__standard__code__call__print_binary(workspace->workspace, &workspace->standard_offsets, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error));

                    break;
                case ANVIL__act__integer__add:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_add, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__subtract:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_subtract, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__multiply:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_multiply, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__divide:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_division, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__modulous:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__integer_modulous, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__integer__within_range:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__operate__flag(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error), ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 3), error), ANVIL__srt__temp__flag_ID_0);

                    break;
                case ANVIL__act__binary__or:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_or, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__invert:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_invert, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__and:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_and, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__xor:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_xor, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__shift_higher:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_shift_higher, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__shift_lower:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_shift_lower, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__binary__overwrite:
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__bits_overwrite, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__flag__get:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_get, ANVIL__srt__temp__flag_ID_0, ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error));

                    break;
                case ANVIL__act__flag__set:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_set, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_0);

                    break;
                case ANVIL__act__flag__invert:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_1);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_invert, ANVIL__srt__temp__flag_ID_0, ANVIL__unused_cell_ID, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_1);

                    break;
                case ANVIL__act__flag__or:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__srt__temp__flag_ID_1);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_2);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_or, ANVIL__srt__temp__flag_ID_0, ANVIL__srt__temp__flag_ID_1, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_2);

                    break;
                case ANVIL__act__flag__and:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__srt__temp__flag_ID_1);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_2);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_and, ANVIL__srt__temp__flag_ID_0, ANVIL__srt__temp__flag_ID_1, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_2);

                    break;
                case ANVIL__act__flag__xor:
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__temp__flag_ID_0);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__srt__temp__flag_ID_1);
                    ANVIL__code__write_cell(workspace->workspace, (ANVIL__cell)(ANVIL__cell_integer_value)ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__srt__temp__flag_ID_2);
                    ANVIL__code__operate(workspace->workspace, ANVIL__sft__always_run, ANVIL__ot__flag_xor, ANVIL__srt__temp__flag_ID_0, ANVIL__srt__temp__flag_ID_1, ANVIL__unused_cell_ID, ANVIL__srt__temp__flag_ID_2);

                    break;
                case ANVIL__act__jump:
                    ANVIL__code__jump__static(workspace->workspace, ANVIL__translate__accountling_flag_index_to_flag_ID(ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), (((ANVIL__offset*)generation_abstraction->offsets.statement_offsets.buffer.start)[ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1).index]));

                    break;
                case ANVIL__act__open_context:
                    ANVIL__code__setup__context(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error));

                    break;
                case ANVIL__act__compile:
                    ANVIL__code__compile(
                        workspace->workspace,
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 0), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 1), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 2), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 3), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 4), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 5), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 6), error),
                        ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, 7), error)
                    );

                    break;
                case ANVIL__act__run:
                    ANVIL__code__run(workspace->workspace, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 2), error));

                    break;
                case ANVIL__act__reset_error_code_cell:
                    ANVIL__code__write_cell(workspace->workspace, ANVIL__et__no_error, ANVIL__rt__error_code);

                    break;
                case ANVIL__act__set_program_outputs:
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 0), error), ANVIL__srt__output_buffer_start);
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, 1), error), ANVIL__srt__output_buffer_end);

                    break;
                default:
                    // should not be reachable
                    break;
                }
            // if statement is abstraction call
            } else {
                // pass inputs
                for (ANVIL__input_count i = 0; i < ANVIL__calculate__list_content_count(statement.inputs, sizeof(ANVIL__accountling_argument)); i++) {
                    // pass input
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.inputs, i), error), generation_abstraction->cells.function_input_range.start + i);
                }

                // call function
                ANVIL__code__call__static(workspace->workspace, ANVIL__sft__always_run, ((ANVIL__generation_abstraction*)workspace->abstractions.buffer.start)[ANVIL__calculate__abstraction_index_from_call_index(statement.header.call_index)].offsets.function_start);

                // pass outputs
                for (ANVIL__output_count i = 0; i < ANVIL__calculate__list_content_count(statement.outputs, sizeof(ANVIL__accountling_argument)); i++) {
                    // pass output
                    ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, generation_abstraction->cells.function_output_range.start + i, ANVIL__translate__accountling_variable_index_to_cell_ID(generation_abstraction, ANVIL__get__abstractling_statement_argument_by_index(statement.outputs, i), error));
                }
            }
        // statement is offset
        } else if (statement.type == ANVIL__stt__offset) {
            // declare offset
            ((ANVIL__offset*)generation_abstraction->offsets.statement_offsets.buffer.start)[statement.offset_ID] = ANVIL__get__offset(workspace->workspace);
        // unrecognized statement type, error
        } else {
            *error = ANVIL__open__error("Internal Error: In code generator, invalid statement type, oops.", statement.header.header.name.text.lexling.location);

            return;
        }

        // check for error
        if (ANVIL__check__error_occured(error)) {
            return;
        }

        // next statement
        current_statement.start += sizeof(ANVIL__accountling_statement);
    }

    // setup function return offset
    generation_abstraction->offsets.function_return = ANVIL__get__offset(workspace->workspace);

    // pass outputs
    for (ANVIL__output_count i = 0; i < generation_abstraction->cells.output_count; i++) {
        // pass output
        ANVIL__code__cell_to_cell(workspace->workspace, ANVIL__sft__always_run, generation_abstraction->cells.workspace_output_range.start + i, generation_abstraction->cells.function_output_range.start + i);
    }

    // setup function epilogue
    ANVIL__code__restore_workspace(workspace->workspace, ANVIL__sft__always_run, generation_abstraction->cells.workspace_total_range.start, generation_abstraction->cells.workspace_total_range.end);

    // return to caller
    ANVIL__code__jump__explicit(workspace->workspace, ANVIL__sft__always_run, ANVIL__srt__return_address);

    // setup data offset
    generation_abstraction->offsets.function_data = ANVIL__get__offset(workspace->workspace);

    // setup strings
    for (ANVIL__string_index i = 0; i < ANVIL__calculate__list_content_count(generation_abstraction->converted_strings, sizeof(ANVIL__buffer)); i++) {
        // setup offset
        ((ANVIL__offset*)generation_abstraction->offsets.strings_offsets.buffer.start)[i] = ANVIL__get__offset(workspace->workspace);

        // embed string
        ANVIL__code__buffer(workspace->workspace, ((ANVIL__buffer*)generation_abstraction->converted_strings.buffer.start)[i]);
    }

    return;
}

// generate program
void ANVIL__forge__anvil_program(ANVIL__buffer* final_program, ANVIL__accountling_program accountlings, ANVIL__stack_size stack_size, ANVIL__error* error) {
    ANVIL__generation_workspace workspace;
    
    // setup generation workspace
    ANVIL__open__generation_workspace(final_program, accountlings, &workspace, error);
    if (ANVIL__check__error_occured(error)) {
        return;
    }

    // forge program
    for (ANVIL__pt pass = ANVIL__pt__get_offsets; pass < ANVIL__pt__COUNT; pass++) {
        // setup pass
        ANVIL__setup__pass(workspace.workspace, pass);

        // build program
        // setup kickstarter
        ANVIL__code__start(&workspace.anvil, stack_size, ANVIL__find__generation_abstraction_by_index(&workspace, workspace.entry_point).offsets.function_start);

        // weave user defined abstractions
        // setup current
        ANVIL__current current_abstraction = ANVIL__calculate__current_from_list_filled_index(&workspace.abstractions);
        ANVIL__abstraction_index index = 0;

        // for each abstraction
        while (ANVIL__check__current_within_range(current_abstraction)) {
            // weave abstraction
            ANVIL__forge__anvil_abstraction(&workspace, (ANVIL__generation_abstraction*)current_abstraction.start, ANVIL__find__accountling_abstraction_by_index(accountlings, index), error);

            // next abstraction
            current_abstraction.start += sizeof(ANVIL__generation_abstraction);
            index++;
        }

        // forge built in functions
        ANVIL__standard__code__package(workspace.workspace, &workspace.standard_offsets);
    }

    // close workspace
    ANVIL__close__generation_workspace(workspace);

    return;
}

/* Compile */
// one compiled object across multiple stages
typedef struct ANVIL__compilation_unit {
    ANVIL__buffer user_codes;
    ANVIL__list lexling_buffers;
    ANVIL__list parsling_buffers;
    ANVIL__accountling_program accountlings;
    ANVIL__pst stages_completed;
} ANVIL__compilation_unit;

// close a compilation unit
void ANVIL__close__compilation_unit(ANVIL__compilation_unit unit) {
    // close lexling buffers
    if (unit.stages_completed > ANVIL__pst__invalid) {
        // setup current
        ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&unit.lexling_buffers);

        // close each buffer
        while (ANVIL__check__current_within_range(current)) {
            // close parsling buffer
            ANVIL__close__lexlings(*(ANVIL__lexlings*)current.start);

            // next program
            current.start += sizeof(ANVIL__lexlings);
        }

        // close parslings buffer
        ANVIL__close__list(unit.lexling_buffers);
    }

    // close parsling buffers
    if (unit.stages_completed > ANVIL__pst__lexing) {
        // setup current
        ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&unit.parsling_buffers);

        // close each buffer
        while (ANVIL__check__current_within_range(current)) {
            // close parsling buffer
            ANVIL__close__parsling_program(*(ANVIL__parsling_program*)current.start);

            // next program
            current.start += sizeof(ANVIL__parsling_program);
        }

        // close parslings buffer
        ANVIL__close__list(unit.parsling_buffers);
    }

    // close accountling data
    if (unit.stages_completed > ANVIL__pst__parsing) {
        ANVIL__close__accountling_program(unit.accountlings);
    }

    return;
}

// compile a program
void ANVIL__compile__files(ANVIL__buffer user_codes, ANVIL__bt print_debug, ANVIL__buffer* final_program, ANVIL__error* error) {
    ANVIL__compilation_unit compilation_unit;
    ANVIL__current current;
    ANVIL__file_index current_file_index = 0;

    // setup blank error
    *error = ANVIL__create_null__error();

    // check for empty buffer
    if (ANVIL__check__empty_buffer(user_codes)) {
        // set error
        *error = ANVIL__open__error("Compilation Error: No source files were passed.", ANVIL__create_null__character_location());

        goto quit;
    }

    // setup compilation unit user code list and lexling list
    compilation_unit.user_codes = user_codes;
    compilation_unit.lexling_buffers = ANVIL__open__list_with_error(sizeof(ANVIL__lexlings) * 5, error);
    compilation_unit.parsling_buffers = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_program) * 5, error);
    compilation_unit.stages_completed = ANVIL__pst__invalid;

    // check for error
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // setup current
    current = compilation_unit.user_codes;

    // print compilation message
    if (print_debug) {
        printf("Compiling Files.\n");
    }

    // lex and parse each file
    while (ANVIL__check__current_within_range(current)) {
        // get file
        ANVIL__buffer user_code = *(ANVIL__buffer*)current.start;

        // inform user of compilation start
        if (print_debug) {
            printf("Compiling User Code:\n");
            ANVIL__print__buffer(user_code);
            printf("\n");
        }

        // lex file
        ANVIL__lexlings lexlings = ANVIL__compile__lex(user_code, current_file_index, error);

        // append lexlings
        ANVIL__append__lexlings(&compilation_unit.lexling_buffers, lexlings, error);

        // print lexlings
        if (print_debug) {
            ANVIL__debug__print_lexlings(lexlings);
        }

        // mark as done (unfinished input acceptable)
        if (compilation_unit.stages_completed < ANVIL__pst__lexing) {
            compilation_unit.stages_completed = ANVIL__pst__lexing;
        }

        // check for error
        if (ANVIL__check__error_occured(error)) {
            goto quit;
        }

        // parse file
        ANVIL__parsling_program parslings = ANVIL__parse__program(lexlings, error);

        // append program
        ANVIL__append__parsling_program(&compilation_unit.parsling_buffers, parslings, error);

        // print parslings
        if (print_debug) {
            ANVIL__print__parsed_program(parslings);
        }

        // mark as done (unfinished input acceptable)
        if (compilation_unit.stages_completed < ANVIL__pst__parsing) {
            compilation_unit.stages_completed = ANVIL__pst__parsing;
        }

        // check for error
        if (ANVIL__check__error_occured(error)) {
            goto quit;
        }

        // advance current
        current.start += sizeof(ANVIL__buffer);
        current_file_index++;
    }

    // account
    compilation_unit.accountlings = ANVIL__account__program(compilation_unit.parsling_buffers, error);

    // mark as done
    compilation_unit.stages_completed = ANVIL__pst__accounting;

    // check for errors
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // print accountlings
    if (print_debug) {
        ANVIL__print__accountling_program(compilation_unit.accountlings);
    }

    // inform user of code generation
    if (print_debug) {
        printf("Generating code...\n");
    }

    // generate program
    ANVIL__forge__anvil_program(final_program, compilation_unit.accountlings, 65536, error);

    // quit label
    quit:

    // clean up
    ANVIL__close__compilation_unit(compilation_unit);

    return;
}

#endif
