// DlgSliceManager.cpp : implementation file
//

#include "stdafx.h"
#include "Inv3D.h"
#include "DlgSliceManager.h"
#include "DlgSlice.h"


// CDlgSliceManager dialog

IMPLEMENT_DYNAMIC(CDlgSliceManager, CDialog)
CDlgSliceManager::CDlgSliceManager(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSliceManager::IDD, pParent)
{
}

CDlgSliceManager::~CDlgSliceManager()
{
}

void CDlgSliceManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CDlgSliceManager, CDialog)
	ON_BN_CLICKED(IDC_PROPERTIES, OnBnClickedProperties)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
	ON_LBN_DBLCLK(IDC_LIST, OnLbnDblclkList)
	ON_BN_CLICKED(IDC_CURRENT, OnBnClickedCurrent)
END_MESSAGE_MAP()


// CDlgSliceManager message handlers

BOOL CDlgSliceManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i=0; i<(int)m_pSlices->GetSize(); i++) {
		CSlice3D* pSlc;
		pSlc = m_pSlices->GetAt(i);
		CString str;
		str.Format(_T("Slice %d on profile %d"), pSlc->nType, pSlc->nProf);
		m_list.AddString(str);
	}

	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSliceManager::OnBnClickedProperties()
{
	int nSel = m_list.GetCurSel();

	SliceProperties(nSel);
}

void CDlgSliceManager::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
}

void CDlgSliceManager::OnBnClickedDelete()
{
	int nSel = m_list.GetCurSel();

	if( nSel>m_pSlices->GetCount() || nSel<0 ) {
		AfxMessageBox(_T("Invalid selection!"));
		return;
	}

	CSlice3D* pSlice = m_pSlices->GetAt(nSel);
	m_pSlices->RemoveAt(nSel);
	delete pSlice;

	if(m_nSliceSel > m_pSlices->GetUpperBound())
		m_nSliceSel = (int)m_pSlices->GetUpperBound();

}

void CDlgSliceManager::OnLbnDblclkList()
{
	int nSel = m_list.GetCurSel();

	SliceProperties(nSel);
}

void CDlgSliceManager::SliceProperties(int nSlice)
{
	if( nSlice>m_pSlices->GetCount() ) {
		AfxMessageBox(_T("Invalid selection!"));
		return;
	}

	CSlice3D* pSlice = m_pSlices->GetAt(nSlice);

	CDlgSlice dlg;

	dlg.m_nProfileMaxXY = m_nProfileMaxXY;
	dlg.m_nProfileMaxXZ = m_nProfileMaxXZ;
	dlg.m_nProfileMaxYZ = m_nProfileMaxYZ;
	dlg.m_nProfile = pSlice->GetProf();
	dlg.m_nType = pSlice->GetType();
	dlg.m_fAlpha = pSlice->GetAlpha();
	if( dlg.DoModal() == IDOK ) {
		pSlice->SetProf( dlg.m_nProfile );
		pSlice->SetAlpha( dlg.m_fAlpha );
		switch( dlg.m_nType ) {
			case 0:
				pSlice->SetType( CSlice3D::sliceXZ );
				break;
			case 1:
				pSlice->SetType( CSlice3D::sliceYZ );
				break;
			case 2:
				pSlice->SetType( CSlice3D::sliceXY );
				break;
		}
	}
}
void CDlgSliceManager::OnBnClickedCurrent()
{
	int nSel = m_list.GetCurSel();
	if( nSel > m_pSlices->GetCount() ) {
		AfxMessageBox(_T("Invalid selection!"));
		return;
	}

	m_nSliceSel = nSel;
	CSlice3D* pSlc = m_pSlices->GetAt(nSel);
	CString	str;
	str.Format(_T("Selected slice is: slice %d on profile %d"), pSlc->nType, pSlc->nProf);
	AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
}
