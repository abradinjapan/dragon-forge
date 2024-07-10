#ifndef DRAGON__compiler__parser
#define DRAGON__compiler__parser

/* Include */
#include "lexer.h"

/* Parser */
// namespace struct
typedef struct COMPILER__namespace {
    ANVIL__list lexlings; // COMPILER__lexling
    COMPILER__lexling_index count;
} COMPILER__namespace;

// create name
COMPILER__namespace COMPILER__create__namespace(ANVIL__list lexlings, COMPILER__lexling_index count) {
    COMPILER__namespace output;

    output.lexlings = lexlings;
    output.count = count;

    return output;
}

// create null namespace
COMPILER__namespace COMPILER__create_null__namespace() {
    return COMPILER__create__namespace(ANVIL__create_null__list(), 0);
}

// parsling argument
typedef struct COMPILER__parsling_argument {
    // definition
    COMPILER__gat category;
    COMPILER__namespace name;

    // optional type
    COMPILER__namespace type;
} COMPILER__parsling_argument;

// create a custom argument
COMPILER__parsling_argument COMPILER__create__parsling_argument(COMPILER__gat category, COMPILER__namespace name, COMPILER__namespace type) {
    COMPILER__parsling_argument output;

    // setup output
    output.category = category;
    output.name = name;
    output.type = type;

    return output;
}

// setup null argument
COMPILER__parsling_argument COMPILER__create_null__parsling_argument() {
    return COMPILER__create__parsling_argument(COMPILER__gat__invalid, COMPILER__create_null__namespace(), COMPILER__create_null__namespace());
}

// one scope
typedef struct COMPILER__parsling_scope {
    ANVIL__list statements; // COMPILER__parsling_statement
} COMPILER__parsling_scope;

// create custom scope
COMPILER__parsling_scope COMPILER__create__parsling_scope(ANVIL__list statements) {
    COMPILER__parsling_scope output;

    // setup output
    output.statements = statements;

    return output;
}

// create null scope
COMPILER__parsling_scope COMPILER__create_null__parsling_scope() {
    // return empty
    return COMPILER__create__parsling_scope(ANVIL__create_null__list());
}

// one statement
typedef struct COMPILER__parsling_statement {
    // statement type
    COMPILER__stt type;

    // name
    COMPILER__parsling_argument name; // offset name, subscope name & function call name

    // function call data
    ANVIL__list inputs; // COMPILER__parsling_argument
    ANVIL__list outputs; // COMPILER__parsling_argument
    COMPILER__input_count input_count;
    COMPILER__output_count output_count;

    // subscope data
    COMPILER__parsling_argument subscope_flag_name;
    COMPILER__parsling_scope subscope;
} COMPILER__parsling_statement;

// create a custom statement
COMPILER__parsling_statement COMPILER__create__parsling_statement(COMPILER__stt type, COMPILER__parsling_argument name, ANVIL__list inputs, ANVIL__list outputs, COMPILER__input_count input_count, COMPILER__output_count output_count, COMPILER__parsling_argument subscope_flag_name, COMPILER__parsling_scope subscope) {
    COMPILER__parsling_statement output;

    // setup output
    output.type = type;
    output.name = name;
    output.inputs = inputs;
    output.outputs = outputs;
    output.input_count = input_count;
    output.output_count = output_count;
    output.subscope_flag_name = subscope_flag_name;
    output.subscope = subscope;

    return output;
}

// create a null statement
COMPILER__parsling_statement COMPILER__create_null__parsling_statement() {
    // return empty
    return COMPILER__create__parsling_statement(COMPILER__stt__invalid, COMPILER__create_null__parsling_argument(), ANVIL__create_null__list(), ANVIL__create_null__list(), 0, 0, COMPILER__create_null__parsling_argument(), COMPILER__create_null__parsling_scope());
}

// one structure
typedef struct COMPILER__parsling_structure {
    ANVIL__list type_names; // COMPILER__parsling_argument
    ANVIL__list arguments; // COMPILER__parsling_argument
} COMPILER__parsling_structure;

// create a custom structure
COMPILER__parsling_structure COMPILER__create__parsling_structure(ANVIL__list type_names, ANVIL__list arguments) {
    COMPILER__parsling_structure output;

    // setup output
    output.type_names = type_names;
    output.arguments = arguments;

    return output;
}

// create a null structure
COMPILER__parsling_structure COMPILER__create_null__parsling_structure() {
    // return empty
    return COMPILER__create__parsling_structure(ANVIL__create_null__list(), ANVIL__create_null__list());
}

// one function
typedef struct COMPILER__parsling_function {
    COMPILER__parsling_statement header;
    COMPILER__parsling_scope scope;
} COMPILER__parsling_function;

// create a custom function
COMPILER__parsling_function COMPILER__create__parsling_function(COMPILER__parsling_statement header, COMPILER__parsling_scope scope) {
    COMPILER__parsling_function output;

    // setup output
    output.header = header;
    output.scope = scope;

    return output;
}

// create a null function
COMPILER__parsling_function COMPILER__create_null__parsling_function() {
    // return empty
    return COMPILER__create__parsling_function(COMPILER__create_null__parsling_statement(), COMPILER__create_null__parsling_scope());
}

// one program
typedef struct COMPILER__parsling_program {
    ANVIL__list functions; // COMPILER__parsling_function
    ANVIL__list structures; // COMPILER__parsling_structure
} COMPILER__parsling_program;

// create a custom program
COMPILER__parsling_program COMPILER__create__parsling_program(ANVIL__list functions, ANVIL__list structures) {
    COMPILER__parsling_program output;

    // setup output
    output.functions = functions;
    output.structures = structures;

    return output;
}

// create a null program
COMPILER__parsling_program COMPILER__create_null__parsling_program() {
    // return empty
    return COMPILER__create__parsling_program(ANVIL__create_null__list(), ANVIL__create_null__list());
}

// append namespace
void COMPILER__append__namespace(ANVIL__list* list, COMPILER__namespace data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__namespace), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__namespace*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__namespace);

    return;
}

// append parsling argument
void COMPILER__append__parsling_argument(ANVIL__list* list, COMPILER__parsling_argument data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__parsling_argument), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__parsling_argument*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__parsling_argument);

    return;
}

// append parsling statement
void COMPILER__append__parsling_statement(ANVIL__list* list, COMPILER__parsling_statement data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__parsling_statement), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__parsling_statement*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__parsling_statement);

    return;
}

// append parsling structure
void COMPILER__append__parsling_structure(ANVIL__list* list, COMPILER__parsling_structure data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__parsling_structure), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__parsling_structure*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__parsling_structure);

    return;
}

// append parsling function
void COMPILER__append__parsling_function(ANVIL__list* list, COMPILER__parsling_function data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__parsling_function), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__parsling_function*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__parsling_function);

    return;
}

// append parsling program
void COMPILER__append__parsling_program(ANVIL__list* list, COMPILER__parsling_program data, COMPILER__error* error) {
    // request space
    ANVIL__list__request__space(list, sizeof(COMPILER__parsling_program), &(*error).memory_error_occured);

    // append data
    (*(COMPILER__parsling_program*)ANVIL__calculate__list_current_address(list)) = data;

    // increase fill
    (*list).filled_index += sizeof(COMPILER__parsling_program);

    return;
}

// close namespace
void COMPILER__close__parsling_namespace(COMPILER__namespace namespace) {
    // close list
    ANVIL__close__list(namespace.lexlings);

    return;
}

// close argument
void COMPILER__close__parsling_argument(COMPILER__parsling_argument argument) {
    // close namespaces
    COMPILER__close__parsling_namespace(argument.name);
    if (ANVIL__check__empty_list(argument.type.lexlings) == ANVIL__bt__false) {
        COMPILER__close__parsling_namespace(argument.type);
    }

    return;
}

// close arguments
void COMPILER__close__parsling_arguments(ANVIL__list* arguments) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(arguments);

    // close arguments
    while (ANVIL__check__current_within_range(current)) {
        // close argument
        COMPILER__close__parsling_argument(*(COMPILER__parsling_argument*)current.start);

        // next argument
        current.start += sizeof(COMPILER__parsling_argument);
    }

    // close arguments buffer
    ANVIL__close__list(*arguments);

    return;
}

// forward declaration
void COMPILER__close__parsling_statements(ANVIL__list*);

// close scope
void COMPILER__close__parsling_scope(COMPILER__parsling_scope scope) {
    // close statements
    if (ANVIL__check__empty_list(scope.statements) == ANVIL__bt__false) {
        // close statements
        COMPILER__close__parsling_statements(&scope.statements);
    }

    return;
}

// close statement
void COMPILER__close__parsling_statement(COMPILER__parsling_statement statement) {
    // close name
    COMPILER__close__parsling_argument(statement.name);

    // close io
    if (ANVIL__check__empty_list(statement.inputs) == ANVIL__bt__false) {
        COMPILER__close__parsling_arguments(&statement.inputs);
    }
    if (ANVIL__check__empty_list(statement.outputs) == ANVIL__bt__false) {
        COMPILER__close__parsling_arguments(&statement.outputs);
    }

    // close scope
    COMPILER__close__parsling_scope(statement.subscope);
    COMPILER__close__parsling_argument(statement.subscope_flag_name);

    return;
}

// close statements
void COMPILER__close__parsling_statements(ANVIL__list* list) {
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(list);

    // clean up each statement
    while (ANVIL__check__current_within_range(current_statement)) {
        // clean up statement
        COMPILER__close__parsling_statement(*(COMPILER__parsling_statement*)current_statement.start);

        // next statement
        current_statement.start += sizeof(COMPILER__parsling_statement);
    }

    // clean up statements buffer
    ANVIL__close__list(*list);

    return;
}

// close structure
void COMPILER__close__parsling_structure(COMPILER__parsling_structure structure) {
    // close lists
    COMPILER__close__parsling_arguments(&structure.type_names);
    COMPILER__close__parsling_arguments(&structure.arguments);

    return;
}

// close function
void COMPILER__close__parsling_function(COMPILER__parsling_function function) {
    // close header
    COMPILER__close__parsling_statement(function.header);
    
    // close scope
    COMPILER__close__parsling_scope(function.scope);

    return;
}

// close parsling program
void COMPILER__close__parsling_program(COMPILER__parsling_program program) {
    ANVIL__current current = ANVIL__calculate__current_from_list_filled_index(&(program.functions));

    // free each function
    while (ANVIL__check__current_within_range(current)) {
        // free function
        COMPILER__close__parsling_function(*(COMPILER__parsling_function*)current.start);

        // next function
        current.start += sizeof(COMPILER__parsling_function);
    }
    
    // next current
    current = ANVIL__calculate__current_from_list_filled_index(&(program.structures));

    // free each structure
    while (ANVIL__check__current_within_range(current)) {
        // free structure
        COMPILER__close__parsling_structure(*(COMPILER__parsling_structure*)current.start);

        // next structure
        current.start += sizeof(COMPILER__parsling_structure);
    }

    // free the lists
    ANVIL__close__list(program.functions);
    ANVIL__close__list(program.structures);

    return;
}

// advance the current buffer by N number of lexlings
void COMPILER__advance__lexling_current(ANVIL__current* current, COMPILER__lexling_index lexling_count) {
    // advance the pointer
    (*current).start += sizeof(COMPILER__lexling) * lexling_count;

    return;
}

// revert lexling current by N number of lexlings
void COMPILER__revert__lexling_current(ANVIL__current* current, COMPILER__lexling_index lexling_count) {
    (*current).start -= sizeof(COMPILER__lexling) * lexling_count;

    return;
}

// parse a namepspace with only one name
COMPILER__namespace COMPILER__parse__namespace__one_name_only(ANVIL__current* current, COMPILER__error* error) {
    COMPILER__namespace output;

    // setup count
    output.count = 0;

    // open lexling list
    output.lexlings = COMPILER__open__list_with_error(sizeof(COMPILER__lexling) * 8, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // check for name
    if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__name) {
        // append lexling
        COMPILER__append__lexling(&output.lexlings, COMPILER__read__lexling_from_current(*current), error);
        if (COMPILER__check__error_occured(error)) {
            return output;
        }

        // one lexling
        output.count++;

        // next lexling
        COMPILER__advance__lexling_current(current, 1);
    // not found, error
    } else {
        // set error
        *error = COMPILER__open__error("Parsing Error: Expected a name.", COMPILER__read__lexling_from_current(*current).location);
    }

    return output;
}

// parse namespace
COMPILER__namespace COMPILER__parse__namespace(ANVIL__current* current, COMPILER__error* error) {
    COMPILER__namespace output;

    // setup count
    output.count = 0;

    // open lexling list
    output.lexlings = COMPILER__open__list_with_error(sizeof(COMPILER__lexling) * 8, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // check for initial name
    if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__name) {
        // append lexling
        COMPILER__append__lexling(&output.lexlings, COMPILER__read__lexling_from_current(*current), error);
        if (COMPILER__check__error_occured(error)) {
            return output;
        }

        // count lexling
        output.count++;

        // next lexling
        COMPILER__advance__lexling_current(current, 1);

        // get other lexlings
        while (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__colon) {
            // skip past colon
            COMPILER__advance__lexling_current(current, 1);

            // get name
            if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__name) {
                // append name
                COMPILER__append__lexling(&output.lexlings, COMPILER__read__lexling_from_current(*current), error);
                if (COMPILER__check__error_occured(error)) {
                    return output;
                }

                // count lexling
                output.count++;

                // next lexling
                COMPILER__advance__lexling_current(current, 1);
            // name missing, error
            } else {
                // set error
                *error = COMPILER__open__error("Parsing Error: Expected a name.", COMPILER__read__lexling_from_current(*current).location);

                return output;
            }
        }
    // if it is a string
    } else if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__string_literal) {
        // append string
        COMPILER__append__lexling(&output.lexlings, COMPILER__read__lexling_from_current(*current), error);
        if (COMPILER__check__error_occured(error)) {
            return output;
        }

        // next lexling
        COMPILER__advance__lexling_current(current, 1);
    // not found, error
    } else {
        // set error
        *error = COMPILER__open__error("Parsing Error: Expected a name.", COMPILER__read__lexling_from_current(*current).location);
    }

    return output;
}

// parse one argument
COMPILER__parsling_argument COMPILER__parse__function_argument(ANVIL__current* current, ANVIL__bt is_function_header_argument, COMPILER__error* error) {
    COMPILER__parsling_argument output = COMPILER__create_null__parsling_argument();
    //ANVIL__cell_integer_value value = 0;

    // check type
    // is variable / literal
    if (COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__name) {
        // parse namespace
        output = COMPILER__create__parsling_argument(COMPILER__gat__variable, COMPILER__parse__namespace(current, error), COMPILER__create_null__namespace());
        
        // check for optional type
        // is type definition
        if (COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__exclamation_point) {
            // next lexling
            COMPILER__advance__lexling_current(current, 1);

            // parse type
            output.type = COMPILER__parse__namespace__one_name_only(current, error);
        }

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
    // offset
    } else if (COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__at) {
        // advance current past at
        COMPILER__advance__lexling_current(current, 1);

        // if correct type
        if (COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__name) {
            // get name
            output = COMPILER__create__parsling_argument(COMPILER__gat__offset, COMPILER__parse__namespace(current, error), COMPILER__create_null__namespace());
        // error
        } else {
            *error = COMPILER__open__error("Parse Error: Offset is missing name.", COMPILER__read__lexling_from_current(*current).location);

            return output;
        }
    // string literal
    } else if (COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__string_literal) {
        // get argument
        output = COMPILER__create__parsling_argument(COMPILER__gat__literal__string, COMPILER__parse__namespace(current, error), COMPILER__create_null__namespace());
    // error
    } else {
        *error = COMPILER__open__error("Parse Error: Unrecognized argument type.", COMPILER__read__lexling_from_current(*current).location);

        return output;
    }

    // check argument for variable only
    if (is_function_header_argument == ANVIL__bt__true && output.category != COMPILER__gat__variable) {
        // set error
        *error = COMPILER__open__error("Parse Error: A non-variable / non-type argument was detected in a header.", COMPILER__read__lexling_from_current(*current).location);

        return output;
    }

    // check for error
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    return output;
}

// parse function call statement arguments
ANVIL__list COMPILER__parse__function_call_statement_arguments(ANVIL__current* current, COMPILER__io_count* count, ANVIL__bt is_function_header_argument, COMPILER__error* error) {
    ANVIL__list output;

    // init count
    *count = 0;

    // open names list
    output = COMPILER__open__list_with_error(sizeof(COMPILER__parsling_argument) * 8, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }
    
    // check for opening parenthesis
    if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__left_parenthesis) {
        // next lexling
        COMPILER__advance__lexling_current(current, 1);
    // not found, error
    } else {
        // set error
        *error = COMPILER__open__error("Parse Error: Arguments is missing opening parenthesis.", COMPILER__read__lexling_from_current(*current).location);

        return output;
    }

    // get arguments
    while (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type != COMPILER__lt__right_parenthesis) {
        // parse argument
        COMPILER__parsling_argument argument = COMPILER__parse__function_argument(current, is_function_header_argument, error);
        if (COMPILER__check__error_occured(error)) {
            return output;
        }

        // append argument
        COMPILER__append__parsling_argument(&output, argument, error);
    
        // increment count
        *count = *count + 1;
    }

    // check for closing parenthesis
    if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__right_parenthesis) {
        // next lexling
        COMPILER__advance__lexling_current(current, 1);
    // not found, error
    } else {
        // set error
        *error = COMPILER__open__error("Parse Error: Arguments is missing closing parenthesis.", COMPILER__read__lexling_from_current(*current).location);

        return output;
    }

    return output;
}

// predefine
COMPILER__parsling_scope COMPILER__parse__scope(ANVIL__current* current, COMPILER__error* error);

// parse one statement
COMPILER__parsling_statement COMPILER__parse__statement(ANVIL__current* current, ANVIL__bt is_header, COMPILER__error* error) {
    COMPILER__parsling_statement output = COMPILER__create_null__parsling_statement();
    ANVIL__bt is_scoped_offset = ANVIL__bt__false;

    // check for offset
    if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__at) {
        // advance current past @ symbol
        COMPILER__advance__lexling_current(current, 1);

        // check for offset name
        if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__name) {
            // set name
            output.name = COMPILER__create__parsling_argument(COMPILER__gat__offset, COMPILER__parse__namespace(current, error), COMPILER__create_null__namespace());
            if (COMPILER__check__error_occured(error)) {
                return output;
            }
        // invalid syntax
        } else {
            // set error
            *error = COMPILER__open__error("Parse Error: Offset statement name is an invalid lexling.", COMPILER__read__lexling_from_current(*current).location);

            return output;
        }

        // look ahead for a scope setter
        {
            // setup temps
            ANVIL__current look_ahead_current = *current;
            COMPILER__error look_ahead_error = COMPILER__create_null__error();

            // parse ahead one namespace
            COMPILER__namespace temp = COMPILER__parse__namespace(&look_ahead_current, &look_ahead_error);

            // check for error
            if (COMPILER__check__error_occured(&look_ahead_error)) {
                // clear error (already set to not a lookahead scope)
                COMPILER__close__error(look_ahead_error);
            }

            // close temporary namespace
            COMPILER__close__parsling_namespace(temp);

            // check for the equals sign
            if (ANVIL__check__current_within_range(look_ahead_current) && COMPILER__read__lexling_from_current(look_ahead_current).type == COMPILER__lt__equals) {
                // is a named scope
                is_scoped_offset = ANVIL__bt__true;
            }
        }

        // is a scope
        if (is_scoped_offset == ANVIL__bt__true) {
            // is a named scope
            output.type = COMPILER__stt__subscope;

            // get scope flag namespace
            output.subscope_flag_name = COMPILER__create__parsling_argument(COMPILER__gat__variable, COMPILER__parse__namespace(current, error), COMPILER__create_null__namespace());
            if (COMPILER__check__error_occured(error)) {
                return output;
            }

            // check for invalid namespace
            if (output.subscope_flag_name.category != COMPILER__gat__variable) {
                // set error
                *error = COMPILER__open__error("Parse Error: A named scope doesn't have its flagging variable.", COMPILER__read__lexling_from_current(*current).location);

                return output;
            }

            // skip past equals, already checked for it in lookahead
            COMPILER__advance__lexling_current(current, 1);

            // parse scope
            output.subscope = COMPILER__parse__scope(current, error);
        // is a normal offset
        } else {
            // setup type
            output.type = COMPILER__stt__offset__normal;
        }
    // is a function call
    } else if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__name) {
        // get name
        output.name = COMPILER__create__parsling_argument(COMPILER__gat__offset, COMPILER__parse__namespace(current, error), COMPILER__create_null__namespace());
        if (COMPILER__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // get inputs
        output.inputs = COMPILER__parse__function_call_statement_arguments(current, &output.input_count, is_header, error);
        if (COMPILER__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // get outputs
        output.outputs = COMPILER__parse__function_call_statement_arguments(current, &output.output_count, is_header, error);
        if (COMPILER__check__error_occured(error) == ANVIL__bt__true) {
            return output;
        }

        // set type
        if (is_header == ANVIL__bt__true) {
            output.type = COMPILER__stt__function_header;
        } else {
            output.type = COMPILER__stt__function_call;
        }
    // error
    } else {
        // set error
        if (is_header) {
            *error = COMPILER__open__error("Parse Error: Unrecognized header type.", COMPILER__read__lexling_from_current(*current).location);
        } else {
            *error = COMPILER__open__error("Parse Error: Unrecognized statement type.", COMPILER__read__lexling_from_current(*current).location);
        }
    }

    return output;
}

// parse a scope
COMPILER__parsling_scope COMPILER__parse__scope(ANVIL__current* current, COMPILER__error* error) {
    COMPILER__parsling_scope output;

    // check for scope opener
    if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__left_curly_bracket) {
        // advance current
        COMPILER__advance__lexling_current(current, 1);
    // scope opener not found, error
    } else {
        // set error
        *error = COMPILER__open__error("Parse Error: Scope is missing left curly bracket.", COMPILER__read__lexling_from_current(*current).location);

        return output;
    }

    // parse statements
    // open statements list
    output.statements = COMPILER__open__list_with_error(sizeof(COMPILER__parsling_statement) * 16, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }
    
    // get statements
    while (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type != COMPILER__lt__right_curly_bracket) {
        // parse statement
        COMPILER__parsling_statement statement = COMPILER__parse__statement(current, ANVIL__bt__false, error);
        if (COMPILER__check__error_occured(error)) {
            return output;
        }

        // add statement
        COMPILER__append__parsling_statement(&output.statements, statement, error);
        if (COMPILER__check__error_occured(error)) {
            return output;
        }
    }

    // check for scope closer
    if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__right_curly_bracket) {
        // advance current
        COMPILER__advance__lexling_current(current, 1);
    // scope opener not found, error
    } else {
        // setup error
        *error = COMPILER__open__error("Parse Error: Scope is missing right curly bracket.", COMPILER__read__lexling_from_current(*current).location);

        return output;
    }

    return output;
}

// parse a structure
COMPILER__parsling_structure COMPILER__parse__structure(ANVIL__current* current, COMPILER__error* error) {
    COMPILER__parsling_structure output = COMPILER__create_null__parsling_structure();
    COMPILER__input_count input_count = 0;

    // open type names list
    output.type_names = COMPILER__open__list_with_error(sizeof(COMPILER__parsling_argument) * 8, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // parse type names
    while (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__exclamation_point) {
        // skip past exclamation point
        COMPILER__advance__lexling_current(current, 1);
        
        // parse type name
        COMPILER__namespace name = COMPILER__parse__namespace__one_name_only(current, error);
        if (COMPILER__check__error_occured(error)) {
            return output;
        }

        // append argument
        COMPILER__append__parsling_argument(&output.type_names, COMPILER__create__parsling_argument(COMPILER__gat__type, name, COMPILER__create_null__namespace()), error);
    }

    // parse arguments
    output.arguments = COMPILER__parse__function_call_statement_arguments(current, &input_count, ANVIL__bt__true, error);

    return output;
}

// parse a function
COMPILER__parsling_function COMPILER__parse__function(ANVIL__current* current, COMPILER__error* error) {
    COMPILER__parsling_function output = COMPILER__create_null__parsling_function();

    // parse header
    output.header = COMPILER__parse__statement(current, ANVIL__bt__true, error);
    if (COMPILER__check__error_occured(error)) {
        return output;
    }

    // check for equals sign
    if (ANVIL__check__current_within_range(*current) && COMPILER__read__lexling_from_current(*current).type == COMPILER__lt__equals) {
        // no saving data necessary, next lexling
        COMPILER__advance__lexling_current(current, 1);
    // error
    } else {
        // set error
        *error = COMPILER__open__error("Parse Error: A function definition has an equals sign missing.", COMPILER__read__lexling_from_current(*current).location);

        // quit
        return output;
    }

    // parse scope
    output.scope = COMPILER__parse__scope(current, error);

    return output;
}

// parse a program (file)
COMPILER__parsling_program COMPILER__parse__program(COMPILER__lexlings lexlings, COMPILER__error* error) {
    COMPILER__parsling_program output;
    COMPILER__parsling_function temp_function;
    COMPILER__parsling_structure temp_structure;
    ANVIL__current current;

    // setup current
    current = ANVIL__calculate__current_from_list_filled_index(&lexlings.data);

    // open the function list
    output.functions = COMPILER__open__list_with_error(sizeof(COMPILER__parsling_function) * 64, error);
    if (COMPILER__check__error_occured(error)) {
        goto quit;
    }

    // open the structure list
    output.structures = COMPILER__open__list_with_error(sizeof(COMPILER__parsling_structure) * 64, error);
    if (COMPILER__check__error_occured(error)) {
        goto quit;
    }

    // parse functions & structures
    while (ANVIL__check__current_within_range(current)) {
        // if end of file
        if (COMPILER__read__lexling_from_current(current).type == COMPILER__lt__end_of_file) {
            // finished parsing
            goto quit;
        }
        
        // determine abstraction type
        if (COMPILER__read__lexling_from_current(current).type == COMPILER__lt__exclamation_point) {
            // parse structure
            temp_structure = COMPILER__parse__structure(&current, error);

            // append structure
            COMPILER__append__parsling_structure(&(output.structures), temp_structure, error);
        } else {
            // parse function
            temp_function = COMPILER__parse__function(&current, error);

            // append function
            COMPILER__append__parsling_function(&(output.functions), temp_function, error);
        }

        // check for error
        if (COMPILER__check__error_occured(error) == ANVIL__bt__true) {
            goto quit;
        }
    }

    // quit
    quit:

    return output;
}

// print namespace
void COMPILER__print__namespace(COMPILER__namespace name) {
    ANVIL__current current_lexling;

    // setup current
    current_lexling = ANVIL__calculate__current_from_list_filled_index(&name.lexlings);

    // print starting name lexling
    ANVIL__print__buffer(COMPILER__read__lexling_from_current(current_lexling).value);
    current_lexling.start += sizeof(COMPILER__lexling);

    // for each remaining lexling
    while (ANVIL__check__current_within_range(current_lexling)) {
        // print colon
        printf(":");

        // print name
        ANVIL__print__buffer(COMPILER__read__lexling_from_current(current_lexling).value);
        current_lexling.start += sizeof(COMPILER__lexling);
    }

    return;
}

// print argument
void COMPILER__print__parsling_argument(COMPILER__parsling_argument argument) {
    // print type
    printf("[");
    ANVIL__print__buffer(COMPILER__convert__general_argument_type_to_string_buffer(argument.category));
    printf("]");

    // print data
    COMPILER__print__namespace(argument.name);

    // print type modifier if exists
    if (ANVIL__check__empty_list(argument.type.lexlings) == ANVIL__bt__false) {
        // print type modifier
        printf("[type]");
        COMPILER__print__namespace(argument.type);
    }

    return;
}

// print arguments
void COMPILER__print__parsling_arguments(ANVIL__list* arguments) {
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
        COMPILER__print__parsling_argument(*(COMPILER__parsling_argument*)current.start);

        // next buffer
        current.start += sizeof(COMPILER__parsling_argument);
    }

    // print closer
    printf(")");

    return;
}

// forward declaration
void COMPILER__print__parsed_scope(COMPILER__parsling_scope scope, ANVIL__tab_count tab_depth);

// print statement
void COMPILER__print__parsed_statement(COMPILER__parsling_statement statement, ANVIL__tab_count tab_depth) {
    // print tabs
    ANVIL__print__tabs(tab_depth);

    // print statement
    if (statement.type == COMPILER__stt__offset__normal) {
        // print offset information
        printf("@[normal]");
        COMPILER__print__namespace(statement.name.name);

        // print new line
        printf("\n");
    } else if (statement.type == COMPILER__stt__offset__top) {
        // print offset information
        printf("@[top]");
        COMPILER__print__namespace(statement.name.name);

        // print new line
        printf("\n");
    } else if (statement.type == COMPILER__stt__offset__bottom) {
        // print offset information
        printf("@[bottom]");
        COMPILER__print__namespace(statement.name.name);

        // print new line
        printf("\n");
    } else if (statement.type == COMPILER__stt__function_call || statement.type == COMPILER__stt__function_header) {
        // print statement name
        COMPILER__print__namespace(statement.name.name);

        // print inputs
        COMPILER__print__parsling_arguments(&statement.inputs);

        // print outputs
        COMPILER__print__parsling_arguments(&statement.outputs);

        // print new line
        printf("\n");
    } else if (statement.type == COMPILER__stt__subscope) {
        // print scope
        printf("@");
        COMPILER__print__namespace(statement.name.name);
        printf(" ");
        COMPILER__print__namespace(statement.subscope_flag_name.name);
        printf(":\n");
        COMPILER__print__parsed_scope(statement.subscope, tab_depth + 1);
    }

    return;
}

// print a scope
void COMPILER__print__parsed_scope(COMPILER__parsling_scope scope, ANVIL__tab_count tab_depth) {
    ANVIL__current current_statement = ANVIL__calculate__current_from_list_filled_index(&scope.statements);

    // print statements
    if (ANVIL__check__empty_list(scope.statements) == ANVIL__bt__false) {
        // print each statement
        while (ANVIL__check__current_within_range(current_statement)) {
            // print statement
            COMPILER__print__parsed_statement(*(COMPILER__parsling_statement*)current_statement.start, tab_depth);

            // advance current
            current_statement.start += sizeof(COMPILER__parsling_statement);
        }
    }

    return;
}

// print a function
void COMPILER__print__parsed_function(COMPILER__parsling_function function, ANVIL__tab_count tab_depth) {
    // print header
    ANVIL__print__tabs(tab_depth);
    printf("Function: ");
    if (function.header.type != COMPILER__stt__invalid) {
        COMPILER__print__parsed_statement(function.header, 0);

        // print scope(s)
        COMPILER__print__parsed_scope(function.scope, tab_depth + 1);
    }

    return;
}

// print a structure
void COMPILER__print__parsed_structure(COMPILER__parsling_structure structure, ANVIL__tab_count tab_depth) {
    ANVIL__current current_name = ANVIL__calculate__current_from_list_filled_index(&structure.type_names);

    // print header
    ANVIL__print__tabs(tab_depth);
    printf("Structure: ");

    // print names
    while (ANVIL__check__current_within_range(current_name)) {
        // print name
        COMPILER__print__parsling_argument((*(COMPILER__parsling_argument*)current_name.start));

        // next name
        current_name.start += sizeof(COMPILER__parsling_argument);
    }

    // print arguments
    COMPILER__print__parsling_arguments(&structure.arguments);

    // next line
    printf("\n");

    return;
}

// print a program
void COMPILER__print__parsed_program(COMPILER__parsling_program program) {
    ANVIL__current current_function = ANVIL__calculate__current_from_list_filled_index(&(program.functions));
    ANVIL__current current_structure = ANVIL__calculate__current_from_list_filled_index(&(program.structures));
    
    // print header
    printf("Parsed Program:\n");

    // print each function
    while (ANVIL__check__current_within_range(current_function)) {
        // print function
        COMPILER__print__parsed_function(*(COMPILER__parsling_function*)current_function.start, 1);

        // advance current
        current_function.start += sizeof(COMPILER__parsling_function);
    }

    // print each structure
    while (ANVIL__check__current_within_range(current_structure)) {
        // print structure
        COMPILER__print__parsed_structure(*(COMPILER__parsling_structure*)current_structure.start, 1);

        // advance current
        current_structure.start += sizeof(COMPILER__parsling_structure);
    }

    return;
}

#endif
