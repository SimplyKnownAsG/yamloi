#include "Characters.hpp"

namespace yamloi {

    const std::unordered_set<char> Characters::whitespace = { ' ', '\t', '\r', '\n' };

    const std::unordered_set<char> Characters::empty = { };

    const std::unordered_set<char> Characters::sequence_flow_separation = { ']', ',' };

}

