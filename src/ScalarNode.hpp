#pragma once

#include "Node.hpp"
#include "Loader.hpp"
#include "Exceptions.hpp"

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

        bool need_quote() const ;

    public:
        ScalarNode(std::string data) { this->data = data; };

        ScalarNode(long long data) { this->data = std::to_string(data); };

        ScalarNode(double data) { this->data = std::to_string(data); };

        static Node *parse(Loader *loader, std::unordered_set<char>& break_chars);

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

        bool const is_scalar() const {
            return true;
        };

        const std::string dump() const;


#ifdef SWIG

    %extend {
        %template(as_string) as<std::string>;
        %template(as_int) as<int>;
        %template(as_long) as<long long>;
        %template(as_float) as<float>;
        %template(as_double) as<double>;
    }

#endif

    };

    template <> std::string ScalarNode::as<std::string>();

}

