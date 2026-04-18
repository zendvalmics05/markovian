# Markovian

A modular, experimental implementation of n-gram language models written organically in modern C++20. 

This isn't a chatbot or a neural network. It's a bare-metal exploration into classical probabilistic text generation, built specifically to act as a "learning laboratory." If you've ever wondered how context lengths shift text from absolute gibberish into structured hallucination, or why tokenization controls everything downstream, this codebase makes those mechanics explicitly visible.

## Features

- **Arbitrary n-gram scaling:** Command the math to use any context depth.
- **Dynamic Backoff:** If the engine hits an out-of-vocabulary context, it doesn't crash. It drops the oldest context strings incrementally until it finds a matching transition fallback.
- **Pluggable Tokenization:** Out-of-the-box support for both semantic (word-level) and byte sequence (character-level) training limits to compare how the algorithm behaves when forced to learn spelling natively.
- **Inspect Mode:** Literally look under the hood. Passing a seed with this flag dumps the top branch generation probabilities so you can physically watch the model choose its path.

## Compiling

Everything is handled by native Standard Library headers. No massive dependencies. You just need CMake (3.20+) and a C++20 compatible compiler.

```bash
cmake -S . -B build
cmake --build build
```

## Usage

You can launch the executable bare to trigger the interactive console prompts (super useful for running directly inside CLion), or you can drive the math using explicit script parameters:

```bash
./markovian --file dataset.txt --n 3 --length 100 --seed "the king" --tokenizer word
```

**Parameters:**
* `--file [path]`: (Required) Path to the raw txt corpus to map. 
* `--n [int]`: The memory window (default 3). 
* `--length [int]`: Output volume before disconnecting (default 50).
* `--tokenizer [word|char]`: Strategy flag.
* `--seed [string]`: Seed the context window proactively (optional). 
* `--inspect`: Calculate the exact Top-K probabilities for your seed and exit without running a generation loop. Requires `--seed`.

## Why Build This?

Before moving layers onto giant self-attention transformer matrices, understanding the limits of classical Hidden Markov logic is incredibly useful. 

Why does jumping to `n=5` cause the model to completely stop hallucinating new text and just repeat the exact string it read? Why does `n=8` on the character tokenizer magically recreate accurate punctuation spacing without ever actually being taught English grammar rules? If you can dissect those limits using this architecture, you already understand more about modern LLM failure edges than most people pushing them to production.

---

*Released under the MIT License.*
