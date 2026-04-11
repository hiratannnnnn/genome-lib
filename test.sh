#!/bin/bash

cd ~/graph-lib
make re
./make_input 5
echo 5 >> result
cat input | xargs ./check | tail -1 >> result
./make_input 50
echo 50 >> result
cat input | xargs ./check | tail -1 >> result
./make_input 500
echo 500 >> result
cat input | xargs ./check | tail -1 >> result
./make_input 5000
echo 5000 >> result
cat input | xargs ./check | tail -1 >> result
./make_input 50000
echo 50000 >> result
cat input | xargs ./check | tail -1 >> result
