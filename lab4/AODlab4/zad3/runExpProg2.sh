#!/bin/bash

# Kompilacja programu
g++ -o program2 program2.cpp
./program2 --size 6 --degree 6 --printMatching --glpk model2.jl
julia model2.jl
