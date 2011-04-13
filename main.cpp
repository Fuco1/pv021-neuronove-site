#include <iostream>
#include "net.h"
#include "neuron.h"
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
	neuronCounts.push_back(1/*image.getSize()*/);
	neuronCounts.push_back(2/* \todo determine the best number of hidden neurons */);
	neuronCounts.push_back(1);
	Net net(neuronCounts);

	/*
	Net net;

	if (!net.loadFromFile("./test.net")) {
		cerr << "File does not exist. Bye bye." << endl;
		exit(1);
	}
	*/

	//net.run(image);





	// Prepare training data.
	std::vector<pair<Image<double>, double> > examples;
  for (size_t i = 0; i < 10000; ++i) {
  	Image<double> example(1, 1, 1);
  	double value = randPlusMinusOne();
  	example.setVoxel(0, value);
  	//examples.push_back(make_pair(example, value >= 0 ? 1 : -1));
  	examples.push_back(make_pair(example, (value*value)));
  }

  // \todo Train.
  const size_t trainingIterationCount = 100;
  for (size_t trainingIteration = 0; trainingIteration < trainingIterationCount; ++trainingIteration) {
  	for (size_t exampleIndex = 0; exampleIndex < examples.size(); ++exampleIndex) {
  		net.train(examples[exampleIndex].first, examples[exampleIndex].second);
  	}
  }

	std::cout << net;


	// Train.
	double inputValue;
  /*
	double expectedOutput;
  do {
		std::cout << net;
  	Image<double> input(1, 1, 1);

		std::cout << "enter a value: ";
		std::cin >> inputValue;
  	input.setVoxel(0, inputValue);
		std::cout << "net output: " << net.run(input) << std::endl;
		std::cout << "enter expected output: ";
		std::cin >> expectedOutput;
  	input.setVoxel(0, inputValue);
		net.train(input, expectedOutput);
  } while (inputValue != 0);
	std::cout << net;
  */

	// Test on non-training data.
	//double inputValue;
  do {
		std::cout << "enter a value: ";
		std::cin >> inputValue;
  	Image<double> input(1, 1, 1);
  	input.setVoxel(0, inputValue);
		std::cout << "net output is " << net.run(input) << std::endl;
  } while (inputValue != 0);

	//net.saveToFile("./test.net");

	return 0;
}
