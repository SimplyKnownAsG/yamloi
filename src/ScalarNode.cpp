#include "Dumper.hpp"
#include "Characters.hpp"
#include "MappingNode.hpp"
#include "ScalarNode.hpp"
#include "SequenceNode.hpp"


namespace yamloi {

    // trim from end
    static inline std::string rtrim(const std::string &s) {
        int ii = s.length() - 1;
        for (; ii >= 0; ii--) {
            if (!Characters::whitespace.count(s.at(ii))) {
                break;
            }
        }
        return s.substr(0, ii + 1);
    }

    Node *ScalarNode::parse(Loader *loader, const std::unordered_set<char>& break_chars) {
        char c;
        TODO("ScalarNode::parse");
        while (loader->next_char(c) && !break_chars.count(c)) {
            continue;
        }
        auto node = (Node *)new ScalarNode(rtrim(loader->consume()));
        return node;
    }

    template <> std::string ScalarNode::as<std::string>() {
        return this->data;
    }

    bool ScalarNode::lt(const Node* node) const {
        if (!node->is_scalar()) {
            return true;
        }
        auto that = (ScalarNode*)node;
        return this->data < that->data;
    }

    bool ScalarNode::eq(const Node* node) const {
        if (!node->is_scalar()) {
            return false;
        }
        auto that = (ScalarNode*)node;
        return this->data == that->data;
    }

    bool ScalarNode::gt(const Node* node) const {
        if (!node->is_scalar()) {
            return false;
        }
        auto that = (ScalarNode*)node;
        /* std::cout<<"comparing `" << this->data << "` > `" << that->data << "`" << std::endl; */
        return this->data > that->data;
    }

    bool ScalarNode::need_quote() const {
        for (char c : this->data) {
            switch (c) {
                case ',':
                case '\n':
                case '[':
                case ']':
                case '{':
                case '}':
                    return true;
                default:
                    continue;
            }
        }
        return false;
    }

    void ScalarNode::_dump(Dumper& dumper) const {
        bool needs_quote = this->need_quote();
        /* bool has_lf = this->data.find('\n') != std::string::npos; */

        char quote = 0;
        if (needs_quote) {
            bool has_dquote = this->data.find('"') != std::string::npos;
            bool has_squote = this->data.find('\'') != std::string::npos;
            if (has_dquote) {
                quote = '\'';
            } else if (has_squote) {
                quote = '"';
            } else {
                quote = '\'';
            }
        }
        if (quote != 0) {
            dumper << quote;
        }
        for (auto c : this->data) {
            switch (c) {
                case '\n':
                    dumper << "\\n";
                    break;
                case '\'':
                case '"':
                    if (c == quote) {
                        dumper << '\\';
                    }
                    dumper << c;
                    break;
                default:
                    dumper << c;
                    break;
            }
        }
        if (quote) {
            dumper << quote;
        }
    };
}

