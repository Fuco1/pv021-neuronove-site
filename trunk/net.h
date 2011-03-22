#include <iostream>
#include <vector>
#include "neuron.h"

using namespace std; 

class Net {
	private:
		vector<Neuron> neurons;
		Neuron& head;
	public:
		//TODO: constructor: Net(...);
		bool run(vector<vector<int> >& img);
		void train(vector<vector<int> >& img, bool exp_val);
};
