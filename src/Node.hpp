#pragma once

#include <string>

#include <memory>
#include <algorithm>
#include <functional>

// TODO: this is crappy, and wrong...
#include <iostream>

#define TODO(msg) std::cout << "[yamloi.debug] " << msg << std::endl;


namespace yamloi {

    class Node {
    private:
        friend struct std::less<std::shared_ptr<Node>>;
        friend struct std::greater<std::shared_ptr<Node>>;

        virtual bool lt(const Node* node) const = 0;

        virtual bool eq(const Node* node) const = 0;

        virtual bool gt(const Node* node) const = 0;

        /* virtual bool le(const Node* node) const { */
        /*     return lt(node) || eq(node); */
        /* }; */

        /* virtual bool ge(const Node* node) const { */
        /*     return gt(node) || eq(node); */
        /* }; */

    protected:
        virtual ~Node() {};

    public:
        virtual const bool is_scalar() const {
            return false;
        };

        bool used_gt = 0;

        virtual const bool is_sequence() const {
            return false;
        };

        virtual const bool is_mapping() const {
            return false;
        };

        virtual const std::string dump() const = 0;

        bool operator<(const Node* node) const { return this->lt(node); };

        /* bool operator<=(const Node* node) { return le(node); }; */

        bool operator==(const Node* node) const { return this->eq(node); };

        bool operator!=(const Node* node) const { return !this->eq(node); };

        bool operator>(const Node* node) const { return this->gt(node); };

        /* bool operator>=(const Node* node) { return ge(node); }; */

    };


}

namespace std {
    template<> struct less<shared_ptr<yamloi::Node>> {
        bool operator()(const shared_ptr<yamloi::Node>& left, const shared_ptr<yamloi::Node>& right) const {
            return left->lt(right.get());
        };
    };

    template<> struct greater<shared_ptr<yamloi::Node>> {
        bool operator()(const shared_ptr<yamloi::Node>& left, const shared_ptr<yamloi::Node>& right) const {
            return left->gt(right.get());
        };
    };
}

