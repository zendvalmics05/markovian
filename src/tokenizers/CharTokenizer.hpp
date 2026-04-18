#ifndef CHARTOKENIZER_H
#define CHARTOKENIZER_H
#include "Tokenizer.hpp"

namespace mkv::tokenizers {
    class CharTokenizer : public Tokenizer {
    public:
        [[nodiscard]] std::vector<Token> tokenize(const std::string &text) const override {
            std::vector<Token> tokens;
            for (char ch : text) {
                tokens.emplace_back(1, ch);
            }
            return tokens;
        }
    };
}
#endif //CHARTOKENIZER_H
