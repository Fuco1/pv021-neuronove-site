#ifndef NEURON_H_INCLUDED
#define NEURON_H_INCLUDED

#include <vector>

typedef double(*actFuncPtr)(double);

class Net;

class Neuron {
	private:
		Net *thisNeuronsNetwork; // Pointer to the network owning this neuron.
		size_t indexOfThisNeuronsLayer; // 0 for neurons in the input (0th) layer, 1 for neurons in the 1st layer, ...
		size_t indexOfThisNeuronInItsLayer; // 0 for the 1st neuron of a layer, 1 for the 2nd neuron, ...

		std::vector<double> inputWeights; // Bias and the weights of individual inputs of this neuron. The number of inputs of each neuron is equal to the number of neurons in the layer below it.

		double potential; // Potential, accumulated by propagation of values from lower neurons. In the case of the input (0th) layer, this directly stores the output value of the neuron (no activation function will be applied before the value is propagated to the 1st layer).
		// \todo double delta; // rozdil vysledku a ocekavaneho/pozadovaneho vysledku pro tento neuron
		double (*actFunc)(double); // Pointer to the activation function of this neuron.
	public:
		// Constructor
		Neuron(Net *aThisNeuronsNetwork,
		       int aIndexOfThisNeuronsLayer,
		       int aIndexOfThisNeuronInItsLayer,
		       int numberOfInputNeurons,
		       double (*aActFunc)(double)) {
			thisNeuronsNetwork = aThisNeuronsNetwork;
			indexOfThisNeuronsLayer = aIndexOfThisNeuronsLayer;
			indexOfThisNeuronInItsLayer = aIndexOfThisNeuronInItsLayer;
			actFunc = aActFunc;

			inputWeights.resize(numberOfInputNeurons + 1); // Bias + one weight for each input neuron.
			for (size_t i = 0; i < inputWeights.size(); ++i) {
				inputWeights[i] = 0.1; // \todo Initialize the bias and the input weights to random numbers from the appropriate range.
			}
			//std::cout << "creating layer " << aIndexOfThisNeuronsLayer << ", neuron " << aIndexOfThisNeuronInItsLayer << " (" << numberOfInputNeurons << " inputs)" << std::endl;
		};

		// Sets the potential of this neuron.
		void setPotential(double aPotential) {
			potential = aPotential;
		};

		// Returns the potential of this neuron.
		double getPotential(void) {
			return potential;
		}

		// Returns the output value of this neuron.
		// If this is an input neuron (in the 0th layer), simply return the value stored in its potential (without applying the activation function).
		// If this is a regular neuron, calculate its output value of from its potential, using the activation function.
		double getValue(void) {
			if (indexOfThisNeuronsLayer == 0) {
				return potential;
			} else {
				if (actFunc == NULL) {
					std::cerr << "error: Activation function not specified" << std::endl;
					exit(2);
				}
				return actFunc(potential);
			}
		}

		// Returns the input weight with the specified index (when indexOfInputWeight == 0, the function returns the bias of this neuron).
		double getInputWeight(size_t indexOfInputWeight) {
			return inputWeights[indexOfInputWeight];
		}
		
		size_t getInputWeightsSize() {
			return inputWeights.size();
		}

		// Sets the input weight with the specified index (when indexOfInputWeight == 0, the function sets the bias of this neuron).
		void setInputWeight(size_t indexOfInputWeight, double newWeight) {
			inputWeights[indexOfInputWeight] = newWeight;
		}

		// Sets the activation function.
		void setActFunc(double (*newActFunc)(double)) {
			actFunc = newActFunc;
		}

		// get pointer to an activation function
		actFuncPtr getActFunc() {
			return actFunc;
		}

		void propagateValueToUpperLayer(void);
};


#endif // NEURON_H_INCLUDED
