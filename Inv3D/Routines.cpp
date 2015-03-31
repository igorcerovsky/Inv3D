#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void FindAxisMinMaxStep(double& min, double& max, double& step, double& start, int& digM, int& digN, int n)
{
	// quit if automatic if dasabled
	double a, b, end, dec;

	a = max - min;
	if(a == 0) return;
	b = 1.0;
	if(a > 1) {
		b = 1.0;
		while(a > 10) {
			a /= 10;
			b *= 10;
		}
	}
	else {
		b = 1.0;
		while(a <= 1) {
			a *= 10;
			b /= 10;
		}
	}
	start = floor(min/b) * b;
	end = ceil(max/b) * b;
	a = (end - start) / n;
	b = 1.0;
	if(a > 1) {
		while(a >= 10) {
			a /= 10;
			b *= 10;
		}
	}
	else {
		while(a <= 1) {
			a *= 10;
			b /= 10;
		}
	}
	step = floor(((end - start) /n)/b) * b;

	dec = log10(step/20);
	digM = 0;
	digN = 0;
	if( dec < 0 ) {
		dec = ceil( fabs(dec) )-1;
		digM = (int)dec;
		digN = (int)dec+1;
	}
}

