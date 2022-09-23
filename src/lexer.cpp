#include "lexer.h"
#include <stdexcept>

void Lexer::registerHandler(LexerHandler handler) {
    handlers.push_back(handler);
}

std::vector<std::shared_ptr<LexerToken>> Lexer::lex(std::string str) {
    SessionList<std::string, char> sl(str);
    std::vector<std::shared_ptr<LexerToken>> tks;

    while (sl.available()) {
        std::shared_ptr<LexerToken> tk;
        bool lexed = false;
        for (auto& handler : handlers) {
            // Try lexing
            tk = handler(sl, lexed);
            
            // If valid, save and stop
            if (lexed) {
                if (tk) { tks.push_back(tk); }
                break;
            }
        }
        if (!lexed) {
            throw std::runtime_error("Lexing failed");
        }
    }

    return tks;
}