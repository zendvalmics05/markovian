#ifndef NGRAM_MODEL_H
#define NGRAM_MODEL_H
#include <ranges>
#include <unordered_map>
#include "BasicHash.hpp"

namespace mkv::model {
    using NextTokenCounts = std::unordered_map<TokenID, uint32_t>;
    using TransitionTable = std::unordered_map<State, NextTokenCounts, BasicHash>;

    class MarkovModel {
    public:
        explicit MarkovModel(const size_t order) : k(order) {}

        void train(std::vector<TokenID> seq) {
            if (seq.size() <= k) return;
            seq.insert(seq.begin(), k, Vocabulary::STARTID);
            seq.push_back(Vocabulary::ENDID);

            for (size_t i = 0; i + k < seq.size(); i++) {
                State state(seq.begin() + i, seq.begin() + i + k);
                transitions[state][seq[i+k]]++;
            }
        }

        [[nodiscard]] size_t order() const {return k;}
        
        [[nodiscard]] const NextTokenCounts* get_transitions(const State& state) const {
            auto it = transitions.find(state);
            if (it == transitions.end()) return nullptr;
            return &(it->second);
        }

    private:
        size_t k;
        TransitionTable transitions;
    };
}
#endif //NGRAM_MODEL_H
