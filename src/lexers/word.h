#pragma once
#include "../lexer.h"

namespace token {
    class Word : public LexerToken {
    public:
        Word(std::string name) { _name = name; }

        static std::shared_ptr<LexerToken> lex(SessionList<std::string, char>& sl, bool& lexed) {
            std::string name = "";
            bool first = true;

            // Iterate through each char
            while (sl.available()) {
                char c = sl[0];

                // If invalid, give up
                if ((first && c >= '0' && c <= '9') || !std::isalnum(c)) {
                    break;
                }

                // Save char and continue
                name += c;
                first = false;
                lexed = true;
                sl.consume(1);
            }

            if (!lexed) { return NULL; }
            
            return std::static_pointer_cast<LexerToken>(std::make_shared<Word>(name));
        }

        std::string dump() {
            char buf[1024];
            sprintf(buf, "[WORD '%s']", _name.c_str());
            return buf;
        }

    private:
        std::string _name;
    };
}