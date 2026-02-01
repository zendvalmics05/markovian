//
// Created by sagnik on 01-02-2026.
//

#ifndef WORDTOKENIZER_H
#define WORDTOKENIZER_H
#include "Tokenizer.h"

namespace mkv::corpus {
    class WordTokenizer : public Tokenizer {
    public:
        [[nodiscard]] std::vector<Token> tokenize(const std::string &text) const override {
            std::vector<Token> tokens;
            Token current;

            for (char ch : text) {
                if (std::isalpha(static_cast<unsigned char>(ch))) {
                    current.push_back(std::tolower(static_cast<unsigned char>(ch)));
                } else {
                    if (!current.empty()) {
                        tokens.push_back(current);
                        current.clear();
                    }
                }
            }

            if (!current.empty()) {
                tokens.push_back(current);
            }
            return tokens;
        }
    };
}

#endif //WORDTOKENIZER_H
