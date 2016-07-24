#include "Node.hpp"
#include "Dumper.hpp"

#include <string>


namespace yamloi {

    const Format* Node::get_format(const Format& dumper_format) const {
        return (this->format != NULL) ? this->format : &dumper_format;
    }

    const bool Node::is_scalar() const {
        return false;
    };

    const bool Node::is_sequence() const {
        return false;
    };

    const bool Node::is_mapping() const {
        return false;
    };

    const std::string Node::dump(Format format) const {
        Dumper dumper(format);
        this->_dump(dumper);
        return dumper.str();
    };

    const std::string Node::dump() const {
        Dumper dumper;
        this->_dump(dumper);
        return dumper.str();
    };

    bool Node::operator<(const Node* node) const {
        return this->lt(node);
    };

    bool Node::operator==(const Node* node) const {
        return this->eq(node);
    };

    bool Node::operator!=(const Node* node) const {
        return !this->eq(node);
    };

    bool Node::operator>(const Node* node) const {
        return this->gt(node);
    };

}

