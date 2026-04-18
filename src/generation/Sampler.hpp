#ifndef SAMPLER_H
#define SAMPLER_H
#include <random>
#include <vector>
#include <stdexcept>
#include "../model/NGramModel.hpp"
#include "../model/Vocabulary.hpp"

namespace mkv::generation {
    class Sampler {
    public:
        explicit Sampler(const model::MarkovModel& model, unsigned int seed = std::random_device{}()) 
            : model_(model), rng_(seed) {}

        [[nodiscard]] model::TokenID sample_next(const model::State& state) {
            model::State current_context = state;
            if (current_context.size() > model_.order()) {
                current_context.erase(current_context.begin(), current_context.end() - model_.order());
            }

            while (true) {
                const auto* counts = model_.get_transitions(current_context);
                if (counts && !counts->empty()) {
                    uint32_t total = 0;
                    for (const auto& [_, count] : *counts) {
                        total += count;
                    }

                    std::uniform_int_distribution<uint32_t> dist(0, total - 1);
                    uint32_t r = dist(rng_);

                    for (const auto& [tok, count] : *counts) {
                        if (r < count) return tok;
                        r -= count;
                    }
                }
                if (current_context.empty()) break;
                current_context.erase(current_context.begin()); // Step back! (Backoff)
            }
            return model::Vocabulary::INVALID;
        }

        [[nodiscard]] std::vector<std::pair<model::TokenID, double>> get_top_continuations(const model::State& state, size_t top_k = 5) const {
            model::State current_context = state;
            if (current_context.size() > model_.order()) {
                current_context.erase(current_context.begin(), current_context.end() - model_.order());
            }

            const model::NextTokenCounts* counts = nullptr;
            
            while (true) {
                counts = model_.get_transitions(current_context);
                if (counts && !counts->empty()) break;
                if (current_context.empty()) break;
                current_context.erase(current_context.begin());
            }
            
            if (!counts || counts->empty()) return {};

            uint32_t total = 0;
            for (const auto& [_, count] : *counts) total += count;

            std::vector<std::pair<model::TokenID, double>> probs;
            for (const auto& [tok, count] : *counts) {
                probs.emplace_back(tok, static_cast<double>(count) / total);
            }

            std::sort(probs.begin(), probs.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

            if (probs.size() > top_k) {
                probs.resize(top_k);
            }
            return probs;
        }

    private:
        const model::MarkovModel& model_;
        std::mt19937 rng_;
    };
}
#endif //SAMPLER_H
