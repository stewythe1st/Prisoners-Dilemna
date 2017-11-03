#!/bin/bash

# Compile project
g++ ./src/*.cpp -std=c++11 -I ./inc/ -o prisoners_dilemna.out

# Run with given configuration
./prisoners_dilemna.out "$@"
