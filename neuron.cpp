#include "neuron.h"

double Neuron::get_output() {
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

void Neuron::set_input(double input) {
    this->input = val;
}

void Neuron::set_act_func(double (*act_func)(double)) {
    this->act_func = act_func;
}
