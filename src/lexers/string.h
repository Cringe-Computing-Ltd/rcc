#pragma once
#include "../lexer.h"

namespace token {
    class String : public LexerToken {
    public:
        String(std::string value) { _value = value; }

        static std::shared_ptr<LexerToken> lex(SessionList<std::string, char>& sl, bool& lexed) {
            // Check for the beginning of a string
            if (sl[0] != '"') { return NULL; }
            lexed = true;
            sl.consume(1);

            // Iterate through each char
            std::string value = "";
            bool escape = false;
            while (sl.available()) {
                char c = sl[0];

                // Handle special escapes
                if (escape) {
                    escape = false;
                    if (c == 'a') { c = '\a'; }
                    else if (c == 'b') { c = '\b'; }
                    else if (c == 'f') { c = '\f'; }
                    else if (c == 'n') { c = '\n'; }
                    else if (c == 'r') { c = '\r'; }
                    else if (c == 't') { c = '\t'; }
                    else if (c == 'v') { c = '\v'; }
                    else if (c == '0') { c = '\0'; }
                    // TODO: Handle hex and octal
                }
                else {
                    // Check for end of string
                    if (c == '"') {
                        sl.consume(1);
                        break;
                    }

                    // Check for escape
                    if (c == '\\') {
                        escape = true;
                        sl.consume(1);
                        continue;
                    }
                }
                
                // Save char and continue
                value += c;
                sl.consume(1);
            }
            
            return std::static_pointer_cast<LexerToken>(std::make_shared<String>(value));
        }

        std::string dump() {
            char buf[1024];
            sprintf(buf, "[STRING \'%s\']", _value.c_str());
            return buf;
        }

    private:
        std::string _value;
    };
}