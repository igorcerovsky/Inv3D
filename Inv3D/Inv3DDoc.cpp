// Inv3DDoc.cpp : implementation of the CInv3DDoc class
//

#include "stdafx.h"
#include "Inv3D.h"

#include "Inv3DDoc.h"

#include "DlgInvert.h"
#include "DlgDefineModel.h"

#include "Messages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CInv3DDoc

IMPLEMENT_DYNCREATE(CInv3DDoc, CDocument)

BEGIN_MESSAGE_MAP(CInv3DDoc, CDocument)
	ON_COMMAND(ID_MODEL_DEFINE, OnModelDefine)
	ON_COMMAND(ID_MODEL_INVERT, OnModelInvert)
	ON_UPDATE_COMMAND_UI(ID_MODEL_INVERT, OnUpdateModelInvert)
	ON_COMMAND(ID_MODEL_IMPORT, OnModelImport)
	ON_COMMAND(ID_MODEL_IMPORT_APPEND, OnModelImportAppend)
	ON_COMMAND(ID_MODEL_LOAD_FOWARD, OnModelLoadFoward)
	ON_COMMAND(ID_MODEL_SAVE_FORWARD, OnModelSaveForward)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SAVE_FORWARD, OnUpdateModelSaveForward)
END_MESSAGE_MAP()


// CInv3DDoc construction/destruction

CInv3DDoc::CInv3DDoc()
{
	m_pModel = NULL;
	m_nModels = -1;

	//!!!!
	//m_inv.TestLU();
}

CInv3DDoc::~CInv3DDoc()
{
	if (m_pModel != nullptr)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
}

BOOL CInv3DDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CInv3DDoc serialization

void CInv3DDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CInv3DDoc diagnostics

#ifdef _DEBUG
void CInv3DDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInv3DDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CInv3DDoc commands

void CInv3DDoc::OnModelDefine()
{
	CDlgDefineModel dlg;
	
	dlg.m_pInvFcs = &m_inv;

	dlg.DoModal();

	if( m_inv.IsForwardModel() ) {
		ImportModel(m_inv.GetFilePathFrwInv(), FALSE);
		m_grdFld.Close();
		int row, col, nx, ny, nz;
		double x0, y0, xSize, ySize, tmp;
		m_inv.GetModelPtsNum(col, row);
		m_inv.GetModelSize(nx, ny, nz);
		m_inv.GetCellSize(xSize, ySize, tmp);
		xSize = xSize*(nx)/(double)(col-1);
		ySize = ySize*(ny)/(double)(row-1);
		m_inv.GetOrigo(x0, y0);
		m_grdFld.New(row, col, x0, y0, xSize, ySize);
		m_inv.FldToGrd(m_grdFld.GetData());
		m_grdFld.UpdateMinMaxValue();
	}
	UpdateAllViews(NULL, UPDATE_MODEL_NEW);
}

void CInv3DDoc::OnModelInvert()
{
	CDlgInvert dlg;
	dlg.m_pInvFcs = &m_inv;

	dlg.DoModal();

	if( m_inv.IsForwardModel() ) {
		if(m_nModels>1)
			DeleteModels(1, m_nModels-1);
		ImportModel(m_inv.GetFilePathInv(), TRUE);
	}
}

void CInv3DDoc::OnUpdateModelInvert(CCmdUI *pCmdUI)
{
	if( m_inv.IsForwardModel() )	pCmdUI->Enable(TRUE);
	else							pCmdUI->Enable(FALSE);
}

int CInv3DDoc::ImportModel(CString strFilePath, BOOL bAppend)
{
	CFile file;
	file.Open(strFilePath, CFile::modeRead);

	int		tag;
	int		nTmp;
	double	dTmp;
	char	hdr[1024];

	file.Read(hdr, 1024);

	file.Read(&m_nX, sizeof(int));
	file.Read(&m_nY, sizeof(int));
	file.Read(&m_nZ, sizeof(int));

	file.Read(&m_xMin, sizeof(double));
	file.Read(&m_yMin, sizeof(double));
	file.Read(&m_zMax, sizeof(double));

	file.Read(&m_xCs, sizeof(double));
	file.Read(&m_yCs, sizeof(double));
	file.Read(&m_zCs, sizeof(double));

	file.Read(&nTmp, sizeof(int));		// CG iterations
	file.Read(&nTmp, sizeof(int));		// CG restart
	file.Read(&nTmp, sizeof(int));		// focusing iterations

	file.Read(&dTmp, sizeof(double));	// CG EPS
	file.Read(&dTmp, sizeof(double));	// focusing power
	file.Read(&dTmp, sizeof(double));	// initial model
	//file.Read(&dTmp, sizeof(double));	// penalization minimum
	//file.Read(&dTmp, sizeof(double));	// penalization maximum

	file.Read(&tag, sizeof(int));

	m_xMax = m_xMin + m_nX*m_xCs;
	if(m_nY>1)
		m_yMax = m_yMin +m_nY*m_yCs;
	else
		m_yMax = m_yMin + m_xCs;		// 2D model
	m_zMin = m_zMax - m_nZ*m_zCs;

	UINT nRead=0;
	int nModelsOld = m_nModels;
	if(!bAppend || m_nModels==-1) {
		m_nModels = 0;
		if(m_pModel != NULL) {
			for(int i=0; i<100; i++) {
				if(m_pModel[i] != NULL){
					delete[] m_pModel[i];
				}
			}
			delete[] m_pModel;
		}
		m_pModel = new double*[100];
		for(int i=0; i<100; i++)
			m_pModel[i]=NULL;
	}
	UINT nModSize = m_nX*m_nZ*m_nY;
	do {
		UINT nSize;
		if(m_nModels!=0 && m_nModels!=nModelsOld )
			file.Read(&tag, sizeof(int));
		file.Read(&nSize, sizeof(UINT));

		if(m_pModel[m_nModels] != NULL)
			delete[] m_pModel[m_nModels];

		m_pModel[m_nModels] = new double[nModSize];

		ASSERT(nSize==nModSize);
		nRead = file.Read(m_pModel[m_nModels], nSize*sizeof(double));
		if(nRead==0) {
			delete[] m_pModel[m_nModels];
			m_pModel[m_nModels] = NULL;
		}
		m_nModels++;
	} while(nRead!=0);
	m_nModels--;
	file.Close();

	UpdateAllViews(NULL, UPDATE_MODEL_NEW);
	return 0;
}

void CInv3DDoc::OnModelImport()
{
	CFileDialog fileDlg (TRUE, _T(""), _T("*.*"), OFN_HIDEREADONLY, _T(""));
	OPENFILENAME& ofn = fileDlg.GetOFN();
	CString strDlg("Import model");
	ofn.lpstrTitle = strDlg;
	if( fileDlg.DoModal() == IDOK ) {
		CString strFilePath = fileDlg.GetPathName();
		ImportModel(strFilePath, FALSE);
	}
}

void CInv3DDoc::OnModelImportAppend()
{
	CFileDialog fileDlg (TRUE, _T(""), _T("*.*"), OFN_HIDEREADONLY, _T(""));
	OPENFILENAME& ofn = fileDlg.GetOFN();
	CString strDlg("Append model");
	ofn.lpstrTitle = strDlg;
	if( fileDlg.DoModal() == IDOK ) {
		CString strFilePath = fileDlg.GetPathName();
		ImportModel(strFilePath, TRUE);
	}
}

int CInv3DDoc::DeleteModels(int first, int last)
{
	ASSERT(first>=0);
	ASSERT(last<100);
	ASSERT(last<m_nModels);

	int j=0;
	for(int i=first; i<=last; i++) {
		delete m_pModel[i];
		m_pModel[i]=NULL;
		j++;
	}
	m_nModels=1;

	return j;
}

void CInv3DDoc::OnModelLoadFoward()
{
	m_inv.LoadForwardModel();
	if( m_inv.IsForwardModel() ) {
		ImportModel(m_inv.GetFilePathFrwInv(), FALSE);
		m_grdFld.Close();
		int row, col, nx, ny, nz;
		double x0, y0, xSize, ySize, tmp;
		m_inv.GetModelPtsNum(col, row);
		m_inv.GetModelSize(nx, ny, nz);
		m_inv.GetCellSize(xSize, ySize, tmp);
		xSize = xSize*(nx)/(double)(col-1);
		ySize = ySize*(ny)/(double)(row-1);
		m_inv.GetOrigo(x0, y0);
		m_grdFld.New(row, col, x0, y0, xSize, ySize);
		m_inv.FldToGrd(m_grdFld.GetData());
		m_grdFld.UpdateMinMaxValue();
	}
	UpdateAllViews(NULL, UPDATE_MODEL_NEW);
}

void CInv3DDoc::OnModelSaveForward()
{
	m_inv.SaveForwardModel();
}

void CInv3DDoc::OnUpdateModelSaveForward(CCmdUI *pCmdUI)
{
	if( m_inv.IsForwardModel() )	pCmdUI->Enable(TRUE);
	else							pCmdUI->Enable(FALSE);
}
