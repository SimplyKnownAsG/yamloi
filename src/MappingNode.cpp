
#include "Dumper.hpp"
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

    void MappingNode::_dump(Dumper &dumper) const {
        auto format = this->get_format(dumper.format);
        if (format->style & FlowStyle) {
            dumper << "{";
            unsigned int count = 0;
            for (auto key_val : this->nodes) {
                dumper
                    << key_val.first
                    << ": "
                    << key_val.second;
                if (++count < this->nodes.size()) {
                    dumper << ", ";
                }
            }
            dumper << "}";
        }
        else {
            std::string indent = dumper.get_indent();
            for (auto key_val : this->nodes) {
                dumper
                    << indent
                    << key_val.first;
                if (key_val.second->is_scalar() ||
                        (key_val.second->get_format(dumper.format)->style & FlowStyle)) {
                    dumper << ": ";
                }
                else {
                    dumper << ":" << std::endl;
                }

                dumper << key_val.second;
                if (key_val.second->is_scalar()
                        || (key_val.second->get_format(dumper.format)->style & FlowStyle)) {
                    dumper << std::endl;
                }
            }
        }
    };

}

