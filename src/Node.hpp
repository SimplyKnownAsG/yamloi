#pragma once

#include "Format.hpp"

#include <string>

#include <memory>
#include <algorithm>
#include <functional>

// TODO: this is crappy, and wrong...
#include <iostream>

#define TODO(msg) std::cout << "[yamloi.debug] " << msg << std::endl;


namespace yamloi {

    class Dumper;

    class Node {
    private:

        friend class Dumper;

        virtual bool lt(const Node* node) const = 0;

        virtual bool eq(const Node* node) const = 0;

        virtual bool gt(const Node* node) const = 0;

        virtual void _dump(Dumper &dumper) const = 0;

    public:
        Format *format = 0;

        virtual ~Node() {};

        virtual const bool is_scalar() const;

        virtual const bool is_sequence() const;

        virtual const bool is_mapping() const;

        const std::string dump(Format format) const;

        const std::string dump() const;

        bool operator<(const Node* node) const;

        bool operator==(const Node* node) const;

        bool operator!=(const Node* node) const;

        bool operator>(const Node* node) const;

        const Format *get_format(const Format& dumper_format) const;

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

