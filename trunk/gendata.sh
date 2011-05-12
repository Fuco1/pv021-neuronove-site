#!/bin/bash

ITERS="100 200 500"
RATES="0.05"
NEURONS="5 7 8 9 11 13 50"
DATASETS="$(seq 0 5)"

allout="results.txt"
echo -n "" > $allout
for iter in $ITERS
do
    for rate in $RATES
    do
	for neuron in $NEURONS
	do
	    echo -n "Processing net ${neuron}_${iter}_${rate}..." 1>&2
	    net="${neuron}_${iter}_${rate}: "
	    fptotal=0
	    fntotal=0
	    
	    echo -n "$net" >> $allout
	    for dataset in $DATASETS
	    do
		echo -n "." 1>&2
		fp=$(head -n 1 "result_0_${neuron}_${iter}_${rate}_${dataset}.txt")
		fn=$(head -n 2 "result_0_${neuron}_${iter}_${rate}_${dataset}.txt" | tail -n 1)
		#echo "$fp $fn" 1>&2
		echo -n "$fp $fn," >> $allout
		fptotal=$(($fptotal + $fp))
		fntotal=$(($fntotal + $fn))
	    done
	    echo
	    echo "$fptotal $fntotal" >> $allout
	done
    done
done