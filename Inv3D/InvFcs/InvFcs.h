// CInvFcs class
// encapsulates 2D model and computations
// 2D regularized inversion

// copyright Igor Cerovsky 2004

/*
	measurement points MUST be imported first in order
	to know the size of inversion matrix
*/

#pragma once
#include "afx.h"
#include "voxelfld.h"

typedef struct ItrHistory{
	int		nType;			// either regularized or focused iteration
	int		nIter;
	double	dResidual;
	double	dTime;
} ITRHST, *PITRHST;

// computation thread
UINT ThreadFocusing( LPVOID pParam );

class CInvFcs :
	public CObject
{
public:
	enum saveTag {
		saveWhileIter = 0,
		storeWhileIter = 1
	};

	enum inversionType {
		invNull = 0,
		inv2D = 1,
		inv3D = 2
	};

	enum modelType {
		gravGx = 1,
		gravGy = 2,
		gravGz = 3,
		gravG = 4,
		magMx = 5,
		magMy = 6,
		magMz = 7,
		magM = 8
	};

	enum ptsFormat {
		frmX = 0,
		frmY = 1,
		frmZ = 2,
		frmF = 3,
	};

	enum modelSaveTag{
		saveFrw = -1,		// save forward model
		saveBin,			// save as binary file
		saveBinSRF,			// save as Surfer binary grid file *.grd
		saveBinINV,			// save as Inv3D binary grid file *.gri
		saveTxt,			// save as txt file
		saveAppend,			// appends all models to one file
		saveSepar,			// saves all the models to separate files
	};

	enum invertTypeTag{
		invertNormal = 0,		// just CG
		invertPrecondDiag = 1,	// CG with diagonal preconditioner
		invertPrecondLU = 2,	// CG with LU preconditioner
		invertRegularized = 3,	// all data shrinking
		invertRglrzPrcnd = 4,	// all data shrinking
	};

public:
	CInvFcs();
	virtual ~CInvFcs();

	// 2D construction
	CInvFcs(int nx, int nz, double x0, double z0, double csx, double csz);
	CInvFcs(double x0, double xLen, double z0, double zLen, int nx, int nz);
	void	New2D(int nx, int nz, double x0, double z0, double csx, double csz);
	void	New2D(double x0, double xLen, double z0, double zLen, int nx, int nz);

	// 3D construction
	CInvFcs(int nx, int ny, int nz, double x0, double y0, double z0, double xcs, double ycs, double zcs);
	CInvFcs(double x0, double xLen, double y0, double yLen, double z0, double zLen, int nx, int ny, int nz);
	void	New3D(int nx, int ny, int nz, double x0, double y0, double z0, double xcs, double ycs, double zcs);
	void	New3D(double x0, double xLen, double y0, double yLen, double z0, double zLen, int nx, int ny, int nz);

	void	NullInit();
	void	Destroy();
	void	DestroyData();
	void	DestroyFrwModel();
	void	DestroyModels();

//protected:
//	void	Init();


protected:
	// model
	int			m_nType;		// 2D or 3D inversion
	int			m_nInvType;		// inversion type
	BOOL		m_bShrink;		// matrix shrinking
	BOOL		m_bSaveIter;	// save while iterating
	// 2D array of models as they follow in focusing process
	double**	m_pModels;		// !!! pointer to EXTERN array to be filled by models
	int			m_nModels;		// number of currently active/computed models
	int			m_nX;			// number of model cells in x direction
	int			m_nY;
	int			m_nZ;
	double		m_x0;			// origin of model
	double		m_y0;
	double		m_z0;
	double		m_xCs;			// cell size in z direction
	double		m_yCs;
	double		m_zCs;

	// voxel field
	CVoxelFld	m_vox;

	// CG and focusing computing inversion parameters
	int		m_nCgIter;		// maximum CG iterations
	int		m_nCgRestart;	// CG restart
	double	m_dCgEps;
	int		m_nFcsIter;		// maximum focusing iterations
	double	m_dFcsPow;		// focusing power exponent
	double	m_dInitModel;
	double	m_dRegMiu;		// regularization parameter decrease
	BOOL	m_bMatrixTmpDel;// delete matrix after computation

	// forward model for testing
	double*		m_pModFrw;	// sizes and extent are equal to model

	int		m_nPtsX;		// number of measurement points
	int		m_nPtsY;		// number of measurement points
	int		m_nPts;			// number of measurement points
	int		m_nFormat;		// format of stored data -> number of entries per point
	double*	m_pPts;			// measurement points coordinates; length = nFormat*m_nPts; storage [{x0,y0,z0,v0},  ...]


	// penalization
	//double	m_dPenMin;	// penalization minimum
	//double	m_dPenMax;	// penalization maximum
	double*	m_pPen;		// cell penalization array m_pPen[i]==min; m_pPen[i+1]==max; 

	// saving options
	CString		m_strFileRoot;		// model file root "D:/MyFolder/MyModel"
	CString		m_strFilePathTmp;	// model file root "D:/MyFolder/MyModel"
	int			m_nFileType;
	int			m_nFileAppend;

private:
	// inversion variables
	__int32		nM;		// number of matrix rows == number of measured points
	__int32		nN;		// number of matrix columns == number of model parameters
	__int32		nNs;	// SHRINKED number of matrix columns == number of model parameters
	double**	pA;		// matrix A[M x N]=A[rows x cols] -> forward operator; Ax=b
	double**	pAt;	// matrix A[M x N]=A[rows x cols] for temporary storage
	double*		px;		// model data x[N]=x[cols]
	double*		pr;		// model error r[N]=r[cols]
	double*		pd;		// conjugate search directions d[n]=d[cols]
	double*		pb;		// measured data, b[M+N]=b[rows+cols]
	double*		pbt;	// temporary storage for Ax regularized, pbt[N]=ptb[rows]
	double*		pq;		// CG q variable q=Ad; q[M+N]=q[rows+cols]
	double*		pR;		// diagonal regularization parameter matrix R[N x N]
	double*		pV;		// diagonal matrix for weighting V[N x N]
	double*		pW;		// diagonal weighting matrix W[N x N]
	double*		pWt;	// diagonal weighting matrix W[N x N] SHRINKED
	double*		pxt;	// temporary vector x[N] for weighting
	double*		pM;		// diagonal preconditioner M[N x N]
	double**	pLU;	// LU preconditioner M[N x N]
	__int32*	pLUi;	// LU preconditioner M[N x N] indexing array
	//double**	pC;		// Cholesky preconditioner M[N x N triangular]
	double*		pMt;	// diagonal preconditioner M[N x N] temporary storage
	double*		ps;		// used in preconditionong s[N]
	__int32*	pi;		// MATRIX SHRINKING indexing array pi[N] for 
	
	int Allocate(BOOL bAlloc);
	public: void ForcedFree();

protected:
	void	InitMatrix(double** pA, int m, int n);
	void	InitMatrix2D(double** pA, int m, int n);
	void	GetCellFld2D(double& fld, int i, int j, double xPt, double zPt);
	void	InitMatrix3D(double** pA, int m, int n);
	void	GetCellFld3D(double& fld, int i, int j, int k, double xPt, double yPt, double zPt);

	// 
	CString GetFileNameTmp();
	// binary file
	void	SaveIter(double* pData, int nData, int tag);
	void	SaveModelInv(CFile& file, double* pData, int tag=-1);	// saves 2D or 3D model in Inv3D grid *.gri
	void	SaveModel2Db(CFile& file, double* pData, int tag=-1);
	void	SaveModel3Db(CFile& file, double* pData, int tag=-1);
	// text file
	void	SaveModel2Dt(CStdioFile& file, double* pData, int tag);
	void	SaveModel3Dt(CStdioFile& file, double* pData, int tag);

public:
	double** GetModels()				{return m_pModels;}
	void	GetModels(int& nModels)		{nModels=m_nModels;}
	BOOL	IsEmpty()					{return (m_nX==-1 || m_nZ==-1);}
	BOOL	IsEmptyData()				{return (m_nPts==-1);}
	BOOL	IsEmptyPenal()				{return (m_pPen==NULL);}
	BOOL	IsForwardModel()			{return (m_pModFrw!=NULL);}
	void	Invert();
	void	MatrixTmpSave();
	void	MatrixTmpLoad();
	void	MatrixTmpDeleteFile();
	void	MatrixSaveTxt(double** A, double* b, double* mod, int m, int n);
	void	SetPenalization(int nxMin, int nxMax, int nyMin, int nyMax, int nzMin, int nzMax, double penMin, double penMax);

	void CInvFcs::SerializeMatrix(CArchive &ar);

	void	SetInverType(int nType)		{m_nInvType=nType;}
	int		GetInverType()				{return m_nInvType;}
	
	int		GetModelType()				{return m_nType;}
	
	void	SetFileType(int nType)		{m_nFileType=nType;}
	void	SetFileAppend(int nType)	{m_nFileAppend=nType;}
	
	void	GetFilePathInv(CString& strFilePath)	{strFilePath = m_strFileRoot + CString("_all.gri");}
	CString	GetFilePathInv()						{CString str = m_strFileRoot + CString("_all.gri"); return str;}

	void	GetFilePathFrwInv(CString& strFilePath)	{strFilePath = m_strFileRoot + CString("_frw.gri");}
	CString	GetFilePathFrwInv()						{CString str = m_strFileRoot + CString("_frw.gri"); return str;}

	void	SaveFrwModel();
	void	SaveFrwInv3D();

	void	SetMeasuredPts(double* pData, int nData, int nFormat=4);

	void	SetFileRoot(CString fileRoot)	{ m_strFileRoot = fileRoot;}
	void	GetFileRoot(CString& fileRoot)	{ fileRoot = m_strFileRoot;}

	void	SetPathTmp(CString tmpPath)		{ m_strFilePathTmp = tmpPath;}
	void	GetPathTmp(CString& tmpPath)	{ tmpPath = m_strFilePathTmp;}

	BOOL	GetSaveIter()									{return m_bSaveIter;}
	void	SetSaveIter(BOOL bSaveIter=TRUE)				{m_bSaveIter = bSaveIter;}

	BOOL	GetShrink()										{return m_bShrink;}
	void	SetShrink(BOOL bShrink=TRUE)					{m_bShrink = bShrink;}

	//void	GetPenalization(double& min, double& max)		{min=m_dPenMin; max=m_dPenMax;}
	//void	SetPenalization(double min, double max)			{m_dPenMin=min; m_dPenMax=max;}

	//void	SetMaxIter(int maxIter)							{m_nModMax = maxIter;}
	//void	GetMaxIter(int& maxIter)						{ maxIter = m_nModMax ;}

	// model parameters
	void	GetModelSize(int& nx, int& ny, int& nz)			{nx=m_nX; ny=m_nY; nz=m_nZ;}
	void	GetModelSize(int& nx, int& nz)					{nx=m_nX; nz=m_nZ;}

	void	GetExtent(double& xMin, double& xMax, double& yMin, double& yMax, double& zMin, double& zMax)	{xMin=m_x0; xMax=m_x0+m_xCs*m_nX; yMin=m_y0; yMax=m_y0+m_yCs*m_nY; zMin=m_z0; zMax=m_z0+m_zCs*m_nZ;}

	void	GetOrigo(double& x0, double& y0, double& z0)	{x0=m_x0; y0=m_y0; z0=m_z0;}
	void	GetOrigo(double& x0, double& z0)				{x0=m_x0; z0=m_z0;}

	void	GetCellSize(double& xcs, double& ycs, double& zcs)	{xcs=m_xCs; ycs=m_yCs; zcs=m_zCs;}
	void	GetCellSize(double& xcs, double& zcs)				{xcs=m_xCs; zcs=m_zCs;}

	void	SetCgParam(int maxIter, int restart, double eps)	{ m_dCgEps = eps;	m_nCgIter = maxIter;	m_nCgRestart = restart;}
	void	GetCgParam(int& maxIter, int& restart, double& eps)	{ eps=m_dCgEps;	maxIter=m_nCgIter;	restart=m_nCgRestart;}

	void	SetFcsIter(int fiter)							{ ASSERT(m_pModels==NULL); m_nFcsIter = fiter;}
	void	GetFcsIter(int& fiter)							{ fiter = m_nFcsIter;}
	int		GetFcsIter()									{ return m_nFcsIter;}

	void	SetFcsPow(double pow)							{ m_dFcsPow = pow;}
	void	GSetFcsPow(double& pow)							{ pow = m_dFcsPow;}
	double	GetFcsPow()										{ return m_dFcsPow;}

	void	SetIntialModel(double val)						{ m_dInitModel = val;}
	void	GetIntialModel(double& val)						{ val = m_dInitModel;}
	double	GetIntialModel()								{ return m_dInitModel;}

	void	SetCgEpsilon(double eps)						{ m_dCgEps = eps;}
	void	GetCgEpsilon(double& eps)						{ eps = m_dCgEps;}
	double	GetCgEpsilon()									{ return m_dCgEps;}

	void	SetCgIterMax(int iter)							{ m_nCgIter = iter;}
	void	GetCgIterMax(int& iter)							{ iter = m_nCgIter;}
	int		GetCgIterMax()									{ return m_nCgIter;}

	void	SetCgRestart(int rst)							{ m_nCgRestart = rst;}
	void	GetCgRestart(int& rst)							{ rst = m_nCgRestart;}
	int		GetCgRestart()									{ return m_nCgRestart;}

	void	SetRegMiu(double miu)							{ m_dRegMiu = miu;}
	void	GetRegMiu(double& miu)							{ miu = m_dRegMiu;}
	double	GetRegMiu()										{ return m_dRegMiu;}

	int		GetMemoryCosts(int m, int n, int tag=-1);
	void	GetMemoryCosts(int m, int n, int tag, double& tot, double& mtrx, double& mod, double& data);
	void	GetMemoryCosts(double& total, double& matrix, double& model, double& data);

	// forward model for testing
	void	NewForwardModel2D(int nPts, double elev);
	void	NewForwardModel3D(int nPtx, int nPty, double elev);
	void	SetModel2D(int nxMin, int nxMax, int nzMin, int nzMax, double val);
	void	SetModel3D(int nxMin, int nxMax, int nyMin, int nyMax, int nzMin, int nzMax, double val);
	void	InitForwardModel(BOOL bDestroyFrwModel=TRUE);
	void	InitForwardModelComp(BOOL bDestroyFrwModel=TRUE);
	void	ComputeModel2D();
	void	ComputeModel3D();
	void	AddNoise(double noise);

	double*	GetModelFrw()		{return m_pModFrw;}
	UINT	GetModelFrwSize()	{return m_nX*m_nY*m_nZ*sizeof(double);}

	double*	GetModelPts()		{return m_pPts;}
	void	GetModelPtsNum(int& nx, int& ny)	{nx = m_nPtsX; ny = m_nPtsY;}
	UINT	GetModelPtsNum()	{return m_nPts;}
	UINT	GetModelPtsSize()	{return m_nPts*m_nFormat*sizeof(double);}

	void	SaveForwardModel();
	void	LoadForwardModel();

	void	TestShrink();
	void	TestCholesky();
	void	TestLU();

	int		FldToGrd(double** pGrd);
	void	SaveIterHistory();
	void	GetModelRange(double& min, double& max);
};
