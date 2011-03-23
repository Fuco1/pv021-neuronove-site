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
		Neuron *head; //Neuron &head; -- Opravdu se tu mela pouzit reference misto ukazatele? Mne to prijde jako chyba, proto jsem to zakomentoval. Karel
	public:
		//TODO: constructor: Net(...);
		bool run(const Image<VoxelType> &img);
		void train(const Image<VoxelType> &img, bool exp_val);

		void saveToFile(const std::string &fileName); // \todo Save the net to a file.
		void loadFromFile(const std::string &fileName);// \todo Load the net from a file.
};

#endif // NET_H_INCLUDED
