#include "Dumper.hpp"

namespace yamloi {

    Dumper::~Dumper() {
        if (this->stream) {
            delete this->stream;
            this->stream = NULL;
        }
    }

    std::string Dumper::str() {
        return this->stream->str();
    }

    Dumper& Dumper::operator<<(std::ostream& (*os)(std::ostream&)) {
        *(this->stream) << os;
        return *this;
    };

    Dumper& Dumper::operator<<(Node* node) {
        this->indent_level += 1;
        node->_dump(*this);
        this->indent_level -= 1;
        return *this;
    }

    Dumper& Dumper::operator<<(std::shared_ptr<Node>& node) {
        this->indent_level += 1;
        node->_dump(*this);
        this->indent_level -= 1;
        return *this;
    }

    std::string Dumper::get_indent() {
        return std::string(this->format.indent_spaces * this->indent_level, ' ');
    }

    std::string Dumper::get_seq_start() {
        return "-" + std::string(std::max(this->format.indent_spaces - 1, 0), ' ');
    }

    std::string Dumper::get_seq_separator() {
        return std::string(std::max(this->format.indent_spaces * this->indent_level, 0), ' ')
            + "-"
            + std::string(this->format.indent_spaces - 1, ' ');
    }

}

