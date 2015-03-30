#pragma once

#include "InvFcs.h"

// CDlgPenal dialog

class CDlgPenal : public CDialog
{
	DECLARE_DYNAMIC(CDlgPenal)

public:
	CDlgPenal(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPenal();

// Dialog Data
	enum { IDD = IDD_DLG_PENAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CInvFcs* m_pInvFcs;
	int m_nMinX;
	int m_nMinY;
	int m_nMinZ;
	int m_nMaxX;
	int m_nMaxY;
	int m_nMaxZ;

	int m_xMax;
	int m_yMax;
	int m_zMax;

	double m_dPenMin;
	double m_dPenMax;
	afx_msg void OnBnClickedSetPenval();
	void SaveRegistry(void);
	void LoadRegistry(void);
	afx_msg void OnBnClickedWholeModel();
	virtual BOOL OnInitDialog();
};
