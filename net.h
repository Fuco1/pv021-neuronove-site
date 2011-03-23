#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include <iostream>
#include <vector>
#include "neuron.h"
#include "image.h"

using namespace std;

template <class VoxelType> class Net {
	private:
		vector<Neuron> neurons;
		Neuron& head;
	public:
		//TODO: constructor: Net(...);
		bool run(const Image<VoxelType> &img);
		void train(const Image<VoxelType> &img, bool exp_val);
};

#endif // NET_H_INCLUDED
