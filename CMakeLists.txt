cmake_minimum_required(VERSION 3.31)
project(markovian)

set(CMAKE_CXX_STANDARD 20)

add_executable(markovian main.cpp
        corpus/Tokenizer.h
        corpus/Vocabulary.h
        model/StateHash.h
        model/BasicHash.h
        model/Model.h
        corpus/WordTokenizer.h
        example.h
)
