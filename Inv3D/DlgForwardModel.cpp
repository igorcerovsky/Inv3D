// DlgForwardModel.cpp : implementation file
//

#include "stdafx.h"
#include "Inv3D.h"
#include "DlgForwardModel.h"


// CDlgForwardModel dialog

IMPLEMENT_DYNAMIC(CDlgForwardModel, CDialog)
CDlgForwardModel::CDlgForwardModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgForwardModel::IDD, pParent)
	, m_nPtsX(0)
	, m_nPtsY(0)
	, m_dPtsElev(10)
	, m_dPtsNoise(2)
	, m_nMinX(4)
	, m_nMinY(4)
	, m_nMinZ(2)
	, m_nMaxX(6)
	, m_nMaxY(6)
	, m_nMaxZ(3)
	, m_dModVal(1000)
{
}

CDlgForwardModel::~CDlgForwardModel()
{
}

void CDlgForwardModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PTS_X, m_nPtsX);
	DDX_Text(pDX, IDC_PTS_Y, m_nPtsY);
	DDX_Text(pDX, IDC_PTS_ELEV, m_dPtsElev);
	DDX_Text(pDX, IDC_PTS_NOISE, m_dPtsNoise);
	DDX_Text(pDX, IDC_X_MIN, m_nMinX);
	DDX_Text(pDX, IDC_Y_MIN, m_nMinY);
	DDX_Text(pDX, IDC_Z_MIN, m_nMinZ);
	DDX_Text(pDX, IDC_X_MAX, m_nMaxX);
	DDX_Text(pDX, IDC_Y_MAX, m_nMaxY);
	DDX_Text(pDX, IDC_Z_MAX, m_nMaxZ);
	DDX_Text(pDX, IDC_MOD_VAL, m_dModVal);
}


BEGIN_MESSAGE_MAP(CDlgForwardModel, CDialog)
	ON_BN_CLICKED(IDC_SET_MODEL, OnBnClickedSetModel)
	ON_BN_CLICKED(IDC_INIT_MOD, OnBnClickedInitMod)
	ON_BN_CLICKED(IDC_ADD_NOISE, OnBnClickedAddNoise)
	ON_BN_CLICKED(IDC_CREATE_FRW_MODEL, OnBnClickedCreateFrwModel)
END_MESSAGE_MAP()


// CDlgForwardModel message handlers

BOOL CDlgForwardModel::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pInvFcs->GetModelType() == CInvFcs::inv2D) {
		CWnd* pWnd = GetDlgItem(IDC_Y_MIN);
		pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_Y_MAX);
		pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_PTS_Y);
		pWnd->EnableWindow(FALSE);
	}

	if(m_pInvFcs->IsEmptyData()) {
		CWnd* pWnd = GetDlgItem(IDC_ADD_NOISE);
		pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_SET_MODEL);
		pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_INIT_MOD);
		pWnd->EnableWindow(FALSE);

	}
	else {
	}

	if(m_pInvFcs->GetModelType() == CInvFcs::inv2D) {
		m_pInvFcs->GetModelSize(m_nPtsX, m_nPtsY);
	}
	if(m_pInvFcs->GetModelType() == CInvFcs::inv3D) {
		int nz;
		m_pInvFcs->GetModelSize(m_nPtsX, m_nPtsY, nz);
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgForwardModel::OnBnClickedCreateFrwModel()
{
	UpdateData();

	int m, n;
	int nX, nY, nZ;
	int nModType = m_pInvFcs->GetModelType();
	m_pInvFcs->GetModelSize(nX, nY, nZ);
	if(nModType==CInvFcs::inv2D) {
		m = m_nPtsX;
		n = nX*nZ;
		m_pInvFcs->NewForwardModel2D(m, m_dPtsElev);
	}
	if(nModType==CInvFcs::inv3D) {
		m = m_nPtsX*m_nPtsY;
		n = nX*nY*nZ;
		m_pInvFcs->NewForwardModel3D(m_nPtsX, m_nPtsY, m_dPtsElev);
	}
	CWnd* pWnd = GetDlgItem(IDC_SET_MODEL);
	pWnd->EnableWindow(TRUE);

}

void CDlgForwardModel::OnBnClickedSetModel()
{
	int nX, nY, nZ;
	int nModType = m_pInvFcs->GetModelType();
	m_pInvFcs->GetModelSize(nX, nY, nZ);

	UpdateData();

	if(m_nMinX<0 || m_nMinX>m_nMaxX || m_nMaxX>(nX-1)) {
		AfxMessageBox(_T("Invalid X range! Canceling."), MB_OK );
		return;
	}

	if(nModType==CInvFcs::inv3D) {
		if(m_nMinY<0 || m_nMinY>m_nMaxY || m_nMaxY>(nY-1)) {
			AfxMessageBox(_T("Invalid Y range! Canceling."), MB_OK );
			return;
		}
	}

	if(m_nMinZ<0 || m_nMinZ>m_nMaxZ || m_nMaxZ>(nZ-1)) {
		AfxMessageBox(_T("Invalid Z range! Canceling."), MB_OK );
		return;
	}

	if(nModType==CInvFcs::inv2D)
		m_pInvFcs->SetModel2D(m_nMinX, m_nMaxX, m_nMinZ, m_nMaxZ, m_dModVal);

	if(nModType==CInvFcs::inv3D)
		m_pInvFcs->SetModel3D(m_nMinX, m_nMaxX, m_nMinY, m_nMaxY, m_nMinZ, m_nMaxZ, m_dModVal);

	CWnd* pWnd = GetDlgItem(IDC_INIT_MOD);
	pWnd->EnableWindow(TRUE);

	AfxMessageBox(_T("New model values were set."), MB_OK | MB_ICONINFORMATION);
}

void CDlgForwardModel::OnBnClickedInitMod()
{
	m_pInvFcs->InitForwardModelComp();
	CWnd* pWnd = GetDlgItem(IDC_ADD_NOISE);
	pWnd->EnableWindow();
	AfxMessageBox(_T("Forward model is computed."), MB_OK | MB_ICONINFORMATION);
}

void CDlgForwardModel::OnBnClickedAddNoise()
{
	UpdateData();
	int ret = AfxMessageBox(_T("All the original data will be lost! Continue?"), MB_YESNO | MB_ICONQUESTION);
	if( ret!=IDYES ) return;
	m_pInvFcs->AddNoise( m_dPtsNoise/100.0 );
	AfxMessageBox(_T("Data are noisy."), MB_OK | MB_ICONINFORMATION);
}

