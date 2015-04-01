#ifndef MAT_VEC_H_
#define MAT_VEC_H_
// Matrix - vector algebra

// Copyright Igor Cerovsky 2004

#include "lu_decomp.h"
#include <math.h>
#include <memory>



//global variables about computing
UINT	g_nIter;
double	g_dResid;
extern ITRHST* g_pItrHistory;
extern UINT	g_nItrHistory;

/*
	creates indexing vector for shrinking
	'pi': indexing array to keep track on the parameters
	!!! 'pi'	MUST be initialized
	!!! 'n'		is reference
	original size 'n' is replaced by new size of indexing array
*/
inline void VectorShrinkIndex(double* px, __int32* pi, int& n)
{
	int nIndex=0;
	for(int j=0; j<n; j++) {
		if( px[j] != 0.0 ) {
			//pi[nIndex] = pi[j];
			pi[nIndex] = j;
			nIndex++;
		}
	}
	n=nIndex;
}

/*
	creates indexing vector for shrinking
	!!! iterative shrinking using previousli shrinked 'pi'
	'pi': indexing array to keep track on the parameters
	!!! 'pi'	MUST be initialized
	!!! 'n'		is reference
	original size 'n' is replaced by new size of indexing array
*/
inline void VectorShrinkIndexIter(double* px, __int32* pi, int& n)
{
	int nIndex=0;
	for(int j=0; j<n; j++) {
		if( px[j] != 0.0 ) {
			pi[nIndex] = pi[j];
			nIndex++;
		}
	}
	n=nIndex;
}

/*
	shrinks vector removes all zero values
	pi: indexing array to keep the track to restore values
	!!! 'pi'	MUST be initialized
	!!! 'n'		is reference
*/
inline void VectorShrink(double* px, __int32* pi, int nIndex)
{
	for(int i=0; i<nIndex; i++) {
		px[i] = px[ pi[i] ];
	}
}

/*
	STRETCH solution OUTPUT VECTOR previously "shrinked" by VectorShrink 
	'pi': indexing array to keep track on model parameters
	!!! 'pi'	MUST be initialized
	'n': maximum size of indexing array
*/
inline void VectorStretch(double* px, __int32* pi, int n, int nOrigSize)
{
	// go backward!
	int in = n-1;
	for(int i=nOrigSize-1; i>=0; i--) {
		if( pi[in] == i ) {
			px[i] = px[ in ];
			in--;
		}
		else
			px[i] = 0.0;
	}
}

/*
	shrinks matrix A, removes all zero columns
	pi: indexing array to keep track on model parameters
	!!! 'pi'	MUST be initialized
	!!! 'n'		is reference
*/
inline void MatrixShrink(double** pA, __int32* pi, int m, int nIndex)
{
	for(int i=0; i<m; i++) {
		for(int j=0; j<nIndex; j++) {
			pA[i][j] = pA[ i ][ pi[j] ];
		}
	}
	
}

// copy matrix
inline void CopyMatrix(double** dest, double** src, int m, int n)
{
	for(int i=0; i<m; i++) {
		memcpy( dest[i], src[i], n*sizeof(double) );
		//for(int j=0; j<n; j++) {
		//	dest[i][j] = src[i][j];
		//}
	}
	
}

// Ax=b
inline void MultAxb(double** A, double* x, double* b, int m, int n)
{
	double d;
	for(int i=0; i<m; i++) {
		d = 0.0;
		for(int j=0; j<n; j++) {
			d += A[i][j]*x[j];
		}
		b[i] = d;
	}
}

// C=A*D;	D is diagonal matrix
inline void MultDiag(double** C, double** A, double* d, int m, int n)
{
	for(int i=0; i<m; i++) {
		for(int j=0; j<n; j++) {
			C[i][j] = A[i][j]*d[j];
		}
	}
}

// r = b-Ax
inline void MultAxb(double** A, double* x, double* b, double* r, int m, int n)
{
	double d;
	for(int i=0; i<m; i++) {
		d = 0.0;
		for(int j=0; j<n; j++) {
			d += A[i][j]*x[j];
		}
		r[i] = b[i]-d;
	}
}

// b = trans(A)*x
inline void MultTransAxb(double** A, double* x, double* b, int m, int n)
{
	double d;
	for(int j=0; j<n; j++) {
		d = 0.0;
		for(int i=0; i<m; i++) {
			d += A[i][j]*x[i];
		}
		b[j] = d;
	}
}

// r = trans(A)*(b-Ax)
inline void MultTransAxb(double** A, double* x, double* b, double* r, double* t, int m, int n)
{
	MultAxb(A, x, b, t, m, n);
	MultTransAxb(A, t, r, m, n);
}

// regularized Ax=b
// m, n are dimensions of NON-REGULARIZED matrix
inline void MultrAxb(double** A, double* R, double* x, double* b, int m, int n)
{
	double d;
	for(int i=0; i<m; i++) {
		d = 0.0;
		for(int j=0; j<n; j++) {
			d += A[i][j]*x[j];
		}
		b[i] = d;
	}

	for(int k=0; k<n; k++) {
		b[m+k] = R[k]*x[k];
	}
}

// regularized r = b-Ax
// m, n are dimensions of NON-REGULARIZED matrix
inline void MultrAxb(double** A, double* R, double* x, double* b, double* r, int m, int n)
{
	double d;
	for(int i=0; i<m; i++) {
		d = 0.0;
		for(int j=0; j<n; j++) {
			d += A[i][j]*x[j];
		}
		r[i] = b[i] - d;
	}

	for(int k=0; k<n; k++) {
		r[m+k] = b[m+k] - R[k]*x[k];
	}
}

// regularized b = transp(A)*x
// m, n are dimensions of NON-REGULARIZED matrix
inline void MultrTransAxb(double** A, double* R, double* x, double* b, int m, int n)
{
	double d;
	for(int j=0; j<n; j++) {
		d = 0.0;
		for(int i=0; i<m; i++) {
			d += A[i][j]*x[i];
		}
		b[j] = d + x[m+j]*R[j];
	}
}

// regularized r = transp(A)*(b-Ax)
// m, n are dimensions of NON-REGULARIZED matrix
inline void MultrTransAxb(double** A, double* R, double* x, double* b, double* r, double* t, int m, int n)
{
	MultrAxb(A, R, x, b, t, m, n);
	MultrTransAxb(A, R, t, r, m, n);
}

/*
	fills matrix C with transp(A)*A
	output matrix is symmetric and positive
*/
inline void TranspAA(double** A, double** C, int m, int n)
{
	for(int i=0; i<n; i++) {		// rows or resulting matrix C
		for(int j=0; j<=i; j++) {	// columns of resulting matrix C
			C[i][j] = 0.0;
			for(int k=0; k<m; k++) {
				C[i][j] += A[k][j]*A[k][i];
			}
			C[j][i] = C[i][j];
		}
	}
}

/*
	fills matrix C with transp(A)*A
	!!! C is TRIANGULAR MATRIX !!!! C[NxN]
*/
inline void TransptAA(double** A, double** C, int m, int n)
{
	for(int i=0; i<n; i++) {		// rows or resulting matrix C
		for(int j=0; j<=i; j++) {	// columns of resulting matrix C
			C[i][j] = 0.0;
			for(int k=0; k<m; k++) {
				C[i][j] += A[k][j]*A[k][i];
			}
		}
	}
}

/*
	Cholesky decomposition
	matrix pC is replaced by Chlesky factor on output
*/
inline int CholesskyDC(double** pC, int n)
{
	double sum;

	for(int i=0; i<n; i++) {
		for(int j=0; j<=i; j++) {
			sum = pC[i][j];
			for(int k=0; k<j; k++) {
				sum -= pC[i][k]*pC[j][k];
			}
			if(i!=j) {
				pC[i][j] = sum/pC[j][j];
			}
			else {
				if(sum <= 0)  
					return 0;			// matrix is not positive definite
				pC[i][i] = sqrt(sum);
			}
		}
	}

	return 1;
}

// vector dot product
inline double Dot(double* a, double* b, int n)
{
	double d=0.0;
	for(int i=0; i<n; i++) {
		d += a[i]*b[i];
	}
	return d;
}

// vector dot product with itself
inline double Dot(double* a, int n)
{
	double d=0.0;
	for(int i=0; i<n; i++) {
		d += a[i]*a[i];
	}
	return d;
}

// a = d
inline void SetVal(double* a, double d, int n)
{
	for(int i=0; i<n; i++) {
		a[i] = d;
	}
}

// a += d
inline void AddVal(double* a, double d, int n)
{
	for(int i=0; i<n; i++) {
		a[i] += d;
	}
}

// a += d
inline void ScaleVec(double* a, double d, int n)
{
	for(int i=0; i<n; i++) {
		a[i] *= d;
	}
}

// c = a+b
inline void AddVec(double* c, double* a, double* b,  int n)
{
	for(int i=0; i<n; i++) {
		c[i] = a[i] + b[i];
	}

}

// vc = va+scb*vb
// should be more intuitive
inline void AddVec(double* c, double* a, double scb, double* b,  int n)
{
	for(int i=0; i<n; i++) {
		c[i] = a[i] + scb*b[i];
	}

}

// vc = sca*va+scb*vb
inline void AddVec(double* c, double sca, double* a, double scb, double* b,  int n)
{
	for(int i=0; i<n; i++) {
		c[i] = sca*a[i] + scb*b[i];
	}

}

// vc = sca*va+scb*vb
inline void CopyVec(double* dest, double* src, int n)
{
	memcpy(dest, src, n*sizeof(double));
}

// zero vector
inline void ZeroVec(double* a, int n)
{
	memset((void*)a, 0, n*sizeof(double));
}

// zero vector
template<typename T> void ZeroVec(const std::unique_ptr<T> a, int n)
{
	memset((void*)a, 0, n*sizeof(T));
}

// one norm
inline double OneNorm(double* a, int n)
{
	double d = 0.0;
	for(int i=0; i<n; i++) {
		d += fabs(a[i]);
	}
	return d;
}

// two norm
inline double TwoNorm(double* a, int n)
{
	double d = 0.0;
	for(int i=0; i<n; i++) {
		d += a[i]*a[i];
	}
	return sqrt(d);
}

// C norm
inline double NormC1(double* a, double* b, int n)
{
	double d = 0.0;
	for(int i=0; i<n; i++) {
		d += fabs(a[i]-b[i]);
	}
	return d;
}

// C norm
inline double NormC2(double* a, double* b, int n)
{
	double d = 0.0;
	for(int i=0; i<n; i++) {
		d += (a[i]-b[i])*(a[i]-b[i]);
	}
	return d;
}

// C norm
inline void NormC(double* a, double* b, double& c1, double& c2, int n)
{
	c1 = 0.0;
	c2 = 0.0;
	for(int i=0; i<n; i++) {
		c2 += (a[i]-b[i])*(a[i]-b[i]);
		c1 += fabs(a[i]-b[i]);
	}
}

// penalize
inline void Penalize(double* a, int n, double min, double max)
{
	for(int i=0; i<n; i++) {
		if(a[i] < min) 
			a[i]=min;
		if(a[i] > max) 
			a[i]=max;
	}
}

// penalize pxt
// px=pW*pxt
inline void PenalizeEx(double* pxt, double* px, double* pW, int n, double min, double max)
{
	for(int i=0; i<n; i++) {
		px[i]=pW[i]*pxt[i];
		if(px[i] < min) { 
			pxt[i] = min/(pW[i]+1.0e-15);
		}
		if(px[i] > max) 
			pxt[i] = max/(pW[i]+1.0e-15);
	}
}

// diagonal matrix s => inverse diagonal matrix of squared row sum from A
inline void InvRowSumSq(double** A, double* s, int m, int n)
{
	for(int j=0; j<n; j++) {
		s[j] = 0.0;
		for(int i=0; i<m; i++) {
			s[j] += A[i][j]*A[i][j];	// squared sum
		}
		s[j] = 1.0 / s[j];				// inverse
	}
}

// square elements of vector
inline void VecElemSq(double* a, int n)
{
	for(int i=0; i<n; i++) {
		a[i] = a[i]*a[i];
	}
}

// square elements of vector
inline void VecElemSqrt(double* a, int n)
{
	for(int i=0; i<n; i++) {
		a[i] = sqrt(a[i]);
	}
}

// multiply vector elements c= a*b
inline void VecElemMult(double* c, double* a, double* b, int n)
{
	for(int i=0; i<n; i++) {
		c[i] = a[i]*b[i];
	}
}

// multiply vector elements with diagonal inverse matrix
// c=inv(diag[b])*a; c=a/b
inline void VecElemMultInv(double* c, double* a, double* b, int n)
{
	for(int i=0; i<n; i++) {
		if( b[i]==0 )
			c[i] = a[i]/1.0e-15;
		else
			c[i] = a[i]/b[i];
	}
}


// multiply vector elements
inline void VecElemMultW(double* w, double* x, double* v, int n)
{
	for(int i=0; i<n; i++) {
		w[i] = sqrt(x[i]*x[i]*v[i]);
	}
}

// multiply vector elements penalized
inline void VecElemMultPen(double* w, double* x, double* v, int n, double min, double max)
{
	for(int i=0; i<n; i++) {
		if(x[i] < min) 		x[i]=min;
		if(x[i] > max) 		x[i]=max;
		w[i] = sqrt( x[i]*x[i]*v[i] );
	}
}

// multiply vector elements penalized
// sizeof(pen) = 2*n*sizeof(double) 
// pen[i] == penalization minimum
// pen[i+1] == penalization maximum
inline void VecElemMultPen(double* w, double* x, double* v, int n, double* pen)
{
	for(int i=0; i<n; i++) {
		if( !(pen[2*i]==0.0 && pen[2*i+1]==0.0) ) {
			if(x[i] < pen[2*i]) 		x[i]=pen[2*i];
			if(x[i] > pen[2*i+1]) 		x[i]=pen[2*i+1];
		}
		w[i] = sqrt( x[i]*x[i]*v[i] );
	}
}

// multiply vector elements powered
inline void VecElemMultPow(double* w, double* x, double powx, double* v, int n)
{
	for(int i=0; i<n; i++) {
		w[i] = sqrt(pow(fabs(x[i]), powx)*v[i]);
	}
}

// multiply vector elements powered penalized
inline void VecElemMultPowPen(double* w, double* x, double powx, double* v, int n, double min, double max)
{
	for(int i=0; i<n; i++) {
		if(x[i] < min) 		x[i]=min;
		if(x[i] > max) 		x[i]=max;
		w[i] = sqrt(pow(fabs(x[i]), powx)*v[i]);
	}
}

// multiply vector elements powered cell penalized
// sizeof(pen) = 2*n*sizeof(double) 
// pen[i] == penalization minimum
// pen[i+1] == penalization maximum
inline void VecElemMultPowPen(double* w, double* x, double powx, double* v, int n, double* pen)
{
	for(int i=0; i<n; i++) {
		if( !(pen[2*i]==0.0 && pen[2*i+1]==0.0) ) {
			if(x[i] < pen[2*i]) 	x[i]=pen[2*i];
			if(x[i] > pen[2*i+1]) 	x[i]=pen[2*i+1];
		}
		w[i] = sqrt(pow(fabs(x[i]), powx)*v[i]);
	}
}


// conjugate gradients for symmetric positive definite matrix 
inline void CGsquare(double** pA, int n, double* px, double* pb, double* pr, double* pd, double* pq, int& iter, double eps=1.0e-12, int maxIter=100, int restart=50, int bHistory=1)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double dltNew, dltOld;
	double alpha;
	double beta;
	double eps2;

	iter=0;
	MultAxb(pA, px, pb, pr, n, n);
	CopyVec(pd, pr, n);
	dltOld=dltNew = Dot(pr, n);
	eps2 = eps*eps*dltOld;
	while(iter<maxIter && dltNew>eps2) {
		MultAxb(pA, pd, pq, n, n);
		alpha = dltNew / Dot(pd, pq, n);
		AddVec(px, px, alpha, pd, n);
		if( (iter%restart)==0 && iter!=0 ) {
			MultAxb(pA, px, pb, pr, n, n);
		}
		else {
			AddVec(pr, pr, -alpha, pq, n);
		}
		dltOld = dltNew;
		dltNew = Dot(pr, n);
		beta = dltNew / dltOld;
		AddVec(pd, pr, beta, pd, n);

		//--------- store iteration history here -------------------------------
		// number of iterations, norms, residual...
		if(bHistory) {
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			g_pItrHistory[g_nItrHistory].dTime = duration;
			g_pItrHistory[g_nItrHistory].nType = 1;
			g_pItrHistory[g_nItrHistory].nIter = iter;
			g_pItrHistory[g_nItrHistory].dResidual = sqrt(dltNew);
			g_nItrHistory++;
		}
		//----------------------------------------------------------------------
		iter++;
		g_nIter = iter;
		g_dResid = dltNew;
	}
	g_nIter = maxIter;
}

// conjugate gradients for rectangular matrix
inline void CGrect(double** pA, int m, int n, double* px, double* pb, double* pr, double* pd, double* pq, double* pt, int& iter, double eps=1.0e-12, int maxIter=100, int restart=50, int bHistory=1)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double dltNew, dltOld;
	double alpha;
	double beta;
	double eps2;

	iter=0;
	MultTransAxb(pA, px, pb, pr, pt, m, n);
	CopyVec(pd, pr, n);
	dltOld=dltNew = Dot(pr, n);
	eps2 = eps*eps*dltOld;
	while(iter<maxIter && dltNew>eps2 ) {
		MultAxb(pA, pd, pq, m, n);
		alpha = dltNew / Dot(pq, m);
		AddVec(px, px, alpha, pd, n);
		if( (iter%restart)==0 ) {
			MultTransAxb(pA, px, pb, pr, pt, m, n);
		}
		else {
			MultTransAxb(pA, pq, pt, m, n);
			AddVec(pr, pr, -alpha, pt, n);
		}
		dltOld = dltNew;
		dltNew = Dot(pr, n);
		beta = dltNew / dltOld;
		AddVec(pd, pr, beta, pd, n);

		//--------- store iteration history here -------------------------------
		// number of iterations, norms, residual...
		if(bHistory) {
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			g_pItrHistory[g_nItrHistory].dTime = duration;
			g_pItrHistory[g_nItrHistory].nType = 1;
			g_pItrHistory[g_nItrHistory].nIter = iter;
			g_pItrHistory[g_nItrHistory].dResidual = sqrt(dltNew);
			g_nItrHistory++;
		}
		//----------------------------------------------------------------------
		iter++;
		g_nIter = iter;
		g_dResid = dltNew;
	}
	g_nIter = maxIter;
}

// regularized conjugate gradients
inline void CGrglrz(double** pA, double* pR, int m, int n, double* px, double* pb, double* pr, double* pd, double* pq, double* pt, int& iter, double eps=1.0e-12, int maxIter=100, int restart=50, double miu=0.5, int bHistory=1)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double	dltNew, dltOld, dltMin;
	double	alpha;
	double	beta;
	double	rho, rhoStart;				// regularization parameter
	double	eps2;
	int		diverg=0;
	int		iterMin=0;

	// compute first CG iteration first
	CGrect(pA, m, n, px, pb, pr, pd, pq, pt, iter, eps, 1, 1);

	// continue regularized
	rhoStart = rho = OneNorm(pr, n) / OneNorm(px, n);
	SetVal(pR, sqrt(rho), n);
	MultrTransAxb(pA, pR, px, pb, pr, pt, m, n);
	CopyVec(pd, pr, n);
	dltOld = dltMin = dltNew = Dot(pr, n);
	eps2 = eps*eps*dltOld;
	if(miu==0.0 || miu==1.0)
		miu = pow((eps2/rho), 1.0/maxIter);
	while(iter<maxIter && dltNew>eps2) {
		MultrAxb(pA, pR, pd, pq, m, n);
		alpha = dltNew / Dot(pq, m+n);
		AddVec(px, px, alpha, pd, n);
		if( (iter%restart) == 0 || iter==1 ) {
			MultrTransAxb(pA, pR, px, pb, pr, pt, m, n);
		}
		else {
			MultrTransAxb(pA, pR, pq, pt, m, n);
			AddVec(pr, pr, -alpha, pt, n);
		}
		dltOld = dltNew;
		dltNew = Dot(pr, n);
		if(dltNew<dltMin) {
			dltMin = dltNew;
			iterMin = iter;
			diverg = 0;
		} 
		beta = dltNew / dltOld;
		AddVec(pd, pr, beta, pd, n);
		rho *= miu;
		SetVal(pR, sqrt(rho), n);

		//--------- store iteration history here -------------------------------
		// number of iterations, norms, residual...
		if(bHistory) {
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			g_pItrHistory[g_nItrHistory].dTime = duration;
			g_pItrHistory[g_nItrHistory].nType = 1;
			g_pItrHistory[g_nItrHistory].nIter = iter;
			g_pItrHistory[g_nItrHistory].dResidual = sqrt(dltNew);
			g_nItrHistory++;
		}
		//----------------------------------------------------------------------
		iter++;
		g_nIter = iter;
		g_dResid = dltNew;
	}
	g_nIter = maxIter;
}























///////////////////////////////////////////////////////////////////////////////////////
//************************************************************************************
//***** preconditioned ***************************************************************
// creates diagonal preconditioner from rectangular matrix
// M = inverse( diag[transp(A)*A] )
inline void PrecondDiag(double** pA, double* pM, int m, int n)
{
	for(int j=0; j<n; j++) {
		pM[j] = 0.0;
		for(int i=0; i<m; i++) {
			pM[j] += pA[i][j]*pA[i][j];
		}
		pM[j] = 1.0/pM[j];
	}
}

// conjugate gradients for rectangular matrix
// with diagonal preconditioner
// diagonal precomdotioner MUST be already inversed
inline void CGrect(double** pA, int m, int n, double* px, double* pb, double* pr, double* pd, double* pq, double* pt, double* pM, double* ps, int& iter, double eps=1.0e-12, int maxIter=100, int restart=50, int bHistory=1)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double dltNew, dltOld;
	double alpha;
	double beta;
	double eps2;

	iter=0;
	MultTransAxb(pA, px, pb, pr, pt, m, n);
	VecElemMult(ps, pM, pr, n);
	CopyVec(pd, ps, n);
	dltOld=dltNew = Dot(pr, ps, n);
	eps2 = eps*eps*dltOld;
	while( iter<maxIter && dltNew>eps2 ) {
		MultAxb(pA, pd, pq, m, n);
		alpha = dltNew / Dot(pq, m);
		AddVec(px, px, alpha, pd, n);
		if( (iter%restart)==0 ) {
			MultTransAxb(pA, px, pb, pr, pt, m, n);
		}
		else {
			MultTransAxb(pA, pq, pt, m, n);
			AddVec(pr, pr, -alpha, pt, n);
		}
		VecElemMult(ps, pM, pr, n);
		dltOld = dltNew;
		dltNew = Dot(pr, ps, n);
		beta = dltNew / dltOld;
		AddVec(pd, ps, beta, pd, n);

		//--------- store iteration history here -------------------------------
		// number of iterations, norms, residual...
		if(bHistory) {
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			g_pItrHistory[g_nItrHistory].dTime = duration;
			g_pItrHistory[g_nItrHistory].nType = 1;
			g_pItrHistory[g_nItrHistory].nIter = iter;
			g_pItrHistory[g_nItrHistory].dResidual = sqrt(dltNew);
			g_nItrHistory++;
		}
		//----------------------------------------------------------------------
		iter++;
		g_nIter = iter;
		g_dResid = dltNew;
	}
	g_nIter = maxIter;
}


// conjugate gradients for rectangular matrix
// with LU preconditioner
// LU precomdotioner MUST be initialized! with indexing array
inline void CGrectLU(double** pA, int m, int n, double* px, double* pb, double* pr, double* pd, double* pq, double* pt, double** pM, double* ps, __int32* pIndx, int& iter, double eps=1.0e-12, int maxIter=100, int restart=50, int bHistory=1)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double dltNew, dltOld;
	double alpha;
	double beta;
	double eps2;

	iter=0;
	MultTransAxb(pA, px, pb, pr, pt, m, n);

	//VecElemMult(ps, pM, pr, n);
	CopyVec(ps, pr, n);
	LU_solve(pM, ps, n, pIndx);

	CopyVec(pd, ps, n);
	dltOld=dltNew = Dot(pr, ps, n);
	eps2 = eps*eps*dltOld;
	while( iter<maxIter && dltNew>eps2 ) {
		MultAxb(pA, pd, pq, m, n);
		alpha = dltNew / Dot(pq, m);
		AddVec(px, px, alpha, pd, n);
		if( (iter%restart)==0 ) {
			MultTransAxb(pA, px, pb, pr, pt, m, n);
		}
		else {
			MultTransAxb(pA, pq, pt, m, n);
			AddVec(pr, pr, -alpha, pt, n);
		}

		//VecElemMult(ps, pM, pr, n);
		CopyVec(ps, pr, n);
		LU_solve(pM, ps, n, pIndx);

		dltOld = dltNew;
		dltNew = Dot(pr, ps, n);
		beta = dltNew / dltOld;
		AddVec(pd, ps, beta, pd, n);

		//--------- store iteration history here -------------------------------
		// number of iterations, norms, residual...
		if(bHistory) {
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			g_pItrHistory[g_nItrHistory].dTime = duration;
			g_pItrHistory[g_nItrHistory].nType = 1;
			g_pItrHistory[g_nItrHistory].nIter = iter;
			g_pItrHistory[g_nItrHistory].dResidual = sqrt(dltNew);
			g_nItrHistory++;
		}
		//----------------------------------------------------------------------
		iter++;
		g_nIter = iter;
		g_dResid = dltNew;
	}
	g_nIter = maxIter;
}


#endif //MAT_VEC_H_