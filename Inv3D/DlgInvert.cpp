// DlgInvert.cpp : implementation file
//

#include "stdafx.h"
#include "Inv3D.h"
#include "DlgInvert.h"
#include "WinUser.h"
#include "Windows.h"
#include "Messages.h"

extern UINT g_nIter;
extern UINT g_nFiter;
extern UINT g_nShrink;
extern UINT g_nInit;
extern double g_dResid;
extern ITRHST* g_pItrHistory;
extern UINT	g_nItrHistory;

// CDlgInvert dialog

IMPLEMENT_DYNAMIC(CDlgInvert, CDialog)
CDlgInvert::CDlgInvert(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInvert::IDD, pParent)
	, m_nCgIter(200)
	, m_dCgEps(1.0e-9)
	, m_nCgRestart(1)
	, m_nFcsIter(8)
	, m_dFcsPow(2.0)
	, m_bFcsDiskStorage(FALSE)
	, m_pInvFcs(NULL)
	, m_pThread(NULL)
	, m_strFocusing(_T(""))
	, m_strCg(_T(""))
	, m_bMatrixShrink(TRUE)
	, m_nInvertType(1)
	, m_bSaveWhileIter(TRUE)
	, m_dRegMiu(0.5)
	, m_nOpacity(255)
	, m_bTranspAlpha(TRUE)
	, m_bTranspColor(FALSE)
{
}

CDlgInvert::~CDlgInvert()
{
}

void CDlgInvert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ITER_CG, m_nCgIter);
	DDX_Text(pDX, IDC_CG_EPS, m_dCgEps);
	DDX_Text(pDX, IDC_CG_RESTART, m_nCgRestart);
	DDV_MinMaxInt(pDX, m_nCgRestart, 0, 10000);
	DDX_Text(pDX, IDC_ITER_FCS, m_nFcsIter);
	DDX_Text(pDX, IDC_FCS_POW, m_dFcsPow);
	DDX_Check(pDX, IDC_DISK_MATRIX, m_bFcsDiskStorage);
	DDX_Control(pDX, IDC_PROGRESS_FOCUS, m_progressFocus);
	DDX_Control(pDX, IDC_PROGRESS_CG, m_progressCG);
	DDX_Text(pDX, IDC_STR_FOCUSING, m_strFocusing);
	DDX_Text(pDX, IDC_STR_CG, m_strCg);
	DDX_Check(pDX, IDC_MATRIX_SHRINK, m_bMatrixShrink);
	DDX_Radio(pDX, IDC_INVERT_PRECOND_DIAG, m_nInvertType);
	DDX_Check(pDX, IDC_FOCUSING_MODEL_SAVE, m_bSaveWhileIter);
	DDX_Control(pDX, IDC_PROGRESS_INIT, m_progressInit);
	DDX_Text(pDX, IDC_REG_MIU, m_dRegMiu);
	DDV_MinMaxDouble(pDX, m_dRegMiu, 0, 1);
	DDX_Text(pDX, IDC_DLGINV_OPACITY, m_nOpacity);
	DDV_MinMaxInt(pDX, m_nOpacity, 0, 255);
	DDX_Check(pDX, IDC_TRANSP_ALPHA, m_bTranspAlpha);
	DDX_Check(pDX, IDC_TRANSP_COLOR, m_bTranspColor);
}


BEGIN_MESSAGE_MAP(CDlgInvert, CDialog)
	ON_BN_CLICKED(IDC_INVERT, OnBnClickedInvert)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_DLGINV_OPACITY, OnEnChangeDlginvOpacity)
	ON_BN_CLICKED(IDC_DLGINV_OPACITY_SET, OnBnClickedDlginvOpacitySet)
END_MESSAGE_MAP()


// CDlgInvert message handlers

void CDlgInvert::OnBnClickedInvert()
{
	UpdateData();
	SaveRegistry();

	CButton* pBut = (CButton*) GetDlgItem(IDC_INVERT);
	if(m_pThread==NULL) {
		int ret = AfxMessageBox(_T("This will free memory of all previously computed models! Continue?"), MB_YESNO | MB_ICONQUESTION);
		if(ret!=IDYES) { return;	}
		m_pInvFcs->DestroyModels();
		g_nIter=0;
		g_nFiter=0;
		m_nFcsLast=0;
		pBut->SetWindowText(_T("Stop"));
		SetDlgItemText(IDC_STR_FOCUSING, _T(""));
		SetDlgItemText(IDC_STR_CG, _T(""));
		m_progressCG.SetRange32(0, m_nCgIter);
		m_progressCG.SetPos(0);
		m_progressFocus.SetRange(0, m_nFcsIter);
		m_progressFocus.SetPos(0);
		m_progressInit.SetRange32(0, m_pInvFcs->GetModelPtsNum());
		m_progressInit.SetPos(0);

		// set inversion parameters
		if(g_pItrHistory!=nullptr)
			delete [] g_pItrHistory;
		g_pItrHistory = new ITRHST[(m_nCgIter+1)*m_nFcsIter];
		g_nItrHistory = 0;

		m_pInvFcs->SetFcsIter(m_nFcsIter);
		m_pInvFcs->SetCgParam(m_nCgIter, m_nCgRestart, m_dCgEps);
		m_pInvFcs->SetRegMiu(m_dRegMiu);
		switch(m_nInvertType) {
			case 0:
				m_pInvFcs->SetInverType(CInvFcs::invertPrecondDiag);
				break;
			case 1:
				m_pInvFcs->SetInverType(CInvFcs::invertRegularized);
				break;
			case 2:
				m_pInvFcs->SetInverType(CInvFcs::invertNormal);
				break;
		}

		m_pInvFcs->SetSaveIter( m_bSaveWhileIter );
		m_pInvFcs->SetShrink( m_bMatrixShrink );
		m_pInvFcs->SetFileType( CInvFcs::saveBinINV );
		m_pInvFcs->SetFileAppend( CInvFcs::saveAppend );
		m_pInvFcs->SetFcsPow(m_dFcsPow);
		// start computing thread
		m_pThread = AfxBeginThread(ThreadFocusing, (LPVOID) m_pInvFcs);
		SetTimer(m_nTimer, 200, 0);
	}
	else {
		m_pThread->SuspendThread();
		int ret = AfxMessageBox(_T("Computation is running! Cancel?"), MB_YESNO | MB_ICONQUESTION);
		if(ret!=IDYES) {
			m_pThread->ResumeThread();
			return;
		}
		else {
			delete m_pThread;
			m_pThread=NULL;
			m_pInvFcs->ForcedFree();
			pBut->SetWindowText(_T("Invert"));
			pBut->SetWindowText(_T("Invert"));
			return;
		}
	}
}

void CDlgInvert::OnTimer(UINT nIDEvent)
{
	m_progressCG.SetPos(g_nIter);
	m_progressFocus.SetPos(g_nFiter);
	m_progressInit.SetPos(g_nInit);

	CString strReport;
	if( g_nFiter!=0 || g_nIter!=0) {
		strReport.Format(_T(" %d. iter.; shrinked to %d"), g_nFiter, g_nShrink);
		SetDlgItemText(IDC_STR_FOCUSING, strReport);
		strReport.Format(_T(" %d. iter.; residual = %.12f"), g_nIter, g_dResid);
		SetDlgItemText(IDC_STR_CG, strReport);
		SetDlgItemText(IDC_STR_INIT, _T("Model initialized. Computation startted."));
		if( m_nFcsLast < g_nFiter) {
			m_nFcsLast = g_nFiter;
			AfxGetApp()->GetMainWnd()->SendMessage(UPDATE_FOCUSING_ITER);
		}
	}
	else {
		SetDlgItemText(IDC_STR_INIT, _T("Initializing model."));
	}

	if( g_nFiter==m_nFcsIter ){
		KillTimer(m_nTimer);
		CButton* pBut = (CButton*) GetDlgItem(IDC_INVERT);
		pBut->SetWindowText(_T("Invert!"));
		m_pThread = NULL;
		AfxMessageBox(_T("Computation finished."), MB_OK | MB_ICONINFORMATION);
	}

	CDialog::OnTimer(nIDEvent);
}



void CDlgInvert::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}

void CDlgInvert::SaveRegistry(void)
{
	CString strSection = _T("Invert Dialog");
	CWinApp* pApp = AfxGetApp();

	UpdateData();
	pApp->WriteProfileBinary(strSection, _T("Matrix disk storage"), (LPBYTE)&m_bFcsDiskStorage, sizeof(m_bFcsDiskStorage));
	pApp->WriteProfileBinary(strSection, _T("Matrix shrink"), (LPBYTE)&m_bMatrixShrink, sizeof(m_bMatrixShrink));
	pApp->WriteProfileBinary(strSection, _T("Save while iteration"), (LPBYTE)&m_bSaveWhileIter, sizeof(m_bSaveWhileIter));
	pApp->WriteProfileBinary(strSection, _T("CG eps"), (LPBYTE)&m_dCgEps, sizeof(m_dCgEps));
	pApp->WriteProfileBinary(strSection, _T("CG max iter"), (LPBYTE)&m_nCgIter, sizeof(m_nCgIter));
	pApp->WriteProfileBinary(strSection, _T("CG restart"), (LPBYTE)&m_nCgRestart, sizeof(m_nCgRestart));
	pApp->WriteProfileBinary(strSection, _T("FCS power"), (LPBYTE)&m_dFcsPow, sizeof(m_dFcsPow));
	pApp->WriteProfileBinary(strSection, _T("FCS iterations"), (LPBYTE)&m_nFcsIter, sizeof(m_nFcsIter));
	pApp->WriteProfileBinary(strSection, _T("Invert type"), (LPBYTE)&m_nInvertType, sizeof(m_nInvertType));
	pApp->WriteProfileBinary(strSection, _T("Regularization param"), (LPBYTE)&m_dRegMiu, sizeof(m_dRegMiu));
}

void CDlgInvert::LoadRegistry(void)
{
	CString strSection = _T("Invert Dialog");
	CWinApp* pApp = AfxGetApp();

	UINT n;
	UINT* uData;
	BOOL* bData;
	double* dData;
	int* nData;
	pApp->GetProfileBinary(strSection, _T("Matrix disk storage"), (LPBYTE*)&bData, &n);
	if( sizeof(m_bFcsDiskStorage)==n ) {
		m_bFcsDiskStorage = *bData;
		delete[] bData;
	}

	pApp->GetProfileBinary(strSection, _T("Matrix shrink"), (LPBYTE*)&bData, &n);
	if(sizeof(m_bMatrixShrink)==n) {
		m_bMatrixShrink = *bData;
		delete[] bData;
	}

	pApp->GetProfileBinary(strSection, _T("Save while iteration"), (LPBYTE*)&bData, &n);
	if(sizeof(m_bSaveWhileIter)==n) {
		m_bSaveWhileIter = *bData;
		delete[] bData;
	}

	pApp->GetProfileBinary(strSection, _T("CG eps"), (LPBYTE*)&dData, &n);
	if(sizeof(m_dCgEps)==n) {
		m_dCgEps = *dData;
		delete[] dData;
	}

	pApp->GetProfileBinary(strSection, _T("CG max iter"), (LPBYTE*)&uData, &n);
	if(sizeof(m_nCgIter)==n) {
		m_nCgIter = *uData;
		delete[] uData;
	}

	pApp->GetProfileBinary(strSection, _T("CG restart"), (LPBYTE*)&nData, &n);
	if(sizeof(m_nCgRestart)==n) {
		m_nCgRestart = *nData;
		delete[] nData;
	}

	pApp->GetProfileBinary(strSection, _T("FCS power"), (LPBYTE*)&dData, &n);
	if(sizeof(m_dFcsPow)==n) {
		m_dFcsPow = *dData;
		delete[] dData;
	}

	pApp->GetProfileBinary(strSection, _T("Invert type"), (LPBYTE*)&nData, &n);
	if(sizeof(m_nInvertType)==n) {
		m_nInvertType = *nData;
		delete[] nData;
	}

	pApp->GetProfileBinary(strSection, _T("FCS iterations"), (LPBYTE*)&nData, &n);
	if(sizeof(m_nFcsIter)==n) {
		m_nFcsIter = *nData;
		delete[] nData;
	}

	pApp->GetProfileBinary(strSection, _T("Regularization param"), (LPBYTE*)&dData, &n);
	if(sizeof(m_dRegMiu)==n) {
		m_dRegMiu = *dData;
		delete[] dData;
	}


	UpdateData(FALSE);
}

BOOL CDlgInvert::OnInitDialog()
{
	const DWORD ANIMATION_MILLIS = 200;
	const BYTE TRANSLUCENCY = 220;
	const BYTE TRANSLUCENCY_STEP = 16;
	const DWORD TRANSLUCENCY_TIMEOUT = TRANSLUCENCY_STEP * ANIMATION_MILLIS / TRANSLUCENCY;

   HWND hwndDlg = GetSafeHwnd();
	// Make it a layered window.
	::SetWindowLong(hwndDlg, GWL_EXSTYLE, ::GetWindowLong(hwndDlg, GWL_EXSTYLE) | WS_EX_LAYERED);

	// Completely transparent window - note the third parameter
	::SetLayeredWindowAttributes(hwndDlg, 0, 0, LWA_ALPHA);

	// Show it _first_
	::ShowWindow(GetSafeHwnd(), SW_SHOW);

	// Redraw contents NOW - no flickering since the window's not visible
	::RedrawWindow(hwndDlg, NULL, NULL, RDW_UPDATENOW); 


	// Set the final translucency
	::SetLayeredWindowAttributes(hwndDlg, 0, m_nOpacity, LWA_ALPHA);

	CDialog::OnInitDialog();

	LoadRegistry();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInvert::OnEnChangeDlginvOpacity()
{
	//COLORREF clrTransparent;
	//clrTransparent = GetSysColor(COLOR_MENU);
	//m_nOpacity = GetDlgItemInt(IDC_DLGINV_OPACITY);
	//if( m_nOpacity >=0 && m_nOpacity<=255) {
	//	BYTE opacity = m_nOpacity;
	//	::SetLayeredWindowAttributes(GetSafeHwnd(), clrTransparent, opacity, LWA_ALPHA);
	//}
}

void CDlgInvert::OnBnClickedDlginvOpacitySet()
{
	UpdateData();
	
	COLORREF clrTransparent;
	clrTransparent = GetSysColor(COLOR_3DFACE);
	m_nOpacity = GetDlgItemInt(IDC_DLGINV_OPACITY);

	if( m_nOpacity>=0 && m_nOpacity<=255 && m_bTranspAlpha) {
		BYTE opacity = m_nOpacity;
		::SetLayeredWindowAttributes(GetSafeHwnd(), 0, opacity, LWA_ALPHA);
	}
	if( m_nOpacity>=0 && m_nOpacity<=255 && m_bTranspColor ) {
		BYTE opacity = m_nOpacity;
		::SetLayeredWindowAttributes(GetSafeHwnd(), clrTransparent, opacity, LWA_COLORKEY);
	}

	if( !m_bTranspColor && !m_bTranspAlpha ) {
		::SetLayeredWindowAttributes(GetSafeHwnd(), 0, 255, LWA_ALPHA);
	}
}
