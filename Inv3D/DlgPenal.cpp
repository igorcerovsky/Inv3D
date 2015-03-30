// DlgPenal.cpp : implementation file
//

#include "stdafx.h"
#include "Inv3D.h"
#include "DlgPenal.h"


// CDlgPenal dialog

IMPLEMENT_DYNAMIC(CDlgPenal, CDialog)
CDlgPenal::CDlgPenal(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPenal::IDD, pParent)
	, m_nMinX(0)
	, m_nMinY(0)
	, m_nMinZ(0)
	, m_nMaxX(0)
	, m_nMaxY(0)
	, m_nMaxZ(0)
	, m_dPenMin(0)
	, m_dPenMax(0)
{
}

CDlgPenal::~CDlgPenal()
{
}

void CDlgPenal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_X_MIN, m_nMinX);
	DDX_Text(pDX, IDC_Y_MIN, m_nMinY);
	DDX_Text(pDX, IDC_Z_MIN, m_nMinZ);
	DDX_Text(pDX, IDC_X_MAX, m_nMaxX);
	DDX_Text(pDX, IDC_Y_MAX, m_nMaxY);
	DDX_Text(pDX, IDC_Z_MAX, m_nMaxZ);
	DDX_Text(pDX, IDC_PEN_MIN, m_dPenMin);
	DDX_Text(pDX, IDC_PEN_MAX, m_dPenMax);
}


BEGIN_MESSAGE_MAP(CDlgPenal, CDialog)
	ON_BN_CLICKED(IDC_SET_PENVAL, OnBnClickedSetPenval)
	ON_BN_CLICKED(IDC_WHOLE_MODEL, OnBnClickedWholeModel)
END_MESSAGE_MAP()


// CDlgPenal message handlers
void CDlgPenal::OnBnClickedWholeModel()
{
	int nX, nY, nZ;
	m_pInvFcs->GetModelSize(nX, nY, nZ);
	m_nMinX = 0;
	m_nMinY = 0;
	m_nMinZ = 0;
	m_nMaxX = nX-1;
	m_nMaxY = nY-1;
	m_nMaxZ = nZ-1;

	UpdateData(FALSE);
}

void CDlgPenal::OnBnClickedSetPenval()
{
	int nX, nY, nZ;
	m_pInvFcs->GetModelSize(nX, nY, nZ);

	UpdateData();

	if(m_nMinX<0 || m_nMinX>m_nMaxX || m_nMaxX>(nX-1)) {
		AfxMessageBox(_T("Invalid X range! Canceling."), MB_OK );
		return;
	}

	if(m_nMinY<0 || m_nMinY>m_nMaxY || m_nMaxY>(nY-1)) {
		AfxMessageBox(_T("Invalid Y range! Canceling."), MB_OK );
		return;
	}

	if(m_nMinZ<0 || m_nMinZ>m_nMaxZ || m_nMaxZ>(nZ-1)) {
		AfxMessageBox(_T("Invalid Z range! Canceling."), MB_OK );
		return;
	}
	m_pInvFcs->SetPenalization(m_nMinX, m_nMaxX, m_nMinY, m_nMaxY, m_nMinZ, m_nMaxZ, m_dPenMin, m_dPenMax );

	AfxMessageBox(_T("Penalization values were set."), MB_OK | MB_ICONINFORMATION);
}

void CDlgPenal::SaveRegistry(void)
{
	CString strSection = _T("Penalization Dialog");
	CWinApp* pApp = AfxGetApp();

	UpdateData();
	pApp->WriteProfileBinary(strSection, _T("Penalization min"), (LPBYTE)&m_dPenMin, sizeof(m_dPenMin));
	pApp->WriteProfileBinary(strSection, _T("Penalization max"), (LPBYTE)&m_dPenMax, sizeof(m_dPenMax));

	pApp->WriteProfileBinary(strSection, _T("x min"), (LPBYTE)&m_nMinX, sizeof(m_nMinX));
	pApp->WriteProfileBinary(strSection, _T("x max"), (LPBYTE)&m_nMaxX, sizeof(m_nMaxX));

	pApp->WriteProfileBinary(strSection, _T("y min"), (LPBYTE)&m_nMinY, sizeof(m_nMinY));
	pApp->WriteProfileBinary(strSection, _T("y max"), (LPBYTE)&m_nMaxY, sizeof(m_nMaxY));

	pApp->WriteProfileBinary(strSection, _T("z min"), (LPBYTE)&m_nMinZ, sizeof(m_nMinZ));
	pApp->WriteProfileBinary(strSection, _T("z max"), (LPBYTE)&m_nMaxZ, sizeof(m_nMaxZ));
}

void CDlgPenal::LoadRegistry(void)
{
	UINT n;
	int* nData;
	double* dData;
	CString strSection = _T("Penalization Dialog");
	CWinApp* pApp = AfxGetApp();

	pApp->GetProfileBinary(strSection, _T("Penalization min"), (LPBYTE*)&dData, &n);
	if(sizeof(m_dPenMin)==n) {
		m_dPenMin = *dData;
		delete[] dData;
	}

	pApp->GetProfileBinary(strSection, _T("Penalization max"), (LPBYTE*)&dData, &n);
	if(sizeof(m_dPenMax)==n) {
		m_dPenMax = *dData;
		delete[] dData;
	}

	pApp->GetProfileBinary(strSection, _T("x min"), (LPBYTE*)&nData, &n);
	if(sizeof(m_nMinX)==n) {
		m_nMinX = *nData;
		delete[] nData;
	}
	pApp->GetProfileBinary(strSection, _T("x max"), (LPBYTE*)&nData, &n);
	if(sizeof(m_nMaxX)==n) {
		m_nMaxX = *nData;
		delete[] nData;
	}

	pApp->GetProfileBinary(strSection, _T("y min"), (LPBYTE*)&nData, &n);
	if(sizeof(m_nMinY)==n) {
		m_nMinY = *nData;
		delete[] nData;
	}
	pApp->GetProfileBinary(strSection, _T("y max"), (LPBYTE*)&nData, &n);
	if(sizeof(m_nMaxY)==n) {
		m_nMaxY = *nData;
		delete[] nData;
	}

	pApp->GetProfileBinary(strSection, _T("z min"), (LPBYTE*)&nData, &n);
	if(sizeof(m_nMinZ)==n) {
		m_nMinZ = *nData;
		delete[] nData;
	}
	pApp->GetProfileBinary(strSection, _T("z max"), (LPBYTE*)&nData, &n);
	if(sizeof(m_nMaxZ)==n) {
		m_nMaxZ = *nData;
		delete[] nData;
	}

	UpdateData(FALSE);
}

BOOL CDlgPenal::OnInitDialog()
{
	CDialog::OnInitDialog();

	int nX, nY, nZ;
	m_pInvFcs->GetModelSize(nX, nY, nZ);
	m_nMaxX = nX-1;
	m_nMaxY = nY-1;
	m_nMaxZ = nZ-1;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
