#pragma once

#include "Node.hpp"

#include <istream>
#include <fstream>
#include <sstream>
#include <unordered_set>

namespace yamloi {

    class Loader {

    private:
        friend class ScalarNode;
        friend class SequenceNode;
        friend class MappingNode;
        std::istream *stream;
        char next_c;
        size_t length;
        std::ostringstream content;
        static std::unordered_set<char> break_chars;
        static std::unordered_set<char> whitespace;

    public:

        Loader(std::string msg, bool string=false) {
            this->length = 0;
            this->next_c = -1;
            if (string) {
                this->stream = new std::istringstream(msg);
            }
            else {
                this->stream = new std::ifstream(msg);
            }
        };

        ~Loader() {
            if (this->stream) {
                delete this->stream;
                this->stream = NULL;
            }
        };

        Node *load();

        Node *parse(std::unordered_set<char>& break_chars);

        bool next_char(char &c, bool skip_whitespace=false);

        std::string consume(bool take_next=false);
    };

};

