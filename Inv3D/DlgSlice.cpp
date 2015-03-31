// DlgSlice.cpp : implementation file
//

#include "stdafx.h"
#include "Inv3D.h"
#include "DlgSlice.h"


// CDlgSlice dialog

IMPLEMENT_DYNAMIC(CDlgSlice, CDialog)
CDlgSlice::CDlgSlice(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSlice::IDD, pParent)
	, m_nProfile(0)
	, m_nType(0)
	, m_fAlpha(0.9f)
{
}

CDlgSlice::~CDlgSlice()
{
}

void CDlgSlice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PROFILE, m_nProfile);
	DDX_Radio(pDX, IDC_TYPE, m_nType);
	DDX_Text(pDX, IDC_ALPHA, m_fAlpha);
}


BEGIN_MESSAGE_MAP(CDlgSlice, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSlice message handlers

void CDlgSlice::OnBnClickedOk()
{
	UpdateData();
	int max=0;
	switch(m_nType) {
		case 0: // xz
			max = m_nProfileMaxXZ;
			break;
		case 1: // xz
			max = m_nProfileMaxYZ;
			break;
		case 2: // xz
			max = m_nProfileMaxXY;
			break;
	}

	if(m_nProfile>max) {
		CString str;
		str.Format(_T("Profile maximum overflow! Enter value <= %d"), max);
		AfxMessageBox(str, MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	OnOK();
}
