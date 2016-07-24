
#include "ScalarNode.hpp"
#include "SequenceNode.hpp"
#include "MappingNode.hpp"
#include "Exceptions.hpp"

#include <algorithm>
#include <sstream>

namespace yamloi {

    void _check_key_type(std::shared_ptr<Node> &key) {
        if (!key->is_scalar()) {
            std::ostringstream msg;
            msg << "Map key must be a scalar, found: " << key->dump();
            throw TypeError(msg.str());
        }
    }

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
        _check_key_type(key);
        this->nodes[key] = value;
    }

    void MappingNode::set(std::shared_ptr<Node> key, std::shared_ptr<Node> value) {
        _check_key_type(key);
        this->nodes[key] = value;
    }

    std::shared_ptr<Node> MappingNode::get(std::shared_ptr<Node> key) {
        auto result = this->nodes.find(key);
        if (result == this->nodes.end()) {
            std::ostringstream msg;
            msg << "Could not find item matching key: " << key->dump();
            throw KeyError(msg.str());
        }
        return result->first;
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

