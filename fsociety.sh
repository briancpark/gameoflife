#! bin/bash

for n in {1..100}
do
    diff "testOutputs/GliderGuns/GliderGuns_0x1808_10`printf %03d $n`.ppm" "studentOutputs/GliderGuns/GliderGuns_0x1808_10`printf %03d $n`.ppm"
done

for file in testInputs/*; do
for n in {0..10000}
do
    oracleOfLife $file 0x`printf '%x' $n` > testo
    ./gameOfLife $file 0x`printf '%x' $n` > student
    diff testo student
done
done

