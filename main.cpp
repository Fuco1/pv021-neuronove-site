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
int main(int argc, char **argv) {
	Image<int> image("image1.dat");
	Net<int> net;
	Neuron neuron;
	return 0;
}
