#include "ScalarNode.hpp"
#include "SequenceNode.hpp"
#include "MappingNode.hpp"


namespace yamloi {

    Node *ScalarNode::parse(Loader *loader, std::unordered_set<char>& break_chars) {
        char c;
        while (loader->next_char(c) && !break_chars.count(c)) {
            continue;
        }
        auto node = (Node *)(new ScalarNode(loader->consume()));
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

    const std::string ScalarNode::dump() const {
        bool needs_quote = this->need_quote();
        /* bool has_lf = this->data.find('\n') != std::string::npos; */
        std::stringstream result;

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
            result << quote;
        }
        for (auto c : this->data) {
            switch (c) {
                case '\n':
                    result << "\\n";
                    break;
                case '\'':
                case '"':
                    if (c == quote) {
                        result << '\\';
                    }
                    result << c;
                    break;
                default:
                    result << c;
                    break;
            }
        }
        if (quote) {
            result << quote;
        }
        return result.str();
    };
}
