// ViewGL.h : interface of the CViewGL class
//


#pragma once


class CViewGL : public CView
{
protected: // create from serialization only
	CViewGL();
	DECLARE_DYNCREATE(CViewGL)

// Attributes
public:
	CInv3DDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CViewGL();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ViewGL.cpp
inline CInv3DDoc* CViewGL::GetDocument() const
   { return reinterpret_cast<CInv3DDoc*>(m_pDocument); }
#endif

