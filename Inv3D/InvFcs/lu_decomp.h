#ifndef LU_DECOMP_H_
#define LU_DECOMP_H_

// header file to LU decomposition & solving
int LU_decomp(double **pA, int n, __int32 *indx);
void LU_solve(double **pA, double* b, int n, __int32 *indx);

#endif // LU_DECOMP_H_
