#pragma once
#include "../parser.h"
#include "../lexers/number.h"
#include "literal.h"
#include "symbol.h"
#include "binary_operator.h"

namespace parsers {
    class Expression : public ASTNode {
    public:
        Expression() {  }

        std::string dump() {
            return "TODO";
        }

    private:
        
    };

    class ExpressionParser : public TokenParser {
    public:
        ExpressionParser() {
            auto literal = std::make_shared<parsers::LiteralParser>();
            auto symbol = std::make_shared<parsers::SymbolParser>();

            std::vector<std::string> lvl3Syms = {
                "*", "/", "%"
            };
            auto lvl3Op = std::make_shared<parsers::BinaryOperatorParser>(lvl3Syms);
            lvl3Op->addPrecedent(literal);
            lvl3Op->addPrecedent(symbol);

            std::vector<std::string> lvl4Syms = {
                "+", "-"
            };
            auto lvl4Op = std::make_shared<parsers::BinaryOperatorParser>(lvl4Syms);
            lvl4Op->addPrecedent(lvl3Op);

            std::vector<std::string> lvl5Syms = {
                "<<", ">>"
            };
            auto lvl5Op = std::make_shared<parsers::BinaryOperatorParser>(lvl5Syms);
            lvl5Op->addPrecedent(lvl4Op);

            std::vector<std::string> lvl6Syms = {
                "<", "<=", ">", ">="
            };
            auto lvl6Op = std::make_shared<parsers::BinaryOperatorParser>(lvl6Syms);
            lvl6Op->addPrecedent(lvl5Op);

            std::vector<std::string> lvl7Syms = {
                "==", "!="
            };
            auto lvl7Op = std::make_shared<parsers::BinaryOperatorParser>(lvl7Syms);
            lvl7Op->addPrecedent(lvl6Op);

            std::vector<std::string> lvl8Syms = {
                "&"
            };
            auto lvl8Op = std::make_shared<parsers::BinaryOperatorParser>(lvl8Syms);
            lvl8Op->addPrecedent(lvl7Op);

            std::vector<std::string> lvl9Syms = {
                "^"
            };
            auto lvl9Op = std::make_shared<parsers::BinaryOperatorParser>(lvl9Syms);
            lvl9Op->addPrecedent(lvl8Op);

            std::vector<std::string> lvl10Syms = {
                "|"
            };
            auto lvl10Op = std::make_shared<parsers::BinaryOperatorParser>(lvl10Syms);
            lvl10Op->addPrecedent(lvl9Op);

            std::vector<std::string> lvl11Syms = {
                "&&"
            };
            auto lvl11Op = std::make_shared<parsers::BinaryOperatorParser>(lvl11Syms);
            lvl11Op->addPrecedent(lvl10Op);

            std::vector<std::string> lvl12Syms = {
                "||"
            };
            auto lvl12Op = std::make_shared<parsers::BinaryOperatorParser>(lvl12Syms);
            lvl12Op->addPrecedent(lvl11Op);


            topLvlParser = lvl12Op;
        }
        std::shared_ptr<ASTNode> parse(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed) {
            return topLvlParser->parse(tl, parsed);
        }

    private:
        std::shared_ptr<TokenParser> topLvlParser;
    };
}