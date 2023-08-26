#!/bin/bash

# Name of the C source file
source_file="sim.cpp"

# Name of the output executable
output_file="sim"

# Compile the C code
clang -o $output_file $source_file -lm
