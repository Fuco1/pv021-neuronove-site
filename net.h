#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include <iostream>
#include <vector>
#include "neuron.h"
#include "image.h"

using namespace std;

class Net {
	private:
		vector<Neuron> neurons;
		Neuron& head;
	public:
		//TODO: constructor: Net(...);
		bool run(const Image &img);
		void train(const Image &img, bool exp_val);
};

#endif // NET_H_INCLUDED
