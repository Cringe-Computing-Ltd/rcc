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

class TokenParser {
public:
    virtual std::shared_ptr<ASTNode> parse(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed) = 0;
};

class Parser {
public:
    void registerParser(std::shared_ptr<TokenParser> parser);

    std::vector<std::shared_ptr<ASTNode>> parse(std::vector<std::shared_ptr<LexerToken>> tks);

private:
    std::vector<std::shared_ptr<TokenParser>> parsers;

};