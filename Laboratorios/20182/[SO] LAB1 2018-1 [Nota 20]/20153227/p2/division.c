#include "fraccion.h"

fraccion division(fraccion a, fraccion b){
	fraccion res; res.num = a.num * b.den; res.den = a.den * b.num;
	return res;
}
