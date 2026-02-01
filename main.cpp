#include <iostream>

#include "corpus/Tokenizer.h"
#include "corpus/Vocabulary.h"
#include "corpus/WordTokenizer.h"
#include "model/Model.h"

#include "example.h";

int main() {
    std::string text = str;
    mkv::corpus::WordTokenizer tokenizer;
    mkv::corpus::Vocabulary vocab;
    auto tokens = tokenizer.tokenize(text);
    std::vector<mkv::corpus::TokenID> ids;
    for (auto& token : tokens) {
        ids.push_back(vocab.getOrAddID(token));
    }
    mkv::model::MarkovModel model(3);
    model.train(ids);

    int l = 1000;
    std::string result;

    mkv::model::State state;
    state.push_back(vocab.getId("mr"));
    state.push_back(vocab.getId("collins"));
    state.push_back(vocab.getId("was"));
    result += "mr collins was ";
    for (int i = 0; i < l; i++) {
        auto id = model.sample_next(state);
        state.erase(state.begin());
        state.push_back(id);
        result += vocab.getToken(id) + " ";
    }
    std::cout << result << std::endl;


}
