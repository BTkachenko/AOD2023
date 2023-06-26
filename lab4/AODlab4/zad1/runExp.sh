#!/bin/bash
# Kompilacja programu
g++ -o zad1 zad1alt.cpp


for k in {1..16}
do
    echo "Running program with --size $k"
    ./zad1 --size $k
done
