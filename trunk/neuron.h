#ifndef NEURON_H_INCLUDED
#define NEURON_H_INCLUDED

#include <vector>

using namespace std;

class Neuron {
	private:
		struct link {
			double weight;
			Neuron *neuron; //NULL for biases
        };

		vector<link> parents;
		double input; //relevant if this neuron represents a piece of input data
		double (*act_func)(double); //pointer to an activation function
	public:
		//TODO: constructor: Neuron(...);
		void set_input(double input);
		void set_act_func(double (*act_func)(double));
        
		double get_output(); //depends on outputs of its parents, weights, and an activation function
};

#endif // NEURON_H_INCLUDED
