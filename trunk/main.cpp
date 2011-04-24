#include <iostream>
#include <sstream>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <getopt.h>
#ifdef WIN_32
  #include <windows.h>
  #include <tchar.h>
#else
  #include <dirent.h>
  #include <errno.h>
#endif
#include "net.h"
#include "neuron.h"
#include "image.h"

using namespace std;

// singleton struct to save values given by user
struct UserOptions {
	vector<LayerSpec> netSpec;
	char *loadPath;
	char *savePath;
	char *trainPath;
	char *runPath;
	double rate;
	unsigned int iters;
	// default values
	UserOptions() {
		loadPath = NULL;
		savePath = NULL;
		trainPath = NULL;
		runPath = NULL;
		rate = 0.05;
		iters = 1;
	}
} userOptions;

void warning(const char* message, ...) {
	va_list args;
	fprintf(stderr, "Warning: ");
	va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);
	fprintf(stderr, "\n");
}

void error(const char* message, ...) {
	va_list args;
	fprintf(stderr, "Error: ");
	va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(1);
}

void printHelp() {
	cerr << "Program options:\n"
		"-c, --createnet <x0>:<func0>,<x1>:<func1>,<x2>:<func2>,...\n"
		"	create a network with <xi> neurons in i-th layer such that\n"
		"	i-th layer uses <funci>, which is name of an activation function\n"
		"	identity funcion (id) should be used for 0-th layer\n"
		"-l, --loadnet <file path>\n"
		"	load a network from the file\n"
		"-s, --savenet <file path>\n"
		"	save the network to the file\n"
		"	if not specified, the network is printed to stdout\n"
		"-t, --train <file path>\n"
		"	train a network on train cases in the file/directory\n"
		"-i, --iters <x>\n"
		"	number of training iterations over the whole data set\n"
		"	default value is 1\n"
		"-x, --rate <x>\n"
		"	learning rate to be used for training\n"
		"	default value is 0.05\n"
		"-r, --run <file path>\n"
		"	run an already trained network on real cases in the file/directory\n"
		"-h, --help\n"
		"	print this help\n";
}

// stores values into global userOptions structure
void parseCmdLine(int argc, char **argv) {
	int c;
	while (true) {
		static struct option long_options[] =
		{
			{"createnet", required_argument, 0, 'c'},
			{"loadnet",   required_argument, 0, 'l'},
			{"savenet",   required_argument, 0, 's'},
			{"train",     required_argument, 0, 't'},
			{"iters",     required_argument, 0, 'i'},
			{"rate",      required_argument, 0, 'x'},
			{"run",       required_argument, 0, 'r'},
			{0, 0, 0, 0}
		};

		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "c:l:s:t:i:x:r:",
					long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1) break;

		string item, first, second;
		stringstream ssline, ssitem;
		int value1;
		int layerIndex = 0;
		double value2;
		switch (c) {
			case 'c':
				ssline << optarg;
				while (getline(ssline, item, ',')) {
					if (item.size() == 0) continue;
					ssitem << item;
					getline(ssitem, first, ':');
					getline(ssitem, second, ':');
					value1= atoi(first.c_str());
					if (value1 == 0)
						error("Zero neuron count specified for layer %i.", layerIndex);
					FunTranslator &ft = funTranslator;
					ft.n2fIterator = ft.name2fun.find(second);
					if (ft.n2fIterator == ft.name2fun.end())
						error("Unknown function specified for layer %i.", layerIndex);
					LayerSpec layerSpec(value1, ft.n2fIterator->second);
					userOptions.netSpec.push_back(layerSpec);
					ssitem.clear();
					layerIndex++;
				}
				break;
			case 'i':
				value1 = atoi(optarg);
				if (value1 <= 0) {
					warning("Iteration count must be positive."
								" Default to %i.", userOptions.iters);
				} else {
					userOptions.iters = value1;
				}
				break;
			case 'x':
				value2 = atof(optarg);
				if (value2 <= 0) {
					warning("Learning rate must be positive."
								" Default to %.2f.", userOptions.rate);
				} else {
					userOptions.rate = value2;
				}
				break;
			case 'r':
				userOptions.runPath = optarg;
				break;
			case 'l':
				userOptions.loadPath = optarg;
				break;
			case 's':
				userOptions.savePath = optarg;
				break;
			case 't':
				userOptions.trainPath = optarg;
				break;
			default:
				printHelp();
				exit(1);
		}
	}
}

void checkUserOptions() {
	UserOptions &ops = userOptions;
	//if (ops.loadPath == NULL && ops.netSpec.size() == 0)
	//	error("Exactly one of the options --load or --create must be specified.");
	if (ops.loadPath != NULL && ops.netSpec.size() > 0)
		error("--load and --create options cannot be specified together.");
	//if (ops.trainPath == NULL && ops.runPath == NULL)
	//	error("One of the options --run or --train must be specified.");
	//if (ops.trainPath != NULL && ops.runPath != NULL)
	//	error("--run and --train options cannot be specified together.");
}

// Returns 0 if the directory exist and we can read it, or errno otherwise
int getFileNames(const std::string &directoryName, vector<string> &files) {
  //std::vector<std::string> fileNames;
#ifdef WIN_32
        // \todo windows
	UINT counter = 0;
	bool working = true;
	string buffer;

	WIN32_FIND_DATA file;
	HANDLE myHandle = FindFirstFile(directoryName + "*.*",&file);

	if(myHandle != INVALID_HANDLE_VALUE) {
		files.push_back(file.cFileName);
		while(working) {
			FindNextFile(myHandle,&file);
			if(file.cFileName!=buffer) {
				files.push_back(file.cFileName);
			}
			else {
				//end of files reached
				working=false;
			}
		}
		return 0;
	}
	else {
		return 1;
	}
#else
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(directoryName.c_str())) == NULL) {
		cerr << "Error(" << errno << ") opening " << directoryName << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		string filename = string(dirp->d_name);
		if (filename != "." && filename != "..")
			files.push_back(filename);
	}
	closedir(dp);
	return 0;
#endif
}

// Fill dataItems with real (run) images. Data are to be found at runPath.
// Return true if loading from files was successful, false otherwise.
bool getRealData(vector<DataItem<double> > &dataItems, const char *runPath) {
	vector<string> files;
	int errno;
	string path = string(runPath) + "/";
	if ((errno = getFileNames(path, files))) {
		//cerr << "Error while loading directory content. Errno: " << errno << endl;
		return false;
	}

	for (size_t i = 0; i < files.size(); i++) {
		Image<double> image(path + files[i]);
		//cerr << "Loading file: " << path + files[i] << endl; //info
		dataItems.push_back(image);
	}

	return true;
}

// Fill dataItems with training images and expectVals with expected 
// results for these images. Data are to be found at trainPath. trainPath
// directory is expected to contain ``Positive'' and ``Negative'' directories.
// Return true if loading from files was successful, false otherwise.
bool getTrainingData(vector<DataItem<double> > &dataItems,
			vector<double> &expectVals, const char *trainPath) {

	vector<string> files;
	int errno;
	string path = string(trainPath) + "/Positive/";
	if ((errno = getFileNames(path, files))) {
		//cerr << "Error while loading directory content. Errno: " << errno << endl;
		return false;
	}

	for (size_t i = 0; i < files.size(); i++) {
		Image<double> image(path + files[i]);
		//cerr << "Loading file: " << path + files[i] << endl; // info
		dataItems.push_back(image);
		expectVals.push_back(1.0);
	}

	files.clear();
	path = string(trainPath) + "/Negative/";
	if ((errno = getFileNames(path, files))) {
		//cerr << "Error while loading directory content. Errno: " << errno << endl;
		return false;
	}

	for (size_t i = 0; i < files.size(); i++) {
		Image<double> image(path + files[i]);
		//cerr << "Loading file: " << path + files[i] << endl; // info
		dataItems.push_back(image);
		expectVals.push_back(0.0);
	}

	return true;
}

/*
EXAMPLES:
	 a.exe --createnet 2000:id,20:tanh,1:unitstep --savenet network.dat
	 	Vytvori novou netrenovanou dvou-vrstvou sit s
		2000 vstupnimi, 20 skrytymi, a jednim vystupnim neuronem.
	 	Vstupni vrstva ma pridelenu identickou aktivacni funkci,
		skryta ma tanh, a vystupni vrstve nalezi funkce unitstep.
		Sit pote ulozi do souboru network.dat
	 a.exe -c 2000:id,20:tanh,1:unitstep -s network.dat
	 	stejne jako predchozi priklad s kratkymi verzemi parametru
	 a.exe --loadnet net.dat --savenet net.dat --train ./data -i 2 -x 0.1
	 	Nacte sit, vytrenuje a ulozi pod stejnym nazvem. Trenovaci
		data se projdou 2x s rychlosti uceni 0.1
	 a.exe --loadnet net.dat --run ./realdata
	 	nacte sit, provede vypocty nad realnymi daty
*/

int main(int argc, char **argv) {
	parseCmdLine(argc, argv);
	checkUserOptions();

	Net net;
	vector<DataItem<double> > dataItems;
	vector<double> expectVals;

	// initialize network
	if (userOptions.loadPath != NULL) {
		cerr << "Loading network..." << endl;
		if (!net.loadFromFile(userOptions.loadPath))
			error("Opening file %s failed.", userOptions.loadPath);
	} else {
		cerr << "Creating network..." << endl;
		net.init(userOptions.netSpec);
	}
	// load input data and run or train network on them
	if (userOptions.trainPath != NULL) {
		cerr << "Loading data..." << endl;
		if (!getTrainingData(dataItems, expectVals, userOptions.trainPath))
			error("Error reading directory %s", userOptions.trainPath);
		cerr << "Training..." << endl;
		net.train(dataItems, expectVals, userOptions.iters);
	}
	dataItems.clear();
	if (userOptions.runPath != NULL) {
		cerr << "Loading data..." << endl;
		if (!getRealData(dataItems, userOptions.runPath))
			error("Error reading directory %s", userOptions.runPath);
		cerr << "Running..." << endl; // info
		for (size_t i = 0; i < dataItems.size(); i++)
			cout << net.run(dataItems[i]) << endl;
	}
	// save the network to a file
	if (userOptions.savePath != NULL) {
		cerr << "Saving network..." << endl;
		net.saveToFile(userOptions.savePath);
	} else {
		//cout << net;
	}

	/*
  	// Create the net.
	std::vector<LayerSpec> netSpec;
	netSpec.push_back(LayerSpec(1, funTranslator.name2fun["id"]));
	netSpec.push_back(LayerSpec(2, funTranslator.name2fun["tanh"]));
	netSpec.push_back(LayerSpec(1, funTranslator.name2fun["tanh"]));
	Net net(netSpec);

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
			net.trainOnce(examples[exampleIndex].first, examples[exampleIndex].second);
		}
	}
	std::cout << net;

	// Test on non-training data.
	//double inputValue;
	do {
		std::cout << "enter a value: ";
		if (!(std::cin >> inputValue)) break;
		Image<double> input(1, 1, 1);
		input.setVoxel(0, inputValue);
		std::cout << "value * value is " << inputValue * inputValue << std::endl;
		std::cout << "net output is " << net.run(input) << std::endl;
	} while (inputValue != 0);
 	*/

	return 0;
}
