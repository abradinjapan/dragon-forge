#pragma once

#include "basic.cpp"

#include <vector>

namespace lex {
    typedef enum lt {
        lt_none,
        lt_name,
        lt_period,
        lt_comma,
        lt_at,
        lt_hashtag,
        lt_left_parenthesis,
        lt_right_parenthesis,
        lt_left_curly_bracket,
        lt_right_curly_bracket,
        lt_string,
    } lt;

    // one lexling
    typedef struct lexling {
        lt type;
        basic::buffer data;
    } lexling;

    // create a lexling
    lexling create_lexling(lt type, basic::buffer data) {
        lexling output;

        // setup output
        output.type = type;
        output.data = data;

        return output;
    }

    // create blank lexling
    lexling create_null_lexling() {
        // return empty
        return create_lexling(lt_none, basic::create_null_buffer());
    }

    // multiple lexlings
    typedef struct lexlings {

    } lexlings;
}
