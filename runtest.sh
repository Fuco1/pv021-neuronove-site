#!/bin/bash

[[ $# -ne 5 ]] && echo "Usage: ./runtest.sh iters rates neurons datasets trainset" && exit

if [ $1 == "--help" ]
then
    echo "iters: list of iterations, example \"100 200 300\""
    echo "rates: list of learning rates"
    echo "neurons: list of neuron count in 1st layer"
    echo "datasets: list of noise levels, example \"1 2 3\""
    echo "trainset: training set of data, expecting noise level"
    exit
fi

# iters 100 200 500 1000
# rate 0.2 0.1 0.05 0.01
# neurons 5 7 8 9 11 13 50
# data sets $(seq 0 9)

ITERS=$1
RATES=$2
NEURONS=$3
DATASETS=$4
TRAINSET=$5

[[ ! -e "lsession" ]] && echo "0" > lsession

SID=$(($(cat lsession) + 1))
echo $SID > lsession

for iter in $ITERS
do
    for rate in $RATES
    do
	for neuron in $NEURONS
	do
	    # train the network on data0 here
	    echo "Training network with params ./net -c 2000:id,${neuron}:tahn,1:tanh -i $iter -x $rate -s net_0_${neuron}_${iter}_${rate}.net -t data${TRAINSET}..." 1>&2
	    ./net -c "2000:id,${neuron}:tanh,1:tanh" -i $iter -x $rate -s "net_0_${neuron}_${iter}_${rate}.net" -t "data${TRAINSET}"
	    lastnet="net_0_${neuron}_${iter}_${rate}.net"
	    for dataset in $DATASETS
	    do
		# test the network on $DATASETS here
		echo "Running test on $lastnet with dataset data${dataset}..." 1>&2
		./net -l $lastnet -r "data${dataset}/test/pos" 2>/dev/null > "tmp_pos.$SID"
		./net -l $lastnet -r "data${dataset}/test/neg" 2>/dev/null > "tmp_neg.$SID"
		res="result_0_${neuron}_${iter}_${rate}_${dataset}.txt"
		echo $(cat "tmp_pos.$SID" | grep - | wc | awk '{ print $1 }') > $res
		echo $(cat "tmp_neg.$SID" | grep - | wc | awk '{ print 30 - $1 }') >> $res
		cat "tmp_pos.$SID" "tmp_neg.$SID" >> $res
	    done
	done
    done
done

rm "tmp_neg.$SID"
rm "tmp_pos.$SID"