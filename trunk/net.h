#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include <iostream>
#include <vector>
#include "neuron.h"
#include "image.h"

using namespace std;

typedef vector<Neuron> Layer;

template <class VoxelType> class Net {
	private:
		vector<Layer> layers; // All neurons of this net.
	public:
		//TODO: constructor: Net(...);
		double run(const Image<VoxelType> &img) {
			// set all potentials to 0
      for (size_t l = 0; l < layers.size(); ++l) {
				for (size_t n = 0; n < layers[l].size(); ++n) {
					layers[l][n].setPotential(0);
				}
      }

      // set potentials in layer 0 according to input
			for (size_t n = 0; n < layers[0].size(); ++n) {
				layers[0][n].setPotential(0/*some value read from the input image*/);
			}

      // propagate from layer 0 upwards
      for (size_t l = 0; l < layers.size() - 1; ++l) {
				for (size_t n = 0; n < layers[l].size(); ++n) {
					layers[l][n].propagateValueToUpperLayer();
				}
      }

      // the potentials of the last layer have been set. apply their activation function and we have the result
		};


		void train(const Image<VoxelType> &img, double exp_val); // will call run() as one step

		void saveToFile(const std::string &fileName) {
		}; // \todo Save the net to a file.
		void loadFromFile(const std::string &fileName) {
		};// \todo Load the net from a file.

};

#endif // NET_H_INCLUDED
