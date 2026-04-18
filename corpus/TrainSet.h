//
// Created by sagnik on 08-02-2026.
//

#ifndef TRAINSET_H
#define TRAINSET_H
#include <vector>

#include "Vocabulary.h"

namespace mkv::corpus {
    template<typename T>
    struct TrainSet {
    private:
        std::vector<TokenID> trainSet;
        Vocabulary vocab;
    public:
        explicit TrainSet(const std::string& text) {
            T tokenizer;
            auto tokens = tokenizer.tokenize(text);
            for (auto& token : tokens) {
                trainSet.push_back(vocab.getOrAddID(token));
            }
        }
        Vocabulary get() const {
            return vocab;
        }
        [[nodiscard]] std::vector<TokenID> access() const {
            return trainSet;
        }
    };
}

#endif //TRAINSET_H
