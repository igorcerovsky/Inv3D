#pragma once

#include "StdAfx.h"
#include "invfcs.h"
#include "MatVec.h"
#include <thread>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// global progress variable
UINT	g_nFiter;
UINT	g_nShrink;
UINT	g_nInit;

ITRHST* g_pItrHistory;
UINT	g_nItrHistory;

#define GRAVCONST 6.67259e-11

// computation thread
UINT ThreadFocusing( LPVOID pParam )
{
	CInvFcs* pInvFcs = (CInvFcs*) pParam;
	pInvFcs->Invert();

	return 1;
}

CInvFcs::CInvFcs(void)
{
	m_nType = invNull;
	m_nX = -1;
	m_nY = -1;
	m_nZ = -1;
	NullInit();
}

CInvFcs::~CInvFcs(void)
{
	Destroy();
}

// free memory
void CInvFcs::Destroy()
{
	if(m_pPts!=NULL) delete[] m_pPts;
	if(m_pPen!=NULL) delete[] m_pPen;
	if (pi != nullptr) delete[] pi;
	if(m_pModFrw!=NULL) {
		delete[] m_pModFrw;
		m_pModFrw = NULL;
	}
	if (m_pModels != nullptr)
	{
		for (int i = 0; i < m_nFcsIter; i++) {
			delete[] m_pModels[i];
		}
	}
	delete[] m_pModels;

	m_pPts = NULL;
}

void CInvFcs::DestroyModels()
{
	if( m_pModels != NULL) {
		for(int i=0; i<m_nFcsIter; i++) {
			delete[] m_pModels[i];
		}
		delete[] m_pModels;
		m_pModels = NULL;
		m_nModels = 0;
	}
}

void CInvFcs::DestroyData()
{
	if(m_pPts!=NULL) delete[] m_pPts;
	m_pPts = NULL;
}

void CInvFcs::DestroyFrwModel()
{
	if(m_pModFrw!=NULL) delete[] m_pModFrw;
	m_pModFrw = NULL;
}

//********************************************************************************
//****** 2D construction *********************************************************

CInvFcs::CInvFcs(int nx, int nz, double x0, double z0, double xsz, double zsz)
{
	m_nType = inv2D;
	m_nX = nx;
	m_nY = 1;
	m_nZ = nz;
	m_x0 = x0;
	m_y0 = x0;
	m_z0 = z0;
	m_xCs = xsz;
	m_zCs = zsz;
	m_dRegMiu = 0.5;
	NullInit();
}

CInvFcs::CInvFcs(double x0, double xLen, double z0, double zLen, int nx, int nz)
{
	m_nType = inv2D;
	m_nX = nx;
	m_nY = 1;
	m_nZ = nz;
	m_x0 = x0;
	m_y0 = x0;
	m_z0 = z0;
	m_xCs = xLen/nx;
	m_zCs = zLen/nz;	
	NullInit();
}

void CInvFcs::New2D(int nx, int nz, double x0, double z0, double xsz, double zsz)
{
	m_nType = inv2D;
	m_nX = nx;
	m_nY = 1;
	m_nZ = nz;
	m_x0 = x0;
	m_y0 = x0;
	m_z0 = z0;
	m_xCs = xsz;
	m_yCs = xsz;
	m_zCs = zsz;
	NullInit();
}

void CInvFcs::New2D(double x0, double xLen, double z0, double zLen, int nx, int nz)
{
	m_nType = inv2D;
	m_nX = nx;
	m_nY = 1;
	m_nZ = nz;
	m_x0 = x0;
	m_y0 = x0;
	m_z0 = z0;
	m_xCs = xLen/nx;
	m_yCs = m_xCs;
	m_zCs = zLen/nz;
	NullInit();
}

void CInvFcs::NullInit()
{
	m_pModels = NULL;
	m_nModels = 0;
	m_pPts = NULL;
	m_pModFrw = NULL;
	m_nPts = -1;
	m_nFormat = -1;

	//m_dPenMin = 0;
	//m_dPenMax = 200;

	m_dCgEps = 1.0e-6;
	m_nCgIter = 100;
	m_nCgRestart = 50;
	m_nFcsIter = 12;
	m_dFcsPow = 2.0;
	m_dInitModel = 0.0;

	m_nInvType = invertPrecondDiag;

	//m_nFileType = saveBin;
	m_nFileType = saveTxt;
	//m_nFileAppend = saveAppend;
	m_nFileAppend = saveSepar;

	m_bShrink = TRUE;
	m_bSaveIter = TRUE;

	pLU = NULL;
	pLUi = NULL;
	pM = NULL;
	pMt = NULL;
	ps = NULL;
	m_pPen = NULL;	
	g_pItrHistory = NULL;
	m_strFilePathTmp = "";
	m_bMatrixTmpDel = FALSE;
}
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

//********************************************************************************
//****** 3D construction *********************************************************
CInvFcs::CInvFcs(int nx, int ny, int nz, double x0, double y0, double z0, double xcs, double ycs, double zcs)
{
	m_nType = inv3D;
	m_nX = nx;
	m_nY = ny;
	m_nZ = nz;
	m_x0 = x0;
	m_y0 = y0;
	m_z0 = z0;
	m_xCs = xcs;
	m_yCs = ycs;
	m_zCs = zcs;
	//m_dPenMax = 200;
	//m_dPenMin = 0;
	m_vox.Init(m_xCs, m_yCs, m_zCs);
	NullInit();
}

CInvFcs::CInvFcs(double x0, double xLen, double y0, double yLen, double z0, double zLen, int nx, int ny, int nz)
{
	m_nType = inv3D;
	m_nX = nx;
	m_nY = ny;
	m_nZ = nz;
	m_x0 = x0;
	m_y0 = y0;
	m_z0 = z0;
	m_xCs = xLen/nx;
	m_yCs = yLen/ny;
	m_zCs = zLen/nz;
	m_vox.Init(m_xCs, m_yCs, m_zCs);
	NullInit();
}

void CInvFcs::New3D(int nx, int ny, int nz, double x0, double y0, double z0, double xcs, double ycs, double zcs)
{
	m_nType = inv3D;
	m_nX = nx;
	m_nY = ny;
	m_nZ = nz;
	m_x0 = x0;
	m_y0 = y0;
	m_z0 = z0;
	m_xCs = xcs;
	m_yCs = ycs;
	m_zCs = zcs;
	m_vox.Init(m_xCs, m_yCs, m_zCs);
	NullInit();
}

void CInvFcs::New3D(double x0, double xLen, double y0, double yLen, double z0, double zLen, int nx, int ny, int nz)
{
	m_nType = inv3D;
	m_nX = nx;
	m_nY = ny;
	m_nZ = nz;
	m_x0 = x0;
	m_y0 = y0;
	m_z0 = z0;
	m_xCs = xLen/nx;
	m_yCs = yLen/ny;
	m_zCs = zLen/nz;
	m_vox.Init(m_xCs, m_yCs, m_zCs);
	NullInit();
}

//void CInvFcs::Invert()
//{
//	if(m_nInvType==invertSmal)
//		InvertSmall();
//
//	if(m_nInvType==invertDiskMatrix)
//		InvertBig();
//
//	if(m_nInvType==invertShrink)
//		InvertShrink();
//
//	if(m_nInvType==invertPrecond)
//		InvertPrecond();
//
//	if(m_nInvType==invertPrecondShrink)
//		InvertPrecondShrink();
//}
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


//////////////////////////////////////////////////////////////////////////////////
//******* allocate/free memory ***************************************************
void CInvFcs::ForcedFree()
{
	Allocate(FALSE);
}

int CInvFcs::Allocate(BOOL bAlloc)
{
	// allocate memory
	if( bAlloc==TRUE ) {
		BOOL bRglrz = (m_nInvType==invertRegularized || m_nInvType == invertRglrzPrcnd);

		pA = new double*[nM];
		for(int i=0; i<nM; i++) {
			pA[i] = new double[nN];
		}

		//pC = new double*[nN];
		//for(int i=0; i<nN; i++) {
		//	pC[i] = new double[i+1];
		//}

		px	= new double[nN];
		pr	= new double[bRglrz ? (nM+nN) : nN];
		pd	= new double[bRglrz ? (nM+nN) : nN];
		pb	= new double[bRglrz ? (nM+nN) : nM];
		pbt = new double[bRglrz ? (nM+nN) : max(nN, nM)]; // used as teporary storage!
		pq	= new double[bRglrz ? (nM+nN) : nM];	
		pR	= new double[nN];
		pV	= new double[nN];
		pW	= new double[nN];
		pxt = new double[nN];
		if( m_nInvType==invertPrecondDiag || m_nInvType==invertRglrzPrcnd) {
			pM = new double[nN];
			pMt = new double[nN];
			ps = new double[nN];
		}
		if( m_nInvType==invertPrecondLU) {
			pLU = new double*[nN];
			for(int i=0; i<nN; i++) {
				pLU[i] = new double[nN];
			}
			ps = new double[nN];
			pLUi = new __int32[nN];
		}

		if( m_bShrink ) {
			pi = new __int32[nN];
		}
		// allocate memory for focused model storage
		ASSERT(m_pModels==NULL);
		m_pModels = new double*[m_nFcsIter];
		for(int i=0; i<m_nFcsIter; i++) {
			m_pModels[i] = new double[nN];
		}
	}
	// free memory
	else {
		if( pA != NULL) {
			for(int i=0; i<nM; i++) {
				delete[] pA[i];
			}
			delete[] pA;
			pA = NULL;
		}

		//if( pC != NULL) {
		//	for(int i=0; i<nN; i++) {
		//		delete pC[i];
		//	}
		//	delete[] pC;
		//	pC = NULL;
		//}

		if(px!=NULL){
			delete[] px;
			px = NULL;
		}
		if(pr!=NULL){
			delete[] pr;
			pr = NULL;
		}
		if(pd!=NULL){
			delete[] pd;
			pd = NULL;
		}
		if(pb!=NULL){
			delete[] pb;
			pb = NULL;
		}
		if(pbt!=NULL){
			delete[] pbt;
			pbt = NULL;
		}
		if(pq!=NULL){
			delete[] pq;
			pq = NULL;
		}
		if(pR!=NULL){
			delete[] pR;
			pR=NULL;
		}
		if(pV!=NULL){
			delete[] pV;
			pV=NULL;
		}
		if(pW!=NULL){
			delete[] pW;
			pW = NULL;
		}
		if(pxt!=NULL){
			delete[] pxt;
			pxt=NULL;
		}
		if(pM!=NULL){
			delete[] pM;
			pM = NULL;
		}
		if(pMt!=NULL){
			delete[] pMt;
			pMt = NULL;
		}
		if(ps!=NULL){
			delete[] ps;
			ps = NULL;
		}
		if (pi != nullptr) {
			delete[] pi;
			pi = nullptr;
		}
		if( pLU!=NULL ) {
			for(int i=0; i<nN; i++) {
				delete[] pLU[i];
			}
			delete[] pLU;
			pLU = NULL;
		}

		if( pLUi!=NULL ) {
			delete[] pLUi;
			pLUi = NULL;
		}
	}

	return 1;
}
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


///////////////////////////////////////////////////////////////////////////////////
//******* save/load model *********************************************************
void CInvFcs::SaveForwardModel()
{
	CString strPathName = m_strFileRoot + ".mod";

	CString str;
	//str = "Save model as '" + strPathName + "'?";
	str.Format(CString("Save model as ' %s '?"), strPathName);
	int ret = AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION);
	if(ret==IDNO) {
		CFileDialog dlg(FALSE, _T("mod"), _T("_testModel.mod"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
				_T("Inv3D model (*.mod)|*.mod||"));

		
		OPENFILENAME& ofn = dlg.GetOFN();
		CString strDlg("Save Inv3D Model");
		if(dlg.DoModal() != IDOK) 
			return;
		strPathName = dlg.GetPathName();
	}

	CFile file(strPathName, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyWrite );
	CArchive ar(&file, CArchive::store);
		
	CString strHeader("Inv3D model 0.0");
	UINT	nSize;

	ar << strHeader;
	ar << m_nType;
	ar << m_nInvType;
	//m_pMod;
	ar << m_nX << m_nY << m_nZ;
	ar << m_x0 << m_y0 << m_z0;
	ar << m_xCs << m_yCs << m_zCs;

	// CG and focusing computing inversion parameters
	ar << m_nCgIter << m_nCgRestart << m_dCgEps;
	ar << m_nFcsIter << m_dFcsPow << m_dInitModel;

	// forward model for testing
	nSize = m_nX*m_nY*m_nZ * sizeof(double);
	ar << nSize;
	ar.Write(m_pModFrw, nSize);

	ar << m_nPts << m_nPtsX << m_nPtsY << m_nFormat;
	nSize = m_nPts*m_nFormat*sizeof(double);
	ar << nSize;
	ar.Write(m_pPts, nSize);

	// penalization
	//ar << m_dPenMin << m_dPenMax;
	nSize =  m_nX*m_nY*m_nZ * sizeof(double) * 2;
	ar << nSize;
	ar.Write(m_pPen, nSize);

	// saving options
	ar << m_strFileRoot << m_strFilePathTmp;
	ar << m_nFileType;
	ar << m_nFileAppend;
	ar.Close();

	//str = "Model saved as: '" + strPathName + "'.";
	//AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
}
//_________________________________________________________________________________
void CInvFcs::LoadForwardModel()
{
	CFileDialog dlg(TRUE, _T("mod"), _T("_testModel.mod"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
			_T("Inv3D model (*.mod)|*.mod||"));

	
	OPENFILENAME& ofn = dlg.GetOFN();
	CString strDlg("Load Inv3D Model");
	if(dlg.DoModal() != IDOK) 
		return;

	CFile file(dlg.GetPathName(), CFile::modeRead );
	CArchive ar(&file, CArchive::load);
		
	CString strHeader;
	UINT	nSize;

	ar >> strHeader;
	ar >> m_nType;
	ar >> m_nInvType;
	//m_pMod;
	ar >> m_nX >> m_nY >> m_nZ;
	ar >> m_x0 >> m_y0 >> m_z0;
	ar >> m_xCs >> m_yCs >> m_zCs;

	// CG and focusing computing inversion parameters
	ar >> m_nCgIter >> m_nCgRestart >> m_dCgEps;
	ar >> m_nFcsIter >> m_dFcsPow >> m_dInitModel;

	// forward model for testing
	ar >> nSize;
	if( m_pModFrw!=NULL )
		delete[] m_pModFrw;
	m_pModFrw = new double[m_nX*m_nY*m_nZ];
	ar.Read(m_pModFrw, nSize);

	ar >> m_nPts >> m_nPtsX >> m_nPtsY >> m_nFormat;
	ar >> nSize;
	if( m_pPts!=NULL )
		delete[] m_pPts;
	m_pPts = new double[m_nPts*m_nFormat];
	ar.Read(m_pPts, nSize);

	// penalization
	//ar >> m_dPenMin >> m_dPenMax;
	ar >> nSize;
	if( m_pPen!=NULL )
		delete[] m_pPen;
	m_pPen = new double[nSize];
	ar.Read(m_pPen, nSize);

	// saving options
	ar >> m_strFileRoot >> m_strFilePathTmp;
	ar >> m_nFileType;
	ar >> m_nFileAppend;
	ar.Close();
	m_vox.Init(m_xCs, m_yCs, m_zCs);
}
//_________________________________________________________________________________
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

void CInvFcs::GetMemoryCosts(double& total, double& matrix, double& model, double& data)
{
	ASSERT(m_nPts>0);

	double dm, dn, ds;

	if(m_nType==inv2D)
		dm = m_nX*m_nZ;
	if(m_nType==inv3D)
		dm = m_nX*m_nY*m_nZ;

	dn = m_nPts;
	ds = sizeof(double);

	matrix = dm*dn* ds;

	model  = dn * ds;

	data = m_nFormat*dn * ds;

	total = matrix + data + model + ds*(5*dn + 5*(dm+dn));

}

void CInvFcs::GetMemoryCosts(int m, int n, int tag, double& total, double& matrix, double& model, double& data)
{
	double dm, dn, ds;

	dm = m;
	dn = n;
	ds = sizeof(double);

	matrix = dm*dn* ds;

	model  = dn * ds;

	data = m_nFormat*dn * ds;

	total = matrix + data + model + ds*(5*dn + 5*(dm+dn));

}

/*
	pData: pointer to array containing data
	nData: number of measured points
	nFormat: format of measured points; MUST be 4 {xi,yi,zi,vk_i, ...}
*/
void CInvFcs::SetMeasuredPts(double* pData, int nData, int nFormat)
{
	m_nPts = nData;
	m_nFormat = nFormat;
	m_pPts = new double[m_nPts*m_nFormat*sizeof(double)];

	memcpy(m_pPts, pData, m_nPts*m_nFormat*sizeof(double) );
}
//_________________________________________________________________________________

void CInvFcs::InitMatrix(double** pA, int m, int n)
{
	if(m_nType==inv2D)
		InitMatrix2D( pA, m, n);
	if(m_nType==inv3D)
		InitMatrix3D( pA, m, n);
}
//_________________________________________________________________________________

void CInvFcs::GetCellFld2D(double& fld, int i, int j, double xPt, double zPt )
{
	// i: depth row
	// j: depth col
	double x, z, r;
	x = m_x0 + j*m_xCs + m_xCs/2.0;
	z = m_z0 - i*m_zCs - m_zCs/2.0;
	r = sqrt((xPt-x)*(xPt-x) + (zPt-z)*(zPt-z));
	fld = 1/(r*r);

	//fld = 1.0e-6/( (xPt-x)*(xPt-x) + (zPt-z)*(zPt-z) );
   
	double ht, hb;
	double xl, xr;
	ht = zPt + i*m_zCs;
	hb = zPt + (i+1)*m_zCs;
	xl = m_x0 + j*m_xCs;
	xr = m_x0 + (j+1)*m_xCs;
	fld = 1.0e5*GRAVCONST*((xr-xPt)*log((xr-xPt)*(xr-xPt)+hb*hb)+2*hb*atan((xr-xPt)/hb)-
	   (xl-xPt)*log((xl-xPt)*(xl-xPt)+hb*hb)-2*hb*atan((xl-xPt)/hb)-
	   (xr-xPt)*log((xr-xPt)*(xr-xPt)+ht*ht)-2*ht*atan((xr-xPt)/ht)+
	   (xl-xPt)*log((xl-xPt)*(xl-xPt)+ht*ht)+2*ht*atan((xl-xPt)/ht));   

}

void CInvFcs::InitMatrix2D(double** pA, int m, int n)
{
	ASSERT(n==(m_nX*m_nZ));
	double xp, zp;	// measurement point coordinates
	int indx;

	for(int l=0; l<m; l++) {
		xp = m_pPts[m_nFormat*l + frmX];
		zp = m_pPts[m_nFormat*l + frmZ];
		
		// move over invert area
		for(int i=0; i<m_nZ; i++) {
			for(int j=0; j<m_nX; j++) {
				indx = i*m_nX + j;
				GetCellFld2D( pA[l][indx], i, j, xp, zp );
			}
		}
	}
}
//_________________________________________________________________________________
void CInvFcs::GetCellFld3D(double& fld, int i, int j, int k, double xPt, double yPt, double zPt)
{
	double x, y, z;

	// voxel centroid is in the center of top facet
	x = m_x0 + j*m_xCs + m_xCs/2;
	y = m_y0 + i*m_yCs + m_yCs/2;
	z = m_z0 - k*m_zCs - m_zCs;

	x = (xPt-x);
	y = (yPt-y);
	z = (zPt-z);
	fld = 0;
	m_vox.FldGrav(x, y, z, fld);
	//fld = 1.0 / ( (xPt-x)*(xPt-x) + (yPt-y)*(yPt-y) + (zPt-z)*(zPt-z) );
}

void CInvFcs::InitMatrix3D(double** pA, int m, int n)
{
	ASSERT(n == m_nX*m_nY*m_nZ);
	double xp, yp, zp;	// point coordinates
	int indx;

	g_nInit = 0;
	m_vox.Init();
	// over matrix rows
	for (int l = 0; l<m; l++) {
		xp = m_pPts[m_nFormat*l+0];
		yp = m_pPts[m_nFormat*l+1];
		zp = m_pPts[m_nFormat*l+2];

		// over 3D grid
		for(int k=0; k<m_nZ; k++) {			// depth layers
			for(int i=0; i<m_nY; i++) {		// "layer grid" rows
				for(int j=0; j<m_nX; j++) {	// "layer grid" columns
					indx = (k*m_nX*m_nY) + i*m_nX + j;
					GetCellFld3D( pA[l][indx], i, j, k, xp, yp, zp);
				} // for(j)
			} // for(i)
		} // for(k)
		// end over matrix columns
		g_nInit++;
	}	// for(l); matrix rows
	g_nInit = m;
}
//_________________________________________________________________________________
void CInvFcs::SetPenalization(int nxMin, int nxMax, int nyMin, int nyMax, int nzMin, int nzMax, double penMin, double penMax)
{
	ASSERT(m_pModFrw!=0);

	ASSERT(nxMin>=0 && nxMax<m_nX);
	ASSERT(nyMin>=0 && nyMax<m_nY);
	ASSERT(nzMin>=0 && nzMax<m_nZ);

	int indx;

	if(m_pPen==NULL) {
		m_pPen = new double[m_nX*m_nY*m_nZ*2];
	}
	// over 3D grid
	for(int k=nzMin; k<=nzMax; k++) {			// depth layers
		for(int i=nyMin; i<=nyMax; i++) {		// "layer grid" rows
			for(int j=nxMin; j<=nxMax; j++) {	// "layer grid" columns
				indx = (k*m_nX*m_nY) + i*m_nX + j;
				m_pPen[2*indx+0] = penMin;
				m_pPen[2*indx+1] = penMax;
			} // for(j)
		} // for(i)
	} // for(k)
}
//_________________________________________________________________________________



//********************************************************************************
//****** forward models for testing **********************************************
void CInvFcs::NewForwardModel2D(int nPts, double elev)
{
	//ASSERT(m_pPts==NULL);		// MUST be NULL! model should be empty
	//ASSERT(m_pModFrw==NULL);	// MUST be NULL! model should be empty

	if( m_pPts != NULL) {
		delete[] m_pPts;
		m_pPts = NULL;
	}
	m_nPts = nPts;
	m_nPtsX = nPts;
	m_nPtsY = 1;
	m_nFormat = 4;
	m_pPts = new double[m_nPts*m_nFormat];

	double dx = (m_xCs*m_nX)/(nPts-1);
	for(int i=0; i<m_nPts; i++) {
		m_pPts[i*m_nFormat+frmX] = m_x0 + i*dx;		// x coordinate
		m_pPts[i*m_nFormat+frmZ] = elev;			// z coordinate -> elevation
		m_pPts[i*m_nFormat+frmF] = 0.0;				// field
	}

	// allocate forard model
	if( m_pModFrw != NULL) {
		delete[] m_pModFrw;
	}
	m_pModFrw = new double[m_nX*m_nZ];
	ZeroVec(m_pModFrw, m_nX*m_nZ);
}

void CInvFcs::SetModel2D(int nxMin, int nxMax, int nzMin, int nzMax, double val)
{
	ASSERT(m_pModFrw!=0);

	ASSERT(nxMin>=0 && nxMax<m_nX);
	ASSERT(nzMin>=0 && nzMax<m_nZ);

	int indx;

	// over 3D grid
	for(int i=nzMin; i<=nzMax; i++) {			// depth layers
		for(int j=nxMin; j<=nxMax; j++) {	
			indx = i*m_nX + j;
			m_pModFrw[indx] = val;
		} 
	} 
}
//________________________________________________________________________________

void CInvFcs::NewForwardModel3D(int nPtx, int nPty, double elev)
{
	//ASSERT(m_pPts==NULL);	// MUST be NULL! model should be empty

	if(m_pPts!=NULL)
		delete[] m_pPts;
	if(m_pModFrw!=NULL)
		delete[] m_pModFrw;

	m_nPts = nPtx*nPty;
	m_nFormat = 4;
	m_pPts = new double[m_nPts*m_nFormat];

	int indx;
	double dx, dy;

	m_nPtsX = nPtx;
	m_nPtsY = nPty;
	dx = (m_xCs*m_nX)/(nPtx-1);
	dy = (m_yCs*m_nY)/(nPty-1);
	for(int i=0; i<nPty; i++) {
		for(int j=0; j<nPtx; j++) {
			indx = i*nPtx + j;
			m_pPts[indx*m_nFormat+frmX] = m_x0 + j*dx;
			m_pPts[indx*m_nFormat+frmY] = m_y0 + i*dy;
			m_pPts[indx*m_nFormat+frmZ] = elev;
			m_pPts[indx*m_nFormat+frmF] = 0.0;
		}
	}

	// allocate forard model
	m_pModFrw = new double[m_nX*m_nY*m_nZ];
	ZeroVec(m_pModFrw, m_nX*m_nY*m_nZ);
}

void CInvFcs::SetModel3D(int nxMin, int nxMax, int nyMin, int nyMax, int nzMin, int nzMax, double val)
{
	ASSERT(m_pModFrw!=0);

	ASSERT(nxMin>=0 && nxMax<m_nX);
	ASSERT(nyMin>=0 && nyMax<m_nY);
	ASSERT(nzMin>=0 && nzMax<m_nZ);

	int indx;

	// over 3D grid
	for(int k=nzMin; k<=nzMax; k++) {			// depth layers
		for(int i=nyMin; i<=nyMax; i++) {		// "layer grid" rows
			for(int j=nxMin; j<=nxMax; j++) {	// "layer grid" columns
				indx = (k*m_nX*m_nY) + i*m_nX + j;
				m_pModFrw[indx] = val;
			} // for(j)
		} // for(i)
	} // for(k)
}
//________________________________________________________________________________

/*
	computes forvard model from defined inversion model
	fills the measurement points
	this routines are just for testing
*/
void CInvFcs::InitForwardModel(BOOL bDestroyFrwModel)
{
	int			nM, nN;
	double**	pA;
	double*		pb;

	// init sizes
	nM = m_nPts;
	if(m_nType == inv2D)	nN = m_nX*m_nZ;
	if(m_nType == inv3D)	nN = m_nX*m_nY*m_nZ;

	pA = new double*[nM];
	for(int i=0; i<nM; i++) {
		pA[i] = new double[nN];
	}
	pb	= new double[nM];

	InitMatrix(pA, nM, nN);

	// compute forward model
	MultAxb(pA, m_pModFrw, pb, nM, nN);

	// set field values for measured poind
	for(int i=0; i<m_nPts; i++) {
		m_pPts[i*m_nFormat+frmF] = pb[i];				// field
	}

	// free memory
	for(int i=0; i<nM; i++) {
		delete[] pA[i];
	}
	delete[] pA;
	delete[] pb;
	if(bDestroyFrwModel) 
		DestroyFrwModel();
}
/*
	computes forvard model from defined inversion model
	fills the measurement points
	this routines are just for testing
	moves over all points and all cells and computes the model
	doesn't allocate extra memory
*/
void CInvFcs::InitForwardModelComp(BOOL bDestroyFrwModel)
{
	if(m_nType==inv2D)
		ComputeModel2D();
	if(m_nType==inv3D)
		ComputeModel3D();
}

void CInvFcs::ComputeModel2D()
{
	int n=m_nX*m_nZ;
	double xp, zp;	// measurement point coordinates
	double fld;
	int indx;

	// for all measurement points
	for(int l=0; l<m_nPts; l++) {
		xp = m_pPts[m_nFormat*l + frmX];
		zp = m_pPts[m_nFormat*l + frmZ];
		m_pPts[m_nFormat*l + frmF]=0.0;

		// move over invert area
		for(int i=0; i<m_nZ; i++) {
			for(int j=0; j<m_nX; j++) {
				indx = i*m_nX + j;
				if(m_pModFrw[indx] != 0.0) {
					GetCellFld2D( fld, i, j, xp, zp );
					m_pPts[m_nFormat*l + frmF] += fld * m_pModFrw[indx];
				}
			}
		}
	}
}


void CInvFcs::ComputeModel3D()
{
	int indx;
	int n = m_nX*m_nY*m_nZ;
	double xp, yp, zp;	// point coordinates
	double fld;

	m_vox.Init();
	g_nInit = 0;
	// over matrix rows
	for(int l=0; l<m_nPts; ++l) {
		xp = m_pPts[m_nFormat*l+frmX];
		yp = m_pPts[m_nFormat*l+frmY];
		zp = m_pPts[m_nFormat*l+frmZ];
		m_pPts[m_nFormat*l + frmF]=0.0;

		// over 3D grid
		for(int k=0; k<m_nZ; k++) {			// depth layers
			for(int i=0; i<m_nY; i++) {		// "layer grid" rows
				for(int j=0; j<m_nX; j++) {	// "layer grid" columns
					indx = (k*m_nX*m_nY) + i*m_nX + j;
					if(m_pModFrw[indx] != 0.0) {
						GetCellFld3D( fld, i, j, k, xp, yp, zp);
						m_pPts[m_nFormat*l + frmF] += m_pModFrw[indx]*fld;
					}
				} // for(j)
			} // for(i)
		} // for(k)
		// end over matrix columns
	}	// for(l); matrix rows
}



void CInvFcs::AddNoise(double noise)
{
	double min, max, rnd;

	min=max=m_pPts[frmF];
	for(int i=0; i<m_nPts; i++) {
		min = min(min, m_pPts[i*m_nFormat + frmF]);
		max = max(max, m_pPts[i*m_nFormat + frmF]);
	}

	noise = (max-min) * noise;
	for(int i=0; i<m_nPts; i++) {
		rnd = rand();
		rnd /= RAND_MAX;
		m_pPts[i*m_nFormat + frmF] += noise*rnd;
	}

}


int CInvFcs::FldToGrd(double** pGrd)
{
	ASSERT(m_pPts!=NULL && pGrd!=NULL);

	for(int i=0; i<m_nPtsY; i++) {
		for(int j=0; j<m_nPtsX; j++) {
			int indx = i*m_nPtsX + j;
			pGrd[i][j] = m_pPts[indx*m_nFormat+frmF];
		}
	}
	return 1;
}

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||




//********************************************************************************
//****** model saving ************************************************************
void CInvFcs::SaveIter(double* pData, int nData, int tag)
{

	CString strFilePath;
	if(tag==0) {
		strFilePath = m_strFileRoot + CString("00");
	}

	if(tag==-1) {
		strFilePath = m_strFileRoot;
	}

	if(tag>0) {
		CString strTmp;
		strTmp.Format(CString("%02d"), tag);
		strFilePath = m_strFileRoot + strTmp;
	}

	// in case of appending to file
	if(m_nFileAppend == saveAppend) {
		strFilePath = m_strFileRoot + CString("_all");
	}

	if(m_nFileType==saveTxt) {
		strFilePath += ".dat";
		CStdioFile file;
		if(tag==0 || tag==-1  || m_nFileAppend==saveSepar) {	// open new file
			file.Open( strFilePath, CFile::modeCreate   | CFile::modeWrite | CFile::typeText );
		}
		else {		// append to an existing file
			file.Open(strFilePath, CFile::modeWrite | CFile::typeText );
			file.SeekToEnd();
		}
		if(m_nType==inv2D)
			SaveModel2Dt(file, pData, tag);
		if(m_nType==inv3D)
			SaveModel3Dt(file, pData, tag);
	}

	if(m_nFileType==saveBin) {
		strFilePath += ".grd";
		CFile file;
		if(tag==0 || tag==-1 || m_nFileAppend==saveSepar) {
			// open new file
			file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite);
		}
		else {
			// append to an existing file
			file.Open(strFilePath, CFile::modeWrite);
			file.SeekToEnd();
		}
		if(m_nType==inv2D) 
			SaveModel2Db(file, pData, tag);
		if(m_nType==inv3D)
			SaveModel3Db(file, pData, tag);
	}

	if(m_nFileType==saveBinINV) {
		strFilePath += ".gri";
		CFile file;
		if(tag==0 || tag==-1 || m_nFileAppend==saveSepar) {
			// open new file
			file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite);
		}
		else {
			// append to an existing file
			file.Open( strFilePath, CFile::modeWrite);
			file.SeekToEnd();
		}
		
		SaveModelInv(file, pData, tag);
	}
}

void CInvFcs::SaveModel2Db(CFile& file, double* pData, int tag)
{
	// save 2D model as SRF 7 grid
	double min, max;

	min=max=pData[0];
	for(int i=0; i<(m_nX*m_nZ); i++) {
		min = min(min, pData[i]);
		max = max(max, pData[i]);
	}

	// header section	0x42525344 hex	
	long	nId, nSize, nVersion;
	// grid section		0x44495247 hex
	long	nGRow, nGCol;
	double	xLL, yLL, xSize, ySize, zMin, zMax, dRotation, dBlank;

	// header
	nId = 0x42525344;
	nSize = sizeof(long);
	nVersion = 1;
	file.Write(&nId, 4);
	file.Write(&nSize, 4);
	file.Write(&nVersion, 4);
	// grid section
	nId = 0x44495247;
	nSize = 8*sizeof(double) + 2*sizeof(long);
	nGRow = m_nZ;
	nGCol = m_nX;
	xLL = m_x0;
	yLL = m_z0;
	xSize = m_xCs;
	ySize = m_zCs;
	dRotation = 0.0;
	zMin = min;
	zMax = max;
	dBlank = 1.7976931348623158e+308;
	file.Write(&nId, 4);
	file.Write(&nSize, 4);
	file.Write(&nGRow, 4);		// number of rows in the grid
	file.Write(&nGCol, 4);		// number of columns in the grid
	file.Write(&xLL, 8);		// X coordinate of the lower left corner of the grid
	file.Write(&yLL, 8);		// Y coordinate of the lower left corner of the grid
	file.Write(&xSize, 8);		// spacing between adjacent nodes in the X direction  (between columns)
	file.Write(&ySize, 8);		// spacing between adjacent nodes in the Y direction (between rows)
	file.Write(&zMin, 8);		// minimum Z value within the grid
	file.Write(&zMax, 8);		// maximum Z value within the grid
	file.Write(&dRotation, 8);	// not currently used
	file.Write(&dBlank, 8);		// nodes are blanked if  greater or equal to this value
	// data section
	nId = 0x41544144;
	nSize = nGRow * nGCol * sizeof(double);
	file.Write(&nId, 4);
	file.Write(&nSize, 4);
	// fill grid data
	file.Write(pData, nSize);
	file.Close();
}

void CInvFcs::SaveModel2Dt(CStdioFile& file, double* pData, int tag)
{
	int indx;

	CString strTmp, strOut;

	strOut.Format(_T("2D model output\ndimensions: x=%d; z=%d\norigo: x0=%f; z0=%f\ncell size: cx=%f; cz=%f\nnumber od data: %d\nfocusing iteration: %d\n\n"),
		m_nX, m_nZ, m_x0, m_z0, m_xCs, m_zCs, m_nPts, tag);

	for(int i=0; i<m_nZ; i++) {
		for(int j=0; j<m_nX; j++) {
			indx = i*m_nX + j;
			strTmp.Format(_T("%f "), pData[indx]);
			strOut += strTmp;
		}
		strOut += _T("\n");
		file.WriteString(strOut);
		strOut.Empty();
	}
	file.Close();
}
//________________________________________________________________________________

void CInvFcs::SaveModel3Db(CFile& file, double* pData, int tag)
{
	SaveModelInv(file, pData, tag);
}


void CInvFcs::SaveModelInv(CFile& file, double* pData, int tag)
{
	// tag: specifies the focusing iteration
	//      if tag>0 the model is added to the file

	// write 3D grid header
	char hdr[1024];
	strcpy_s(hdr, "Inv3D grid file 1.0");

	if(tag<=0 ) {
		file.Write(hdr, 1024);
		file.Write(&m_nX, sizeof(int));
		file.Write(&m_nY, sizeof(int));
		file.Write(&m_nZ, sizeof(int));

		file.Write(&m_x0, sizeof(double));
		file.Write(&m_y0, sizeof(double));
		file.Write(&m_z0, sizeof(double));

		file.Write(&m_xCs, sizeof(double));
		file.Write(&m_yCs, sizeof(double));
		file.Write(&m_zCs, sizeof(double));

		file.Write(&m_nCgIter, sizeof(int));
		file.Write(&m_nCgRestart, sizeof(int));
		file.Write(&m_nFcsIter, sizeof(int));

		file.Write(&m_dCgEps, sizeof(double));
		file.Write(&m_dFcsPow, sizeof(double));
		file.Write(&m_dInitModel, sizeof(double));
		//file.Write(&m_dPenMin, sizeof(double));
		//file.Write(&m_dPenMax, sizeof(double));

	}
	file.Write(&tag, sizeof(int));
	UINT nSize = m_nX*m_nZ*m_nY;
	file.Write(&nSize, sizeof(UINT));

	file.Write(pData, m_nX*m_nZ*m_nY*sizeof(double));
	file.Close();
}


void CInvFcs::SaveModel3Dt(CStdioFile& file, double* pData, int tag)
{
	int indx;
	CString strOut, strTmp;

	strOut.Format(_T("3D model output\ndimensions: x=%d; y=%d; z=%d\norigo: x0=%f; y0=%f; z0=%f\ncell size: cx=%f cy=%f; cz=%f\nnumber od data: %d\nfocusing iteration: %d\n\n"),
		m_nX, m_nY, m_nZ, m_x0, m_y0, m_z0, m_xCs, m_yCs, m_zCs, m_nPts, tag);

	for(int k=0; k<m_nZ; k++) {
		for(int i=0; i<m_nY; i++) {
			for(int j=0; j<m_nX; j++) {
				indx = k*m_nX*m_nY + i*m_nX + j;
				strTmp.Format(_T("%f "), pData[indx]);
				strOut += strTmp;
			}
			strOut += "\n";
		}
		strOut += "\n";
		file.WriteString(strOut);
		strOut.Empty();
	}
	file.Close();
}
//________________________________________________________________________________

void CInvFcs::SaveFrwModel()
{
	int tag=-1;
	CString strFilePath;
	strFilePath = m_strFileRoot + _T("_frw");

	if(m_nFileType==saveTxt) {
		strFilePath += ".dat";
		CStdioFile file;
		if(tag==0 || tag==-1) {	// open new file
			file.Open( strFilePath, CFile::modeCreate   | CFile::modeWrite | CFile::typeText );
		}
		else {		// append to an existing file
			file.Open(strFilePath, CFile::modeWrite | CFile::typeText );
		}
		if(m_nType==inv2D)
			SaveModel2Dt(file, m_pModFrw, tag);
		if(m_nType==inv3D)
			SaveModel3Dt(file,  m_pModFrw, tag);
	}

	if(m_nFileType==saveBin) {
		strFilePath += ".grd";
		CFile file;
		if(tag==0 || tag==-1) {
			// open new file
			file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite);
		}
		else {
			// append to an existing file
			file.Open(strFilePath, CFile::modeWrite);
		}
		if(m_nType==inv2D) 
			SaveModel2Db(file,  m_pModFrw, tag);
		if(m_nType==inv3D)
			SaveModel3Db(file,  m_pModFrw, tag);
	}
}
//________________________________________________________________________________

void CInvFcs::SaveFrwInv3D()
{
	if( m_pModFrw == NULL ) return;

	int tag=-1;
	CString strFilePath;
	GetFilePathFrwInv(strFilePath);

	CFile file;
	// open new file
	file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite);
	SaveModelInv(file, m_pModFrw, -1);
}
//________________________________________________________________________________
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

///////////////////////////////////////////////////////////////////////////////////
void CInvFcs::SerializeMatrix(CArchive &ar)
{
	// saving code
	if( ar.IsStoring() ) {
		ar << m_nX << m_nY << m_nZ; 
		UINT nSz = (UINT)m_nX * (UINT)m_nY * (UINT)m_nZ * sizeof(double);
		for(int i=0; i<nM; i++) {
			ar.Write( pA[i], nSz );
		}
	}
	// loading code
	else {
		ar >> m_nX >> m_nY >> m_nZ; 
		UINT nSz = (UINT)m_nX * (UINT)m_nY * (UINT)m_nZ * sizeof(double);
		for(int i=0; i<nM; i++) {
			ar.Read( pA[i], nSz );
		}
	}
}

void CInvFcs::MatrixTmpSave()
{
	time_t osBinaryTime;
	time( &osBinaryTime ) ;
	CTime time( osBinaryTime );
	CString strTime = time.Format("%Y%m%d%H%M");
	m_strFilePathTmp = m_strFileRoot + _T("Mtrx_") + strTime + _T(".bin");

	CFile file;
	file.Open(m_strFilePathTmp, CFile::modeCreate | CFile::modeWrite);
	CArchive ar(&file, CArchive::store);
	SerializeMatrix(ar);
	ar.Close();
		//for(int i=0; i<nM; i++) {
		//	file.Write( pA[i], nN*sizeof(double) );
		//}
	file.Close();
}
//________________________________________________________________________________

void CInvFcs::MatrixTmpLoad()
{
	CFile file;
	if( file.Open(m_strFilePathTmp, CFile::modeRead) ) {
		CArchive ar(&file, CArchive::load);
		SerializeMatrix(ar);
		ar.Close();
		//for(int i=0; i<nM; i++) {
		//	file.Read( pA[i], nN*sizeof(double) );
		//}
		file.Close();
	}
	else
		m_strFilePathTmp = "";
}
//________________________________________________________________________________

void CInvFcs::MatrixTmpDeleteFile()
{
	if(!m_bMatrixTmpDel) return;

	CFile file;
	file.Remove(m_strFilePathTmp);
}
//________________________________________________________________________________

void CInvFcs::MatrixSaveTxt(double** A, double* b, double* mod, int m, int n)
{
	CStdioFile file;
	CString str;
	file.Open(_T("__testMatrix.dat"), CFile::modeCreate | CFile::modeWrite );
	for(int i=0; i<m; i++) {
		for(int j=0; j<n; j++) {
			str.Format(_T("%.15f\n"), A[i][j]);
			file.WriteString( (LPCTSTR)str );
		}
	}
	file.Close();

	CStdioFile fileD;
	fileD.Open(_T("__testData.dat"), CFile::modeCreate | CFile::modeWrite );
	for(int i=0; i<m; i++) {
		str.Format(_T("%.15f\n"), b[i]);
		fileD.WriteString( (LPCTSTR)str );
	}
	fileD.Close();

	CStdioFile fileM;
	fileM.Open(_T("__testModelTrue.dat"), CFile::modeCreate | CFile::modeWrite );
	for(int j=0; j<n; j++) {
		str.Format(_T("%.15f\n"), mod[j]);
		fileM.WriteString( (LPCTSTR)str );
	}
	fileM.Close();
}
//________________________________________________________________________________
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

///////////////////////////////////////////////////////////////////////////////////
//*********************************************************************************
/*
	shrinked preconditioned focusing inversion with diagonal preconditioner
	diagonal preconditioner MUST be inverted for CG
*/
void CInvFcs::Invert()
{
	int iter;	// CG iteration
	int fiter;	// focusing iteration

	// init sizes
	nM = m_nPts;
	if(m_nType == inv2D)	nN = m_nX*m_nZ;
	if(m_nType == inv3D)	nN = m_nX*m_nY*m_nZ;
	nNs = nN;

	Allocate( TRUE );

	// init variables
	g_nShrink = nN;
	g_nIter = 0;
	fiter = 0;
	if(m_strFilePathTmp!="") {
		MatrixTmpLoad();
	}
	if(m_strFilePathTmp=="") {
		InitMatrix(pA, nM, nN);
		MatrixTmpSave();
	}
	
	ZeroVec(pb, (m_nInvType==invertRegularized) ? (nM+nN) : nM);
	for(int i=0; i<nM; i++) {
		pb[i] = m_pPts[i*m_nFormat+3];
	}
	if(m_bShrink) {
		for(int i=0; i<nN; i++) {	pi[i] = i;	}
	}

////!!!!!!
//	AfxMessageBox("Remove here from code!");
//	MatrixSaveTxt(pA, pb, m_pModFrw, nM, nN);
////!!!!!!

	InvRowSumSq(pA, pV, nM, nN);	
	// zero model initialization
	if( m_dInitModel == 0.0 ) {
		ZeroVec(px, nN);
		SetVal(pW, 1.0, nN);	// init V matrix
	}
	// nonzero initialization
	else {
		SetVal(px, m_dInitModel, nN);
		VecElemMultW(pW, px, pV, nN);
		MultDiag(pA, pA, pW, nM, nN);
	}

	if( m_nInvType==invertPrecondDiag || m_nInvType==invertRglrzPrcnd ) {
		PrecondDiag(pA, pM, nM, nN);
		//CopyVec(pM, pV, nN);
		CopyVec(pMt, pM, nN);
	}
	if( m_nInvType==invertPrecondLU ) {
		m_bShrink = FALSE;
		TranspAA(pA, pLU, nM, nN);
		LU_decomp(pLU, nN, pLUi);
	}

	CopyVec(pxt, px, nN);
	clock_t start, finish;
	double  duration;
	start = clock();
	while( fiter < m_nFcsIter) {
		// compute inversion
		if( m_nInvType == invertNormal )
			CGrect(pA, nM, nNs, pxt, pb, pr, pd, pq, pbt, iter, m_dCgEps, m_nCgIter, m_nCgRestart);
	
		if( m_nInvType == invertRegularized ) {
			CGrglrz(pA, pR, nM, nNs, pxt, pb, pr, pd, pq, pbt, iter, m_dCgEps, m_nCgIter, m_nCgRestart, m_dRegMiu);
		}
		
		//if( m_nInvType == invertRglrzPrcnd ) {
		//	CGrglrz(pA, pR, nM, nNs, pxt, pb, pr, pd, pq, pbt, pM, ps, iter, m_dCgEps, m_nCgIter, m_nCgRestart, m_dRegMiu);
		//}
		
		if( m_nInvType == invertPrecondDiag ) {
			CGrect(pA, nM, nNs, pxt, pb, pr, pd, pq, pbt, pM, ps, iter, m_dCgEps, m_nCgIter, m_nCgRestart);
		}

		if( m_nInvType == invertPrecondLU ) {
			CGrectLU(pA, nM, nNs, pxt, pb, pr, pd, pq, pbt, pLU, ps, pLUi, iter, m_dCgEps, m_nCgIter, m_nCgRestart);
		}

		// stretch matrix
		if( m_bShrink )	
			VectorStretch(pxt, pi, nNs, nN);

		// weighting
		VecElemMult(px, pW, pxt, nN);						// elementwise px=pW*pxt

		// save model
		CopyVec( m_pModels[fiter], px, nN );
		if( m_bSaveIter ) SaveIter(px, nN, fiter);

		if(m_dFcsPow==2.0) {
			VecElemMultPen(pW, px, pV, nN, m_pPen);		}				// penalizing & computing weighting matrix in one step: Penalize(px, nN, m_dPenMin, m_dPenMax);	VecElemMultW(pW, px, pV, nN);
		else	{
			VecElemMultPowPen(pW, px, m_dFcsPow, pV, nN, m_pPen);	}	// penalizing & computing weighting matrix in one step: Penalize(px, nN, m_dPenMin, m_dPenMax);	VecElemMultW(pW, px, pV, nN);

		MatrixTmpLoad();
		MultDiag(pA, pA, pW, nM, nN);
		ZeroVec(pxt, nN);
		//VecElemMultInv(pxt, px, pW, nN);

		if( m_bShrink ) {
			nNs = nN;
			VectorShrinkIndex(px, pi, nNs);
			MatrixShrink(pA, pi, nM, nNs);	
			if( m_nInvType == invertPrecondDiag ) {
				CopyVec(pM, pMt, nN);
				VectorShrink(pM, pi, nNs);
			}
			VectorShrink(pxt, pi, nNs);
		}

		//--------- store iteration history here -------------------------------
		// number of iterations, norms, residual...
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		g_pItrHistory[g_nItrHistory].dTime = duration;
		g_pItrHistory[g_nItrHistory].nType = 0;
		g_pItrHistory[g_nItrHistory].nIter = fiter;
		g_pItrHistory[g_nItrHistory].dResidual = 0;
		g_nItrHistory++;
		SaveIterHistory();
		//----------------------------------------------------------------------
		fiter++;
		m_nModels++;
		g_nFiter = fiter;
		g_nShrink = nNs;
	}

	Allocate( FALSE );
	SaveIterHistory();
	MatrixTmpDeleteFile();
}
//_________________________________________________________________________________
void CInvFcs::SaveIterHistory()
{
	CStdioFile file, fileFcs;
	double timePrevFcs=0.0;
	double timePrevCG=0.0;
	double timeDif=0.0;
	CString str;
	file.Open(m_strFileRoot+"_iterHistoryCG.dat", CFile::modeCreate | CFile::modeWrite );
	fileFcs.Open(m_strFileRoot+"_iterHistoryFcs.dat", CFile::modeCreate | CFile::modeWrite );
	for(UINT i=0; i<g_nItrHistory; i++) {
		if(g_pItrHistory[i].nType == 0 ) {
			timeDif = g_pItrHistory[i].dTime - timePrevFcs;
			str.Format(_T("%d %.3f %.3f\n"), g_pItrHistory[i].nIter, g_pItrHistory[i].dTime, timeDif);
			fileFcs.WriteString( str );
			timePrevFcs = g_pItrHistory[i].dTime;
			timePrevCG = 0.0;
			file.WriteString( _T("\n") );
		}
		else {
			timeDif = g_pItrHistory[i].dTime - timePrevCG;
			str.Format(_T("%d %e %.3f %.3f \n"), g_pItrHistory[i].nIter, g_pItrHistory[i].dResidual, g_pItrHistory[i].dTime, timeDif);
			file.WriteString( (LPCTSTR)str );
			timePrevCG = g_pItrHistory[i].dTime;
		}
	}
	file.Close();
	fileFcs.Close();
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


//void CInvFcs::TestCholesky()
//{
//	ASSERT(pA!=NULL && pC!=NULL);
//
//	nM=2;
//	nN=3;
//	Allocate(TRUE);
//	pA[0][0] = 9;	pA[0][1] = 5;	pA[0][2] = 3;	
//	pA[1][0] = 3;	pA[1][1] = 4;	pA[1][2] = 6;	
//	TranspA(pA, pC, nM, nN);
//	TRACE("\n Transposed Matrix\n");
//	for(int i=0; i<nN; i++) {
//		for(int j=0; j<=i; j++) {
//			TRACE("%f ", pC[i][j]);
//		}
//		TRACE("\n");
//	}
//
//	int chol = CholesskyDC(pC, nN);
//	if( chol==0 ) 
//		AfxMessageBox("Cholesky decomposition failed.", MB_OK | MB_ICONQUESTION);
//	TRACE("\n Cholesky decomposition\n");
//	for(int i=0; i<nN; i++) {
//		for(int j=0; j<=i; j++) {
//			TRACE("%f ", pC[i][j]);
//		}
//		TRACE("\n");
//	}
//	Allocate(FALSE);
//}

void CInvFcs::GetModelRange(double& min, double& max)
{
	min = m_pModels[0][0];
	max = m_pModels[0][0];
	for(int i=0; i<m_nModels; i++) {
		for(int j=0; j<m_nX*m_nY*m_nZ; j++) {
			min = min(m_pModels[i][j], min);
			max = max(m_pModels[i][j], max);
		}
	}
}

void CInvFcs::TestLU()
{
	double* x1;

	m_nInvType = invertPrecondLU;
	nM=3;
	nN=3;
	x1 = new double[nN];

	Allocate(TRUE);
	pA[0][0] = 1.0;	pA[0][1] = 2.0;	pA[0][2] = 3.0;
	pA[1][0] = 2.0;	pA[1][1] = 2.0;	pA[1][2] = 3.0;
	pA[2][0] = 3.0;	pA[2][1] = 3.0;	pA[2][2] = 3.0;

	x1[0] = 1.0;	x1[1] = 1.0;	x1[2] = 1.0;

	LU_decomp(pA, nN, pLUi);

	printf("original\n");
	LU_solve(pA, x1, nN, pLUi);
	// output: x1[0]=0.0; x1[1]=0.0; x1[2]=0.33333; 
	Allocate(FALSE);



	nM=2;
	nN=3;
	Allocate(TRUE);
	pA[0][0] = 1.0;	pA[0][1] = 2.0;	pA[0][2] = 3.0;
	pA[1][0] = 2.0;	pA[1][1] = 2.0;	pA[1][2] = 3.0;

	x1[0] = 1.0;	x1[1] = 1.0;	x1[2] = 1.0;

	TranspAA(pA, pLU, nM, nN);
	for(int i=0; i<nN; i++) {
		for(int j=0; j<nN; j++) {
			TRACE("%f ", pLU[i][j]);
		}
		TRACE("\n");
	}

	LU_decomp(pLU, nN, pLUi);

	TRACE("\n");
	for(int i=0; i<nN; i++) {
		for(int j=0; j<nN; j++) {
			TRACE("%f ", pLU[i][j]);
		}
		TRACE("\n");
	}
	LU_solve(pLU, x1, nN, pLUi);
	// output: x1[0]=0.0; x1[1]=0.0; x1[2]=0.33333; 

	int iter;
	SetVal(pb, 1.0, nN);
	ZeroVec(px, nN);
	CGrect(pA, nM, nN, px, pb, pr, pd, pq, pxt, iter, 1.0e-12, 2, 1, 0);
	Allocate(FALSE);

}
