#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include "net.h"

#define SQR(x) ((x)*(x))

using namespace std;

// one instance of Funtranslator to be shared among all cpp files
FunTranslator funTranslator;

// Activation function "1.7159*tanh(2.0/3.0*x)" and its derivative.
double actFuncTanh(double potential, bool diff) {
	const double a = 1.7159;
	const double b = 2.0/3.0;

	if (diff) {
		return -a * b * (SQR(tanh(b * potential)) - 1);
	} else {
		return a * tanh(b * potential);
	}
}

// identity for input neurons to use
double actFuncId(double potential, bool diff) {
	if (diff) {
		return 1;
	} else {
		return potential;
	}
}

/*
 * Save the net to a file.
 * Params:
 * 		filename - name of the file to load a network from
 */
void Net::saveToFile(const char* fileName) {
	ofstream file(fileName);

	file << this->layers.size() << endl;
	for (size_t x = 0; x < this->layers.size(); ++x) {
		file << this->layers[x].size() << " ";
		// take an activation function of a layer representant 
		actFuncPtr actFunc = this->layers[x][0].getActFunc();
		// store its name to file
		file << funTranslator.fun2name[actFunc] << endl;
	}

	for (size_t x = 0; x < this->layers.size(); ++x) {
		for (size_t y = 0; y < this->layers[x].size(); ++y) {
			Neuron &neuron = this->layers[x][y];
			for (size_t z = 0; z < neuron.getInputWeightsSize(); ++z) {
				file << neuron.getInputWeight(z) << " ";
			}
			file << endl;
		}
	}
}

/*
 * Load the net from a file. Lacks checks for the file format.
 * Params:
 * 		filename - name of the file to load a network from
 * Returns: false if the file does not exist
 */
bool Net::loadFromFile(const char* fileName) {
	vector<LayerSpec> netSpec;
	ifstream file(fileName);
	size_t layerCnt;

	if (file.fail()) return false;
	file >> layerCnt;
	for (size_t x = 0; x < layerCnt; ++x) {
		string actFuncName;
		LayerSpec layerSpec;
		file >> layerSpec.neuronCnt;
		file >> actFuncName;
		layerSpec.actFunc = funTranslator.name2fun[actFuncName];
		netSpec.push_back(layerSpec);
	}

	this->init(netSpec);
	for (size_t x = 0; x < this->layers.size(); ++x) {
		for (size_t y = 0; y < this->layers[x].size(); ++y) {
			Neuron &neuron = this->layers[x][y];
			for (size_t z = 0; z < neuron.getInputWeightsSize(); ++z) {
				double weight;
				file >> weight;
				neuron.setInputWeight(z, weight);
			}
		}
	}
	return true;
}

std::ostream& operator <<(std::ostream &os, const Net &net) {
	for (size_t layerIndex = 0; layerIndex < net.layers.size(); ++layerIndex) {
		for (size_t neuronIndex = 0; neuronIndex < net.layers[layerIndex].size(); ++neuronIndex) {
			os << "layer " << layerIndex << ", neuron " << neuronIndex << " (bias and " << net.layers[layerIndex][neuronIndex].getInputWeightsSize() - 1 << " inputs): ";
			for (size_t inputWeightIndex = 0; inputWeightIndex < net.layers[layerIndex][neuronIndex].getInputWeightsSize(); ++inputWeightIndex) {
				os << net.layers[layerIndex][neuronIndex].getInputWeight(inputWeightIndex) << " ";
			}
			os << std::endl;
		}
		os << "-----" << std::endl;
	}
	return os;
}
