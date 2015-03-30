#pragma once

#include "Slice3D.h"
#include "afxwin.h"

// CDlgSliceManager dialog

class CDlgSliceManager : public CDialog
{
	DECLARE_DYNAMIC(CDlgSliceManager)

public:
	CDlgSliceManager(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSliceManager();

// Dialog Data
	enum { IDD = IDD_DLG_SLICE_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	SlcPtrArray* m_pSlices;
	afx_msg void OnBnClickedProperties();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	CListBox m_list;
	afx_msg void OnLbnDblclkList();
	void SliceProperties(int nSlice);

	int m_nProfileMaxXZ;
	int m_nProfileMaxYZ;
	int m_nProfileMaxXY;

	int m_nSliceSel;	
	afx_msg void OnBnClickedCurrent();
};
