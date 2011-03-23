#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include <iostream>
#include <vector>
#include "neuron.h"
#include "image.h"

using namespace std;

template <class VoxelType> class Net {
	private:
		vector<Neuron> neurons; // All neurons of this net.
		Neuron *head; 
    public:
		//TODO: constructor: Net(...);
		double run(const Image<VoxelType> &img);
		void train(const Image<VoxelType> &img, double exp_val);

		void saveToFile(const std::string &fileName); // \todo Save the net to a file.
		void loadFromFile(const std::string &fileName);// \todo Load the net from a file.
};

#endif // NET_H_INCLUDED
