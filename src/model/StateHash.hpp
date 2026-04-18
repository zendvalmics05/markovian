#ifndef STATEHASH_H
#define STATEHASH_H
#include <vector>
#include <cstddef>
#include "Vocabulary.hpp"

namespace mkv::model {
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
