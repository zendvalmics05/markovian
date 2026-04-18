#include <iostream>

#include "corpus/Tokenizer.h"
#include "corpus/Vocabulary.h"
#include "corpus/WordTokenizer.h"
#include "model/Model.h"

#include "example.h"

int main() {
    std::string text = str;
    mkv::corpus::TrainSet<mkv::corpus::WordTokenizer> trainSet(text);
    mkv::corpus::Vocabulary vocab = trainSet.get();
    mkv::model::MarkovModel model(3);
    model.train(trainSet);

    int l = 1000;
    std::string result;

    mkv::model::State state;
    std::string seed[] = {"mr", "collins", "was"};
    result += "mr collins was ";
    
    for (const auto& w : seed) {
        auto id = vocab.getId(w);
        if (id == mkv::corpus::Vocabulary::INVALID) {
            std::cerr << "Seed word not found in vocabulary: " << w << std::endl;
            return 1;
        }
        state.push_back(id);
    }
    
    for (int i = 0; i < l; i++) {
        auto id = model.sample_next(state);
        if (id == mkv::corpus::Vocabulary::INVALID) {
            std::cerr << "\n[Generation stopped: Unseen context]" << std::endl;
            break;
        }
        state.erase(state.begin());
        state.push_back(id);
        result += vocab.getToken(id) + " ";
    }
    std::cout << result << std::endl;


}
