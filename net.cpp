#include "cmath"
#include "net.h"
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

double actFuncTanh(double potential) {
	const double a = 1.7159;
	const double b = 2.0/3.0;

	return a * tanh(b * potential);
}

/* 
 * Save the net to a file. 
 * Params: 
 * 		filename - name of the file to load a network from
 */
void Net::saveToFile(const char* fileName) {
	ofstream file(fileName);
	map<double (*)(double), string> fun2name;
	fun2name[actFuncTanh] = "tanh";

	file << this->layers.size() << endl;
	for (size_t x = 0; x < this->layers.size(); ++x) {
		file << this->layers[x].size() << endl;
	}

	for (size_t x = 0; x < this->layers.size(); ++x) {
		for (size_t y = 0; y < this->layers[x].size(); ++y) {
			Neuron &neuron = this->layers[x][y];
			file << fun2name[neuron.getActFunc()] << " ";
			for (size_t z = 0; z < neuron.getInputWeightsSize(); ++z) {
				file << neuron.getInputWeight(z) << " ";
			}
			file << endl;
		}
		file << "-----" << endl;
	}
}

/* 
 * Load the net from a file. Lacks checks for the file format.
 * Params: 
 * 		filename - name of the file to load a network from
 * Returns: false if the file does not exist
 */
bool Net::loadFromFile(const char* fileName) {
	map<string, double (*)(double)> name2fun;
	name2fun["tanh"] = &actFuncTanh;
	vector<size_t> neuronCounts;
	ifstream file(fileName);
	size_t ilayers;
	
	if (file.fail()) return false;
	file >> ilayers;
	for (size_t x = 0; x < ilayers; ++x) {
		size_t ineurons;
		file >> ineurons;
		neuronCounts.push_back(ineurons);
	}

	this->init(neuronCounts);
	for (size_t x = 0; x < this->layers.size(); ++x) {
		for (size_t y = 0; y < this->layers[x].size(); ++y) {
			Neuron &neuron = this->layers[x][y];
			string actFuncName;
			file >> actFuncName;
			neuron.setActFunc(name2fun[actFuncName]);
			for (size_t z = 0; z < neuron.getInputWeightsSize(); ++z) {
				double weight;
				file >> weight;
				neuron.setInputWeight(z, weight);
			}
		}
		string delimiter;
		file >> delimiter;	
	}
	return true;
}
