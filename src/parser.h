#pragma once
#include <string>
#include <vector>
#include <memory>
#include "session_list.h"
#include "lexer.h"

class ASTNode {
public:
    virtual std::string dump() { return "[UNKNOWN]"; }
};

typedef std::shared_ptr<ASTNode> (*ParserHandler)(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed);

class Parser {
public:
    void registerHandler(ParserHandler handler);

    std::vector<std::shared_ptr<ASTNode>> lex(std::vector<std::shared_ptr<LexerToken>> tks);

private:
    std::vector<ParserHandler> handlers;

};