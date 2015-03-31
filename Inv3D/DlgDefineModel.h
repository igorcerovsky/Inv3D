#pragma once

#include "InvFcs.h"
// CDlgDefineModel dialog

class CDlgDefineModel : public CDialog
{
	DECLARE_DYNAMIC(CDlgDefineModel)

public:
	CDlgDefineModel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDefineModel();

	int DestroyModel();
	void Enable2D(int bEnable);

// Dialog Data
	enum { IDD = IDD_DLG_DEFINE_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_nModType;
	int m_nX;
	int m_nY;
	int m_nZ;
	double m_x0;
	double m_y0;
	double m_z0;
	double m_xCs;
	double m_yCs;
	double m_zCs;
	afx_msg void OnBnClickedCreateModel();
	afx_msg void OnBnClickedForwardModel();
	afx_msg void OnBnClickedBrowseModelName();
	CString m_strFilePathModel;
	CString m_strFilePathTmp;

	// model
	CInvFcs* m_pInvFcs;
	CString m_strMemTotal;
	CString m_strMemMatrix;
	CString m_strMemModel;
	CString m_strMemData;
	CString m_strMemOther;
	afx_msg void OnBnClickedModType();
	afx_msg void OnBnClickedModType3d();
	virtual BOOL OnInitDialog();
	void SetMemoryCosts();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedPenalization();
	afx_msg void OnBnClickedBrowseModel();
};
