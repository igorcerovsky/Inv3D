// DlgDefineModel.cpp : implementation file
//

#include "stdafx.h"
#include "Inv3D.h"
#include "DlgDefineModel.h"
#include "DlgForwardModel.h"
#include "DlgPenal.h"


// CDlgDefineModel dialog

IMPLEMENT_DYNAMIC(CDlgDefineModel, CDialog)
CDlgDefineModel::CDlgDefineModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDefineModel::IDD, pParent)
	, m_nModType(1)
	, m_nX(10)
	, m_nY(10)
	, m_nZ(10)
	, m_x0(0)
	, m_y0(0)
	, m_z0(0)
	, m_xCs(10)
	, m_yCs(10)
	, m_zCs(10)
	, m_strFilePathModel(_T("a.mod"))
	, m_strFilePathTmp(_T("./"))
	, m_strMemTotal(_T(""))
	, m_strMemMatrix(_T(""))
	, m_strMemModel(_T(""))
	, m_strMemData(_T(""))
	, m_strMemOther(_T(""))
{
}

CDlgDefineModel::~CDlgDefineModel()
{
}

void CDlgDefineModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_MOD_TYPE, m_nModType);
	DDX_Text(pDX, IDC_NX, m_nX);
	DDX_Text(pDX, IDC_NY, m_nY);
	DDX_Text(pDX, IDC_NZ, m_nZ);
	DDX_Text(pDX, IDC_X0, m_x0);
	DDX_Text(pDX, IDC_Y0, m_y0);
	DDX_Text(pDX, IDC_Z0, m_z0);
	DDX_Text(pDX, IDC_XCS, m_xCs);
	DDX_Text(pDX, IDC_YCS, m_yCs);
	DDX_Text(pDX, IDC_ZCS, m_zCs);
	DDX_Text(pDX, IDC_MODEL_NAME, m_strFilePathModel);
	DDX_Text(pDX, IDC_TEMP_DIR, m_strFilePathTmp);
	DDX_Text(pDX, IDC_MEM_TOTAL, m_strMemTotal);
	DDX_Text(pDX, IDC_MEM_MATRIX, m_strMemMatrix);
	DDX_Text(pDX, IDC_MEM_MODEL, m_strMemModel);
	DDX_Text(pDX, IDC_MEM_DATA, m_strMemData);
	DDX_Text(pDX, IDC_MEM_OTHER, m_strMemOther);
}


BEGIN_MESSAGE_MAP(CDlgDefineModel, CDialog)
	ON_BN_CLICKED(IDC_CREATE_MODEL, OnBnClickedCreateModel)
	ON_BN_CLICKED(IDC_FORWARD_MODEL, OnBnClickedForwardModel)
	ON_BN_CLICKED(IDC_BROWSE_MODEL_NAME, OnBnClickedBrowseModelName)
	ON_BN_CLICKED(IDC_MOD_TYPE, OnBnClickedModType)
	ON_BN_CLICKED(IDC_MOD_TYPE_3D, OnBnClickedModType3d)
	ON_BN_CLICKED(IDC_PENALIZATION, OnBnClickedPenalization)
END_MESSAGE_MAP()

BOOL CDlgDefineModel::OnInitDialog()
{
	CDialog::OnInitDialog();

	BOOL bEnable;
	if(m_pInvFcs->IsEmpty()) {
		bEnable = FALSE;
		CWnd* pWnd = GetDlgItem(IDC_FORWARD_MODEL);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_BROWSE_MODEL_NAME);
		pWnd->EnableWindow(FALSE);
	}
	else {
		bEnable = TRUE;
		if(m_pInvFcs->GetModelType() == CInvFcs::inv2D)
			m_nModType=0;
		if(m_pInvFcs->GetModelType() == CInvFcs::inv3D)
			m_nModType=1;
		m_pInvFcs->GetFileRoot(m_strFilePathModel);
		m_pInvFcs->GetPathTmp(m_strFilePathTmp);
		UpdateData(FALSE);
		SetMemoryCosts();
	}
	if(m_nModType==0)
		Enable2D(FALSE);
	else
		Enable2D(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CDlgDefineModel message handlers

void CDlgDefineModel::OnBnClickedCreateModel()
{
	UpdateData();

	if( !m_pInvFcs->IsEmpty() ) {
		if( !DestroyModel() ) return;
	}

	CString strMod;
	// 2D model
	if(m_nModType==0) {
		strMod.Format(_T("2D Model\n   number of cells: nx=%d; nz=%d\n   origin:      x0=%.2f; z0=%.2f\n   cell sizes: csx=%.2f; csz=%.2f\n   length:     lx=%.2f; lz=%.2f\n\nCraete model?"),
			m_nX, m_nZ, m_x0, m_z0, m_xCs, m_zCs, m_x0+m_nX*m_xCs, m_z0+m_nZ*m_zCs);
		int ret = AfxMessageBox(strMod, MB_YESNO | MB_ICONQUESTION);
		if(ret != IDYES) return;
		m_pInvFcs->New2D(m_nX, m_nZ, m_x0, m_x0, m_xCs, m_zCs);
		int nx = m_nX+1;
		SetDlgItemInt(IDC_PTS_X, nx);
	}

	// 3D model
	if(m_nModType==1) {
		strMod.Format(_T("3D Model\n   number of cells: nx=%d; ny=%d; nz=%d\n   origin:      x0=%.2f; y0=%.2f; z0=%.2f\n   cell sizes: csx=%.2f; csy=%.2f; csz=%.2f\n   length:     lx=%.2f; ly=%.2f; lz=%.2f\n\nCraete model?"),
			m_nX, m_nY, m_nZ, m_x0, m_y0, m_z0, m_xCs, m_yCs, m_zCs, m_x0+m_nX*m_xCs, m_y0+m_nY*m_yCs, m_z0+m_nZ*m_zCs);
		int ret = AfxMessageBox(strMod, MB_YESNO | MB_ICONQUESTION);
		if(ret != IDYES) return;
		m_pInvFcs->New3D(m_nX, m_nY, m_nZ, m_x0, m_y0, m_z0, m_xCs, m_yCs, m_zCs);
		int nx = m_nY+1;
		int ny = m_nY+1;
		SetDlgItemInt(IDC_PTS_X, nx);
		SetDlgItemInt(IDC_PTS_Y, ny);
	}

	CWnd* pWnd = GetDlgItem(IDC_FORWARD_MODEL);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_BROWSE_MODEL_NAME);
	pWnd->EnableWindow(TRUE);

	SetDlgItemText(IDC_MEM_DATA,  _T(""));
	SetDlgItemText(IDC_MEM_MODEL, _T(""));
	SetDlgItemText(IDC_MEM_MATRIX,_T(""));
	SetDlgItemText(IDC_MEM_TOTAL, _T(""));
	SetDlgItemText(IDC_MEM_OTHER, _T(""));

	AfxMessageBox(_T("New model has been created."), MB_OK | MB_ICONINFORMATION);
}

void CDlgDefineModel::OnBnClickedForwardModel()
{
	CDlgForwardModel dlg;
	dlg.m_pInvFcs = m_pInvFcs;
	dlg.DoModal();

	// compute memory costs
	if( !m_pInvFcs->IsEmptyData() ) {
		SetMemoryCosts();
	}
}

void CDlgDefineModel::OnBnClickedBrowseModelName()
{
	CFileDialog fileDlg (FALSE, _T(""), _T("*.*"), OFN_HIDEREADONLY, _T(""), this);
	OPENFILENAME& ofn = fileDlg.GetOFN();
	CString strDlg("Specify model root name");
	ofn.lpstrTitle = strDlg;
	if( fileDlg.DoModal() == IDOK ) {
		CString strExt = fileDlg.GetFileExt();
		CString strName = fileDlg.GetFileName();
		m_strFilePathModel = fileDlg.GetPathName();
		if( !strExt.IsEmpty()) {
			m_strFilePathModel.Replace( _T(".")+strExt, _T(""));
		}
		if( !strName.IsEmpty()) {
			m_strFilePathTmp.Replace( _T("\\")+strName, _T(""));
		}
		m_pInvFcs->SetFileRoot( m_strFilePathModel );
		m_pInvFcs->SetPathTmp( m_strFilePathModel );
		SetDlgItemText(IDC_MODEL_NAME, m_strFilePathModel);
		SetDlgItemText(IDC_TEMP_DIR, m_strFilePathModel);
	}
}

void CDlgDefineModel::OnBnClickedModType()
{
	int old = m_nModType;
	UpdateData();
	if(m_nModType==0) 
		Enable2D(FALSE);

	if(old != m_nModType && !m_pInvFcs->IsEmpty() ) {
		DestroyModel();
	}
}

void CDlgDefineModel::OnBnClickedModType3d()
{
	int old = m_nModType;
	UpdateData();
	if(m_nModType==1) 
		Enable2D(TRUE);

	if(old != m_nModType && !m_pInvFcs->IsEmpty() ) {
		DestroyModel();
	}
}

int CDlgDefineModel::DestroyModel()
{
	int ret = AfxMessageBox(_T("Model is not empty. Replace?"), MB_YESNO | MB_ICONQUESTION);
	if( ret != IDYES ) return 0;
	m_pInvFcs->Destroy();

	SetDlgItemText(IDC_MEM_DATA,  _T(""));
	SetDlgItemText(IDC_MEM_MODEL, _T(""));
	SetDlgItemText(IDC_MEM_MATRIX,_T(""));
	SetDlgItemText(IDC_MEM_TOTAL, _T(""));
	SetDlgItemText(IDC_MEM_OTHER, _T(""));

	AfxMessageBox(_T("Old model is destroyed."), MB_OK | MB_ICONINFORMATION);
	return 1;
}

void CDlgDefineModel::Enable2D(int bEnable)
{
	CWnd* pWnd;

	pWnd = GetDlgItem(IDC_NY);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_Y0);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_YCS);
	pWnd->EnableWindow(bEnable);
}

void CDlgDefineModel::SetMemoryCosts()
{
	// set the memory costs
	CString strTmp;
	double d1, d2, d3;
	double total, matrix, model, data, other;

	m_pInvFcs->GetMemoryCosts(total, matrix, model, data);

	d1 = data/1.0e3;
	d2 = data/1.0e6;
	d3 = data/1.0e9;
	if( d3>1)
		m_strMemData.Format(_T("%6.6f GB;  %6.6f MB;  %9.3f kB;"), d3, d2, d1);
	else
		m_strMemData.Format(_T("%6.6f MB;  %9.3f kB;  %f B;"), d2, d1, data);
	SetDlgItemText(IDC_MEM_DATA, m_strMemData);

	d1 = model/1.0e3;
	d2 = model/1.0e6;
	d3 = model/1.0e9;
	if( d3>1)
		m_strMemModel.Format(_T("%6.6f GB;  %6.6f MB;  %9.3f kB;  %f B;"), d3, d2, d1, model);
	else
		m_strMemModel.Format(_T("%6.6f MB;  %9.3f kB;  %f B;"), d2, d1, model);
	SetDlgItemText(IDC_MEM_MODEL, m_strMemModel);

	d1 = matrix/1.0e3;
	d2 = matrix/1.0e6;
	d3 = matrix/1.0e9;
	if( d3>1)
		m_strMemMatrix.Format(_T("%6.6f GB;  %6.6f MB;  %9.3f kB;"), d3, d2, d1);
	else
		m_strMemMatrix.Format(_T("%6.6f MB;  %9.3f kB;  %f B;"), d2, d1, matrix);
	SetDlgItemText(IDC_MEM_MATRIX, m_strMemMatrix);

	d1 = total/1.0e3;
	d2 = total/1.0e6;
	d3 = total/1.0e9;
	if( d3>1)
		m_strMemTotal.Format(_T("%6.6f GB;  %6.6f MB;  %9.3f kB;"), d3, d2, d1);
	else
		m_strMemTotal.Format(_T("%6.6f MB;  %9.3f kB;  %f B;"), d2, d1, total);
	SetDlgItemText(IDC_MEM_TOTAL, m_strMemTotal);

	other = total-matrix-model-data;
	d1 = other/1.0e3;
	d2 = other/1.0e6;
	d3 = other/1.0e9;
	if( d3>1)
		m_strMemOther.Format(_T("%6.6f GB;  %6.6f MB;  %9.3f kB;"), d3, d2, d1);
	else
		m_strMemOther.Format(_T("%6.6f MB;  %9.3f kB;  %f B;"), d2, d1, other);
	SetDlgItemText(IDC_MEM_OTHER, m_strMemOther);
}




void CDlgDefineModel::OnOK()
{
	UpdateData();

	if(m_strFilePathModel.IsEmpty()) {
		AfxMessageBox(_T("Specify model root name!"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	if(m_pInvFcs->IsEmptyPenal()) {
		AfxMessageBox(_T("Model penalization is not initialized!/n Specify penalization first!"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	if(m_pInvFcs->IsForwardModel()) {
		m_pInvFcs->SaveFrwInv3D();
	}

	CDialog::OnOK();
}


void CDlgDefineModel::OnBnClickedPenalization()
{
	if(m_pInvFcs->IsEmptyData()==TRUE) {
		AfxMessageBox(_T("Import data or create forward model!"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	CDlgPenal dlg;

	dlg.m_pInvFcs = m_pInvFcs;
	dlg.DoModal();
}
