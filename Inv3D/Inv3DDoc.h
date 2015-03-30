// Inv3DDoc.h : interface of the CInv3DDoc class
//


#pragma once

#include "InvFcs.h"
#include "Grid.h"


class CInv3DDoc : public CDocument
{
protected: // create from serialization only
	CInv3DDoc();
	DECLARE_DYNCREATE(CInv3DDoc)

// Attributes
public:
	CInvFcs m_inv;

// Operations
public:
	// model
	CInvFcs* GetInvFcs() {return &m_inv;}
	double** m_pModel;		// model as 1D array, second dimension is for different degrees of focusing
	//std::vector<std::vector<double>> m_model;
	int		m_nModels;		// number of models
	int		m_nX;			// number of cells in x direction
	int		m_nY;
	int		m_nZ;
	double	m_xCs;			// number of cells in x direction
	double	m_yCs;
	double	m_zCs;
	double	m_xMin;			// model extent
	double	m_xMax;
	double	m_yMin;
	double	m_yMax;
	double	m_zMin;
	double	m_zMax;

	// model field gris
	CGrid	m_grdFld;

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	BOOL	IsEmpty() {return m_pModel==NULL;}
	//bool IsEmpty() { return m_model.empty(); }


// Implementation
public:
	virtual ~CInv3DDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnModelDefine();
	afx_msg void OnModelInvert();
	afx_msg void OnUpdateModelInvert(CCmdUI *pCmdUI);
	int ImportModel(CString strFilePath, BOOL bAppend=FALSE);
	afx_msg void OnModelImport();
	afx_msg void OnModelImportAppend();
	int DeleteModels(int first, int last);
	afx_msg void OnModelLoadFoward();
	afx_msg void OnModelSaveForward();
	afx_msg void OnUpdateModelSaveForward(CCmdUI *pCmdUI);
};


