// CViewGL.h : interface of the CViewGL class
//


#pragma once

#include <memory>
#include <vector>

// OpenGL includes
#include "gl/gl.h"
#include "gl/glu.h"

#include "ClrGrad\gradient.h"

#include "Slice3D.h"
#include "glsurface.h"

class CViewGL : public CView
{
	enum TextTypeGL{
		GLTXT_NONE = 0,
		GLTXT_2D = 1,
		GLTXT_3D = 2,
	};

	enum projection {
		projOrtho,
		projPerspective
	};

protected: // create from serialization only
	CViewGL();
	virtual void	OnInitialUpdate();
	DECLARE_DYNCREATE(CViewGL)

//***********************************************************************************
private:
	CClientDC*	m_pDC;	// WinGDI Device Context
	HGLRC	m_hRC;	// OpenGL Rendering Context
	BOOL	m_bExternGLCall;
	BOOL	m_bExternDispListCall;
	BOOL	SetupPixelFormat();
	int		m_glErrorCode;

	CDC* GetCDC() {return m_pDC;};
//***********************************************************************************

//********************************************************************************************************
	// Model extent in GL coordinates
	GLdouble m_minX, m_maxX;
	GLdouble m_maxY, m_minY;		
	GLdouble m_minZ, m_maxZ;		
	GLdouble m_midX, m_midY, m_midZ;		// model middle point
	GLdouble m_lenX, m_lenY, m_lenZ;		// axis model lengths
	GLdouble	m_diag;						// model diagonal

	// projection
	int		m_nProjection;
	double	m_dOrthoScale;			// orthogonal projection scaling

	// Model rotation - in degrees
	float	m_fRotX , m_fRotY , m_fRotZ;

	// Centered-transformation
	GLdouble m_tX, m_tY, m_tZ;

	// Constants for motion and rotation
	float	m_mX,	m_mY, m_mZ;	
	float	m_rX, m_rY, m_rZ; 

	// Constants for plane flight motion and rotation
	// in GL coordinates
	double	m_plX,	m_plY, m_plZ;						// PLane x, y, z position
	double	m_pld;										// PLane motion
	double	m_plRotX, m_plRotY, m_plRotZ, m_plRotV;		// rotations
	double	m_plRotdX, m_plRotdY, m_plRotdZ, m_plRotdV;		// rotations
	BOOL	m_bPlane;
	int		m_nTimer;

	// Camera position
	GLdouble m_dCameraX , m_dCameraY , m_dCameraZ;
	GLdouble m_dCameraEyeZ;
	GLdouble m_dProjAngle;					// perspective projection angle

	CRect	m_oldRect;						// client rectangle
	CPoint	m_ptOld;					// previous mouse clicked point; used in translation, rotation

	// light position
	double	m_dLightIncl;
	double	m_dLightAzim;
	
	// fonts
	GLuint		m_charsetDListBase2D;				// Base Display List For The Font Set
	GLuint		m_charsetDListBase3D;				// Base Display List For The Font Set
	int			m_nChars;
	LOGFONT		m_lf;
	//GLYPHMETRICSFLOAT* m_gmfvector;
	std::vector<GLYPHMETRICSFLOAT> m_gmfvector;
	BOOL		m_bAxisLabels;

	CGradient	m_clrGrad;
	std::vector<GLfloat>	m_pCltTex;		// color legend texture
	GLuint		m_nCltTex;		// textute display list
	//std::unique_ptr<GLfloat>	m_pCltFld;		// color legend texture
	std::vector<GLfloat>	m_pCltFld;
	GLuint		m_nCltFld;		// textute display list
	GLuint		m_nCltTexWidth;	


	// scaling
	double m_dScZ;

	// text labels in GL coordinates
	GLdouble m_minXl, m_minYl, m_minZl, m_stepXl, m_stepYl, m_stepZl;	
	GLuint	m_decXl, m_decYl, m_decZl;
	int		m_nTextType;

	BOOL m_bVoxels;
	BOOL m_bSlices;
	BOOL m_bField;

	// window division
	int		m_nWRows;
	int		m_nWCols;
	int		m_nWFirst;
	int		m_nWCurrent;

	// slices
	SlcPtrArray m_slices;
	int		m_nSliceSel;

	// value treshold
	BOOL	m_bTreshold;
	double	m_dTreshold;

	//
	CGlSurface m_srfFld;
	BOOL	m_bUpdateFld;
//********************************************************************************************************

//********************************************************************************************************
public:
	void			OnCreateGL();
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnDestroy();
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);

	void	SetupLights(BOOL bEnable);
	void	SetProjectionMatrix(int w, int h);
	void	SetProjectionMatrix(CRect rcScene);

	void	DrawScene(CRect rcScene);
	//void	DrawScene();
	void	DrawSceneMulti();
	void	DrawOrientation();

	void	DrawCoordSys(void);
	void	DrawModelPrism(void);
	void	DrawModelVoxels();
	void	DrawLegend(void);
	void	DrawLegend(int x, int y, int w, int h, double minData, double maxData, int nTex, CString strName);
	void	DrawModelSlice(int nTag, int nSlice, float fAlpha, int nIndex);

	void	InitTextLabels(void);
	void	DrawAxisLabels(void);

	// 2D text
	GLvoid	PrepareCharset2D(const LOGFONT* pLf);
	GLvoid	Text2D(CStringA str);

	// 3D text
	void	PrepareCharset3D(const LOGFONT* pLf, float extrusion, BOOL uselines=false, float precision=0.01f);
	float	Text3D(CString str);

//********************************************************************************************************

	// Attributes
public:
	CInv3DDoc* GetDocument() const;
	CInv3DDoc* m_pDoc;

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
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

public:
	void UpdateTexture(GLuint& nTex, GLfloat* pTex, GLuint nWidth);
	afx_msg void OnEditColors();

	// rebar messages
	afx_msg void OnCbnEditchangeCmbFocus();
	afx_msg void OnCbnSelokCmbFocus();
	afx_msg void OnCbnEditchangeCmbRows();
	afx_msg void OnCbnSelokCmbRows();
	afx_msg void OnCbnEditchangeCmbCols();
	afx_msg void OnCbnSelokCmbCols();
	afx_msg void OnEditchangeTreshold();

protected:
	int m_nModFcs;		// focusing iteration to display
public:
	afx_msg void OnModelFirst();
	afx_msg void OnUpdateModelFirst(CCmdUI *pCmdUI);
//	afx_msg void OnModelInvert();
	afx_msg void OnModelLast();
	afx_msg void OnUpdateModelLast(CCmdUI *pCmdUI);
	afx_msg void OnModelNext();
	afx_msg void OnUpdateModelNext(CCmdUI *pCmdUI);
	afx_msg void OnModelPrevious();
	afx_msg void OnUpdateModelPrevious(CCmdUI *pCmdUI);
	afx_msg void OnViewShowtext();
	afx_msg void OnUpdateViewShowtext(CCmdUI *pCmdUI);
	afx_msg void OnViewShowSlices();
	afx_msg void OnUpdateViewShowSlices(CCmdUI *pCmdUI);
	afx_msg void OnViewShowVoxels();
	afx_msg void OnUpdateViewShowVoxels(CCmdUI *pCmdUI);
	afx_msg void OnViewSliceNew();
	afx_msg void OnEditSlices();
	afx_msg void OnUpdateEditSlices(CCmdUI *pCmdUI);
	afx_msg void OnViewSliceNext();
	afx_msg void OnUpdateViewSliceNext(CCmdUI *pCmdUI);
	afx_msg void OnViewSlicePrevious();
	afx_msg void OnUpdateViewSlicePrevious(CCmdUI *pCmdUI);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewSliceNextActivate();
	afx_msg void OnUpdateViewSliceNextActivate(CCmdUI *pCmdUI);
	afx_msg void OnViewSlicePreviousActivate();
	afx_msg void OnUpdateViewSlicePreviousActivate(CCmdUI *pCmdUI);
	void GlDefaultMaterial(void);
	void LoadGridSurface(CGrid* pGrd, CGlSurface* pSrf, int nClrType=CTBL_FLD, double min=1.0f, double max=1.0f, BOOL bRelief=FALSE);
	afx_msg void OnEditField();
	afx_msg void OnViewShowField();
	afx_msg void OnUpdateViewShowField(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in CViewGL.cpp
inline CInv3DDoc* CViewGL::GetDocument() const
   { return reinterpret_cast<CInv3DDoc*>(m_pDocument); }
#endif

