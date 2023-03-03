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

        const std::string& getSymbol() {
            return _symbol;
        }

        std::shared_ptr<ASTNode> getA() {
            return _a;
        }

        std::shared_ptr<ASTNode> getB() {
            return _b;
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
        BinaryOperatorParser(std::vector<std::string> symbols) {
            _symbols = symbols;
        }

        void addPrecedent(std::shared_ptr<TokenParser> parser) {
            precedents.push_back(parser);
        }

        std::shared_ptr<ASTNode> parse(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed) {
            tl.begin();
            std::shared_ptr<ASTNode> a;
            bool aParsed = false;

            // Check for precendents as the left argument first if not recursive
            for (auto& parser : precedents) {
                a = parser->parse(tl, aParsed);
                if (aParsed) { break; }
            }
            
            // If no valid left argument was found, give up
            if (!aParsed) {
                tl.abort();
                return NULL;
            }

            // If the symbol is invalid, return left argument
            std::string symbol = "";
            bool symbolFound = false;
            for (const auto& _sym : _symbols) {
                bool wrong = false;
                if (tl.available() < _sym.length()) { continue; }
                for (int i = 0; i < _sym.length(); i++) {
                    std::shared_ptr<token::Symbol> sym = std::dynamic_pointer_cast<token::Symbol>(tl[i]);
                    if (!sym || sym->getValue() != _sym[i]) {
                        wrong = true;
                        break;
                    }
                }
                if (!wrong) {
                    symbol = _sym;
                    break;
                }
            }
            if (symbol.empty()) {
                tl.commit();
                parsed = true;
                return a;
            }
            
            tl.consume(symbol.length());

            // If no more symbols are available for parsing, give up with an error
            if (!tl.available()) {
                tl.abort();
                throw std::runtime_error("Expected token");
            }

            // Check for same level as the right argument
            std::shared_ptr<ASTNode> b;
            bool bParsed = false;
            b = parse(tl, bParsed);

            // If no valid right precedent argument was found, try peers
            if (!bParsed) {
                tl.abort();
                throw std::runtime_error(std::string("No valid second argument found for operator '") + symbol + "'");
            }

            // If the right argument was an operator with the same precedence, reverse the order
            std::shared_ptr<BinaryOperator> binOp = std::dynamic_pointer_cast<BinaryOperator>(b);
            if (binOp && std::find(_symbols.begin(), _symbols.end(), binOp->getSymbol()) != _symbols.end()) {
                tl.commit();
                parsed = true;
                auto newA = std::make_shared<BinaryOperator>(symbol, a, binOp->getA());
                return std::dynamic_pointer_cast<ASTNode>(std::make_shared<BinaryOperator>(binOp->getSymbol(), newA, binOp->getB()));
            }
            else {
                tl.commit();
                parsed = true;
                return std::dynamic_pointer_cast<ASTNode>(std::make_shared<BinaryOperator>(symbol, a, b));
            }
        }

    private:
        std::vector<std::string> _symbols;
        std::vector<std::shared_ptr<TokenParser>> precedents;
    };
};