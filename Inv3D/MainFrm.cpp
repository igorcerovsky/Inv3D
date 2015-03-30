// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Inv3D.h"

#include "MainFrm.h"
#include "Messages.h"

#include "Inv3DDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UPDATE_FOCUSING_ITER, OnUpdateFocusing)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

LRESULT CMainFrame::OnUpdateFocusing(WPARAM wParam, LPARAM lParam)
{
	CMDIChildWnd* pChildWnd;
	CInv3DDoc*	pDoc;
	CView*		pView;

	pChildWnd = (CMDIChildWnd *) MDIGetActive();
	pView = (CView *) pChildWnd->GetActiveView();
	ASSERT( pView != NULL );
	pDoc = (CInv3DDoc*) pView->GetDocument();
	ASSERT( pDoc != NULL );

	pDoc->UpdateAllViews(NULL);

	return 0;
}

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

double CMainFrame::GetReBarComboVal(int nID)
{
	CComboBox* pCmb = (CComboBox*) m_wndDlgBar.GetDlgItem(nID);

	int		nIndex;
	double a;
	CString	str;

	nIndex = pCmb->GetCurSel();
	if( nIndex == CB_ERR ) {
		pCmb->GetWindowText(str);
	}
	else {
		pCmb->GetLBText(nIndex, str);
	}

	a = _tstof(str);
	return a;
}

void CMainFrame::SetReBarComboVal(int nID, double d)
{
	CString str;
	CComboBox* pCmb = (CComboBox*) m_wndDlgBar.GetDlgItem(nID);

	str.Format(_T("%.0f"), d);
	pCmb->SetWindowText(str);
}

double CMainFrame::GetReBarEditVal(int nID)
{
	CEdit* pEdit = (CEdit*) m_wndDlgBar.GetDlgItem(nID);

	double a;
	CString	str;

	pEdit->GetWindowText(str);

	a = _tstof(str);
	return a;
}

void CMainFrame::SetReBarEditVal(int nID, double d)
{
	CString str;
	CEdit* pEdit = (CEdit*) m_wndDlgBar.GetDlgItem(nID);

	str.Format(_T("%.2f"), d);
	pEdit->SetWindowText(str);
}
