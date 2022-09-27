#pragma once
#include "../parser.h"
#include "../lexers/symbol.h"
#include <stdexcept>

namespace parsers {
    class CategoryParser : public TokenParser {
    public:
        CategoryParser(std::vector<std::shared_ptr<TokenParser>> parsers = {}) {
            this->parsers = parsers;
        }

        void addParser(std::shared_ptr<TokenParser> parser) {
            parsers.push_back(parser);
        }

        std::shared_ptr<ASTNode> parse(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed) {
            for (auto& parser : parsers) {
                std::shared_ptr<ASTNode> an = parser->parse(tl, parsed);
                if (parsed) { return an; }
            }
            return NULL;
        }

    private:
        std::vector<std::shared_ptr<TokenParser>> parsers;

    };
};