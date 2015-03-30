#pragma once

#include "InvFcs.h"

// CDlgForwardModel dialog

class CDlgForwardModel : public CDialog
{
	DECLARE_DYNAMIC(CDlgForwardModel)

public:
	CDlgForwardModel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgForwardModel();

// Dialog Data
	enum { IDD = IDD_DLG_FORWARD_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	// model
	CInvFcs* m_pInvFcs;

	virtual BOOL OnInitDialog();
	int m_nPtsX;
	int m_nPtsY;
	double m_dPtsElev;
	double m_dPtsNoise;
	int m_nMinX;
	int m_nMinY;
	int m_nMinZ;
	int m_nMaxX;
	int m_nMaxY;
	int m_nMaxZ;
	double m_dModVal;
	afx_msg void OnBnClickedSetModel();
	afx_msg void OnBnClickedInitMod();
	afx_msg void OnBnClickedAddNoise();
	afx_msg void OnBnClickedCreateFrwModel();
};
