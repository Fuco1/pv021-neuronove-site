#ifndef NEURON_H_INCLUDED
#define NEURON_H_INCLUDED

#include <vector>
#include <utility>

using namespace std;

class Neuron {
	private:
		struct link {
			double weight;
			Neuron *neuron; //Neuron &neuron; -- Opravdu se tu mela pouzit reference misto ukazatele? Mne to prijde jako chyba, proto jsem to zakomentoval. Karel
		};

		vector<link> parents;
		double input; //relevant if this neuron is an input neuron
		double (*act_func)(double); //pointer to an activation function
	public:
		//TODO: constructor: Neuron(...);
		void set_input(double val);
		void set_act_func(double (*act_func)(double));

		// Get the output value of this neuron. Depends on outputs of its parents, weights, and an activation function.
		double get_output() {
			// If this neuron has no parent...
			if (parents.size() == 0) {
				// ...just return the value (parent-less neurons work as input).
				return input;
			// Otherwise...
			} else {
				// ...calculate the weighted sum of its inputs...
				double weightedSum = 0;
				for (vector<link>::iterator i = parents.begin(); i != parents.end(); ++i) {
					if (i->neuron == NULL) {
						weightedSum += i->weight; // Return weight_k * 1 if the link is NULL; this will be the case with the 0-th link, which will be representing the neuron's bias (x_0 = 1).
					} else {
						weightedSum += i->weight * i->neuron->get_output(); // Return weight_k * x_k
					}
				}
				// ...and apply the activation function.
				return act_func(weightedSum);
			}
		}
};

#endif // NEURON_H_INCLUDED
