#include "cmath"
#include "net.h"

double actFuncTanh(double potential) {
	const double a = 1.7159;
	const double b = 2.0/3.0;

	return a * tanh(b * potential);
};
