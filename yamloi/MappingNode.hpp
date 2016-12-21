#pragma once

#include "yamloi/Node.hpp"
#include "yamloi/ScalarNode.hpp"

#include <map>
#include <vector>

namespace yamloi {

    class MappingNode : public Node {
    private:

        std::map<std::shared_ptr<Node>, std::shared_ptr<Node>> nodes;

        bool lt(const Node* node) const;

        bool eq(const Node* node) const;

        bool gt(const Node* node) const;

        void _dump(Dumper& dumper) const override;
 
    public:
        MappingNode() = default;

        bool const is_mapping() const {
            return true;
        };

        void add(std::shared_ptr<Node> key, std::shared_ptr<Node> value);

#ifdef SWIG
        %feature("python:slot", "mp_ass_subscript", functype="objobjargproc") set;
        %feature("python:slot", "mp_ass_subscript", functype="objobjargproc") set_k;
        %feature("python:slot", "mp_ass_subscript", functype="objobjargproc") set_v;
#endif
        void set(std::shared_ptr<Node> key, std::shared_ptr<Node> value);

        template<typename Tkey, typename Tvalue> void set(Tkey key, Tvalue value) {
            auto key_node = std::shared_ptr<Node>(new ScalarNode(key));
            auto value_node = std::shared_ptr<Node>(new ScalarNode(value));
            this->set(key_node, value_node);
        };

        template<typename Tkey> void set_k(Tkey key, std::shared_ptr<Node> value_node) {
            auto key_node = std::shared_ptr<Node>(new ScalarNode(key));
            this->set(key_node, value_node);
        };

        template<typename Tvalue> void set_v(std::shared_ptr<Node> key_node, Tvalue value) {
            auto value_node = std::shared_ptr<Node>(new ScalarNode(value));
            this->set(key_node, value_node);
        };

#ifdef SWIG
        %feature("python:slot", "mp_subscript", functype="binaryfunc") get;
#endif
        std::shared_ptr<Node> get(std::shared_ptr<Node> key);

    };

#ifdef SWIG
    %template(set) MappingNode::set<double, double>;
    %template(set) MappingNode::set<double, std::string>;
    %template(set) MappingNode::set<double, long long>;
    %template(set) MappingNode::set_k<double>;

    %template(set) MappingNode::set<std::string, double>;
    %template(set) MappingNode::set<std::string, std::string>;
    %template(set) MappingNode::set<std::string, long long>;
    %template(set) MappingNode::set_k<std::string>;

    %template(set) MappingNode::set<long long, double>;
    %template(set) MappingNode::set<long long, std::string>;
    %template(set) MappingNode::set<long long, long long>;
    %template(set) MappingNode::set_k<long long>;

    %template(set) MappingNode::set_v<double>;
    %template(set) MappingNode::set_v<std::string>;
    %template(set) MappingNode::set_v<long long>;
#endif

}

