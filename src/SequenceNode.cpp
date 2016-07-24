#include "Dumper.hpp"
#include "Characters.hpp"
#include "Loader.hpp"
#include "MappingNode.hpp"
#include "ScalarNode.hpp"
#include "SequenceNode.hpp"

#include <iostream>

namespace yamloi {
    int level = 0;

    Node *SequenceNode::parse(Loader *loader) {
        SequenceNode *node = NULL;
        if (loader->length == 1 && loader->next_c == '[') {
            level += 1;
            loader->consume(true);
            node = new SequenceNode();
            char c;
            while (loader->next_c != ']') {
                if (Node *child = loader->parse(Characters::sequence_flow_separation)) {
                    node->add(std::shared_ptr<Node>(child));
                    if (loader->next_c == ',') {
                        loader->consume(true);
                    }
                }
                else {
                    break;
                }
            }
            loader->consume(true);
            loader->next_char(c, true);
            level -= 1;
        }
        return (Node*)node;
    };

    bool SequenceNode::lt(const Node* node) const {
        if (node->is_scalar()) {
            return false;
        }
        else if (node->is_mapping()) {
            return true;
        }
        auto that = (SequenceNode*)node;
        size_t size = this->nodes.size() < that->nodes.size() 
            ? this->nodes.size()
            : that->nodes.size();
        for (size_t ii = 0; ii < size; ii++) {
            auto mine = (const Node*)(this->nodes.at(ii).get());
            auto theirs = (const Node*)(that->nodes.at(ii).get());
            if (*mine < theirs) {
                return true;
            }
            else if (*mine > theirs) {
                return false;
            }
        }
        if (this->nodes.size() != that->nodes.size() &&
                this->nodes.size() < that->nodes.size()) {
            return true;
        }
        return false;
    }

    bool SequenceNode::eq(const Node* node) const {
        if (!node->is_sequence()) {
            return false;
        }
        auto that = (SequenceNode*)node;
        if (this->nodes.size() != that->nodes.size()) {
            return false;
        }
        for (size_t ii = 0; ii < this->nodes.size(); ii++) {
            auto mine = (const Node*)(this->nodes.at(ii).get());
            auto theirs = (const Node*)(that->nodes.at(ii).get());
            if (*mine != theirs) {
                return false;
            }
        }
        return true;
    }

    bool SequenceNode::gt(const Node* node) const {
        if (node->is_scalar()) {
            return true;
        }
        else if (node->is_mapping()) {
            return false;
        }
        auto that = (SequenceNode*)node;
        size_t size = this->nodes.size() < that->nodes.size() 
            ? this->nodes.size()
            : that->nodes.size();
        for (size_t ii = 0; ii < size; ii++) {
            auto mine = (const Node*)(this->nodes.at(ii).get());
            auto theirs = (const Node*)(that->nodes.at(ii).get());
            if (*mine > theirs) {
                return true;
            }
            else if (*mine < theirs) {
                return false;
            }
        }
        if (this->nodes.size() != that->nodes.size() &&
                this->nodes.size() > that->nodes.size()) {
            return true;
        }
        return false;
    }

    void SequenceNode::set(int index, std::shared_ptr<Node> value) {
        this->nodes.at(index) = value;
    }

    std::shared_ptr<Node> SequenceNode::get(int index) {
        return this->nodes.at(index);
    }

    void SequenceNode::_dump(Dumper &dumper) const {
        auto format = this->get_format(dumper.format);
        if (format->style & FlowStyle) {
            dumper << "[";
            unsigned int count = 0;
            for (auto node : this->nodes) {
                dumper << node;
                if (++count < this->nodes.size()) {
                    dumper << ", ";
                }
            }
            dumper << "]";
        }
        else {
            auto sep = dumper.get_seq_start();
            for (auto node : this->nodes) {
                dumper << sep << node;
                if (node->is_scalar() || (node->get_format(dumper.format)->style & FlowStyle)) {
                    dumper << std::endl;
                }
                sep = dumper.get_seq_separator();
            }
        }
    }

}

