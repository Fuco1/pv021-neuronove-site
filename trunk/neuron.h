#ifndef NEURON_H_INCLUDED
#define NEURON_H_INCLUDED

#include <vector>

using namespace std;

/*
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

*/

class Neuron {
	private:
		vector<double> inputWeights;
		double potential; // set directly if this is an input neuron, or accumulated if this is an inner neuron
		double (*actFunc)(double); //pointer to an activation function
	public:
		//TODO: constructor: Neuron(...);
		void setPotential(double newPotential) {
			potential = newPotential;
		};

		void setActFunc(double (*newActFunc)(double)) {
			actFunc = newActFunc;
		};

		void propagateValueToUpperLayer() {
      /*
      double value = actFunc(potential);
      for each neuron in upper layer {
      	neuron->setPotential(neuron->potential + value * neuron.inputWeights[our index]);
      }
      */
		};
};

#endif // NEURON_H_INCLUDED
