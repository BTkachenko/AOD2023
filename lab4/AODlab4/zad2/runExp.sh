#!/bin/bash

# Kompilacja programu
g++ -o program main.cpp

# Uruchomienie programu dla różnych wartości k i i
for k in {3..10}
do
    for i in $(seq 1 $k)
    do
        echo "Running program with --size $k and --degree $i"
        ./program --size $k --degree $i
    done
done


