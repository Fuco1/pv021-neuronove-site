#ifndef NEURON_H_INCLUDED
#define NEURON_H_INCLUDED

#include <vector>
#include <utility>

using namespace std;

class Neuron {
	private:
		struct link {
			double weight;
			Neuron& parent;
		};

		vector<link> parents;
		double input; //relevant if this neuron is an input neuron
		double (*act_func)(double); //pointer to an activation function
	public:
		//TODO: constructor: Neuron(...);
		void set_input(double val);
		void set_act_func(double (*act_func)(double));
		double output(); //depends on outputs from its parents, weights, and an activation function
};

#endif // NEURON_H_INCLUDED
