#include "fraccion.h"

fraccion producto(fraccion a, fraccion b){
	fraccion res; res.num = a.num*b.num; res.den = a.den * b.den;
	return res;
}
