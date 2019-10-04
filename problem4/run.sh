#!/bin/bash

./mmul

for i in {1..4}
do
    echo ./mmul $i;
    ./mmul $i;
done
