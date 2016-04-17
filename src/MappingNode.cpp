
#include "ScalarNode.hpp"
#include "SequenceNode.hpp"
#include "MappingNode.hpp"

#include <algorithm>

namespace yamloi {

    bool MappingNode::lt(const Node* node) const {
        if (!node->is_mapping()) {
            return false;
        }
        return this->dump() < node->dump();
    }

    bool MappingNode::eq(const Node* node) const {
        if (!node->is_mapping()) {
            return false;
        }
        return this->dump() == node->dump();
    }

    bool MappingNode::gt(const Node* node) const {
        if (!node->is_mapping()) {
            return true;
        }
        return this->dump() > node->dump();
    }

    void MappingNode::add(std::shared_ptr<Node> key, std::shared_ptr<Node> value) {
        this->nodes[key] = value;
    }

    const std::string MappingNode::dump() const {
        std::stringstream ss;
        ss << "{";
        unsigned int count = 0;
        for (auto key_val : this->nodes) {
            ss << key_val.first->dump();
            ss << ": ";
            ss << key_val.second->dump();
            if (++count < this->nodes.size()) {
                ss << ", ";
            }
        }
        ss << "}";
        return ss.str();
    };

}

