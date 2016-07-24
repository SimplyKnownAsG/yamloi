#pragma once

#include "Node.hpp"
#include "ScalarNode.hpp"

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

        void _dump(Dumper& dumper) const override;

    public:
        SequenceNode() = default;

        SequenceNode(std::vector<std::shared_ptr<Node>> nodes) {
            for (auto node : nodes) {
                this->nodes.push_back(node);
            }
        };

#ifdef SWIG
        %feature("python:slot", "mp_ass_subscript", functype="objobjargproc") set;
#endif
        void set(int index, std::shared_ptr<Node> value);

#ifdef SWIG
        %feature("python:slot", "mp_subscript", functype="binaryfunc") get;
#endif
        std::shared_ptr<Node> get(int index);

        template <typename T> void add(T value) {
            auto node = std::make_shared<ScalarNode>(value);
            this->nodes.push_back(node);
        };

        void add(std::shared_ptr<Node> node) {
            this->nodes.push_back(node);
        };

        bool const is_sequence() const {
            return true;
        };

        static Node *parse(Loader *loader);

    };

#ifdef SWIG
    %template(add) SequenceNode::add<double>;

    %template(add) SequenceNode::add<long long>;

    %template(add) SequenceNode::add<std::string>;
#endif

}

