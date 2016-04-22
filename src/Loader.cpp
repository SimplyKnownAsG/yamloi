#include "SequenceNode.hpp"
#include "ScalarNode.hpp"
#include "Loader.hpp"

namespace yamloi {

    std::unordered_set<char> Loader::break_chars = { };
    std::unordered_set<char> Loader::whitespace = { ' ', '\t', '\r', '\n' };

    Node *Loader::load() {
        return this->parse(Loader::break_chars);
    }

    Node *Loader::parse(std::unordered_set<char>& break_chars) {
        char c;
        Node *node = NULL;
        while (this->next_char(c) && !break_chars.count(c)) {
            if (Loader::whitespace.count(c)) {
                this->next_c = -1;
                continue;
            }
            if ((node = SequenceNode::parse(this))) {
                return node;
            }
            else if ((node = ScalarNode::parse(this, break_chars))) {
                return node;
            }
        }
        return node;
    }

    bool Loader::next_char(char &c, bool skip_whitespace) {
        if (this->next_c != -1) {
            this->content << this->next_c;
            this->next_c = -1;
        }
        if (skip_whitespace) {
            while (this->stream->get(c) && Loader::whitespace.count(c)) {
                continue;
            }
        }
        else {
            this->stream->get(c);
        }
        if (this->stream->good()) {
            this->next_c = c;
            this->length += 1;
        }
        else {
            this->next_c = -1;
        }
        return this->stream->good();
    }

    std::string Loader::consume(bool take_next) {
        if (take_next) {
            this->content << this->next_c;
            this->next_c = -1;
        }
        std::string result = this->content.str();
        this->content.str("");
        this->content.clear();
        return result;
    }

}

