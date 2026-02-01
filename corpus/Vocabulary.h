//
// Created by sagnik on 01-02-2026.
//

#ifndef VOCABULARY_H
#define VOCABULARY_H
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Tokenizer.h"

namespace mkv::corpus {
    using TokenID = uint32_t;
    class Vocabulary {
    public:
        static constexpr TokenID INVALID = static_cast<TokenID>(-1);
        static constexpr TokenID STARTID = static_cast<TokenID>(0);
        static constexpr TokenID ENDID = static_cast<TokenID>(1);
        static constexpr TokenID IDENTIFIER = static_cast<TokenID>(2);
        static constexpr Token START = "<START>";
        static constexpr Token END = "<END>";
        static constexpr Token UNK = "<UNK>";

        Vocabulary() {
            id_to_token.reserve(10'000);
            getOrAddID(START);
            getOrAddID(END);
            getOrAddID(UNK);
        }

        TokenID getOrAddID(const Token& token) {
            if (const auto it = token_to_id.find(token); it != token_to_id.end()) return it->second;

            auto id = static_cast<TokenID>(id_to_token.size());
            id_to_token.push_back(token);
            token_to_id.emplace(token, id);
            return id;
        }

        TokenID getId(const Token& token) const {
            if (const auto it = token_to_id.find(token); it == token_to_id.end()) return INVALID;
            else return it->second;
        }

        Token getToken(const TokenID& id) const {
            if (id >= id_to_token.size()) throw std::out_of_range("Token ID does not exist.");
            return id_to_token[id];
        }

        size_t size() const {
            return id_to_token.size();
        }

    private:
        std::unordered_map<Token, TokenID> token_to_id;
        std::vector<Token> id_to_token;
    };
}

#endif //VOCABULARY_H
