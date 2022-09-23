#pragma once
#include "../lexer.h"

namespace token::Space {
    std::shared_ptr<LexerToken> lex(SessionList<std::string, char>& sl, bool& lexed) {
        // Skip all spaces
        while (sl.available()) {
            char c = sl[0];
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') { break; }
            sl.consume(1);
            lexed = true;
        }
        return NULL;
    }
}