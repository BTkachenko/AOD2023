#!/bin/bash
# Kompilacja programu
g++ -o zad4 zad4.cpp


for k in {1..16}
do
    echo "Running program with --size $k"
    ./zad4 --size $k
done