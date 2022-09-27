#pragma once
#include "../parser.h"
#include "../lexers/symbol.h"
#include <stdexcept>

namespace parsers {
    class BinaryOperator : public ASTNode {
    public:
        BinaryOperator(std::string symbol, std::shared_ptr<ASTNode> a, std::shared_ptr<ASTNode> b) {
            _symbol = symbol;
            _a = a;
            _b = b;
        }

        std::string dump() {
            return "(" + _a->dump() + _symbol + _b->dump() + ")";
        }

    private:
        std::string _symbol;
        std::shared_ptr<ASTNode> _a;
        std::shared_ptr<ASTNode> _b;
    };

    class BinaryOperatorParser : public TokenParser {
    public:
        BinaryOperatorParser(std::string symbol) {
            _symbol = symbol;
        }

        void addPrecedent(std::shared_ptr<TokenParser> parser) {
            precedents.push_back(parser);
        }

        void addPeer(std::shared_ptr<TokenParser> parser) {
            peers.push_back(parser);
        }

        std::shared_ptr<ASTNode> parse(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed) {
            tl.begin();
            
            // Check for predecents as the left argument
            std::shared_ptr<ASTNode> a;
            bool aParsed = false;
            for (auto& parser : precedents) {
                a = parser->parse(tl, aParsed);
                if (aParsed) { break; }
            }
            
            // If no valid left argument was found, give up
            if (!aParsed || tl.available() < _symbol.length()) {
                tl.abort();
                return NULL;
            }

            // If the symbol is invalid, give up
            for (int i = 0; i < _symbol.length(); i++) {
                std::shared_ptr<token::Symbol> sym = std::dynamic_pointer_cast<token::Symbol>(tl[i]);
                if (!sym || sym->getValue() != _symbol[i]) {
                    tl.abort();
                    return NULL;
                }
            }
            tl.consume(_symbol.length());

            // If no more symbols are available for parsing, give up with an error
            if (!tl.available()) {
                tl.abort();
                throw std::runtime_error("Expected token");
            }

            // Check for peers as the right argument
            std::shared_ptr<ASTNode> b;
            bool bParsed = false;
            for (auto& parser : peers) {
                b = parser->parse(tl, bParsed);
                if (bParsed) { break; }
            }

            // If no valid right argument was found, give up
            if (!bParsed) {
                tl.abort();
                throw std::runtime_error(std::string("No valid second argument found for operator '") + _symbol + "'");
            }

            tl.commit();
            parsed = true;
            return std::dynamic_pointer_cast<ASTNode>(std::make_shared<BinaryOperator>(_symbol, a, b));
        }

    private:
        std::string _symbol;
        std::vector<std::shared_ptr<TokenParser>> precedents;
        std::vector<std::shared_ptr<TokenParser>> peers;

    };
};