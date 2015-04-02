#include "stdafx.h"
#include "lu_decomp.h"
#include "math.h"

// matrix pA[n][n] is replaced by its LU decomposition on output
int LU_decomp(double **pA, int n, __int32 *indx)
{
	int		i, imax = 0, j, k;
	double	big, dum, sum, temp;
	double	*vv;
	double	d;

	vv = new double[n];
	d = 1.0;
	for (i = 0; i < n; i++) {
		big = 0.0;
		for (j = 0; j < n; j++)
			if ((temp = fabs(pA[i][j])) > big)
				big = temp;
		if (big == 0.0) return 0;
		vv[i] = 1.0 / big;
	}
	for (j = 0; j < n; j++) {
		for (i = 0; i < j; i++) {
			sum = pA[i][j];
			for (k = 0; k < i; k++)
				sum -= pA[i][k] * pA[k][j];
			pA[i][j] = sum;
		}
		big = 0.0;
		for (i = j; i < n; i++) {
			sum = pA[i][j];
			for (k = 0; k < j; k++)
				sum -= pA[i][k] * pA[k][j];
			pA[i][j] = sum;
			if ((dum = vv[i] * fabs(sum)) >= big) {
				big = dum;
				imax = i;
			}
		}
		if (j != imax) {
			for (k = 0; k < n; k++) {
				dum = pA[imax][k];
				pA[imax][k] = pA[j][k];
				pA[j][k] = dum;
			}
			d = -(d);
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if (pA[j][j] == 0.0) pA[j][j] = 1.0e-30;
		if (j != n) {
			dum = 1.0 / (pA[j][j]);
			for (i = j + 1; i < n; i++)
				pA[i][j] *= dum;
		}
	}
	delete[] vv;

	return 1;
}

void LU_solve(double **pA, double* b, int n, __int32 *indx)
{
	int		i, ii = 0, ip, j;
	double	sum;

	for (i = 0; i < n; i++) {
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if (ii) {
			for (j = ii - 1; j <= i - 1; j++)
				sum -= pA[i][j] * b[j];
		}
		else if (sum)
			ii = i + 1;
		b[i] = sum;
	}
	for (i = n - 1; i >= 0; i--) {
		sum = b[i];
		for (j = i + 1; j < n; j++)
			sum -= pA[i][j] * b[j];
		b[i] = sum / pA[i][i];
	}
}
