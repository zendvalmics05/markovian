#ifndef NGRAM_MODEL_H
#define NGRAM_MODEL_H
#include <ranges>
#include <unordered_map>
#include <random>
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
                for (size_t j = 0; j <= k; ++j) {
                    State context(seq.begin() + i + k - j, seq.begin() + i + k);
                    transitions[context][seq[i+k]]++;
                }
            }
        }

        [[nodiscard]] size_t order() const {return k;}
        
        [[nodiscard]] const NextTokenCounts* get_transitions(const State& state) const {
            auto it = transitions.find(state);
            if (it == transitions.end()) return nullptr;
            return &(it->second);
        }

        [[nodiscard]] State find_state_by_suffix(const std::vector<TokenID>& suffix, unsigned int seed = std::random_device{}()) const {
            if (suffix.size() >= k) {
                return State(suffix.end() - k, suffix.end());
            }
            std::vector<State> matching;
            for (const auto& kv : transitions) {
                const State& s = kv.first;
                
                // Safety: Only search for full-length deterministic context matches to prevent out-of-bounds access
                // on smaller fallback layers populated by the training step.
                if (s.size() != k) continue;

                bool match = true;
                for (size_t i = 0; i < suffix.size(); ++i) {
                    if (s[k - suffix.size() + i] != suffix[i]) {
                        match = false;
                        break;
                    }
                }
                if (match) matching.push_back(s);
            }
            if (matching.empty()) return State(); 
            std::mt19937 rng(seed);
            std::uniform_int_distribution<size_t> dist(0, matching.size() - 1);
            return matching[dist(rng)];
        }

    private:
        size_t k;
        TransitionTable transitions;
    };
}
#endif //NGRAM_MODEL_H
