#pragma once


// CDlgSlice dialog

class CDlgSlice : public CDialog
{
	DECLARE_DYNAMIC(CDlgSlice)

public:
	CDlgSlice(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSlice();

// Dialog Data
	enum { IDD = IDD_DLG_SLICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nProfile;
	int m_nProfileMaxXZ;
	int m_nProfileMaxYZ;
	int m_nProfileMaxXY;
	int m_nType;
	float m_fAlpha;
	afx_msg void OnBnClickedOk();
};
