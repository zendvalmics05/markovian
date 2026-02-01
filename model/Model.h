//
// Created by sagnik on 01-02-2026.
//

#ifndef MODEL_H
#define MODEL_H
#include <ranges>

#include "BasicHash.h"

namespace mkv::model {
    using NextTokenCounts = std::unordered_map<TokenID, uint32_t>;
    using TransitionTable = std::unordered_map<State, NextTokenCounts, BasicHash>;

    class MarkovModel {
    public:
        explicit MarkovModel(const size_t order) {
            k = order;
        }

        void train(std::vector<corpus::TokenID>& seq) {
            if (seq.size() <= k) return;
            for (size_t i = 0; i < k; ++i)
                seq.insert(seq.begin(), corpus::Vocabulary::STARTID);

            seq.push_back(corpus::Vocabulary::ENDID);

            for (size_t i = 0; i + k < seq.size(); i++) {
                State state(seq.begin() + static_cast<std::ptrdiff_t>(i), seq.begin() + static_cast<std::ptrdiff_t>(i) + static_cast<std::ptrdiff_t>(k));
                transitions[state][seq[i+k]]++;
            }
        }

        [[nodiscard]] corpus::TokenID sample_next(const State& state) const {
            const auto it = transitions.find(state);
            if (it == transitions.end()) return corpus::Vocabulary::INVALID;
            const auto& counts = it->second;
            uint32_t total = 0;
            for (const auto &c: counts | std::views::values) total += c;
            uint32_t r = rand() % total;
            for (auto& [tok, c]: counts) {
                if (r < c) return tok;
                r-= c;
            }
            return corpus::Vocabulary::INVALID;
        }

        [[nodiscard]] size_t order() const {return k;}

    private:
        size_t k;
        TransitionTable transitions;
    };
}

#endif //MODEL_H
