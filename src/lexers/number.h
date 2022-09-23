#pragma once
#include <cstdlib>
#include <stdint.h>
#include <inttypes.h>
#include "../lexer.h"

namespace token {
    class Number : public LexerToken {
    public:
        Number(uint64_t value) { _value = value; }

        static std::shared_ptr<LexerToken> lex(SessionList<std::string, char>& sl, bool& lexed) {
            uint64_t value;

            // Iterate through each char
            std::string numstr = "";
            while (sl.available()) {
                char c = sl[0];

                // If invalid, give up
                if (c < '0' || c > '9') { break; }

                // Save char and continue
                numstr += c;
                lexed = true;
                sl.consume(1);
            }

            if (!lexed) { return NULL; }
            
            return std::static_pointer_cast<LexerToken>(std::make_shared<Number>(std::atoll(numstr.c_str())));
        }

        uint64_t getValue() { return _value; }

        std::string dump() {
            char buf[1024];
            sprintf(buf, "[NUMBER %" PRIu64 "]", _value);
            return buf;
        }

    private:
        uint64_t _value;
    };
}