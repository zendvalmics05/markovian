//
// Created by sagnik on 01-02-2026.
//

#ifndef STATEHASH_H
#define STATEHASH_H
#include "../corpus/Tokenizer.h"
#include "../corpus/Vocabulary.h"

namespace mkv::model {
    using TokenID = corpus::TokenID;
    using Token = corpus::Token;

    using State = std::vector<TokenID>;

    class StateHash {
    public:
        size_t operator()(const State& s) const {
            return hash(s);
        }
        virtual ~StateHash() = default;

        [[nodiscard]] virtual size_t hash(const State&) const = 0;
    };

}

#endif //STATEHASH_H
