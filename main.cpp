#include <iostream>
#include "net.h"
#include "image.h"

using namespace std;

/*
 * suggested parameters:
 * 	-net <file path> - load a network from the file; if omitted, a new network is created
 * 	-train <file path> - train a network on cases in the file and print the resulting network to stdout
 * 	-run <file path> - run a network on cases in the file; print the results to stdout
 */
int main(int argc, char **argv)
{
	Image<int> image("image1.dat");
	Net<int> net;
	return 0;
}
