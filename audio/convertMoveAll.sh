#!/bin/bash
for filename in *.wav; do
    # xxd -i $filename ${filename%%.*}.c
    python convert.py $filename 9 2000
    name=${filename%%.*}
    mkdir -p $name
    mv $name.c $name/
    mv $name.h $name/
    rm -rf ../lib/$name
    mv $name ../lib
done

