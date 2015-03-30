// ViewGL.cpp : implementation of the CViewGL class
//

#include "stdafx.h"
#include "Inv3D.h"

#include "Inv3DDoc.h"
#include "ViewGL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CViewGL

IMPLEMENT_DYNCREATE(CViewGL, CView)

BEGIN_MESSAGE_MAP(CViewGL, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CViewGL construction/destruction

CViewGL::CViewGL()
{
	// TODO: add construction code here

}

CViewGL::~CViewGL()
{
}

BOOL CViewGL::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CViewGL drawing

void CViewGL::OnDraw(CDC* /*pDC*/)
{
	CInv3DDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CViewGL printing

BOOL CViewGL::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewGL::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CViewGL::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CViewGL diagnostics

#ifdef _DEBUG
void CViewGL::AssertValid() const
{
	CView::AssertValid();
}

void CViewGL::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInv3DDoc* CViewGL::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInv3DDoc)));
	return (CInv3DDoc*)m_pDocument;
}
#endif //_DEBUG


// CViewGL message handlers
