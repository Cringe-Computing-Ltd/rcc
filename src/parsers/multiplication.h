#pragma once
#include "../parser.h"
#include "../lexers/symbol.h"
#include "literal.h"
#include "symbol.h"

namespace parsers {
    class Multiplication : public ASTNode {
    public:
        Multiplication(std::shared_ptr<ASTNode> a, std::shared_ptr<ASTNode> b) {
            _a = a;
            _b = b;
        }

        static std::shared_ptr<ASTNode> parse(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed) {
            tl.begin();

            // First argument can be either a literal or symbol
            bool aParsed = false;
            std::shared_ptr<ASTNode> a = Literal::parse(tl, aParsed);
            if (!aParsed) {
                a = Symbol::parse(tl, aParsed);
            }
            if (!aParsed) {
                tl.abort();
                return NULL;
            }

            // If not a multiplication, return the first argument instead
            if (!tl.available()) {
                tl.commit();
                parsed = true;
                return a;
            }
            std::shared_ptr<token::Symbol> mulsym = std::dynamic_pointer_cast<token::Symbol>(tl[0]);
            if (!mulsym || mulsym->getValue() != '*') {
                tl.commit();
                parsed = true;
                return a;
            }
            tl.consume(1);

            // The second argument can be a multiplicatio or division (TODO: Implement)
            if (!tl.available()) {
                tl.abort();
                return NULL;
            }
            bool bParsed = false;
            std::shared_ptr<ASTNode> b = parse(tl, bParsed);
            if (!bParsed) {
                tl.abort();
                return NULL;
            }

            // Commit changes to list and return AST npde
            tl.commit();
            parsed = true;
            return std::dynamic_pointer_cast<ASTNode>(std::make_shared<Multiplication>(a, b));
        }

        std::string dump() {
            return "(" + _a->dump() + " * " + _b->dump() + ")";
        }

    private:
        std::shared_ptr<ASTNode> _a;
        std::shared_ptr<ASTNode> _b;
        
    };
}