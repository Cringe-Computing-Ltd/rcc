#pragma once
#include "../lexer.h"

namespace token::Comment {
    void lexSingleLine(SessionList<std::string, char>& sl, bool& lexed) {
        if (sl.available() < 2 || sl[0] != '/' || sl[1] != '/') { return; }
        sl.consume(2);
        lexed = true;
        while (sl.available() && sl[0] != '\n') { sl.consume(1); }
    }

    void lexMultiLine(SessionList<std::string, char>& sl, bool& lexed) {
        if (sl.available() < 2 || sl[0] != '/' || sl[1] != '*') { return; }
        lexed = true;
        while (sl.available()) {
            if (sl.available() >= 2 && sl[0] == '*' && sl[1] == '/') {
                sl.consume(2);
                return;
            }
            sl.consume(1);
        }
    }

    std::shared_ptr<LexerToken> lex(SessionList<std::string, char>& sl, bool& lexed) {
        lexSingleLine(sl, lexed);
        if (!lexed) {
            lexMultiLine(sl, lexed);
        }
        return NULL;
    }
}