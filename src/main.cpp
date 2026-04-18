#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

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
    bool inspect_mode = false;

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

        std::cout << "Enable inspect mode? (y/N): ";
        std::getline(std::cin, input);
        if (!input.empty() && (input == "y" || input == "Y" || input == "yes")) {
            inspect_mode = true;
        }
    } else {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--file" && i + 1 < argc) file_path = argv[++i];
            else if (arg == "--n" && i + 1 < argc) n = std::stoi(argv[++i]);
            else if (arg == "--length" && i + 1 < argc) length = std::stoi(argv[++i]);
            else if (arg == "--seed" && i + 1 < argc) seed_str = argv[++i];
            else if (arg == "--tokenizer" && i + 1 < argc) tokenizer_type = argv[++i];
            else if (arg == "--inspect") inspect_mode = true;
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
        std::vector<model::TokenID> seed_ids;
        for (const auto& w : seed_tokens) {
            auto id = vocab.getId(w);
            if (id == model::Vocabulary::INVALID) {
                std::cerr << "[Warning] Seed token not in vocabulary: " << w << "\n";
            } else {
                seed_ids.push_back(id);
                generated_text += w + (tokenizer_type == "word" ? " " : "");
            }
        }
        
        state = model.find_state_by_suffix(seed_ids);
        if (state.empty()) {
             std::cerr << "\n[Error] The provided seed sequence was never observed in the text corpus.\n";
             return 1;
        }
    } else {
        state = model::State(n, model::Vocabulary::STARTID);
    }

    if (inspect_mode) {
        if (seed_str.empty()) {
            std::cerr << "Error: Inspect mode requires a --seed starting context.\n";
            return 1;
        }
        std::cout << "\n[Inspect Mode]\nContext: \"" << seed_str << "\"\n";
        std::cout << "Top continuations:\n";
        
        auto top_k = sampler.get_top_continuations(state, 5);
        if (top_k.empty()) {
            std::cout << "- [No known continuations / Context completely unseen]\n";
        } else {
            for (const auto& [id, prob] : top_k) {
                std::cout << "- " << vocab.getToken(id) << " (" << std::fixed << std::setprecision(4) << prob << ")\n";
            }
        }
        return 0; // Terminate!
    }

    // Classic Generation Mode
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
