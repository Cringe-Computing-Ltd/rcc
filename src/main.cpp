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
#include "parsers/sum.h"

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
    Parser ps;
    ps.registerHandler(parsers::Sum::parse);

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