#pragma once

#include "Node.hpp"

#include <vector>
#include <sstream>
#include <memory>


namespace yamloi {

    class Loader;

    class SequenceNode : public Node {

    private:
        std::vector<std::shared_ptr<Node>> nodes;

        bool lt(const Node* node) const;

        bool eq(const Node* node) const;

        bool gt(const Node* node) const;

    public:
        SequenceNode() {
        };

        SequenceNode(std::vector<std::shared_ptr<Node>> nodes) {
            for (auto node : nodes) {
                this->nodes.push_back(node);
            }
        };

        void add(std::shared_ptr<Node> node) {
            this->nodes.push_back(node);
        };

        bool const is_sequence() const {
            return true;
        };

        const std::string dump() const;

        static Node *parse(Loader *loader);

    };

}

