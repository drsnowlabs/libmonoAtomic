#!/bin/bash

mkdir -p build
cd build
rm -rf bin/*
CC=clang CXX=clang++ cmake .. && cmake  --build . $1

cd ..

