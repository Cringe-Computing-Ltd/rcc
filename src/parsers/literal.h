#pragma once
#include "../parser.h"
#include "../lexers/number.h"

namespace parsers {
    class Literal : public ASTNode {
    public:
        Literal(uint64_t value) { _value = value; }

        std::string dump() {
            char buf[1024];
            sprintf(buf, "%" PRIu64, _value);
            return buf;
        }

    private:
        uint64_t _value;
        
    };

    class LiteralParser : public TokenParser {
    public:
        LiteralParser() {}
        std::shared_ptr<ASTNode> parse(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed) {
            std::shared_ptr<token::Number> num = std::dynamic_pointer_cast<token::Number>(tl[0]);
            if (!num) { return NULL; }
            parsed = true;
            tl.consume(1);
            return std::dynamic_pointer_cast<ASTNode>(std::make_shared<Literal>(num->getValue()));
        }
    };
}