#!/bin/bash

OUTFILE="prisoners_dilenma.out"

if [ -e $OUTFILE  ]
then
	rm $OUTFILE
fi

# Compile project
g++ ./src/*.cpp -std=c++11 -I ./inc/ -o $OUTFILE

# Run with given configuration
./$OUTFILE "$@"
