#include <cstdlib>

#include "net.h"
#include "neuron.h"

void Neuron::propagateValueToUpperLayer(void) {
	// We can't propagate above the topmost layer.
	if (indexOfThisNeuronsLayer >= thisNeuronsNetwork->layers.size()) {
		return;
	}

	// Get the output value of this neuron.
	double value = getValue();

	// For all neurons in the layer above us...
	for (size_t i = 0; i < thisNeuronsNetwork->layers[indexOfThisNeuronsLayer + 1].size(); ++i) {
		// ...pick the neuron...
		Neuron *targetNeuron = &(thisNeuronsNetwork->layers[indexOfThisNeuronsLayer + 1][i]);
		// ...and increase its potential by our output value, with the weight the target neuron has for us.
		targetNeuron->setPotential(targetNeuron->getPotential() + value * targetNeuron->inputWeights[indexOfThisNeuronInItsLayer + 1]);
	}

}
