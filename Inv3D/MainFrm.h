// MainFrm.h : interface of the CMainFrame class
//


#pragma once
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar      m_wndDlgBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	void	SetReBarComboVal(int nID, double d);
	double	GetReBarComboVal(int nID);
	void	SetReBarEditVal(int nID, double d);
	double	GetReBarEditVal(int nID);

	afx_msg LRESULT OnUpdateFocusing(WPARAM wParam, LPARAM lParam);

};



