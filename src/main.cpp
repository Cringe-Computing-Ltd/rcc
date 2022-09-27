#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "session_list.h"

#include "lexer.h"
#include "lexers/space.h"
#include "lexers/comment.h"
#include "lexers/word.h"
#include "lexers/string.h"
#include "lexers/number.h"
#include "lexers/symbol.h"

#include "parser.h"
#include "parsers/binary_operator.h"
#include "parsers/literal.h"
#include "parsers/symbol.h"
#include "parsers/category.h"

#include <map>

int main() {
    // Load file
    std::ifstream file("../test/test.c");
    std::stringstream source;
    source << file.rdbuf();

    // Create lexer
    Lexer lx;
    lx.registerHandler(token::Space::lex);
    lx.registerHandler(token::Comment::lex);
    lx.registerHandler(token::Word::lex);
    lx.registerHandler(token::String::lex);
    lx.registerHandler(token::Number::lex);
    lx.registerHandler(token::Symbol::lex);

    // Lex source code
    std::vector<std::shared_ptr<LexerToken>> tks;
    try {
        tks = lx.lex(source.str());
    }
    catch (std::exception e) {
        printf("Error: %s\n", e.what());
        return -1;
    }
    
    // Dump all tokens
    for (auto& tk : tks) {
        printf("%s\n", tk->dump().c_str());
    }
    printf("=== LEXING DONE ===\n");

    // Create parser
    std::vector<std::shared_ptr<TokenParser>> lvl0Parsers = {
        std::make_shared<parsers::LiteralParser>(),
        std::make_shared<parsers::SymbolParser>()
    };
    auto lvl0Parser = std::make_shared<parsers::CategoryParser>(lvl0Parsers);

    std::vector<std::shared_ptr<TokenParser>> lvl1Parsers = {
        std::make_shared<parsers::BinaryOperatorParser>("*"),
        std::make_shared<parsers::BinaryOperatorParser>("/"),
        std::make_shared<parsers::BinaryOperatorParser>("%")
    };
    auto lvl1Parser = std::make_shared<parsers::CategoryParser>(lvl1Parsers);
    for (auto& parser : lvl1Parsers) {
        std::dynamic_pointer_cast<parsers::BinaryOperatorParser>(parser)->addPrecedent(lvl0Parser);
        std::dynamic_pointer_cast<parsers::BinaryOperatorParser>(parser)->addPeer(lvl1Parser);
        std::dynamic_pointer_cast<parsers::BinaryOperatorParser>(parser)->addPeer(lvl0Parser);
    }

    std::vector<std::shared_ptr<TokenParser>> lvl2Parsers = {
        std::make_shared<parsers::BinaryOperatorParser>("+"),
        std::make_shared<parsers::BinaryOperatorParser>("-"),
    };
    auto lvl2Parser = std::make_shared<parsers::CategoryParser>(lvl2Parsers);
    for (auto& parser : lvl2Parsers) {
        std::dynamic_pointer_cast<parsers::BinaryOperatorParser>(parser)->addPrecedent(lvl1Parser);
        std::dynamic_pointer_cast<parsers::BinaryOperatorParser>(parser)->addPrecedent(lvl0Parser);
        std::dynamic_pointer_cast<parsers::BinaryOperatorParser>(parser)->addPeer(lvl2Parser);
        std::dynamic_pointer_cast<parsers::BinaryOperatorParser>(parser)->addPeer(lvl1Parser);
        std::dynamic_pointer_cast<parsers::BinaryOperatorParser>(parser)->addPeer(lvl0Parser);
    }

    Parser ps;
    ps.registerParser(lvl2Parser);

    // Parse tokens
    std::vector<std::shared_ptr<ASTNode>> ast;
    try {
        ast = ps.parse(tks);
    }
    catch (std::exception e) {
        printf("Error: %s\n", e.what());
        return -1;
    }

    // Dump AST
    for (auto& an : ast) {
        printf("%s\n", an->dump().c_str());
    }
    printf("=== PARSING DONE ===\n");
    
    return 0;
}