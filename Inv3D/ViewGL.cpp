// CViewGL.cpp : implementation of the CViewGL class
//
#pragma once

#include "stdafx.h"
#include "Routines.h"
#include "Inv3d.h"

#include "inv3dDoc.h"
#include "ViewGL.h"

#include "Messages.h"


#include "DlgColorGrad.h"

#include <math.h>

#include "MainFrm.h"

#include "DlgSlice.h"
#include "DlgSliceManager.h"

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
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_COLORS, OnEditColors)

	// combo box messages
	ON_CBN_EDITCHANGE(IDC_COMBO_FOCUSING, OnCbnEditchangeCmbFocus)
	ON_CBN_SELENDOK(IDC_COMBO_FOCUSING, OnCbnSelokCmbFocus)

	ON_CBN_EDITCHANGE(IDC_COMBO_ROWS, OnCbnEditchangeCmbRows)
	ON_CBN_SELENDOK(IDC_COMBO_ROWS, OnCbnSelokCmbRows)

	ON_CBN_EDITCHANGE(IDC_COMBO_COLS, OnCbnEditchangeCmbCols)
	ON_CBN_SELENDOK(IDC_COMBO_COLS, OnCbnSelokCmbCols)

	ON_EN_CHANGE(IDC_EDIT_TRESHOLD, OnEditchangeTreshold)



	ON_COMMAND(ID_MODEL_FIRST, OnModelFirst)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FIRST, OnUpdateModelFirst)
	ON_COMMAND(ID_MODEL_LAST, OnModelLast)
	ON_UPDATE_COMMAND_UI(ID_MODEL_LAST, OnUpdateModelLast)
	ON_COMMAND(ID_MODEL_NEXT, OnModelNext)
	ON_UPDATE_COMMAND_UI(ID_MODEL_NEXT, OnUpdateModelNext)
	ON_COMMAND(ID_MODEL_PREVIOUS, OnModelPrevious)
	ON_UPDATE_COMMAND_UI(ID_MODEL_PREVIOUS, OnUpdateModelPrevious)
	ON_COMMAND(ID_VIEW_SHOWTEXT, OnViewShowtext)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTEXT, OnUpdateViewShowtext)
	ON_COMMAND(ID_VIEW_SHOW_SLICES, OnViewShowSlices)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_SLICES, OnUpdateViewShowSlices)
	ON_COMMAND(ID_VIEW_SHOW_VOXELS, OnViewShowVoxels)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_VOXELS, OnUpdateViewShowVoxels)
	ON_COMMAND(ID_VIEW_SLICE_NEW, OnViewSliceNew)
	ON_COMMAND(ID_EDIT_SLICES, OnEditSlices)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SLICES, OnUpdateEditSlices)
	ON_COMMAND(ID_VIEW_SLICE_NEXT, OnViewSliceNext)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SLICE_NEXT, OnUpdateViewSliceNext)
	ON_COMMAND(ID_VIEW_SLICE_PREVIOUS, OnViewSlicePrevious)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SLICE_PREVIOUS, OnUpdateViewSlicePrevious)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_VIEW_SLICE_NEXT_ACTIVATE, OnViewSliceNextActivate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SLICE_NEXT_ACTIVATE, OnUpdateViewSliceNextActivate)
	ON_COMMAND(ID_VIEW_SLICE_PREVIOUS_ACTIVATE, OnViewSlicePreviousActivate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SLICE_PREVIOUS_ACTIVATE, OnUpdateViewSlicePreviousActivate)
	ON_COMMAND(ID_EDIT_FIELD, OnEditField)
	ON_COMMAND(ID_VIEW_SHOW_FIELD, OnViewShowField)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_FIELD, OnUpdateViewShowField)
END_MESSAGE_MAP()

//______________________________________________________________________________
void CViewGL::OnEditchangeTreshold()
{
	TRACE(_T("InAI"));
	CMainFrame* pMain;
	double a;

	pMain = (CMainFrame*)AfxGetMainWnd();
	a=pMain->GetReBarEditVal(IDC_EDIT_TRESHOLD);
	if( a>=0.0 && a<=150 ) {
		m_dTreshold = a;
		Invalidate(FALSE);
	}
	else {
		AfxMessageBox(_T("Set value in range <>"), MB_OK | MB_ICONINFORMATION);
	}
}
//______________________________________________________________________________
void CViewGL::OnCbnSelokCmbFocus()
{
	TRACE("InAI");
	CMainFrame* pMain;
	int a;

	pMain = (CMainFrame*)AfxGetMainWnd();
	a=int(pMain->GetReBarComboVal(IDC_COMBO_FOCUSING));
	if( a>=0.0 && a<GetDocument()->m_nModels ) {
		m_nModFcs = a;
		Invalidate(FALSE);
	}
	else {
		AfxMessageBox(_T("Set value in range <>"), MB_OK | MB_ICONINFORMATION);
	}
}

void CViewGL::OnCbnEditchangeCmbFocus()
{
	TRACE("InAI");
	CMainFrame* pMain;
	double a;

	pMain = (CMainFrame*)AfxGetMainWnd();
	a=pMain->GetReBarComboVal(IDC_COMBO_FOCUSING);
	if( a>=0.0 && a<=GetDocument()->m_nModels ) {
		m_nModFcs = (int)a;
		Invalidate(FALSE);
	}
	else {
		AfxMessageBox(_T("Set value in range <>"), MB_OK | MB_ICONINFORMATION);
	}
}

//_________________________________________________________________________________
void CViewGL::OnCbnSelokCmbRows()
{
	TRACE(_T("InAI"));
	CMainFrame* pMain;
	double a;

	pMain = (CMainFrame*)AfxGetMainWnd();
	a=pMain->GetReBarComboVal(IDC_COMBO_ROWS);
	if( a>=1.0 && a<7 ) {
		m_nWRows = (int)a;
		Invalidate(FALSE);
	}
	else {
		AfxMessageBox(_T("Set value in range <1,6>"), MB_OK | MB_ICONINFORMATION);
	}
}

void CViewGL::OnCbnEditchangeCmbRows()
{
	TRACE(_T("InAI"));
	CMainFrame* pMain;
	double a;

	pMain = (CMainFrame*)AfxGetMainWnd();
	a=pMain->GetReBarComboVal(IDC_COMBO_ROWS);
	if( a>=0.0 && a<=GetDocument()->m_nModels ) {
		m_nWRows = (int)a;
		Invalidate(FALSE);
	}
	else {
		AfxMessageBox(_T("Set value in range <1,6>"), MB_OK | MB_ICONINFORMATION);
	}
}
//________________________________________________________________________________

//_________________________________________________________________________________
void CViewGL::OnCbnSelokCmbCols()
{
	TRACE(_T("InAI"));
	CMainFrame* pMain;
	double a;

	pMain = (CMainFrame*)AfxGetMainWnd();
	a=pMain->GetReBarComboVal(IDC_COMBO_COLS);
	if( a>=1.0 && a<7 ) {
		m_nWCols = (int)a;
		Invalidate(FALSE);
	}
	else {
		AfxMessageBox(_T("Set value in range <1,6>"), MB_OK | MB_ICONINFORMATION);
	}
}

void CViewGL::OnCbnEditchangeCmbCols()
{
	TRACE(_T("InAI"));
	CMainFrame* pMain;
	double a;

	pMain = (CMainFrame*)AfxGetMainWnd();
	a=pMain->GetReBarComboVal(IDC_COMBO_COLS);
	if( a>=0.0 && a<=GetDocument()->m_nModels ) {
		m_nWCols = (int)a;
		Invalidate(FALSE);
	}
	else {
		AfxMessageBox(_T("Set value in range <1,6>"), MB_OK | MB_ICONINFORMATION);
	}
}
//________________________________________________________________________________

// CViewGL construction/destruction

CViewGL::CViewGL()
{
	//m_pDoc = NULL;

    m_fRotX = 0;
	m_fRotY = 180.0f;
	m_fRotZ = 0;

	m_dProjAngle = 45.0;
	m_dCameraX = 0;
	m_dCameraY = 0;
	m_dCameraZ = 0;

	m_mX = 0.01f;
	m_mY = 0.01f;
	m_mZ = 0.01f;

	m_rX = 2.0f;
	m_rY = 2.0f;
	m_rZ = 2.0f;

	//m_nProjection = projPerspective;
	m_nProjection = projOrtho;
	m_dOrthoScale = 1.0;

	// vertical scale
	m_dScZ = 1.0;

	m_bPlane = FALSE;

	m_dLightAzim = 0;
	m_dLightIncl = 45;

	// font
	memset(&m_lf, 0, sizeof(LOGFONT));
	m_lf.lfHeight=12;	
	m_lf.lfWidth=0;
	m_lf.lfEscapement=m_lf.lfOrientation=0;
	m_lf.lfItalic=0;
	m_lf.lfWeight=FW_NORMAL;
	m_lf.lfUnderline=FALSE;
	m_lf.lfStrikeOut=FALSE;
	m_lf.lfCharSet=ANSI_CHARSET;
	m_lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	m_lf.lfClipPrecision=CLIP_DEFAULT_PRECIS; 
	m_lf.lfQuality=DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily=DEFAULT_PITCH |FF_DONTCARE;
	strcpy(CT2A(m_lf.lfFaceName), "Arial");        

	// text
	m_charsetDListBase2D = 0;
	m_charsetDListBase3D = 0;
	m_nChars = 255;
	//m_gmfvector = NULL;
	m_nTextType = GLTXT_2D;
	m_bAxisLabels = FALSE;

	m_nModFcs = 0;

	// color legend
	m_nCltTex = 0;
	m_nCltFld = 0;
	m_nCltTexWidth = 512;
	m_pCltTex.resize(m_nCltTexWidth*4);
	m_pCltFld.resize(m_nCltTexWidth*4);

	float f=3;
	m_clrGrad.SetStartPegColour( RGB(255,	255,	255));
	m_clrGrad.AddPeg( RGB(0,	0,		164), 1/f);
	m_clrGrad.AddPeg( RGB(216,	0,	0), 2/f);
	m_clrGrad.SetEndPegColour( RGB(255,	200,	255));

	//m_clrGrad.SetStartPegColour( RGB(255,	255,	255));
	//m_clrGrad.AddPeg( RGB(0,	0,		153), 1/f);
	//m_clrGrad.AddPeg( RGB(0,	204,	255), 2/f);
	//m_clrGrad.AddPeg( RGB(0,	255,	0), 3/f);
	//m_clrGrad.AddPeg( RGB(255,	255,	0), 4/f);
	//m_clrGrad.AddPeg( RGB(255,	0,		0), 5/f);
	//m_clrGrad.AddPeg( RGB(255,	0,		255), 6/f);
	//m_clrGrad.SetEndPegColour( RGB(255,	200,	255));

	m_nWRows = 3;
	m_nWCols = 3;
	m_nWFirst = 0;

	m_bVoxels = TRUE;
	m_bSlices = TRUE;
	m_bField = TRUE;

	m_bTreshold = TRUE;
	m_dTreshold = 1.0;

	m_bUpdateFld = TRUE;
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

int CViewGL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	TRACE(_T(" Entering CViewGL::OnCreate(LPCREATESTRUCT lpCreateStruct\n)"));
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// OpenGL rendering context creation
	PIXELFORMATDESCRIPTOR pfd;

	// initialize the private member
	m_pDC = new CClientDC(this);

	// choose the requested video mode
	 if (!SetupPixelFormat()) return 0;
	
	// ask the system if the video mode is supported
	   int n=::GetPixelFormat(m_pDC->GetSafeHdc());
	   ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	//----------------------------
	//// get the best available match of pixel format for the device context  
	//int iPixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd); 
	//	
	//// make that the pixel format of the device context 
	//SetPixelFormat(m_pDC->GetSafeHdc(), iPixelFormat, &pfd); 
	//----------------------------

	// link the Win Device Context with the OGL Rendering Context
    m_hRC = wglCreateContext( m_pDC->GetSafeHdc() );
	if( m_hRC == NULL ) {
		AfxMessageBox(_T("Cannot create Open GL rendering context. Closing."), MB_OK | MB_ICONSTOP);
		CView::SendMessage( WM_CLOSE );
		return -1;
	}

	// specify the target DeviceContext (window) of the subsequent OGL calls
    wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// performs default setting of rendering mode,etc..
	OnCreateGL();

	// free the target DeviceContext (window)
    wglMakeCurrent(NULL, NULL);
	
	return 0;
}

void CViewGL::OnCreateGL()
{
	// perform hidden line/surface removal (enabling Z-Buffer)
	glEnable(GL_DEPTH_TEST);

	// set background color to white
	glClearColor(1.f, 1.f, 1.f, 1.0f );

	// set clear Z-Buffer value
	glClearDepth(1.0f);

	// Hints
	//glEnable(GL_POLYGON_SMOOTH);			// very slow
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// fonts
	PrepareCharset2D(&m_lf);
	PrepareCharset3D(&m_lf, 10.0f, FALSE);

	// common display lists
}


void CViewGL::OnDestroy()
{
	// specify the target DeviceContext (window) of the subsequent OGL calls
    wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	if(m_charsetDListBase2D>0) 
		glDeleteLists(m_charsetDListBase2D, m_nChars);	
	if(m_charsetDListBase3D>0) 
		glDeleteLists(m_charsetDListBase3D, m_nChars);	

	// remove all display lists
	//for (int c=0;c<MAX_LISTS;c++) if(m_DispListVector[c]) glDeleteLists(m_DispListVector[c],1);

	// release definitely OGL Rendering Context
	if (m_hRC!=NULL) ::wglDeleteContext(m_hRC);


	// destroy Win Device Context
	//if(m_pDC) delete m_pDC;

	// finally call the base function
	CView::OnDestroy();	
}

void CViewGL::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// when called with a nonzero window:
	if ( 0 < cx && 0 < cy )
	{
		// specify the target DeviceContext of the OGL calls below
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

		// call the virtual sizing procedure (to be overridden by user)
		SetProjectionMatrix(cx,cy); 

		// free the target DeviceContext (window)
		wglMakeCurrent(NULL,NULL);

		// force redraw
		Invalidate(FALSE);
	};
}

BOOL CViewGL::SetupPixelFormat(void)
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}

	return TRUE;
}


void CViewGL::SetProjectionMatrix(int w, int h)
{
	ASSERT(m_nProjection==projOrtho || m_nProjection==projPerspective);
	GLdouble aspect;

	// Prevent a divide by zero, when window is too short
	// (you can't make a window of zero width)
	if(h == 0)
		h = 1;

	if(w == 0)
		w = 1;

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// width/height aspect ratio
 	aspect = (float) w/h;

	glPushMatrix();
		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			// set up ortographic projection
			if( m_nProjection == projOrtho ) {
				double wm, hm;			// model width
				wm = hm = m_diag*0.6;	
				wm/hm < aspect ? wm=aspect*hm : hm=wm/aspect;
				wm *= m_dOrthoScale;
				hm *= m_dOrthoScale;
				glOrtho(-wm, +wm, -hm, +hm, -m_diag, m_diag);
			}

 			// Set perspective projection matrix
			if( m_nProjection == projPerspective ) {
				// Set the clipping volume
				gluPerspective(m_dProjAngle, aspect, 0.001*m_diag, 1.0e8*m_diag );
			}
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void CViewGL::SetProjectionMatrix(CRect rcScene)
{
	ASSERT(m_nProjection==projOrtho || m_nProjection==projPerspective);
	GLdouble aspect;

	// Prevent a divide by zero, when window is too short
	// (you can't make a window of zero width)
	int h = rcScene.Height();
	int w = rcScene.Width();
	if(h == 0)
		h = 1;

	if(w == 0)
		w = 1;

    // Set the viewport to be the entire window
	glViewport(rcScene.left, rcScene.top, w, h);

	// width/height aspect ratio
 	aspect = (float) w/h;

	glPushMatrix();
		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			// set up ortographic projection
			if( m_nProjection == projOrtho ) {
				double wm, hm;			// model width
				wm = hm = m_diag*0.6;	
				wm/hm < aspect ? wm=aspect*hm : hm=wm/aspect;
				wm *= m_dOrthoScale;
				hm *= m_dOrthoScale;
				glOrtho(-wm, +wm, -hm, +hm, -m_diag, m_diag);
			}

 			// Set perspective projection matrix
			if( m_nProjection == projPerspective ) {
				// Set the clipping volume
				gluPerspective(m_dProjAngle, aspect, 0.001*m_diag, 1.0e8*m_diag );
			}
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


// override for custom initialization
void CViewGL::OnInitialUpdate()
{
	GetParent()->SetWindowText(GetDocument()->GetTitle()+ _T(": 3D View"));

	CView::OnInitialUpdate();

	// get document
	m_pDoc = GetDocument();

	CInv3DDoc* pDoc = GetDocument();
	CInvFcs* pInv;
	pInv = pDoc->GetInvFcs();
	if( pDoc->m_pModel!=NULL) {
		m_minX = pDoc->m_xMin;
		m_maxX = pDoc->m_xMax; 

		m_minY = pDoc->m_zMin;
		m_maxY = pDoc->m_zMax; 

		m_minZ = -pDoc->m_yMax; 
		m_maxZ = -pDoc->m_yMin;
	}
	else {
		m_minX = 100.0;
		m_maxX = 200.0; 

		m_minY = 100.0;
		m_maxY = 200.0; 

		m_minZ = 100.0; 
		m_maxZ = 200.0;
	}

	m_midX = (m_maxX+m_minX)/2;
	m_midY = (m_maxY+m_minY)/2;
	m_midZ = (m_maxZ+m_minZ)/2;

	m_lenX = m_maxX-m_minX;
	m_lenY = m_maxY-m_minY;
	m_lenZ = m_maxZ-m_minZ;

	m_diag = (float) sqrt(m_lenX*m_lenX + m_lenY*m_lenY + m_lenZ*m_lenZ );

	float m = (float) m_diag/100;
	m_mX = m;
	m_mY = m;
	m_mZ = m;

	m_dCameraZ = 0.0;	//(m_diag/2.0)/tan(m_dProjAngle*3.141592/360.0);

	GetClientRect(&m_oldRect);

	InitTextLabels();

	m_clrGrad.SetHstMin(0);
	m_clrGrad.SetHstMax(1500);

	// do lenghty initializations
	//CWaitCursor wait;
	//wait.Restore();

}

//+++++ drawing functions +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// do not erase background - avoids blinking
BOOL CViewGL::OnEraseBkgnd(CDC* pDC)
{
	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

void CViewGL::OnDraw(CDC* pDC)
{
	// prepare a semaphore
	static BOOL 	bBusy = FALSE;
	// use the semaphore to enter this critic section
	if(bBusy) return;
	bBusy = TRUE;
	// specify the target DeviceContext of the subsequent OGL calls
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// clear background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// call the virtual drawing procedure (to be overridden by user)
	//DrawScene();
	DrawSceneMulti();


	// execute OGL commands (flush the OGL graphical pipeline)
	//	glFinish(); // useless cause swapbuffers issues an implicit glFinish

	// if double buffering is used it's time to swap the buffers
	SwapBuffers(m_pDC->GetSafeHdc());

	// check for errors
	m_glErrorCode=glGetError();
	if(m_glErrorCode != GL_NO_ERROR)
	{
		const GLubyte *estring;
		CString mexstr;
		// get the error descritption from OGL
		estring = gluErrorString(m_glErrorCode);
		// prepare and show a message box
		mexstr.Format(_T("GLEnabledView:\n\tAn OpenGL error occurred: %s\n"), estring);
		AfxMessageBox(mexstr,MB_OK | MB_ICONEXCLAMATION);
		// replicate mex to debug trace
		TRACE(mexstr);
		// free the target DeviceContext (window)
		wglMakeCurrent(NULL,NULL);
		//bBusy=TRUE;
	}
	// disable GL drawing
	//else
	//{
	//	// turn the semaphore "green"
	//	bBusy = FALSE;
	//}

	// turn the semaphore "green" or uncomment the code before to enable drawing with errors
	bBusy = FALSE;

	// free the target DeviceContext (window)
    wglMakeCurrent(NULL,NULL);

	return;

	CMemDC memDC(*m_pDC, this);
	CDC& dc = memDC.GetDC();

	// draw the division
	CRect clRc;
	GetClientRect(&clRc);
	int dx = clRc.Width()/m_nWCols;
	int dy = clRc.Height()/m_nWRows;
	CPen penBl(PS_SOLID, 0, RGB(0, 0, 180));
	CPen* pPen;
	pPen = dc.SelectObject(&penBl);
	CFont font;
	font.CreateFontIndirect(&m_lf);
	CFont* pFont = dc.SelectObject(&font);
	dc.SetTextColor( RGB(0, 0, 180) );
	dc.SetTextAlign(TA_TOP | TA_CENTER);
	dc.SetBkMode(TRANSPARENT);
	for(int i=1; i<(m_nWCols); i++) {
		dc.MoveTo(clRc.left+i*dx, clRc.top); 
		dc.LineTo(clRc.left+i*dx, clRc.bottom);
	}
	for(int j=1; j<(m_nWRows); j++) {
		dc.MoveTo(clRc.right, clRc.top+j*dy); 
		dc.LineTo(clRc.left, clRc.top+j*dy);
	}
	for(int i=m_nWFirst; i<min(m_nWFirst+m_nWRows*m_nWCols, m_pDoc->m_nModels); i++) {
		int l;
		int x, y;
		l = i-m_nWFirst;
		m_nWCurrent = i;
		x = l%m_nWCols;
		y = l/m_nWCols;

		x = clRc.left + x*dx + dx/2;
		//y = clRc.top + y*dy;
		y = clRc.top + y*dy;
		CString str;
		if(i!=m_nWFirst)
			str.Format(_T("Focusing itaration %d"), i);
		else
			str = _T("True Model");
		dc.TextOut(x, y, str);
	}
	dc.SelectObject(pPen);
	dc.SelectObject(pFont);
}

void CViewGL::SetupLights(BOOL bEnable)
{
	if (bEnable) {
		glEnable(GL_LIGHTING);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0f);

		glFrontFace(GL_CCW);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glShadeModel(GL_SMOOTH);

		// We`ll white light, so Ambient + Diffuse = White (1.0f, 1.0f, 1.0f, 1.0f)
		GLfloat ambientIntensity[] = {0.4f, 0.4f, 0.4f, 1.0f};
		GLfloat diffuseIntensity[] = {0.6f, 0.6f, 0.6f, 1.0f};

		// light in gl coordinates
		float lx, ly, lz, ld, la;
		ld = (float) (m_diag);
		la = (float) (ld*cos( DegToRad(m_dLightIncl) ));
		ly = (float) (m_maxY + ld*sin( DegToRad(m_dLightIncl) ));
		lx = (float) (m_midX + la*cos( DegToRad(m_dLightAzim) ));
		lz = (float) (m_midZ + la*sin( DegToRad(m_dLightAzim) ));

		GLfloat position0[] = {(float) (lx), (float) (ly), (float) (lz), 0.0f};
		GLfloat position1[] = {(float) (m_midX), (float) (m_midY+ly), (float) (m_midZ), 0.0f};
		GLfloat position2[] = {(float) (m_midX+m_lenX*100), (float) (m_midY+m_lenY*100),	(float) (m_midZ, 0.0f)};

		// Light from UP
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientIntensity);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseIntensity);
		glLightfv(GL_LIGHT0, GL_POSITION, position0);

		// Light from viewer
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambientIntensity);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseIntensity);
		glLightfv(GL_LIGHT1, GL_POSITION, position1);

		// Light from left
		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_AMBIENT, ambientIntensity);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseIntensity);
		glLightfv(GL_LIGHT2, GL_POSITION, position2);

		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	} 
	else {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	};

}


// overide to draw custom scene
void CViewGL::DrawSceneMulti()
{
	if( m_pDoc->m_nModels<=0) 
		return;

	ASSERT(m_nWCols>0);
	ASSERT(m_nWRows>0);
	ASSERT(m_nWFirst>=0 && m_nWFirst<m_pDoc->m_nModels);

	CRect clRc;
	int nx, ny;
	int dx, dy;

	nx = m_nWCols;
	ny = m_nWRows;

	GetClientRect(&clRc);
	dx = clRc.Width()/nx;
	dy = clRc.Height()/ny;

	int nModels;
	m_pDoc->m_inv.GetModels(nModels);
	BOOL bFrw = (m_pDoc->m_inv.GetModelFrw()!=NULL);
	if(bFrw) nModels++;
	for(int i=m_nWFirst; i<min(m_nWFirst+nx*ny, nModels); i++) {
		int l;
		int x, y;
		l = i-m_nWFirst;
		if(bFrw) m_nWCurrent = i-1;
		else m_nWCurrent = i;
 		if(i==m_nWFirst && !(m_nWCols==1 && m_nWRows==1) ) m_nWCurrent = -1;
		x = l%nx;
		y = l/nx;

		x = clRc.left + x*dx;
		//y = clRc.top + y*dy;
		y = clRc.bottom - y*dy - dy;

		CRect rcScene = CRect(x, y, x+dx, y+dy);
 		glFlush();
			DrawScene(rcScene);
 		glFlush();
	}

	// small coordinate system
	DrawCoordSys();

	if( m_bPlane )
		DrawOrientation();
}

// overide to draw custom scene
void CViewGL::DrawScene(CRect rcScene)
{
	SetProjectionMatrix(rcScene);

	glPushMatrix();
		if( m_nProjection == projPerspective ) {
			if( !m_bPlane ) {
				// user camera position
				glTranslated(m_dCameraX, m_dCameraY, m_dCameraZ);

				// User rotation
				double theta;
				theta = m_dProjAngle/2;
				m_dCameraEyeZ = (m_diag*2/3)/tan(theta*3.141592/180.0);

				glTranslated(0.0f, 0.0f, -m_dCameraEyeZ );
				glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
				glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
				glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
				glTranslated(0.0f, 0.0f, m_dCameraEyeZ );

				glScaled(1.0, m_dScZ, 1.0);

				// Sets camera position.
				gluLookAt( m_midX, m_midY, m_midZ-m_dCameraEyeZ,
						m_midX, m_midY, m_midZ,
						0, 1, 0);

				glEnable(GL_NORMALIZE);
			}
			else {
				glRotated(m_plRotX, 1.0, 0.0, 0.0);		// heading
				glRotated(m_plRotY, 0.0, 1.0, 0.0);		// pitch
				glRotated(m_plRotZ, 0.0, 0.0, 1.0);		// rool

				glScaled(1.0, m_dScZ, 1.0);

				glTranslated(-m_plX, -m_plY, -m_plZ);
			}
		}

		if( m_nProjection == projOrtho ) {
			glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
			glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
			glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
			glTranslated(-m_midX, -m_midY, -m_midZ );
		}

		// do not move lights -> setup lights afrer all transformations
		SetupLights(TRUE);

		// draw model envelope prism
		DrawModelPrism();
		DrawAxisLabels();

		// draw model
		if(m_bVoxels) {
			DrawModelVoxels();
		}
		
		//glDepthMask(GL_FALSE);
		if(m_bSlices) {
			for(int i=0; i<(int)m_slices.GetCount(); i++) {
				CSlice3D* pSlc;
				pSlc = m_slices.GetAt(i);
				DrawModelSlice(pSlc->GetType(), pSlc->GetProf(), pSlc->GetAlpha(), i);
			}
		}
		//glDepthMask(TRUE);

		if(m_nWCurrent==-1 && m_bField) {
			if((m_srfFld.IsEmpty() || m_bUpdateFld) && m_pDoc->m_inv.IsForwardModel() ) {
				LoadGridSurface(&(m_pDoc->m_grdFld), &m_srfFld, CTBL_FLD, m_maxY, m_maxY+m_lenY/2.0);
				m_bUpdateFld = FALSE;
			}
			m_srfFld.DrawGlShade();
			m_srfFld.DrawGL(TRUE);
		}

		//if( m_bPlane )
		//	DrawOrientation();

		//DrawCoordSys();

	glPopMatrix();

	DrawLegend();

};

//// overide to draw custom scene
//void CViewGL::DrawScene()
//{
//	GetClientRect(&m_oldRect);
//
//	SetProjectionMatrix(m_oldRect.Width(), m_oldRect.Height() );
//
//	glPushMatrix();
//		if( m_nProjection == projPerspective ) {
//			if( !m_bPlane ) {
//				// user camera position
//				glTranslated(m_dCameraX, m_dCameraY, m_dCameraZ);
//
//				// User rotation
//				double theta;
//				theta = m_dProjAngle/2;
//				m_dCameraEyeZ = (m_diag*2/3)/tan(theta*3.141592/180.0);
//
//				glTranslated(0.0f, 0.0f, -m_dCameraEyeZ );
//				glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
//				glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
//				glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
//				glTranslated(0.0f, 0.0f, m_dCameraEyeZ );
//
//				glScaled(1.0, m_dScZ, 1.0);
//
//				// Sets camera position.
//				gluLookAt( m_midX, m_midY, m_midZ-m_dCameraEyeZ,
//						m_midX, m_midY, m_midZ,
//						0, 1, 0);
//
//				glEnable(GL_NORMALIZE);
//			}
//			else {
//				glRotated(m_plRotX, 1.0, 0.0, 0.0);		// heading
//				glRotated(m_plRotY, 0.0, 1.0, 0.0);		// pitch
//				glRotated(m_plRotZ, 0.0, 0.0, 1.0);		// rool
//
//				glScaled(1.0, m_dScZ, 1.0);
//
//				glTranslated(-m_plX, -m_plY, -m_plZ);
//			}
//		}
//
//		if( m_nProjection == projOrtho ) {
//			glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
//			glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
//			glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
//			glTranslated(-m_midX, -m_midY, -m_midZ );
//		}
//
//		// do not move lights -> setup lights afrer all transformations
//		SetupLights(TRUE);
//
//		// draw model envelope prism
//		DrawModelPrism();
//		DrawAxisLabels();
//
//		// draw model
//		DrawModelVoxels();
//
//		if( m_bPlane )
//			DrawOrientation();
//
//		DrawCoordSys();
//
//	glPopMatrix();
//
//	DrawLegend();
//
//};

// overide to draw custom small coordinatte sysem
void CViewGL::DrawModelVoxels(void)
{
	CInv3DDoc* pDoc = GetDocument(); 
	double**	pModels=NULL;
	double*		pModFrw=NULL;
	int			nModels=0;
	pModels = m_pDoc->m_inv.GetModels();
	m_pDoc->m_inv.GetModels(nModels);
	pModFrw = m_pDoc->m_inv.GetModelFrw();
	if((pModels==NULL || nModels==0) && pModFrw==NULL) return;
	//if(pDoc->m_pModel==NULL) return;

 	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	GLUquadricObj*	pQuad;
	float alpha = 1.0f;
	float color[4]={1.0f, 0.0f, 0.0f, alpha};
	float colorN[4]={1.0f, 0.0f, 0.0f, alpha};
	float matSpec[4] = {0.5f, 0.5f, 0.5f, alpha};
	float matEm[4] = {0.1f, 0.1f, 0.1f, alpha};
	pQuad = gluNewQuadric();
	gluQuadricNormals(pQuad, GLU_SMOOTH);
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEm);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 120.0f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// over 3D grid
	float r, g, b;
	int indx;
	int nzMin, nzMax, nyMin, nyMax, nxMin, nxMax;
	int nx, ny, nz;
	double x, y, z;				// world coordinates
	double x0, y0, z0;			// world coordinates
	double xcs, ycs, zcs;		// world coordinates
	x0 = pDoc->m_xMin;
	y0 = pDoc->m_yMin;
	z0 = pDoc->m_zMax;
	xcs = pDoc->m_xCs;
	ycs = pDoc->m_yCs;
	zcs = pDoc->m_zCs;
	nxMin = 0;
	nx = pDoc->m_nX;
	nyMin = 0;
	ny = pDoc->m_nY;
	nzMin = 0;
	nz = pDoc->m_nZ;
	nxMax = nx-1;
	nyMax = ny-1;
	nzMax = nz-1;

	if(m_nWCurrent>nModels) m_nWCurrent=nModels;
	double* pData;
	if(m_nWCurrent==-1 && pModFrw!=NULL)
		pData = m_pDoc->m_inv.GetModelFrw();
	else
		pData = pModels[m_nWCurrent];
	for(int k=nzMin; k<=nzMax; k++) {			// depth layers
		z = z0 - k*zcs - zcs*0.5;
		for(int i=nyMin; i<=nyMax; i++) {		// "layer grid" rows
			y = y0 + i*ycs + ycs*0.5;
			for(int j=nxMin; j<=nxMax; j++) {	// "layer grid" columns
				x = x0 + j*xcs + xcs*0.5;
				indx = (k*nx*ny) + i*nx + j;
				if( (pData[indx]>=m_dTreshold || !m_bTreshold) && m_bVoxels ) {
					m_clrGrad.GetColorFromValue(pData[indx], r, g, b, FALSE);
					color[0] = r/255.0f;	color[1] = g/255.0f;	color[2] = b/255.0f;
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
					glPushMatrix(); 
						glTranslated(x, z, -y);
						GLdouble r = pData[indx]/100.0 * xcs*0.4;
						if(r>xcs/2.0) r=xcs/2.0;
						if(r<xcs/10.0) r=xcs/10.0;
						gluSphere(pQuad, r, 10, 10);
					glPopMatrix();
				}
			} // for(j)
		} // for(i)
	} // for(k)


	// set back the material
	GlDefaultMaterial();
}

// draws slice in nTag direction, n_th slice
void CViewGL::DrawModelSlice(int nTag, int nSlice, float fAlpha, int nIndex)
{
	CInv3DDoc* pDoc = GetDocument(); 
	double**	pModels;
	double*		pModFrw;
	int			nModels;
	pModels = m_pDoc->m_inv.GetModels();
	m_pDoc->m_inv.GetModels(nModels);
	pModFrw = m_pDoc->m_inv.GetModelFrw();
	if((pModels==NULL || nModels==0) && pModFrw==NULL) return;

	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float color[4]={1.0f, 0.0f, 0.0f, fAlpha};
	float colorN[4]={1.0f, 0.0f, 0.0f, fAlpha};
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// over 3D grid
	float r, g, b;
	int indx;
	int nzMin, nzMax, nyMin, nyMax, nxMin, nxMax;
	int nx, ny, nz;
	double x, y, z;				// world coordinates
	double x0, y0, z0;			// world coordinates
	double xcs, ycs, zcs;		// world coordinates
	x0 = pDoc->m_xMin;
	y0 = pDoc->m_yMin;
	z0 = pDoc->m_zMax;
	xcs = pDoc->m_xCs;
	ycs = pDoc->m_yCs;
	zcs = pDoc->m_zCs;
	nxMin = 0;
	nx = pDoc->m_nX;
	nyMin = 0;
	ny = pDoc->m_nY;
	nzMin = 0;
	nz = pDoc->m_nZ;
	nxMax = nx-1;
	nyMax = ny-1;
	nzMax = nz-1;

	ASSERT(nTag==CSlice3D::sliceXY || nTag==CSlice3D::sliceXZ || nTag==CSlice3D::sliceYZ );
	switch(nTag) {
		case CSlice3D::sliceXZ:
			nyMin = nSlice;
			nyMax = nSlice;
			break;
		case CSlice3D::sliceYZ:
			nxMin = nSlice;
			nxMax = nSlice;
			break;
		case CSlice3D::sliceXY:
			nzMin = nSlice;
			nzMax = nSlice;
			break;
	}

	double* pData;
	if(m_nWCurrent==-1 && pModFrw!=NULL)
		pData = m_pDoc->m_inv.GetModelFrw();
	else
		pData = pModels[m_nWCurrent];
	double x2, y2, z2;
	for(int k=nzMin; k<=nzMax; k++) {			// depth layers
		z = z0 - k*zcs - zcs;
		for(int i=nyMin; i<=nyMax; i++) {		// "layer grid" rows
			y = y0 + i*ycs;
			for(int j=nxMin; j<=nxMax; j++) {	// "layer grid" columns
				x = x0 + j*xcs;
				indx = (k*nx*ny) + i*nx + j;
				if(1/*pData[indx] != 0.0*/) {
					m_clrGrad.GetColorFromValue(pData[indx], r, g, b, FALSE);

					color[0] = r/255.0f;	color[1] = g/255.0f;	color[2] = b/255.0f;

					glColor4fv(color);

					x2 = x + xcs;
					y2 = y + ycs;
					z2 = z + zcs;
					switch(nTag) {
						case CSlice3D::sliceXZ:
							y2 = -y - ycs*0.5;
							if( pData[indx]>=m_dTreshold || !m_bTreshold) {
								glBegin(GL_QUADS); 
									glVertex3d(x, z, y2);
									glVertex3d(x2, z, y2);
									glVertex3d(x2, z2, y2);
									glVertex3d(x, z2, y2);
								glEnd();
							}
							if(m_nSliceSel==nIndex) 
								glColor3d(0, 0, 0);
							else
								glColor3d(0.6, 0.6, 0.6);
							glBegin(GL_LINE_STRIP); 
								glVertex3d(x, z, y2);
								glVertex3d(x2, z, y2);
								glVertex3d(x2, z2, y2);
								glVertex3d(x, z2, y2);
								glVertex3d(x, z, y2);
							glEnd();
							break;
						case CSlice3D::sliceYZ:
							x2 = x + xcs*0.5;
							if( pData[indx]>=m_dTreshold || !m_bTreshold) {
								glBegin(GL_QUADS);
									glVertex3d(x2, z, -y);
									glVertex3d(x2, z, -y2);
									glVertex3d(x2, z2, -y2);
									glVertex3d(x2, z2, -y);
								glEnd();
							}
							if(m_nSliceSel==nIndex) 
								glColor3d(0, 0, 0);
							else
								glColor3d(0.6, 0.6, 0.6);
							glBegin(GL_LINE_STRIP);
								glVertex3d(x2, z, -y);
								glVertex3d(x2, z, -y2);
								glVertex3d(x2, z2, -y2);
								glVertex3d(x2, z2, -y);
								glVertex3d(x2, z, -y);
							glEnd();
							break;
						case CSlice3D::sliceXY:
							z2 = z + zcs*0.5;
							if( pData[indx]>=m_dTreshold || !m_bTreshold) {
								glBegin(GL_QUADS);
									glVertex3d(x, z2, -y);
									glVertex3d(x2, z2, -y);
									glVertex3d(x2, z2, -y2);
									glVertex3d(x, z2, -y2);
								glEnd();
							}
							if(m_nSliceSel==nIndex) 
								glColor3d(0, 0, 0);
							else
								glColor3d(0.6, 0.6, 0.6);
							glBegin(GL_LINE_STRIP);
								glVertex3d(x, z2, -y);
								glVertex3d(x2, z2, -y);
								glVertex3d(x2, z2, -y2);
								glVertex3d(x, z2, -y2);
								glVertex3d(x, z2, -y);
							glEnd();
							break;
					}
				}
			} // for(j)
		} // for(i)
	} // for(k)


}

void CViewGL::UpdateTexture(GLuint& nTex, GLfloat* pTex, GLuint nWidth)
{
	ASSERT(wglGetCurrentContext() != NULL);

	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, nTex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, 4, nWidth, 0, GL_RGBA, GL_FLOAT, pTex);
	glDisable(GL_TEXTURE_1D);
}

void CViewGL::DrawLegend(void)
{
	CRect clRc;
	int k=1;
	double minDt, maxDt;

	GetClientRect(&clRc);
	int h = (clRc.Height()-60)/2;
	if( h<100 || clRc.Width()<121) return;
	if(h>200) h=200;
 	
	if( m_bField ) {
		m_pDoc->m_grdFld.GetHistogramMinMax(minDt, maxDt);
		if( !glIsTexture(m_nCltFld) ) {
			glGenTextures(1, &m_nCltFld);
			m_pDoc->m_grdFld.CreateColorTableGL(m_pCltFld.data(), m_nCltTexWidth, 4, 1.0f);
			UpdateTexture(m_nCltFld, m_pCltFld.data(), m_nCltTexWidth);
		}
		DrawLegend(0, clRc.Height()-h, 120, h, minDt, maxDt, m_nCltFld, _T("Field"));
		k = 2;
	}

	minDt = m_clrGrad.GetHstMin();
	maxDt = m_clrGrad.GetHstMax();
	if( !glIsTexture(m_nCltTex) ) {
		glGenTextures(1, &m_nCltTex);
		m_clrGrad.CreateColorTableGL(m_pCltTex.data(), m_nCltTexWidth, 4, 1.0f);
		UpdateTexture(m_nCltTex, m_pCltTex.data(), m_nCltTexWidth);
	}
	DrawLegend(0, clRc.Height()-k*h, 120, h, minDt, maxDt, m_nCltTex, _T("Model"));
	return;
}

void CViewGL::DrawLegend(int x, int y, int w, int h, double minData, double maxData, int nTex, CString strName)
{
	if(!glIsTexture(nTex)) return;

 	glFlush();
	
	glViewport(x, y, w, h);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1, 0, 1,-5, 5);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glLoadIdentity();

		// texture is OK continue drawing
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_TEXTURE_1D);
		glBindTexture(GL_TEXTURE_1D, nTex);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);;
		glDisable(GL_LIGHTING);
		glPointSize(1);
		glLineWidth( 1.0 );
		glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord1d(0);
			glVertex3d(0, 0.2, 0);

			glTexCoord1d(0);
			glVertex3d(0.1, 0.2, 0);

			glTexCoord1d(1);
			glVertex3d(0.1, 1, 0);

			glTexCoord1d(1);
			glVertex3d(0, 1, 0);
		glEnd();
		glDisable(GL_TEXTURE_1D);

		glColor4d(0.0, 0.0, 0.1, 1.0);
		glRasterPos3d(0.01, 0.1, 0);
		Text2D(strName);	

		double min, max, maxFld, step, start, sc, of;
		int nTicks = 6;
		int decM, decN;
		
		min = minData;
		
		max = maxData;
		maxFld = max;
		sc = 0.8/(max-min);
		of = 0.2-sc*min;
		FindAxisMinMaxStep(min, max, step, start, decM, decN, 4);


		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glPointSize(2.0f);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			
		CString str;
		CString strFormat;
		CString strDec;
		float colorText[4] = {0.0f, 0.0f, 0.0f, 1.0f};		// front facet as seen from outside the body

		double yv, yn, ytxt;
		yv = start;
		yn = start;
		while(yv < max) {
			if(yv>=min) {
				strDec.Format(_T("%d"), decM);
				strFormat = _T(" %.") + strDec + _T("f");
				str.Format(strFormat, yv);
				ytxt = of+sc*yv;
				glBegin(GL_LINES);
					glColor4d(0.0, 0.0, 0.1, 1.0);
					glVertex3d(0.1, ytxt, 0.1);
					glVertex3d(0.13, ytxt, 0.1);
				glEnd();

				// 2D text
				glRasterPos3d(0.14, ytxt, 0);
				Text2D(str);	
			}
			yn = yv+step/2.0;
			if(yn>=min) {
				strDec.Format(_T("%d"), decN);
				strFormat = _T(" %.") + strDec + _T("f");
				str.Format(strFormat, yn);
				ytxt = of+sc*yn;
				glBegin(GL_LINES);
					glColor4d(0.0, 0.0, 0.1, 1.0);
					glVertex3d(0.1, ytxt, 0.1);
					glVertex3d(0.11, ytxt, 0.1);
				glEnd();

				// 2D text
				glRasterPos3d(0.12, ytxt, 0);
				Text2D(str);	
			}
			yv += step;
		}
	glPopMatrix();
	glDepthFunc(GL_LESS);

 	glFlush();
}


// overide to draw custom orientation
void CViewGL::DrawOrientation(void)
{
	CRect clRc;
	GetClientRect(&clRc);
 	glFlush();

	glViewport(clRc.right-100, 0, 100, 100);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		GLdouble aspect = 1.0f;
		double wm, hm;			// model width
		wm = hm = m_diag*0.6;	
		wm/hm < aspect ? wm=aspect*hm : hm=wm/aspect;
		wm *= m_dOrthoScale;
		hm *= m_dOrthoScale;
		glOrtho(-wm, +wm, -hm, +hm, -m_diag, m_diag);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glLoadIdentity();
		glRotatef(90, 1.0f, 0.0f, 0.0f);
		glRotatef(0, 0.0f, 1.0f, 0.0f);
		glRotatef(0, 0.0f, 0.0f, 1.0f);
		glTranslated(-m_midX, -m_midY, -m_midZ );
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3d(0.8, 0.8, 0.8);
		glBegin(GL_QUADS);
			glVertex3d(m_minX, m_midY, m_minZ);
			glVertex3d(m_maxX, m_midY, m_minZ);
			glVertex3d(m_maxX, m_midY, m_maxZ);
			glVertex3d(m_minX, m_midY, m_maxZ);
		glEnd();
		glPointSize(4);
		glColor3d(0.8, 0, 0);
		glBegin(GL_POINTS);
			glVertex3d(m_plX, m_plY, m_plZ);
		glEnd();
		glColor3d(0, 0.8, 0);
		glBegin(GL_LINES);
			glVertex3d(m_plX, m_plY, m_plZ);
			double plX = m_plX+sin(DegToRad(m_plRotY)) * m_diag/5*cos(DegToRad(m_plRotV));
			double plY = m_plY-cos(DegToRad(m_plRotX)) * m_diag/5;
			double plZ = m_plZ-cos(DegToRad(m_plRotY)) * m_diag/5*cos(DegToRad(m_plRotV));
			glVertex3d(plX, plY, plZ);
		glEnd();
	glPopMatrix();
}


// overide to draw custom small coordinatte sysem
void CViewGL::DrawCoordSys(void)
{
	CRect rc;
	GetClientRect( &rc);

	if( rc.Height()<100 || rc.Width()<100 )
		return;

 	glFlush();
	// coord. sys
	glViewport(0, 0, 60, 60);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1.74, 1.74, -1.74, 1.74, 0.01, 5);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glLoadIdentity();
		
		glTranslatef(0.0f, 0.0f, -2.2f);
		if(m_bPlane) {
			glRotated(m_plRotX, 1.0f, 0.0f, 0.0f);
			glRotated(m_plRotY+180, 0.0f, 1.0f, 0.0f);
			glRotated(m_plRotZ, 0.0f, 0.0f, 1.0f);
		}
		else {
			glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
			glRotatef((m_nProjection==projOrtho ? 180 : 0)+m_fRotY, 0.0f, 1.0f, 0.0f);
			glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
		}
		GLdouble minX=-0.6f, maxX=0.6f, 
				minY=-0.6f, maxY=0.6f, 
				minZ=-0.6f, maxZ=0.6f;

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);;
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glPointSize(2);
		glColor3f(0.5f, 0.5f, 0.5f);

		glLineWidth( 1.0 );
		glColor3f(0.5f, 0.5f, 0.5f);
		GLdouble a = 0.4f, c = 0.15f;
 		glBegin(GL_LINES);
			// x gl-axis
 			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3d(minX-a, minY, minZ);
			glVertex3d(maxX, minY, minZ);

			// y gl-axis
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3d(maxX, minY, minZ);
			glVertex3d(maxX, minY, maxZ+a);

			// z gl-axis
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3d(maxX, minY, minZ);
			glVertex3d(maxX, maxY+a, minZ);
		glEnd();
		glEnable(GL_LIGHTING);

		GLUquadricObj*	pCone;
		float *pColor = new float[4];
		pCone = gluNewQuadric();

		// Cone for z-gl-axis
		pColor[0] = 0.0f;
		pColor[1] = 0.0f;
		pColor[2] = 1.0f;
		pColor[3] = 1.0f;
		
		glColor4fv(pColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pColor);
		glPushMatrix(); 
			glTranslated(maxX, minY , maxZ + a);
			gluCylinder(pCone, c, 0.0f, a, 6, 4);
			gluQuadricNormals(pCone, GLU_SMOOTH);
		glPopMatrix();

		// Cone for y-gl-axis
		pColor[0] = 1.0f;
		pColor[1] = 0.0f;
		pColor[2] = 0.0f;
		pColor[3] = 1.0f;
		glColor4fv(pColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pColor);
		glPushMatrix(); 
			glTranslated(maxX, maxY+a, minZ );
			glRotated(-90.0f ,1.0f, 0.0f, 0.0f ); 
			gluCylinder(pCone, c, 0.0f, a, 8, 8);
			gluQuadricNormals(pCone, GLU_SMOOTH);
		glPopMatrix();

		// Cone for x-gl-axis
		pColor[0] = 0.0f;
		pColor[1] = 1.0f;
		pColor[2] = 0.0f;
		pColor[3] = 1.0f;
		glColor4fv(pColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pColor);
		glPushMatrix(); 
			glTranslated(minX-a, minY, minZ );
			glRotated(-90.0f ,0.0f, 1.0f, 0.0f ); 
			gluCylinder(pCone, c, 0.0f, a, 8, 8);
			gluQuadricNormals(pCone, GLU_SMOOTH);
		glPopMatrix();

		delete pColor;
		glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();

	glFlush();
}

// override this function to draw custom model prism
void CViewGL::DrawModelPrism(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);;
	glDisable(GL_LIGHTING);
	glPointSize(2);
	glColor3f(0.5f, 0.5f, 0.5f);

	glBegin(GL_POINTS);
		glVertex3d(m_minX, m_maxY, m_maxZ);
		glVertex3d(m_minX, m_minY, m_maxZ);
		glVertex3d(m_maxX, m_maxY, m_maxZ);
		glVertex3d(m_maxX, m_minY, m_maxZ);
		glVertex3d(m_maxX, m_maxY, m_minZ);
		glVertex3d(m_maxX, m_minY, m_minZ);
		glVertex3d(m_minX, m_maxY, m_minZ);
		glVertex3d(m_minX, m_minY, m_minZ);
	glEnd();

	glLineWidth( 1.0 );
	glColor3f(0.5f, 0.5f, 0.5f);
	GLdouble a = m_diag/60.0f, b = m_diag/80.0f, c = m_diag/180.0f;
	glBegin(GL_LINES);
		glVertex3d(m_maxX, m_maxY, m_maxZ);
		glVertex3d(m_maxX, m_minY, m_maxZ);

		glVertex3d(m_maxX, m_maxY, m_minZ);
		glVertex3d(m_maxX, m_minY, m_minZ);

		glVertex3d(m_minX, m_maxY, m_minZ);
		glVertex3d(m_minX, m_minY, m_minZ);

		// x gl-axis
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3d(m_minX, m_minY, m_maxZ);
		glVertex3d(m_maxX+a, m_minY, m_maxZ);

		// y gl-axis
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3d(m_minX, m_minY, m_maxZ);
		glVertex3d(m_minX, m_minY, m_minZ-a);

		// z gl-axis
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3d(m_minX, m_minY, m_maxZ);
		glVertex3d(m_minX, m_maxY+a, m_maxZ);
	glEnd();

	glEnable(GL_LIGHTING);
	GLUquadricObj*	pCone;
	float *pColor = new float[4];
	pCone = gluNewQuadric();

	// Cone for z-gl-axis
	pColor[0] = 0.0f;
	pColor[1] = 0.0f;
	pColor[2] = 1.0f;
	pColor[3] = 1.0f;
	glColor4fv(pColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pColor);
	glPushMatrix(); 
		glTranslated(m_minX, m_minY , m_minZ - a + b);
		glRotatef(180.0f ,0.0f, 1.0f, 0.0f ); 
		gluCylinder(pCone, c, 0.0f, b, 8, 8);
		gluQuadricNormals(pCone, GLU_SMOOTH);
	glPopMatrix();

	// Cone for y-gl-axis
	pColor[0] = 1.0f;
	pColor[1] = 0.0f;
	pColor[2] = 0.0f;
	pColor[3] = 1.0f;
	glColor4fv(pColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pColor);
	glPushMatrix(); 
		glTranslated(m_minX, m_maxY+a-b, m_maxZ );
		glRotatef(-90.0f ,1.0f, 0.0f, 0.0f ); 
		gluCylinder(pCone, c, 0.0f, b, 8, 8);
		gluQuadricNormals(pCone, GLU_SMOOTH);
	glPopMatrix();

	// Cone for x-gl-axis
	pColor[0] = 0.0f;
	pColor[1] = 1.0f;
	pColor[2] = 0.0f;
	pColor[3] = 1.0f;
	glColor4fv(pColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pColor);
	glPushMatrix(); 
		glTranslated(m_maxX+a-b, m_minY, m_maxZ );
		glRotatef(90.0f ,0.0f, 1.0f, 0.0f ); 
		gluCylinder(pCone, c, 0.0f, b, 8, 8);
		gluQuadricNormals(pCone, GLU_SMOOTH);
	glPopMatrix();

	delete pColor;
	glColor3f(1.0f, 1.0f, 1.0f);
}

void CViewGL::InitTextLabels(void)
{
	m_minXl = m_minX;
	m_minYl = m_minY;
	m_minZl = m_minZ;
	//FindLabels(m_minXl, m_maxX, m_stepXl, m_decXl);
	//FindLabels(m_minYl, m_maxY, m_stepYl, m_decYl);
	//FindLabels(m_minZl, m_maxZ, m_stepZl, m_decZl);

	int digM, digN;
	double start;
	FindAxisMinMaxStep(m_minXl, m_maxX, m_stepXl, start, digM, digN, 4);
	m_minXl = start;
	m_decXl = digN;
	FindAxisMinMaxStep(m_minYl, m_maxY, m_stepYl, start, digM, digN, 4);
	m_minYl = start;
	m_decYl = digM;
	FindAxisMinMaxStep(m_minZl, m_maxZ, m_stepZl, start, digM, digN, 4);
	m_minZl = start;
	m_decZl = digM;
}

// draws axis labels 2D or 3D text
void CViewGL::DrawAxisLabels(void)
{
	if( !m_bAxisLabels ) return;
	if( m_nTextType == GLTXT_NONE ) return;

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glPointSize(2.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		
	CString str;
	CString strFormat;
	CString strDec;
	float colorText[4] = {0.0f, 0.0f, 0.0f, 1.0f};		// front facet as seen from outside the body

	double x, y, z;
	x = m_minXl;
	while(x < m_maxX) {
		strDec.Format(_T("%d"), m_decXl);
		strFormat = _T(" %.") + strDec + _T("f");
		str.Format(strFormat, x);
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
			glVertex3d(x, m_minY, m_maxZ);
		glEnd();

		// 3D text
		if( m_nTextType == GLTXT_3D ) {
			glPushMatrix();
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colorText);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorText);
				glTranslated(x, m_minY, m_maxZ);
				glRotated(-90.0f, 0.0f, 1.0f, 0.0f);
				glRotated(-45.0f, 1.0f, 0.0f, 0.0f);
				glScaled((m_maxX-m_minX)/20, (m_maxY-m_minY)/10, 0.0f);
				Text3D(str);								// Print GL Text To The Screen
			glPopMatrix();
		}

		// 2D text
		if( m_nTextType == GLTXT_2D ) {
			glRasterPos3d(x, m_minY, m_maxZ);
			Text2D(str);	
		}// Print GL Text To The Screen

		x += m_stepXl;
	}

	// y GL coordinate
	y = m_minYl;
	while(y < m_maxY) {
		strDec.Format(_T("%d"), m_decXl);
		strFormat = _T(" %.") + strDec + _T("f");
		str.Format(strFormat, y);
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
			glVertex3d(m_minX, y, m_maxZ);
		glEnd();
		// 3D text
		if( m_nTextType == GLTXT_3D ) {
			glPushMatrix();
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colorText);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorText);
				glTranslated(m_minX, y, m_maxZ);
				glRotated(-135.0f, 0.0f, 1.0f, 0.0f);
				glRotated(0.0f, 1.0f, 0.0f, 0.0f);
				glScaled((m_maxX-m_minX)/20, (m_maxY-m_minY)/10, 0.0f);
				Text3D(str);								// Print GL Text To The Screen
			glPopMatrix();
		}

		// 2D text
		if( m_nTextType == GLTXT_2D ) {
			glRasterPos3d(m_minX, y, m_maxZ);
			Text2D(str);							// Print GL Text To The Screen
		}

		y +=m_stepYl;
	}

	// z GL coordinate
	z = m_minZl;
	while(z < m_maxZ) {
		strDec.Format(_T("%d"), m_decZl);
		strFormat = _T(" %.") + strDec + _T("f"); 
		str.Format(strFormat, -z);
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
			glVertex3d(m_minX, m_minY, z);
		glEnd();

		// 3D text
		if( m_nTextType == GLTXT_3D ) {
			float fLen=0.0f;
			int txtLen;
			if( (txtLen=str.GetLength()) >0) {
				// calculate and return the length of the produced outlines
				for(int c=0; c<txtLen; c++) {
					fLen += m_gmfvector[char(str[c])].gmfCellIncX;
				}
			}
			glPushMatrix();
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colorText);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorText);
				glTranslated(m_minX-fLen*(m_maxX-m_minX)/20, m_minY, z);
				glRotated(0.0f, 0.0f, 1.0f, 0.0f);
				glRotated(-45.0f, 1.0f, 0.0f, 0.0f);
				glScaled((m_maxX-m_minX)/20, (m_maxY-m_minY)/10, 0.0f);
				Text3D(str);								// Print GL Text To The Screen
			glPopMatrix();
		}

		// 2D text
		if( m_nTextType == GLTXT_2D ) {
			glRasterPos3d(m_minX, m_minY, z);
			Text2D(str);							// Print GL Text To The Screen
		}
		z +=m_stepZl;
	}
}
//+++++ end drawing functions +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




// CViewGL message handlers

//------- mouse messages --------------------------------------------------------------------
void CViewGL::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptOld = point;

	CView::OnLButtonDown(nFlags, point);
}

void CViewGL::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
}

void CViewGL::OnMouseMove(UINT nFlags, CPoint point)
{
	int xDif, yDif;

	xDif = point.x - m_ptOld.x;  if(xDif!=0) (xDif<0 ? xDif=-1+xDif/10 : xDif=1+xDif/10);
	yDif = point.y - m_ptOld.y;  if(yDif!=0) (yDif<0 ? yDif=-1+yDif/10 : yDif=1+yDif/10);

	CRect rcClient;
	GetClientRect( &rcClient );
	double drx, dry;
	// rotation in degrees relative to client rectangle
	dry = 360.0 * double(point.x - m_ptOld.x)/rcClient.Width();
	drx = 360.0 * double(point.y - m_ptOld.y)/rcClient.Height();

	if( m_nProjection == projPerspective ) {
		switch( nFlags ) {
		case MK_LBUTTON:
			{
			if(!m_bPlane) {
				m_fRotX = (float)AngleDeg( m_fRotX+drx);
				m_fRotY = (float)AngleDeg( m_fRotY+dry);
			}
			// plane mode
			else {
				m_plRotY = AngleDeg(m_plRotY+dry);
				m_plRotV+=drx;
				if(m_plRotV>90.0) m_plRotV=90.0;
				if(m_plRotV<-90.0) m_plRotV=-90.0;
				m_plRotX = m_plRotV;
			}

			Invalidate(FALSE);
			}
			break;
		case MK_LBUTTON | MK_CONTROL | MK_SHIFT:
			{
			if(!m_bPlane) {
				m_fRotX = (float)AngleDeg( m_fRotX+drx);
				m_fRotY = (float)AngleDeg( m_fRotY+dry);
			}
			// plane mode
			else {
				m_plRotY = AngleDeg(m_plRotY+dry);
				m_plRotV+=drx;
				if(m_plRotV>90.0) m_plRotV=90.0;
				if(m_plRotV<-90.0) m_plRotV=-90.0;
				m_plRotX = m_plRotV;
			}

			Invalidate(FALSE);
			}
			break;
		case MK_LBUTTON | MK_CONTROL:
			{
			if(!m_bPlane) {
				m_dCameraZ += yDif * m_mZ;
			}
			// plane mode
			else {
				m_plRotY = AngleDeg(m_plRotY+dry);
				m_plRotV+=drx;
				if(m_plRotV>90.0) m_plRotV=90.0;
				if(m_plRotV<-90.0) m_plRotV=-90.0;
				m_plRotX = m_plRotV;
			}

			Invalidate( FALSE );
			}
			break;
		case MK_LBUTTON | MK_SHIFT:
			{
			if(!m_bPlane) {
				m_dCameraY -= yDif * m_mY;
				m_dCameraX += xDif * m_mX;
			}
			else {
				m_plY = m_plY+m_pld;
			}

			Invalidate( FALSE );
			}
			break;
		}
	}
		
	if( m_nProjection == projOrtho ) {
		switch( nFlags ) {
		case MK_LBUTTON:
			{
			m_fRotX = (float)AngleDeg( m_fRotX+drx);
			m_fRotY = (float)AngleDeg( m_fRotY+dry);
			Invalidate(FALSE);
			}
			break;
		case MK_LBUTTON | MK_CONTROL | MK_SHIFT:
			{
			m_fRotX = (float)AngleDeg(m_fRotX+drx);
			m_fRotY = (float)AngleDeg( m_fRotY+dry);
			Invalidate(FALSE);
			}
			break;
		case MK_LBUTTON | MK_CONTROL:
			{
			if(yDif!=0.0)
				m_dOrthoScale *= 1 - 0.01*fabs(double(yDif))/yDif;
			Invalidate( FALSE );
			}
			break;
		case MK_LBUTTON | MK_SHIFT:
			{
			//Invalidate( FALSE );
			}
			break;
		}
	}
	
	m_ptOld = point;

	CView::OnMouseMove(nFlags, point);
}
//------- end mouse messages ----------------------------------------------------------------


void CViewGL::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	switch(lHint) {
		case UPDATE_MODEL_NEW:
			m_bUpdateFld = TRUE;
			OnInitialUpdate();
			break;
		case UPDATE_MODEL_FLD:
			m_bUpdateFld = TRUE;
			Invalidate(FALSE);
			break;
		default:
			Invalidate(FALSE);
	}
}

void CViewGL::OnEditColors()
{
	CDlgColorGrad dlg;

	dlg.m_pGrad = &m_clrGrad;
	//dlg.m_pView = this;

	dlg.DoModal();
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		m_clrGrad.CreateColorTableGL(m_pCltTex.data(), m_nCltTexWidth, 4, 1.0f);
		UpdateTexture(m_nCltTex, m_pCltTex.data(), m_nCltTexWidth);
	wglMakeCurrent(NULL, NULL);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//****** GL Text ************************************************************************************************************************

GLvoid	CViewGL::PrepareCharset2D(const LOGFONT* pLf)
{
	// debug checks
	ASSERT(pLf!=NULL);

	// ask for the font
	CFont	font;
	CFont*	pDefFont;
	if(!font.CreateFontIndirect(pLf))
	{
		TRACE(_T("CGLEnabledView::PrepareCharset2D:\n\tUnable to create a font from the passed logical font descriptor.\n"));
		return;
	}
	//select the font into the DC
	pDefFont = m_pDC->SelectObject(&font);
	// create or recreate the display lists
	if(m_charsetDListBase2D>0) 
		glDeleteLists(m_charsetDListBase2D, m_nChars);	
	if (0 == (m_charsetDListBase2D=glGenLists(m_nChars)))
	{
		TRACE(_T("CGLEnabledView::PrepareCharset2D:\n\tUnable to create the charset (no memory for display lists).\n"));
		goto end;
	};
	// build the charset display lists
	if(!wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, m_nChars-1, m_charsetDListBase2D))
	{
		TRACE(_T("CGLEnabledView::PrepareCharset2D:\n\tUnable to create the charset (internal error).\n"));
		glDeleteLists(m_charsetDListBase2D, m_nChars);
		m_charsetDListBase2D=0;
	};
	// deselect and dispose of the GDI font
end:
	m_pDC->SelectObject(pDefFont);
	font.DeleteObject();
};

GLvoid	CViewGL::Text2D(CString str)
{
	int textlen=0;

	if(m_charsetDListBase2D!=0)
	{
		if( (textlen=str.GetLength()) >0)
		{
			// output the outlines corresponding to the requested text srting
			glListBase(m_charsetDListBase2D);
			glCallLists(textlen, GL_UNSIGNED_BYTE, LPCTSTR(str));
		}
	}
	else TRACE(_T("CGLEnabledView::Text2D:\n\tNo charset available. Use PrepareCharset2D routines first.\n"));

};

void CViewGL::PrepareCharset3D(const LOGFONT* pLF, float extrusion, BOOL uselines, float precision)
{
	// debug checks
	ASSERT(extrusion>=0.f && precision>=0.f);
	ASSERT(pLF!=NULL);
	// variables initialization
	int mode=uselines ? WGL_FONT_LINES : WGL_FONT_POLYGONS;
	if(m_gmfvector.empty()) 
		m_gmfvector.resize(m_nChars);
	// ask for the font
	CFont fnt;
	if(!fnt.CreateFontIndirect(pLF))
	{
		TRACE(_T("CGLEnabledView::PrepareCharset3D:\n\tUnable to create a font from the passed logical font descriptor.\n"));
		return;
	}
	//select the font into the DC
	CFont* pDefFont = m_pDC->SelectObject(&fnt);
	// create or recreate the display lists
	if(m_charsetDListBase3D>0) 
		glDeleteLists(m_charsetDListBase3D, m_nChars);	
	if (0 == (m_charsetDListBase3D=glGenLists(m_nChars)))
	{
		TRACE(_T("CGLEnabledView::PrepareCharset3D:\n\tUnable to create the charset (no memory for display lists).\n"));
		goto end;
	};
	// build the charset display lists
	if(!wglUseFontOutlines(m_pDC->GetSafeHdc(), 
		0, 
		m_nChars-1,					// number of display lists
		m_charsetDListBase3D,		// display lists base
		precision,					// deviation from true outlines
		extrusion,					// font thickness in z direection
		mode,						// mode - polygons or lines
		m_gmfvector.data()))				// buffer to recieve data
	{
		TRACE(_T("CGLEnabledView::PrepareCharset3D:\n\tUnable to create the charset (internal error).\n"));
		glDeleteLists(m_charsetDListBase3D, m_nChars);
		m_charsetDListBase3D=0;
	};
	// deselect and dispose of the GDI font
end:
	m_pDC->SelectObject(pDefFont);
	fnt.DeleteObject();
}

// dir is direstion write text from begining to end dir == 1
//	reverse text dir == -1
float CViewGL::Text3D(CString str)
{
	float	retLen=0.0f;
	int		txtLen=0;

	if(m_charsetDListBase3D!=0)
	{
		if( (txtLen=str.GetLength()) >0) {	
			// output the outlines corresponding to the requested text srting
			glListBase(m_charsetDListBase3D);
			glCallLists(txtLen, GL_UNSIGNED_BYTE, LPCTSTR(str));
			// calculate and return the length of the produced outlines
			for(int c=0; c<txtLen; c++) {
				retLen += m_gmfvector[char(str[c])].gmfCellIncX;
			}
		}
	}
	else TRACE(_T("CGLEnabledView::Text3D:\n\tNo charset available. Use PrepareCharset3D routines first.\n"));
	return retLen;
}
///////////////////////////////////////////////////////////////////////////////////////////////////



void CViewGL::OnModelFirst()
{
	m_nWFirst = 0;
	Invalidate(FALSE);
}

void CViewGL::OnUpdateModelFirst(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CViewGL::OnModelLast()
{
	m_nWFirst = m_pDoc->m_nModels-1;
	Invalidate(FALSE);
}

void CViewGL::OnUpdateModelLast(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CViewGL::OnModelNext()
{
	int n;
	m_pDoc->m_inv.GetModels(n);
	if(	m_nWFirst < n-1 ) {
		m_nWFirst++;
		Invalidate(FALSE);
	}
}

void CViewGL::OnUpdateModelNext(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CViewGL::OnModelPrevious()
{
	if(	m_nWFirst > 0 ) {
		m_nWFirst--;
		Invalidate(FALSE);
	}
}

void CViewGL::OnUpdateModelPrevious(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CViewGL::OnViewShowtext()
{
	m_bAxisLabels = !m_bAxisLabels;
	Invalidate(FALSE);
}

void CViewGL::OnUpdateViewShowtext(CCmdUI *pCmdUI)
{
	if( m_bAxisLabels )		pCmdUI->SetCheck(1);
	else					pCmdUI->SetCheck(0);
}

void CViewGL::OnViewShowField()
{
	m_bField = !m_bField;
	Invalidate(FALSE);
}

void CViewGL::OnUpdateViewShowField(CCmdUI *pCmdUI)
{
	if(m_bField)	pCmdUI->SetCheck(1);
	else			pCmdUI->SetCheck(0);
}

void CViewGL::OnViewShowSlices()
{
	m_bSlices = !m_bSlices;
	Invalidate(FALSE);
}

void CViewGL::OnUpdateViewShowSlices(CCmdUI *pCmdUI)
{
	if(m_bSlices)	pCmdUI->SetCheck(1);
	else			pCmdUI->SetCheck(0);
}

void CViewGL::OnViewShowVoxels()
{
	m_bVoxels = !m_bVoxels;
	Invalidate(FALSE);
}

void CViewGL::OnUpdateViewShowVoxels(CCmdUI *pCmdUI)
{
	if(m_bVoxels)	pCmdUI->SetCheck(1);
	else			pCmdUI->SetCheck(0);
}

void CViewGL::OnViewSliceNew()
{
	CDlgSlice dlg;

	dlg.m_nProfileMaxXY = m_pDoc->m_nZ;
	dlg.m_nProfileMaxXZ = m_pDoc->m_nY;
	dlg.m_nProfileMaxYZ = m_pDoc->m_nX;
	if( dlg.DoModal() == IDOK ) {
		CSlice3D* pSlice = new CSlice3D;
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
		m_slices.Add( pSlice );
		m_nSliceSel = (int)m_slices.GetUpperBound();
	}
}

void CViewGL::OnEditSlices()
{
	CDlgSliceManager dlg;

	dlg.m_nSliceSel = m_nSliceSel;
	dlg.m_pSlices = &m_slices;
	dlg.m_nProfileMaxXY = m_pDoc->m_nZ;
	dlg.m_nProfileMaxXZ = m_pDoc->m_nY;
	dlg.m_nProfileMaxYZ = m_pDoc->m_nX;

	dlg.DoModal();
	m_nSliceSel = dlg.m_nSliceSel;

	Invalidate(FALSE);
}

void CViewGL::OnUpdateEditSlices(CCmdUI *pCmdUI)
{
	if(m_slices.IsEmpty())	pCmdUI->Enable(FALSE);
	else					pCmdUI->Enable(TRUE);
}

void CViewGL::OnViewSliceNext()
{
	if(m_slices.IsEmpty())
		return;

	if( m_nSliceSel>=0 && m_nSliceSel<m_slices.GetCount() ) {
		CSlice3D* pSlc = m_slices.GetAt(m_nSliceSel);
		int max;
		switch( pSlc->GetType() ) {
			case 0: // xz
				max = m_pDoc->m_nY;
				break;
			case 1: // yz
				max = m_pDoc->m_nX;
				break;
			case 2: // xy
				max = m_pDoc->m_nZ;
				break;
		}
		if((pSlc->GetProf()+1)<max)
			pSlc->SetProf( pSlc->GetProf()+1 );
	}
	Invalidate(FALSE);
}

void CViewGL::OnUpdateViewSliceNext(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CViewGL::OnViewSlicePrevious()
{
	if(m_slices.IsEmpty())
		return;

	if( m_nSliceSel>=0 && m_nSliceSel<m_slices.GetCount() ) {
		CSlice3D* pSlc = m_slices.GetAt(m_nSliceSel);
		if( (pSlc->GetProf()-1)>=0 )
			pSlc->SetProf( pSlc->GetProf()-1 );
	}
	Invalidate(FALSE);
}

void CViewGL::OnUpdateViewSlicePrevious(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CViewGL::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame* pMain;
	pMain = (CMainFrame*)AfxGetMainWnd();
	
	pMain->SetReBarEditVal(IDC_EDIT_TRESHOLD, m_dTreshold);
	pMain->SetReBarComboVal(IDC_COMBO_COLS, m_nWCols);
	pMain->SetReBarComboVal(IDC_COMBO_ROWS, m_nWRows);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CViewGL::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	double l;
	switch(nChar) {
		case VK_UP:
			if(!m_bPlane) {
				m_dCameraZ -= m_mZ;
			}
			else {
				l = m_pld*cos(DegToRad(m_plRotV));
				m_plX += l*sin(DegToRad(m_plRotY));
				m_plY -= m_pld*sin(DegToRad(m_plRotV));
				m_plZ -= l*cos(DegToRad(m_plRotY));
				//m_plX += sin(DegToRad(m_plRotY)) * m_pld;
				//m_plY -= sin(DegToRad(m_plRotX)) * m_pld;
				//m_plZ -= cos(DegToRad(m_plRotY)) * m_pld;
			}
			Invalidate(FALSE);
			break;
		case VK_DOWN:
			if(!m_bPlane) {
				m_dCameraZ += m_mZ;
			}
			else {
				l = m_pld*cos(DegToRad(m_plRotV));
				m_plX -= l*sin(DegToRad(m_plRotY));
				m_plY += m_pld*sin(DegToRad(m_plRotV));
				m_plZ += l*cos(DegToRad(m_plRotY));
			}
			Invalidate(FALSE);
			break;
		case VK_RIGHT:
			if(!m_bPlane) {
				m_fRotY += (int) (m_rY);
			}
			else {
				m_plRotY = AngleDeg(m_plRotY+m_plRotdY);
			}
			Invalidate(FALSE);
			break;
		case VK_LEFT:
			if(!m_bPlane) {
				m_fRotY -= (int) (m_rY);
			}
			else {
				m_plRotY = AngleDeg(m_plRotY-m_plRotdY);
			}
			Invalidate(FALSE);
			break;
		case VK_NEXT:	// page down
			if(!m_bPlane) {
				m_dCameraY -= m_mY;
			}
			else {
				m_plY -= m_pld;
			}
			Invalidate(FALSE);
			break;
		case VK_PRIOR:	// [page up
			if(!m_bPlane) {
				m_dCameraY += m_mY;
			}
			else {
				m_plY += m_pld;
			}
			Invalidate(FALSE);
			break;
		case 69:	// E
			if(!m_bPlane) {
				m_fRotX = 0.0f;
				m_fRotY = 90.0f;
				m_fRotZ = 0.0f;
				Invalidate(FALSE);
			}
			break;
		case 87:	// W
			if(!m_bPlane) {
				m_fRotX = 0.0f;
				m_fRotY = 270.0f;
				m_fRotZ = 0.0f;
				Invalidate(FALSE);
			}
			break;
		case 78:	// N
			if(!m_bPlane) {
				m_fRotX = 0;
				m_fRotY = 0.0f;
				m_fRotZ = 0;
				Invalidate(FALSE);
			}
			break;
		case 83:	// S
			if(!m_bPlane) {
				m_fRotX = 0;
				m_fRotY = 180.0f;
				m_fRotZ = 0;
				Invalidate(FALSE);
			}
			break;
		case 77:	// M
			if(!m_bPlane) {
				m_fRotX = 90.0f;
				m_fRotY = 180.0f;
				m_fRotZ = 0.0f;
				Invalidate(FALSE);
			}
			break;
		case VK_ESCAPE:
			break;
	}

	
	//CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CViewGL::OnViewSliceNextActivate()
{
	if(m_slices.IsEmpty())
		return;

	m_nSliceSel++;
	if( m_nSliceSel>m_slices.GetUpperBound() ) {
		m_nSliceSel=0;
		
	}
	Invalidate(FALSE);
}

void CViewGL::OnUpdateViewSliceNextActivate(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CViewGL::OnViewSlicePreviousActivate()
{
	if(m_slices.IsEmpty())
		return;

	m_nSliceSel--;
	if( m_nSliceSel<0 ) {
		m_nSliceSel=(int)m_slices.GetUpperBound();
		
	}
	Invalidate(FALSE);
}

void CViewGL::OnUpdateViewSlicePreviousActivate(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CViewGL::GlDefaultMaterial(void)
{
	float m[4]={0.2f, 0.2f, 0.2f, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m);
	m[0]=0.8f;	m[1]=0.8f;	m[2]=0.8f;	m[3]=1.0f;	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m);
	m[0]=0.0f;	m[1]=0.0f;	m[2]=0.0f;	m[3]=1.0f;	
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
}

// // loads grid from document to this view structures
void CViewGL::LoadGridSurface(CGrid* pGrd, CGlSurface* pSrf, int nClrType, double min, double max, BOOL bRelief)
{
	ASSERT(wglGetCurrentContext() != NULL);

	if( pGrd->IsEmpty() || pGrd->IsPlanar() ) 	return;

	pSrf->SetSize(pGrd->GetRows(), pGrd->GetCols());
	pGrd->CreateColorTableGL(pSrf->m_tex1D, pSrf->m_nTexWidth, pSrf->m_nTexFormat, nClrType);
	pSrf->UpdateTexture();
	BOOL bOK=FALSE;
	//if( bRelief ) {
	//	CGrid* pRel=m_pDoc->GetReliefGrid();
	//	bOK = pGrd->GetGlRel(&pSrf->m_srfData, pSrf->m_nSrfFormat, min, max, pRel);
	//}
	if(!bRelief || !bOK) {
		pGrd->GetGL(&pSrf->m_srfData, pSrf->m_nSrfFormat, min, max);
	}

	//double minH, maxH,
	pGrd->GetHistogramMinMax(min, max);
	pSrf->SetDataRange(min, max);
	//pSrf->CompileDisplayList(min, max, bRelief);

}


void CViewGL::OnEditField()
{
	m_pDoc->m_grdFld.Properties();
	m_pDoc->UpdateAllViews(NULL, UPDATE_MODEL_FLD);
}

