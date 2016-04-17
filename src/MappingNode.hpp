#pragma once

#include "Node.hpp"

#include <map>
#include <vector>

namespace yamloi {

    class MappingNode : public Node {
    private:

        // TODO: should use std::map, but it is not giving a consistent order
        std::map<std::shared_ptr<Node>, std::shared_ptr<Node>> nodes;

        bool lt(const Node* node) const;

        bool eq(const Node* node) const;

        bool gt(const Node* node) const;
 
    public:
        MappingNode() = default;

        bool const is_mapping() const {
            return true;
        };

        void add(std::shared_ptr<Node> key, std::shared_ptr<Node> value);

        const std::string dump() const;
    };

}

