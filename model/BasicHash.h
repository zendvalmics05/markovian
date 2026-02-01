//
// Created by sagnik on 01-02-2026.
//

#ifndef BASICHASH_H
#define BASICHASH_H

namespace mkv::model {
    using TokenID = corpus::TokenID;
    using Token = corpus::Token;

    using State = std::vector<TokenID>;

    class BasicHash {
    public:
        size_t operator()(const State& s) const {
            size_t h = 0;
            for (const auto id : s) {
                h ^= std::hash<TokenID>{}(id) + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };
}

#endif //BASICHASH_H
