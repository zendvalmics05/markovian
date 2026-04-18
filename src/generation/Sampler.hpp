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
            const auto* counts = model_.get_transitions(state);
            if (!counts) return model::Vocabulary::INVALID;

            uint32_t total = 0;
            for (const auto& [_, count] : *counts) {
                total += count;
            }

            if (total == 0) return model::Vocabulary::INVALID;

            std::uniform_int_distribution<uint32_t> dist(0, total - 1);
            uint32_t r = dist(rng_);

            for (const auto& [tok, count] : *counts) {
                if (r < count) return tok;
                r -= count;
            }
            return model::Vocabulary::INVALID;
        }

    private:
        const model::MarkovModel& model_;
        std::mt19937 rng_;
    };
}
#endif //SAMPLER_H
