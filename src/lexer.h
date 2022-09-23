#pragma once
#include <string>
#include <vector>
#include <memory>
#include "session_list.h"

class LexerToken {
public:
    virtual std::string dump() { return "[UNKNOWN]"; }
};

typedef std::shared_ptr<LexerToken> (*LexerHandler)(SessionList<std::string, char>& sl, bool& lexed);

class Lexer {
public:
    void registerHandler(LexerHandler handler);

    std::vector<std::shared_ptr<LexerToken>> lex(std::string str);

private:
    std::vector<LexerHandler> handlers;

};