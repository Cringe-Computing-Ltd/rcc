#pragma once
#include "../lexer.h"

namespace token {
    class Symbol : public LexerToken {
    public:
        Symbol(char sym) { _sym = sym; }

        static std::shared_ptr<LexerToken> lex(SessionList<std::string, char>& sl, bool& lexed) {
            char c = sl[0];
            lexed = true;
            sl.consume(1);
            return std::static_pointer_cast<LexerToken>(std::make_shared<Symbol>(c));
        }

        char getValue() { return _sym; }

        std::string dump() {
            char buf[1024];
            sprintf(buf, "[SYMBOL '%c']", _sym);
            return buf;
        }

    private:
        char _sym;
    };
}