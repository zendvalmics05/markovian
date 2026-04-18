#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <string>
#include <vector>

namespace mkv::tokenizers {
    using Token = std::string;

    class Tokenizer {
    public:
        virtual ~Tokenizer() = default;
        [[nodiscard]] virtual std::vector<Token> tokenize(const std::string& text) const = 0;
    };
}
#endif //TOKENIZER_H
