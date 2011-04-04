#include <iostream>
#include "net.h"
#include "image.h"

using namespace std;

/*
 * suggested parameters:
 *  -createnet <x> <y> <z> - create a net with the given parameters
 * 	-loadnet <file path> - load a network from the file
 * 	-savenet <file path> - save the network to the file
 * 	-train <directory with positives> <directory with negatives> - train a network on cases in the directories
 * 	-run <directory with real images> - run an already trained network on cases in the directory; print the results to stdout

 example:
 a.exe -createnet 10 10 20 -savenet network.dat                                          // vytvori novou netrenovanou sit pro 10x10x20 voxelu
 a.exe -loadnet network.dat -savenet network.dat -train ./positives ./negatives          // nacte sit, vytrenuje a ulozi pod stejnym nazvem
 a.exe -loadnet network.dat -run ./realdata                                              // nacte sit, provede vypocty nad realnymi daty
 */

// Returns the vector of all filenames in the given directory
std::vector<std::string> getFileNames(const std::string &directoryName) {
	std::vector<std::string> fileNames;
	#ifdef WIN_32
  // \todo windows
	#else
  // \todo unix
	#endif
	return fileNames;
}

int main(int argc, char **argv) {
	Image<double> image("image1.dat");

	std::vector<size_t> neuronCounts;
	neuronCounts.push_back(image.getSize());
	neuronCounts.push_back(20/* \todo determine the best number of hidden neurons */);
	neuronCounts.push_back(1);
	Net net(neuronCounts);

	net.run(image);


	return 0;
}
