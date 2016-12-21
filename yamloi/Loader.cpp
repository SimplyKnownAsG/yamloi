#include "yamloi/SequenceNode.hpp"
#include "yamloi/ScalarNode.hpp"
#include "yamloi/Loader.hpp"
#include "yamloi/Characters.hpp"

namespace yamloi {

    Node *Loader::load() {
        return this->parse(Characters::empty);
    }

    Node *Loader::parse(const std::unordered_set<char>& break_chars) {
        char c;
        Node *node = NULL;
        TODO("Loader::parse");
        while (this->next_char(c, true) && !break_chars.count(c)) {
            if (Characters::whitespace.count(c)) {
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
            while (this->stream->get(c) && Characters::whitespace.count(c)) {
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
            this->length = 0;
        }
        else {
            this->length = 1;
        }
        std::string result = this->content.str();
        this->content.str("");
        this->content.clear();
        return result;
    }

}

