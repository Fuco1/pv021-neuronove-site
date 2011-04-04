#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include <iostream>
#include <vector>
#include <cstdlib>

#include "neuron.h"
#include "image.h"

typedef std::vector<Neuron> Layer;

double actFuncTanh(double potential);

class Net {
	public:
		std::vector<Layer> layers; // All neurons of this net.
	public:
		// \todo Constructor
		Net(const std::vector<size_t> &neuronCounts) {
			Layer currentLayer;
			for (size_t i = 0; i < neuronCounts.size(); ++i) {
				currentLayer.clear();
				for (size_t j = 0; j < neuronCounts[i]; ++j) {
					Neuron currentNeuron(this, i, j, (i == 0) ? (0) : (neuronCounts[i - 1]), &actFuncTanh);
					currentLayer.push_back(currentNeuron);
				}
				layers.push_back(currentLayer);
			}
		};



		double run(const Image<double> &image) {
			if (image.getSize() != layers[0].size()) {
				std::cerr << "error: Wrong image size" << std::endl;
				exit(1);
			}

			// Initialize all potentials (either to the neuron's bias, or -- in the case of input neurons -- to the corresponding input value).
      for (size_t layerIndex = 0; layerIndex < layers.size(); ++layerIndex) {
				for (size_t neuronIndex = 0; neuronIndex < layers[layerIndex].size(); ++neuronIndex) {
					if (layerIndex == 0) {
						layers[layerIndex][neuronIndex].setPotential(image.getVoxel(neuronIndex));
					} else {
						layers[layerIndex][neuronIndex].setPotential(layers[layerIndex][neuronIndex].getInputWeight(0));
					}
				}
      }

      // Propagate the values from layer 0 upwards.
      for (size_t layerIndex = 0; layerIndex < layers.size() - 1; ++layerIndex) {
				for (size_t neuronIndex = 0; neuronIndex < layers[layerIndex].size(); ++neuronIndex) {
					layers[layerIndex][neuronIndex].propagateValueToUpperLayer();
				}
      }

      // All potentials have been set; return the value of the output neuron.
      double returnValue = layers[layers.size() - 1][0].getValue();
      std::cout << "Run finished with return value = " << returnValue << std::endl;
      return returnValue;
		};


		void train(const Image<double> &img, double expectedValue); // will call run() as one (probably the first) step

		// \todo Save the net to a file.
		void saveToFile(const std::string &fileName) {
		};

		// \todo Load the net from a file.
		void loadFromFile(const std::string &fileName) {
		};

};



#endif // NET_H_INCLUDED
