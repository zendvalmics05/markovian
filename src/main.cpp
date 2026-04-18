#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "tokenizers/WordTokenizer.hpp"
#include "tokenizers/CharTokenizer.hpp"
#include "model/Vocabulary.hpp"
#include "model/NGramModel.hpp"
#include "generation/Sampler.hpp"

using namespace mkv;

void print_usage() {
    std::cerr << "Usage: markovian --file <path> [--n <int>] [--length <int>] [--seed <string>] [--tokenizer <word|char>]\n";
}

int main(int argc, char* argv[]) {
    std::string file_path;
    int n = 3;
    int length = 50;
    std::string seed_str;
    std::string tokenizer_type = "word";

    if (argc == 1) {
        std::cout << "--- Interactive Mode (No CLI arguments detected) ---\n";
        std::cout << "Enter path to text file: ";
        std::getline(std::cin, file_path);
        
        std::string input;
        std::cout << "Enter n-gram size [default 3]: ";
        std::getline(std::cin, input);
        if (!input.empty()) n = std::stoi(input);

        std::cout << "Enter generation length [default 50]: ";
        std::getline(std::cin, input);
        if (!input.empty()) length = std::stoi(input);

        std::cout << "Enter seed text [optional]: ";
        std::getline(std::cin, seed_str);

        std::cout << "Enter tokenizer (word | char) [default word]: ";
        std::getline(std::cin, input);
        if (!input.empty()) tokenizer_type = input;
    } else {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--file" && i + 1 < argc) file_path = argv[++i];
            else if (arg == "--n" && i + 1 < argc) n = std::stoi(argv[++i]);
            else if (arg == "--length" && i + 1 < argc) length = std::stoi(argv[++i]);
            else if (arg == "--seed" && i + 1 < argc) seed_str = argv[++i];
            else if (arg == "--tokenizer" && i + 1 < argc) tokenizer_type = argv[++i];
            else {
                print_usage();
                return 1;
            }
        }
    }

    if (file_path.empty()) {
        std::cerr << "Error: --file is required.\n";
        print_usage();
        return 1;
    }

    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        std::cerr << "Error: Could not open file " << file_path << "\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string text = buffer.str();

    std::unique_ptr<tokenizers::Tokenizer> tokenizer;
    if (tokenizer_type == "char") {
        tokenizer = std::make_unique<tokenizers::CharTokenizer>();
    } else {
        tokenizer = std::make_unique<tokenizers::WordTokenizer>();
    }

    model::Vocabulary vocab;
    std::vector<model::TokenID> train_data;
    
    auto tokens = tokenizer->tokenize(text);
    for (const auto& t : tokens) {
        train_data.push_back(vocab.getOrAddID(t));
    }

    model::MarkovModel model(n);
    model.train(train_data);

    generation::Sampler sampler(model);

    model::State state;
    std::string generated_text;

    if (!seed_str.empty()) {
        auto seed_tokens = tokenizer->tokenize(seed_str);
        if (seed_tokens.size() > n) {
            seed_tokens.erase(seed_tokens.begin(), seed_tokens.end() - n);
        }
        for (const auto& w : seed_tokens) {
            auto id = vocab.getId(w);
            if (id == model::Vocabulary::INVALID) {
                std::cerr << "[Warning] Seed token not in vocabulary: " << w << "\n";
            }
            state.push_back(id);
            generated_text += w + (tokenizer_type == "word" ? " " : "");
        }
        while (state.size() < n) {
            state.insert(state.begin(), model::Vocabulary::STARTID);
        }
    } else {
        state = model::State(n, model::Vocabulary::STARTID);
    }

    for (int i = 0; i < length; ++i) {
        auto id = sampler.sample_next(state);
        if (id == model::Vocabulary::INVALID || id == model::Vocabulary::ENDID) {
            std::cout << "\n[Generation stopped]\n";
            break;
        }
        state.erase(state.begin());
        state.push_back(id);
        
        auto tok = vocab.getToken(id);
        generated_text += tok + (tokenizer_type == "word" ? " " : "");
    }

    std::cout << generated_text << std::endl;
    return 0;
}
