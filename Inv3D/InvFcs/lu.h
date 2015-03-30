// header file to LU decomposition & solving

int LUdecomp(double **pA, int n, __int32 *indx);
void LUsolve(double **pA, double* b, int n, __int32 *indx);
