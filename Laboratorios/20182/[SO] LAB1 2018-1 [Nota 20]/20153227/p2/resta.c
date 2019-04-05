#include "fraccion.h"

fraccion resta(fraccion a, fraccion b){
	int numx= a.num * b.den - a.den * b.num, denx = a.den * b.den;
	fraccion res; res.num = numx; res.den = denx;
	return res;
}
