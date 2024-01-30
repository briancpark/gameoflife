#!/bin/bash

# if on macOS, we need `convert` command which is installed with `imagemagick`
./frames.csh GliderGuns 0x1808 100

for n in {1..100}
    do
        diff "testOutputs/GliderGuns/GliderGuns_0x1808_10`printf %03d $n`.ppm" "studentOutputs/GliderGuns/GliderGuns_0x1808_10`printf %03d $n`.ppm"
    done

for file in testInputs/*
    do
    for n in {0..100}
        do
            echo "Testing $file with rule 0x`printf '%x' $n`"
            ./gameOfLife $file 0x`printf '%x' $n` > test.out
            ./gameOfLife $file 0x`printf '%x' $n` > student.out
            diff test.out student.out
            rm test.out student.out
        done
    done
