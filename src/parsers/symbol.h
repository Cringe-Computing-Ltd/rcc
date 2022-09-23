#pragma once
#include "../parser.h"

namespace parsers {
    class Symbol : ASTNode {
    public:
        Symbol(std::string name) { _name = name; }

        static std::shared_ptr<ASTNode> parse(SessionList<std::vector<std::shared_ptr<LexerToken>>, std::shared_ptr<LexerToken>>& tl, bool& parsed) {
            
        }

        std::string dump() {
            return _name;
        }


    private:
        std::string _name;
        
    };
}