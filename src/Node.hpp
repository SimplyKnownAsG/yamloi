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

        virtual bool lt(const Node* node) const = 0;

        virtual bool eq(const Node* node) const = 0;

        virtual bool gt(const Node* node) const = 0;

        /* virtual bool le(const Node* node) const { */
        /*     return lt(node) || eq(node); */
        /* }; */

        /* virtual bool ge(const Node* node) const { */
        /*     return gt(node) || eq(node); */
        /* }; */

    public:

        virtual const bool is_scalar() const {
            return false;
        };

        virtual ~Node() {};

        bool used_gt = 0;

        virtual const bool is_sequence() const {
            return false;
        };

        virtual const bool is_mapping() const {
            return false;
        };

        virtual const std::string dump() const = 0;

        bool operator<(const Node* node) const { return this->lt(node); };

        bool operator==(const Node* node) const { return this->eq(node); };

        bool operator!=(const Node* node) const { return !this->eq(node); };

        bool operator>(const Node* node) const { return this->gt(node); };

    };


}

namespace std {
    template<> struct less<shared_ptr<yamloi::Node>> {
        bool operator()(const shared_ptr<yamloi::Node>& left, const shared_ptr<yamloi::Node>& right) const {
            return (*left.get()) < right.get();
        };
    };

    template<> struct greater<shared_ptr<yamloi::Node>> {
        bool operator()(const shared_ptr<yamloi::Node>& left, const shared_ptr<yamloi::Node>& right) const {
            return (*left.get()) > right.get();
        };
    };

    template<> struct equal_to<shared_ptr<yamloi::Node>> {
        bool operator()(const shared_ptr<yamloi::Node>& left, const shared_ptr<yamloi::Node>& right) const {
            return (*left.get()) == right.get();
        };
    };

    template<> struct not_equal_to<shared_ptr<yamloi::Node>> {
        bool operator()(const shared_ptr<yamloi::Node>& left, const shared_ptr<yamloi::Node>& right) const {
            return (*left.get()) != right.get();
        };
    };
}

