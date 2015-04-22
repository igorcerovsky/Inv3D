#include "MatVec.h"
#include "lu_decomp.h"
#include <time.h>
#include "InvFcs.h"

unsigned int g_nIter;
double	g_dResid;
extern ITRHST* g_pItrHistory;
extern unsigned int	g_nItrHistory;

// conjugate gradients for rectangular matrix
// with LU preconditioner
// LU precomdotioner MUST be initialized! with indexing array
void CGrectLU(double** pA, int m, int n, double* px, double* pb, double* pr, double* pd, double* pq, double* pt, double** pM, double* ps, __int32* pIndx, int& iter, double eps /*= 1.0e-12*/, int maxIter /*= 100*/, int restart /*= 50*/, int bHistory /*= 1*/)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double dltNew, dltOld;
	double alpha;
	double beta;
	double eps2;

	iter = 0;
	MultTransAxb(pA, px, pb, pr, pt, m, n);

	//VecElemMult(ps, pM, pr, n);
	CopyVec(ps, pr, n);
	LU_solve(pM, ps, n, pIndx);

	CopyVec(pd, ps, n);
	dltOld = dltNew = Dot(pr, ps, n);
	eps2 = eps*eps*dltOld;
	while (iter<maxIter && dltNew>eps2) {
		MultAxb(pA, pd, pq, m, n);
		alpha = dltNew / Dot(pq, m);
		AddVec(px, px, alpha, pd, n);
		if ((iter%restart) == 0) {
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
		if (bHistory) {
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



// conjugate gradients for symmetric positive definite matrix 
inline void CGsquare(double** pA, int n, double* px, double* pb, double* pr, double* pd, double* pq, int& iter, double eps /*= 1.0e-12*/, int maxIter /*= 100*/, int restart /*= 50*/, int bHistory /*= 1*/)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double dltNew, dltOld;
	double alpha;
	double beta;
	double eps2;

	iter = 0;
	MultAxb(pA, px, pb, pr, n, n);
	CopyVec(pd, pr, n);
	dltOld = dltNew = Dot(pr, n);
	eps2 = eps*eps*dltOld;
	while (iter<maxIter && dltNew>eps2) {
		MultAxb(pA, pd, pq, n, n);
		alpha = dltNew / Dot(pd, pq, n);
		AddVec(px, px, alpha, pd, n);
		if ((iter%restart) == 0 && iter != 0) {
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
		if (bHistory) {
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
void CGrect(double** pA, int m, int n, double* px, double* pb, double* pr, double* pd, double* pq, double* pt, int& iter, double eps /*= 1.0e-12*/, int maxIter /*= 100*/, int restart /*= 50*/, int bHistory /*= 1*/)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double dltNew, dltOld;
	double alpha;
	double beta;
	double eps2;

	iter = 0;
	MultTransAxb(pA, px, pb, pr, pt, m, n);
	CopyVec(pd, pr, n);
	dltOld = dltNew = Dot(pr, n);
	eps2 = eps*eps*dltOld;
	while (iter<maxIter && dltNew>eps2) {
		MultAxb(pA, pd, pq, m, n);
		alpha = dltNew / Dot(pq, m);
		AddVec(px, px, alpha, pd, n);
		if ((iter%restart) == 0) {
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
		if (bHistory) {
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
void CGrglrz(double** pA, double* pR, int m, int n, double* px, double* pb, double* pr, double* pd, double* pq, double* pt, int& iter, double eps /*= 1.0e-12*/, int maxIter /*= 100*/, int restart /*= 50*/, double miu /*= 0.5*/, int bHistory /*= 1*/)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double	dltNew, dltOld, dltMin;
	double	alpha;
	double	beta;
	double	rho, rhoStart;				// regularization parameter
	double	eps2;
	int		diverg = 0;
	int		iterMin = 0;

	// compute first CG iteration first
	CGrect(pA, m, n, px, pb, pr, pd, pq, pt, iter, eps, 1, 1);

	// continue regularized
	rhoStart = rho = OneNorm(pr, n) / OneNorm(px, n);
	SetVal(pR, sqrt(rho), n);
	MultrTransAxb(pA, pR, px, pb, pr, pt, m, n);
	CopyVec(pd, pr, n);
	dltOld = dltMin = dltNew = Dot(pr, n);
	eps2 = eps*eps*dltOld;
	if (miu == 0.0 || miu == 1.0)
		miu = pow((eps2 / rho), 1.0 / maxIter);
	while (iter<maxIter && dltNew>eps2) {
		MultrAxb(pA, pR, pd, pq, m, n);
		alpha = dltNew / Dot(pq, m + n);
		AddVec(px, px, alpha, pd, n);
		if ((iter%restart) == 0 || iter == 1) {
			MultrTransAxb(pA, pR, px, pb, pr, pt, m, n);
		}
		else {
			MultrTransAxb(pA, pR, pq, pt, m, n);
			AddVec(pr, pr, -alpha, pt, n);
		}
		dltOld = dltNew;
		dltNew = Dot(pr, n);
		if (dltNew<dltMin) {
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
		if (bHistory) {
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
void PrecondDiag(double** pA, double* pM, int m, int n)
{
	for (int j = 0; j<n; j++) {
		pM[j] = 0.0;
		for (int i = 0; i<m; i++) {
			pM[j] += pA[i][j] * pA[i][j];
		}
		pM[j] = 1.0 / pM[j];
	}
}

// conjugate gradients for rectangular matrix
// with diagonal preconditioner
// diagonal precomdotioner MUST be already inversed
inline void CGrect(double** pA, int m, int n, double* px, double* pb, double* pr, double* pd, double* pq, double* pt, double* pM, double* ps, int& iter, double eps /*= 1.0e-12*/, int maxIter /*= 100*/, int restart /*= 50*/, int bHistory /*= 1*/)
{
	clock_t start, finish;
	double  duration;
	start = clock();

	double dltNew, dltOld;
	double alpha;
	double beta;
	double eps2;

	iter = 0;
	MultTransAxb(pA, px, pb, pr, pt, m, n);
	VecElemMult(ps, pM, pr, n);
	CopyVec(pd, ps, n);
	dltOld = dltNew = Dot(pr, ps, n);
	eps2 = eps*eps*dltOld;
	while (iter<maxIter && dltNew>eps2) {
		MultAxb(pA, pd, pq, m, n);
		alpha = dltNew / Dot(pq, m);
		AddVec(px, px, alpha, pd, n);
		if ((iter%restart) == 0) {
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
		if (bHistory) {
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
