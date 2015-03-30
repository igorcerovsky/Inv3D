#pragma once
#include "afxcmn.h"

#include "InvFcs.h"

// CDlgInvert dialog

class CDlgInvert : public CDialog
{
	DECLARE_DYNAMIC(CDlgInvert)

public:
	CDlgInvert(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgInvert();

// Dialog Data
	enum { IDD = IDD_DLG_INVERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CInvFcs* m_pInvFcs;
	CWinThread* m_pThread;
	CString m_strReport;
	int		m_nTimer;
	UINT	m_nCgIter;
	double	m_dCgEps;
	int		m_nCgRestart;
	int		m_nFcsIter;
	double	m_dFcsPow;
	BOOL	m_bFcsDiskStorage;
	CProgressCtrl m_progressFocus;
	CProgressCtrl m_progressCG;

public:
	afx_msg void OnBnClickedInvert();
	afx_msg void OnTimer(UINT nIDEvent);
	CString m_strFocusing;
	CString m_strCg;
	BOOL m_bMatrixShrink;
	int m_nInvertType;
	BOOL m_bSaveWhileIter;
protected:
	virtual void OnCancel();
public:
	CProgressCtrl m_progressInit;
	double m_dRegMiu;
	void SaveRegistry(void);
	void LoadRegistry(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeDlginvOpacity();
	int m_nOpacity;
	afx_msg void OnBnClickedDlginvOpacitySet();
	UINT m_nFcsLast;
	BOOL m_bTranspAlpha;
	BOOL m_bTranspColor;
};
