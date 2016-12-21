#pragma once

#include "yamloi/Node.hpp"
#include "yamloi/Loader.hpp"
#include "yamloi/Exceptions.hpp"

#include <sstream>
#include <typeinfo>


namespace yamloi {

    class Loader;

    class ScalarNode : public Node {
    private:
        std::string data;

        bool lt(const Node* node) const;

        bool eq(const Node* node) const;

        bool gt(const Node* node) const;

        void _dump(Dumper& dumper) const override;

        bool need_quote() const ;

    public:
        ScalarNode(std::string data) { this->data = data; };

        ScalarNode(long long data) { this->data = std::to_string(data); };

        ScalarNode(double data) { this->data = std::to_string(data); };

        static Node *parse(Loader *loader, const std::unordered_set<char>& break_chars);

        template <typename T> T as() {
            std::stringstream ss(this->data);
            T result;
            /* ss >> std::noskipws >> result; */
            if (!(ss >> result)) {
                std::stringstream msg;
                msg << "could not convert `" << this->data << "` to " << typeid(T).name() << ".";
                throw TypeError(msg.str());
            }
            return result;
        };

        template <typename T> T to() {
            return this->as<T>();
        };

        bool const is_scalar() const {
            return true;
        };

    };

#ifdef SWIG

    %template(as_string) ScalarNode::to<std::string>;
    %template(as_long) ScalarNode::to<long long>;
    %template(as_double) ScalarNode::to<double>;

#endif

    template <> std::string ScalarNode::as<std::string>();

}

