#ifndef DRAGON__compiler__parser
#define DRAGON__compiler__parser

/* Include */
#include "lexer.h"

/* Parser */
// namespace struct
typedef struct ANVIL__namespace {
    ANVIL__list lexlings; // ANVIL__lexling
    ANVIL__lexling_index count;
} ANVIL__namespace;

// create name
ANVIL__namespace ANVIL__create__namespace(ANVIL__list lexlings, ANVIL__lexling_index count) {
    ANVIL__namespace output;

    output.lexlings = lexlings;
    output.count = count;

    return output;
}

// create null namespace
ANVIL__namespace ANVIL__create_null__namespace() {
    return ANVIL__create__namespace(ANVIL__create_null__list(), 0);
}

// parsling argument
typedef struct ANVIL__parsling_argument {
    // definition
    ANVIL__gat category;
    ANVIL__namespace namespace;

    // value
    ANVIL__cell_integer_value cellular_value;
} ANVIL__parsling_argument;

// create a custom argument
ANVIL__parsling_argument ANVIL__create__parsling_argument(ANVIL__gat category, ANVIL__namespace namespace, ANVIL__cell_integer_value cellular_value) {
    ANVIL__parsling_argument output;

    // setup output
    output.category = category;
    output.namespace = namespace;
    output.cellular_value = cellular_value;

    return output;
}

// setup null argument
ANVIL__parsling_argument ANVIL__create_null__parsling_argument() {
    return ANVIL__create__parsling_argument(ANVIL__gat__invalid, ANVIL__create_null__namespace(), 0);
}

// statement type
typedef enum ANVIL__stt {
    ANVIL__stt__invalid,

    // statements
    ANVIL__stt__function_call,
    ANVIL__stt__offset,

    // headers
    ANVIL__stt__function_header,
    ANVIL__stt__structure_header,

    // count
    ANVIL__stt__COUNT,
} ANVIL__stt;

// one statement
typedef struct ANVIL__parsling_statement {
    // statement type
    ANVIL__stt type;

    // name
    ANVIL__parsling_argument name;

    // function call data
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

// one structure
typedef struct ANVIL__parsling_structure {
    ANVIL__parsling_statement header;
} ANVIL__parsling_structure;

// create a custom structure
ANVIL__parsling_structure ANVIL__create__parsling_structure(ANVIL__parsling_statement header) {
    ANVIL__parsling_structure output;

    // setup output
    output.header = header;

    return output;
}

// create a null structure
ANVIL__parsling_structure ANVIL__create_null__parsling_structure() {
    // return empty
    return ANVIL__create__parsling_structure(ANVIL__create_null__parsling_statement());
}

// one function
typedef struct ANVIL__parsling_function {
    ANVIL__parsling_statement header;
    ANVIL__list statements; // ANVIL__parsling_statement
} ANVIL__parsling_function;

// create a custom function
ANVIL__parsling_function ANVIL__create__parsling_function(ANVIL__parsling_statement header, ANVIL__list statements) {
    ANVIL__parsling_function output;

    // setup output
    output.header = header;
    output.statements = statements;

    return output;
}

// create a null function
ANVIL__parsling_function ANVIL__create_null__parsling_function() {
    // return empty
    return ANVIL__create__parsling_function(ANVIL__create_null__parsling_statement(), ANVIL__create_null__list());
}

// one program
typedef struct ANVIL__parsling_program {
    ANVIL__list functions; // ANVIL__parsling_function
    ANVIL__list structures; // ANVIL__parsling_structure
} ANVIL__parsling_program;

// create a custom program
ANVIL__parsling_program ANVIL__create__parsling_program(ANVIL__list functions) {
    ANVIL__parsling_program output;

    // setup output
    output.functions = functions;

    return output;
}

// create a null program
ANVIL__parsling_program ANVIL__create_null__parsling_program() {
    // return empty
    return ANVIL__create__parsling_program(ANVIL__create_null__list());
}

// append namespace
void ANVIL__append__namespace(ANVIL__list* list, ANVIL__namespace data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__namespace), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__namespace*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__namespace);

    return;
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

// append parsling structure
void ANVIL__append__parsling_structure(ANVIL__list* list, ANVIL__parsling_structure data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_structure), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_structure*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_structure);

    return;
}

// append parsling function
void ANVIL__append__parsling_function(ANVIL__list* list, ANVIL__parsling_function data, ANVIL__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(ANVIL__parsling_function), &(*error).memory_error_occured);

    // append data
    (*(ANVIL__parsling_function*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(ANVIL__parsling_function);

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

// close namespace
void ANVIL__close__parsling_namespace(ANVIL__namespace namespace) {
    // close list
    ANVIL__close__list(namespace.lexlings);

    return;
}

// close argument
void ANVIL__close__parsling_argument(ANVIL__parsling_argument argument) {
    // close namespace
    ANVIL__close__parsling_namespace(argument.namespace);

    return;
}

// close arguments
void ANVIL__close__parsling_arguments(ANVIL__list* arguments) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(arguments);

    // close arguments
    while (ANVIL__check__current_within_range(current)) {
        // close argument
        ANVIL__close__parsling_argument(*(ANVIL__parsling_argument*)current.start);

        // next argument
        current.start += sizeof(ANVIL__parsling_argument);
    }

    // close arguments buffer
    ANVIL__close__list(*arguments);

    return;
}

// close statement
void ANVIL__close__parsling_statement(ANVIL__parsling_statement statement) {
    // close name
    ANVIL__close__parsling_argument(statement.name);

    // close io
    if (ANVIL__check__empty_list(statement.inputs) == ANVIL__bt__false) {
        ANVIL__close__parsling_arguments(&statement.inputs);
    }
    if (ANVIL__check__empty_list(statement.outputs) == ANVIL__bt__false) {
        ANVIL__close__parsling_arguments(&statement.outputs);
    }

    return;
}

// close statements
void ANVIL__close__parsling_statements(ANVIL__list* list) {
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(list);

    // clean up each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // clean up statement
        ANVIL__close__parsling_statement(*(ANVIL__parsling_statement*)current_statement.start);

        // next statement
        current_statement.start += sizeof(ANVIL__parsling_statement);
    }

    // clean up statements buffer
    ANVIL__close__list(*list);

    return;
}

// close structure
void ANVIL__close__parsling_structure(ANVIL__parsling_structure structure) {
    // close header
    ANVIL__close__parsling_statement(structure.header);

    return;
}

// close function
void ANVIL__close__parsling_function(ANVIL__parsling_function function) {
    // close header
    ANVIL__close__parsling_statement(function.header);
    
    // close statements
    if (ANVIL__check__empty_list(function.statements) == ANVIL__bt__false) {
        ANVIL__close__parsling_statements(&function.statements);
    }

    return;
}

// close parsling program
void ANVIL__close__parsling_program(ANVIL__parsling_program program) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&(program.functions));

    // free each function
    while (ANVIL__check__current_within_range(current)) {
        // free function
        ANVIL__close__parsling_function(*(ANVIL__parsling_function*)current.start);

        // next function
        current.start += sizeof(ANVIL__parsling_function);
    }
    
    // next current
    current = ANVIL__calculate__current_from_list_filled_index(&(program.structures));

    // free each structure
    while (ANVIL__check__current_within_range(current)) {
        // free structure
        ANVIL__close__parsling_structure(*(ANVIL__parsling_structure*)current.start);

        // next structure
        current.start += sizeof(ANVIL__parsling_structure);
    }

    // free the lists
    ANVIL__close__list(program.functions);
    ANVIL__close__list(program.structures);

    return;
}

/*// create a name from a lexing
ANVIL__name ANVIL__create__name_from_lexling_current(ANVIL__current lexling_current) {
    ANVIL__name output;

    // setup
    output.lexling = ANVIL__read__lexling_from_current(lexling_current);

    return output;
}*/

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

// parse a namepspace with only one name
ANVIL__namespace ANVIL__parse__namespace__one_name_only(ANVIL__current* current, ANVIL__error* error) {
    ANVIL__namespace output;

    // open lexling list
    output.lexlings = ANVIL__open__list_with_error(sizeof(ANVIL__lexling) * 8, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // setup count
    output.count = 0;

    // check for name
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
        // append lexling
        ANVIL__append__lexling(&output.lexlings, ANVIL__read__lexling_from_current(*current), error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // one lexling
        output.count++;
    // not found, error
    } else {
        // set error
        *error = ANVIL__open__error("Parsing Error: Expected a name.", ANVIL__read__lexling_from_current(*current).location);
    }

    return output;
}

// parse namespace
ANVIL__namespace ANVIL__parse__namespace(ANVIL__current* current, ANVIL__error* error) {
    ANVIL__namespace output;

    // open lexling list
    output.lexlings = ANVIL__open__list_with_error(sizeof(ANVIL__lexling) * 8, error);
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // setup count
    output.count = 0;

    // check for initial name
    if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
        // append lexling
        ANVIL__append__lexling(&output.lexlings, ANVIL__read__lexling_from_current(*current), error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // count lexling
        output.count++;

        // next lexling
        ANVIL__advance__lexling_current(current, 1);

        // get other lexlings
        while (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__colon) {
            // skip past colon
            ANVIL__advance__lexling_current(current, 1);

            // get name
            if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
                // append name
                ANVIL__append__lexling(&output.lexlings, ANVIL__read__lexling_from_current(*current), error);
                if (ANVIL__check__error_occured(error)) {
                    return output;
                }

                // count lexling
                output.count++;

                // next lexling
                ANVIL__advance__lexling_current(current, 1);
            // name missing, error
            } else {
                // set error
                *error = ANVIL__open__error("Parsing Error: Expected a name.", ANVIL__read__lexling_from_current(*current).location);

                return output;
            }
        }
    // if it is a string
    } else if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__string_literal) {
        // append string
        ANVIL__append__lexling(&output.lexlings, ANVIL__read__lexling_from_current(*current), error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // next lexling
        ANVIL__advance__lexling_current(current, 1);
    // not found, error
    } else {
        // set error
        *error = ANVIL__open__error("Parsing Error: Expected a name.", ANVIL__read__lexling_from_current(*current).location);
    }

    return output;
}

// parse one argument
ANVIL__parsling_argument ANVIL__parse__function_argument(ANVIL__current* current, ANVIL__bt is_function_header_argument, ANVIL__error* error) {
    ANVIL__parsling_argument output = ANVIL__create_null__parsling_argument();
    //ANVIL__cell_integer_value value = 0;

    // check type
    // is variable / literal
    if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
        // parse namespace
        output = ANVIL__create__parsling_argument(ANVIL__gat__variable, ANVIL__parse__namespace(current, error), 0);
        
        /*// translate literal information for accounting to translate
        // boolean
        if (ANVIL__translate__string_to_boolean(ANVIL__read__lexling_from_current(*current).value, &value)) {
            argument = ANVIL__create__parsling_argument(ANVIL__gat__literal__boolean, ANVIL__parse__namespace__one_name_only(current, error), ANVIL__bt__false, ANVIL__create_null__namespace(), value);
        // binary
        } else if (ANVIL__translate__string_to_binary(ANVIL__read__lexling_from_current(*current).value, &value)) {
            argument = ANVIL__create__parsling_argument(ANVIL__gat__literal__binary, ANVIL__parse__namespace__one_name_only(current, error), ANVIL__bt__false, ANVIL__create_null__namespace(), value);
        // integer
        } else if (ANVIL__translate__string_to_integer(ANVIL__read__lexling_from_current(*current).value, &value)) {
            argument = ANVIL__create__parsling_argument(ANVIL__gat__literal__integer, ANVIL__parse__namespace__one_name_only(current, error), ANVIL__bt__false, ANVIL__create_null__namespace(), value);
        // hexadecimal
        } else if (ANVIL__translate__string_to_hexedecimal(ANVIL__read__lexling_from_current(*current).value, &value)) {
            argument = ANVIL__create__parsling_argument(ANVIL__gat__literal__hexadecimal, ANVIL__parse__namespace__one_name_only(current, error), ANVIL__bt__false, ANVIL__create_null__namespace(), value);
        }*/
    // is type definition
    } else if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__exclamation_point) {
        // next lexling
        ANVIL__advance__lexling_current(current, 1);

        // parse namespace
        output = ANVIL__create__parsling_argument(ANVIL__gat__type, ANVIL__parse__namespace(current, error), 0);
    // offset
    } else if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__at) {
        // advance current past at
        ANVIL__advance__lexling_current(current, 1);

        // if correct type
        if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
            // get name
            output = ANVIL__create__parsling_argument(ANVIL__gat__offset, ANVIL__parse__namespace(current, error), 0);
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
            output = ANVIL__create__parsling_argument(ANVIL__gat__flag, ANVIL__parse__namespace(current, error), 0);
        // error
        } else {
            *error = ANVIL__open__error("Parse Error: Flag is missing name.", ANVIL__read__lexling_from_current(*current).location);

            return output;
        }
    // string literal
    } else if (ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__string_literal) {
        // get argument
        output = ANVIL__create__parsling_argument(ANVIL__gat__literal__string, ANVIL__parse__namespace(current, error), 0);
    // error
    } else {
        *error = ANVIL__open__error("Parse Error: Unrecognized argument type.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    // check argument for variable only
    if (is_function_header_argument == ANVIL__bt__true && (output.category != ANVIL__gat__variable && output.category != ANVIL__gat__type)) {
        // set error
        *error = ANVIL__open__error("Parse Error: A non-variable / non-type argument was detected in a header.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    // check for error
    if (ANVIL__check__error_occured(error)) {
        return output;
    }

    // next current
    //ANVIL__advance__lexling_current(current, 1);

    return output;
}

// parse function call statement arguments
ANVIL__list ANVIL__parse__function_call_statement_arguments(ANVIL__current* current, ANVIL__io_count* count, ANVIL__bt is_function_header_argument, ANVIL__error* error) {
    ANVIL__list output;

    // init count
    *count = 0;

    // open names list
    output = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_argument) * 8, error);
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
        // parse argument
        ANVIL__parsling_argument argument = ANVIL__parse__function_argument(current, is_function_header_argument, error);
        if (ANVIL__check__error_occured(error)) {
            return output;
        }

        // append argument
        ANVIL__append__parsling_argument(&output, argument, error);
    
        // increment count
        *count = *count + 1;
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
            output.name = ANVIL__create__parsling_argument(ANVIL__gat__offset, ANVIL__parse__namespace(current, error), 0);

            // advance current
            //ANVIL__advance__lexling_current(current, 1);
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
    // is a function call
    } else if (ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__name) {
        // get name
        output.name = ANVIL__create__parsling_argument(ANVIL__gat__offset, ANVIL__parse__namespace(current, error), 0);
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // advance current
        //ANVIL__advance__lexling_current(current, 1);

        // get inputs
        output.inputs = ANVIL__parse__function_call_statement_arguments(current, &input_count, is_header, error);
        output.input_count = input_count;
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // get outputs
        output.outputs = ANVIL__parse__function_call_statement_arguments(current, &output_count, is_header, error);
        output.output_count = output_count;
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // set type
        if (is_header == ANVIL__bt__true) {
            output.type = ANVIL__stt__function_header;
        } else {
            output.type = ANVIL__stt__function_call;
        }
    // is a structure declaration
    } else if (is_header == ANVIL__bt__true && ANVIL__check__current_within_range(*current) && ANVIL__read__lexling_from_current(*current).type == ANVIL__lt__exclamation_point) {
        // pass exclamation point
        ANVIL__advance__lexling_current(current, 1);

        // parse namespace
        output.name = ANVIL__create__parsling_argument(ANVIL__gat__type, ANVIL__parse__namespace(current, error), 0);
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // parse inputs
        output.inputs = ANVIL__parse__function_call_statement_arguments(current, &input_count, is_header, error);
        output.input_count = input_count;
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // set type
        output.type = ANVIL__stt__structure_header;
    // error
    } else {
        // set error
        *error = ANVIL__open__error("Parse Error: Unrecognized statement type.", ANVIL__read__lexling_from_current(*current).location);

        return output;
    }

    return output;
}

// parse a structure
ANVIL__parsling_structure ANVIL__parse__structure(ANVIL__current* current, ANVIL__error* error) {
    ANVIL__parsling_structure output;

    // parse header
    output.header = ANVIL__parse__statement(current, ANVIL__bt__true, error);

    return output;
}

// parse a function
ANVIL__parsling_function ANVIL__parse__function(ANVIL__current* current, ANVIL__error* error) {
    ANVIL__parsling_function output = ANVIL__create_null__parsling_function();

    // parse header
    output.header = ANVIL__parse__statement(current, ANVIL__bt__true, error);
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
        *error = ANVIL__open__error("Parse Error: A function definition has an equals sign missing.", ANVIL__read__lexling_from_current(*current).location);

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

// parse a program (file)
ANVIL__parsling_program ANVIL__parse__program(ANVIL__lexlings lexlings, ANVIL__error* error) {
    ANVIL__parsling_program output;
    ANVIL__parsling_function temp_function;
    ANVIL__parsling_structure temp_structure;
    ANVIL__current current;

    // setup current
    current = ANVIL__calculate__current_from_list_filled_index(&lexlings.data);

    // open the function list
    output.functions = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_function) * 64, error);
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // open the structure list
    output.structures = ANVIL__open__list_with_error(sizeof(ANVIL__parsling_structure) * 64, error);
    if (ANVIL__check__error_occured(error)) {
        goto quit;
    }

    // parse functions & structures
    while (ANVIL__check__current_within_range(current)) {
        // if end of file
        if (ANVIL__read__lexling_from_current(current).type == ANVIL__lt__end_of_file) {
            // finished parsing
            goto quit;
        }
        
        // determine abstraction type
        if (ANVIL__read__lexling_from_current(current).type == ANVIL__lt__exclamation_point) {
            // parse structure
            temp_structure = ANVIL__parse__structure(&current, error);

            // append structure
            ANVIL__append__parsling_structure(&(output.structures), temp_structure, error);
        } else {
            // parse function
            temp_function = ANVIL__parse__function(&current, error);

            // append function
            ANVIL__append__parsling_function(&(output.functions), temp_function, error);
        }

        // check for error
        if (ANVIL__check__error_occured(error) == ANVIL__bt__true) {
            goto quit;
        }
    }

    // quit
    quit:

    return output;
}

// print namespace
void ANVIL__print__namespace(ANVIL__namespace namespace) {
    ANVIL__current current_lexling;

    // setup current
    current_lexling = ANVIL__calculate__current_from_list_filled_index(&namespace.lexlings);

    // print starting name lexling
    ANVIL__print__buffer(ANVIL__read__lexling_from_current(current_lexling).value);
    current_lexling.start += sizeof(ANVIL__lexling);

    // for each remaining lexling
    while (ANVIL__check__current_within_range(current_lexling)) {
        // print colon
        printf(":");

        // print name
        ANVIL__print__buffer(ANVIL__read__lexling_from_current(current_lexling).value);
        current_lexling.start += sizeof(ANVIL__lexling);
    }

    return;
}

// print argument
void ANVIL__print__parsling_argument(ANVIL__parsling_argument argument) {
    // print type
    printf("[");
    ANVIL__print__buffer(ANVIL__convert__general_argument_type_to_string_buffer(argument.category));
    printf("]");

    // print data
    if (argument.category == ANVIL__gat__variable || argument.category == ANVIL__gat__variable__input || argument.category == ANVIL__gat__variable__output || argument.category == ANVIL__gat__variable__body || argument.category == ANVIL__gat__variable__predefined || argument.category == ANVIL__gat__offset || argument.category == ANVIL__gat__flag || argument.category == ANVIL__gat__literal__string || argument.category == ANVIL__gat__type || argument.category == ANVIL__gat__type__structure || argument.category == ANVIL__gat__type__from_variable || argument.category == ANVIL__gat__type__predefined) {
        ANVIL__print__namespace(argument.namespace);
    } else if (argument.category == ANVIL__gat__literal__boolean || argument.category == ANVIL__gat__literal__binary || argument.category == ANVIL__gat__literal__hexadecimal) {
        ANVIL__print__namespace(argument.namespace);
        printf("[%lu]", argument.cellular_value);
    } else if (argument.category == ANVIL__gat__literal__integer) {
        ANVIL__print__namespace(argument.namespace);
        printf("[%lu, %li]", argument.cellular_value, argument.cellular_value);
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
        ANVIL__print__namespace(statement.name.namespace);
    } else if (statement.type == ANVIL__stt__function_call || statement.type == ANVIL__stt__function_header) {
        // print statement name
        ANVIL__print__namespace(statement.name.namespace);

        // print inputs
        ANVIL__print__parsling_arguments(&statement.inputs);

        // print outputs
        ANVIL__print__parsling_arguments(&statement.outputs);
    }

    return;
}

// print a function
void ANVIL__print__parsed_function(ANVIL__parsling_function function) {
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&function.statements);

    // print header
    printf("\tFunction: ");
    if (function.header.type != ANVIL__stt__invalid) {
        ANVIL__print__parsed_statement(function.header);
    }

    // new line for statements
    printf("\n");

    // print statements
    if (ANVIL__check__empty_list(function.statements) == ANVIL__bt__false) {
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

// print a structure
void ANVIL__print__parsed_structure(ANVIL__parsling_structure structure) {
    // print header
    printf("\tStructure: ");

    // print arguments
    ANVIL__print__parsling_arguments(&structure.header.inputs);

    // next line
    printf("\n");

    return;
}

// print a program
void ANVIL__print__parsed_program(ANVIL__parsling_program program) {
    ANVIL__current current_function = ANVIL__calculate__current_from_list_filled_index(&(program.functions));
    ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&(program.structures));
    
    // print header
    printf("Parsed Program:\n");

    // print each function
    while (ANVIL__check__current_within_range(current_function)) {
        // print function
        ANVIL__print__parsed_function(*(ANVIL__parsling_function*)current_function.start);

        // advance current
        current_function.start += sizeof(ANVIL__parsling_function);
    }

    // print each structure
    while (ANVIL__check__current_within_range(current_structure)) {
        // print structure
        ANVIL__print__parsed_structure(*(ANVIL__parsling_structure*)current_structure.start);

        // advance current
        current_structure.start += sizeof(ANVIL__parsling_structure);
    }

    return;
}

#endif
