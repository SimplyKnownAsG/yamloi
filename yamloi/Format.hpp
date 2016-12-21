#pragma once

#include "yamloi/Style.hpp"

#include <string>
#include <algorithm>


namespace yamloi {

    extern int default_indent_spaces;

    class Format {

    public:
        int indent_spaces;

        Style style;

        Format(int indent_spaces, Style style) :
            indent_spaces(indent_spaces), style(style) {
        };

        Format() : Format(indent_spaces, default_style) {
        };

        Format(Style style) : Format(default_indent_spaces, style) {
        };

        Format(int indent_spaces) : Format(indent_spaces, default_style) {
        };

    };

    extern Format default_format;

}

