#pragma once

#include "yamloi/Format.hpp"
#include "yamloi/Node.hpp"

#include <algorithm>
#include <memory>
#include <ostream>
#include <sstream>
#include <typeinfo>

namespace yamloi {

    class Dumper {

    private:
        std::ostringstream *stream;

    public:

        int indent_level = 0;

        const Format format;

        Dumper(Format format=default_format) : format(format)
        {
            this->stream = new std::ostringstream;
        };

        ~Dumper() ;

        std::string str() ;

        template <class T>
        Dumper& operator<<(T thing) {
            *(this->stream) << thing;
            return *this;
        };

        Dumper& operator<<(std::ostream& (*os)(std::ostream&)) ;

        Dumper& operator<<(Node* node) ;

        Dumper& operator<<(std::shared_ptr<Node>& node) ;

        Dumper& operator<<(const std::shared_ptr<Node>& node) ;

        std::string get_indent() ;

        std::string get_seq_start() ;

        std::string get_seq_separator() ;
    };

};

