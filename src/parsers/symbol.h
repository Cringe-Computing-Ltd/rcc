#pragma once
#include "../parser.h"
#include "../lexers/word.h"

namespace parsers {
    class Symbol : public ASTNode {
    public:
        Symbol(std::string name) { _name = name; }

        std::string dump() {
            return _name;
        }

    private:
        std::string _name;
        
    };

    class SymbolParser : public TokenParser {
    public:
        SymbolParser() {}
        std::shared_ptr<ASTNode> parse(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed) {
            std::shared_ptr<token::Word> word = std::dynamic_pointer_cast<token::Word>(tl[0]);
            if (!word) { return NULL; }
            parsed = true;
            tl.consume(1);
            return std::dynamic_pointer_cast<ASTNode>(std::make_shared<Symbol>(word->getName()));
        }
    };
}