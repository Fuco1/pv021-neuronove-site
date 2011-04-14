#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>

#include "neuron.h"
#include "image.h"

// layer specification
struct LayerSpec {
	size_t neuronCnt;
	actFuncPtr actFunc; 

	LayerSpec() {}
	LayerSpec(size_t neuronCnt, actFuncPtr actFunc) {
		this->neuronCnt = neuronCnt;
		this->actFunc = actFunc;
	}
};

// activation function list
double actFuncTanh(double potential, bool diff);
double actFuncId(double potential, bool diff);

// function_ptr -> function_name, and vice versa
struct FunTranslator {
	std::map<actFuncPtr, std::string> fun2name;
	std::map<std::string, actFuncPtr> name2fun;
	
	FunTranslator() {
		fun2name[actFuncId] = "id";
		name2fun["id"] = actFuncId;
		fun2name[actFuncTanh] = "tanh";
		name2fun["tanh"] = actFuncTanh;
	}
}; 

extern FunTranslator funTranslator;

// layer corresponding to its specification
typedef std::vector<Neuron> Layer;

const double learningSpeed = 0.05;

class Net {
	public:
		std::vector<Layer> layers; // All neurons of this net.
	public:
		// shared code for constructors
		void init(const std::vector<LayerSpec> &netSpec) {
			Layer currentLayer;
			for (size_t i = 0; i < netSpec.size(); ++i) {
				currentLayer.clear();
				for (size_t j = 0; j < netSpec[i].neuronCnt; ++j) {
					int parentCnt = (i == 0) ? (0) : (netSpec[i- 1].neuronCnt);
					Neuron currentNeuron(this, i, j, parentCnt, netSpec[i].actFunc);
					currentLayer.push_back(currentNeuron);
				}
				layers.push_back(currentLayer);
			}
		}

		Net() {} // nonparametric constructor

		Net(const std::vector<LayerSpec> &netSpec) {
			init(netSpec);
		}

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
			//std::cout << "Run finished with return value = " << returnValue << std::endl;
			return returnValue;
		}

		// will call run() as one (probably the first) step
		void train(const Image<double> &image, double expectedValue) {
			//std::cout << std::endl << "Training on value " << image.getVoxel(0) << ", expecting " << expectedValue << std::endl;

			// Forward propagation.
			run(image);

			// \todo Calculate the delta value for the output neuron.
			layers[layers.size() - 1][0].setDelta(expectedValue - layers[layers.size() - 1][0].getValue());

			// \todo Backpropagation.
			for (int layerIndex = layers.size() - 2; layerIndex >= 0; --layerIndex) {
				for (size_t neuronIndex = 0; neuronIndex < layers[layerIndex].size(); ++neuronIndex) {
// \todo Ktera z nasledujicich dvou variant je spravne (pokud vubec)? Tu derivaci aktivacni funkce (actFuncDiff) mame
//       vyhodnocovat jednotlive, na neuronech, do nichz z tohoto neuronu vede spoj (varianta #if PODLE_PREDNASKY),
//       nebo na tomto neuronu (varianta #else)?
//
//       Prvni moznost je podle slidu k predmetu (prednaska 4, slide 16),
//       druha moznost podle http://www.cedar.buffalo.edu/~srihari/CSE574/Chap5/Chap5.3-BackProp.pdf (slide 11)
//       a http://www.willamette.edu/~gorr/classes/cs449/backprop.html
//
//       (Pokud bude nalezena odpoved, tenhle komentar prosim nemazat, ale doplnit o strucne zduvodneni/vysvetleni.)
#undef PODLE_PREDNASKY
#if PODLE_PREDNASKY
					double sum = 0;
					for (size_t neuronInUpperLayerIndex = 0; neuronInUpperLayerIndex < layers[layerIndex+1].size(); ++neuronInUpperLayerIndex) {
						sum +=   layers[layerIndex+1][neuronInUpperLayerIndex].getDelta() * layers[layerIndex+1][neuronInUpperLayerIndex].getInputWeight(neuronIndex)
						       * layers[layerIndex+1][neuronInUpperLayerIndex].getActFunc()(layers[layerIndex+1][neuronInUpperLayerIndex].getPotential(), true);
					}
					layers[layerIndex][neuronIndex].setDelta(sum);
#else
					double sum = 0;
					for (size_t neuronInUpperLayerIndex = 0; neuronInUpperLayerIndex < layers[layerIndex+1].size(); ++neuronInUpperLayerIndex) {
						sum += layers[layerIndex+1][neuronInUpperLayerIndex].getDelta() * layers[layerIndex+1][neuronInUpperLayerIndex].getInputWeight(neuronIndex);
					}
					layers[layerIndex][neuronIndex].setDelta(sum * layers[layerIndex][neuronIndex].getActFunc()(layers[layerIndex][neuronIndex].getPotential(), true));
#endif
				}
			}

			// \todo Update the weights according to the delta values.
			for (size_t layerIndex = 1; layerIndex < layers.size(); ++layerIndex) {
				for (size_t neuronIndex = 0; neuronIndex < layers[layerIndex].size(); ++neuronIndex) {
					for (size_t inputWeightIndex = 0; inputWeightIndex < layers[layerIndex][neuronIndex].getInputWeightsSize(); ++inputWeightIndex) {
						double newWeight = layers[layerIndex][neuronIndex].getInputWeight(inputWeightIndex);
						if (inputWeightIndex == 0) {
							newWeight -= -learningSpeed * layers[layerIndex][neuronIndex].getDelta() * 1;
						} else {
							newWeight -= -learningSpeed * layers[layerIndex][neuronIndex].getDelta() * layers[layerIndex - 1][inputWeightIndex - 1].getValue();
						}
						layers[layerIndex][neuronIndex].setInputWeight(inputWeightIndex, newWeight);
					}
				}
			}
		}

		void saveToFile(const char* fileName);

		bool loadFromFile(const char* fileName);
};


std::ostream& operator <<(std::ostream &os, const Net &net);

#endif // NET_H_INCLUDED
