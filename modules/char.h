#pragma once

inline int is_digit(char c)
{
    return '0' <= c && c <= '9';
}

inline int is_alpha(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

inline int is_punctuation(char c)
{
    return c == '.'
        || c == ','
        || c == '-'
        || c == '_'
        || c == '\''
        || c == '"';
}

inline int is_whitespace(char c)
{
    return c == ' '
        || c == '\t';
}
