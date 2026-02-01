# Markov Language Models in C++

A modular, experimental implementation of **Markov (n-gram) language models** written in modern C++, designed as a learning laboratory for classical language modeling.

This project explores how **representation choices** (tokenization, context size, smoothing) shape language generation—without using neural networks or large external libraries.

---

## Motivation

Before transformers, language modeling was dominated by **n-gram Markov models**.
They are simple, interpretable, and deeply instructive.

This project was built to answer questions like:

* How does changing the context window (`n`) affect fluency?
* Why does tokenization matter more than the algorithm itself?
* How do probabilistic models fail—and how can we soften those failures?
* What does “generation” really mean in a statistical model?

Rather than chasing state-of-the-art performance, this repository focuses on **clarity, control, and insight**.

---

## What This Project Is (and Is Not)

### ✔ This project **is**

* A clean implementation of n-gram language models
* A tokenizer-agnostic experimentation framework
* A demonstration of classical NLP ideas
* A performance-aware C++ codebase
* A tool for introspecting probabilistic text models

### ✘ This project is **not**

* A neural language model
* A chatbot meant for real-world deployment
* A replacement for modern LLMs
* A library optimized for massive datasets

---

## Core Concepts

### Markov Assumption

The model assumes:

> The probability of the next token depends only on the previous `n` tokens.

Formally:

```
P(t_{n+1} | t_1, …, t_n)
```

This assumption enables:

* tractable training
* exact probability tables
* full transparency

---

### Tokenization as a First-Class Design Choice

The same algorithm behaves *very differently* depending on how text is tokenized.

This project treats tokenization as **pluggable and experimental**, not a fixed preprocessing step.

Implemented (or planned) tokenizers include:

* Character-level (with / without punctuation)
* Word-level (boundary-stripped)
* Word-level with punctuation preserved
* Sentence-aware tokenization

---

## Features

### Language Modeling

* Arbitrary n-gram size (`n ≥ 1`)
* Sliding window training
* Frequency-based probability estimation
* Ranked continuation lists

### Generation

* Probabilistic sampling
* Optional seed text
* Fixed-length output
* Deterministic mode via RNG seeding

### Robustness

* Backoff to lower-order n-grams
* Graceful handling of unseen contexts
* Optional smoothing strategies

### Introspection

* Inspect top-k continuations for any context
* View raw frequency tables
* Analyze entropy per context

### Architecture

* Clean separation of concerns:

  * Tokenizer
  * Vocabulary
  * NGramModel
  * Sampler
  * Interface layer
* Easily extensible and testable

---

## Project Structure

```
src/
├── tokenizers/
│   ├── tokenizer.hpp
│   ├── word_tokenizer.cpp
│   ├── char_tokenizer.cpp
│
├── model/
│   ├── vocabulary.hpp
│   ├── ngram_model.hpp
│   ├── ngram_model.cpp
│
├── generation/
│   ├── sampler.hpp
│   ├── sampler.cpp
│
├── main.cpp
```

Each component is designed to be replaceable without modifying others.

---

## Usage

### Basic Generation

The simplest workflow requires four inputs:

1. Training text
2. Context window size (`n`)
3. Output length
4. Optional seed text

Example (conceptual):

```bash
./markov \
  --file pride_and_prejudice.txt \
  --n 2 \
  --length 50 \
  --seed "my dear"
```

---

### Example Output

Training on *Pride and Prejudice* with `n = 2`:

```
my dear mr bennet replied that he had not been in the house by the end of next week
```

With `n = 4`:

```
my dear mr bennet replied that he had not the pleasure of understanding you
```

Higher `n` increases coherence—but also brittleness.

---

## Why C++?

This project intentionally avoids Python and large NLP libraries.

Reasons:

* Explicit memory and performance control
* Clear data structure choices
* Zero “magic” abstractions
* Better understanding of algorithmic cost
* Strong separation between model and representation

Everything is implemented using the C++ standard library.

---

## Design Decisions (and Tradeoffs)

### Why not neural networks?

Because the goal is **understanding**, not benchmark scores.

Markov models:

* are fully interpretable
* fail in understandable ways
* teach core probabilistic thinking

### Why not regex-heavy tokenizers?

Explicit character handling is:

* faster
* easier to debug
* easier to extend
* less opaque

---

## Known Limitations

* No long-range coherence
* No semantic understanding
* Vocabulary grows unbounded
* Performance degrades for very large `n`
* English-centric tokenization assumptions

These limitations are **intentional** and discussed in the code.

---

## Educational Value

This project connects directly to:

* Classical NLP
* Hidden Markov Models
* Compression and prediction
* Information theory
* Why transformers outperform n-grams

It is designed to be read, modified, and experimented with.

---

## Possible Extensions

* Trie-based n-gram storage
* Kneser–Ney smoothing
* Perplexity evaluation
* Sentence boundary modeling
* POS-tag based tokens
* Compression experiments
* Visualization tools

---

## License

This project is released under the **MIT License**.

Training texts used for examples are sourced from **Project Gutenberg** and are in the public domain.

---
