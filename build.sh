#!/bin/bash

mkdir -p build
cmake -B build
make -C build

./build/imshow_tui