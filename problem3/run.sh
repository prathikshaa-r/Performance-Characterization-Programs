#!/bin/bash

iters=1000

echo -e "-----------------------------"
echo -e "Cache Bandwidth Measurement"
echo -e "-----------------------------"
for i in {1..3}
do
#    echo $i;
    ./perf 32 $i $iters;
    echo ./perf 32 $i $iters;
    echo -e "----------------------------"
done

echo -e "Main Memory Bandwidth Measurement"
echo -e "-----------------------------"
for i in {1..3}
do
#    echo $i;
    echo ./perf 4194304 $i $iters;
    #    echo ./perf 2097152 $i;
    ./perf 35840 $i $iters;
    echo -e "-----------------------------"
done;
