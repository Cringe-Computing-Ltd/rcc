#include "parser.h"
#include <stdexcept>

void Parser::registerHandler(ParserHandler handler) {
    handlers.push_back(handler);
}

std::vector<std::shared_ptr<ASTNode>> Parser::parse(std::vector<std::shared_ptr<LexerToken>> tks) {
    SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>> tl(tks);
    std::vector<std::shared_ptr<ASTNode>> ast;

    while (tl.available()) {
        std::shared_ptr<ASTNode> an;
        bool parsed = false;
        for (auto& handler : handlers) {
            // Try parsing
            an = handler(tl, parsed);
            
            // If valid, save and stop
            if (parsed) {
                if (an) { ast.push_back(an); }
                break;
            }
        }
        if (!parsed) {
            throw std::runtime_error("Parsing failed");
        }
    }

    return ast;
}