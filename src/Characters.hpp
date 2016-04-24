#pragma once

#include <unordered_set>

namespace yamloi {

    class Characters {
    public:
        static const std::unordered_set<char> whitespace;

        static const std::unordered_set<char> empty;

        static const std::unordered_set<char> sequence_flow_separation;

    };

}


