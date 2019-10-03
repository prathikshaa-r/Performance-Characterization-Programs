#!/bin/bash

echo -e "-----------------------------"
echo -e "Cache Bandwidth Measurement"
echo -e "-----------------------------"
for i in {1..3}
do
#    echo $i;
    ./perf 32 $i;
    echo ./perf 32 $i;
    echo -e "-----------------------------"
done

echo -e "Main Memory Bandwidth Measurement"
echo -e "-----------------------------"
for i in {1..3}
do
#    echo $i;
    echo ./perf 35840 $i;
    ./perf 35840 $i;
    echo -e "-----------------------------"
done;
