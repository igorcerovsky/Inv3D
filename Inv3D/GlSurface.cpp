#include "stdafx.h"
#include "glsurface.h"

CGlSurface::CGlSurface(void)
{
	m_nTexWidth = 1024;
	m_nTexFormat = 4;	// GL_RGBA
	m_fAlpha = 0.4f;
	m_fAlphaShade = 1.0f;
	
	m_nSrfFormat = 7;	// {x,y,z, nx,ny,nz, fColorIndex} == {0,1,2, 3,4,5, 6}

	m_bSolid = TRUE;
	m_bVisible = TRUE;
	m_nDispLst = -1;

	m_nRows = -1;
	m_nCols = -1;

	m_nPlacement = plcNone;
}

CGlSurface::~CGlSurface(void)
{
	glDeleteLists(m_nDispLst, 1);
	glDeleteTextures(1, &m_nTex);
	m_srfData.RemoveAll();
}

int CGlSurface::IsEmpty(void)
{
	if(m_nCols==-1 || m_nRows==-1) 
		return TRUE;
	return 0;
}

void CGlSurface::UpdateTexture()
{
	ASSERT(wglGetCurrentContext() != NULL);

	if( glIsTexture(m_nTex) )
		glDeleteTextures(1, &m_nTex);

	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, m_nTex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, m_nTexFormat, m_nTexWidth, 0, GL_RGBA, GL_FLOAT, m_tex1D.GetData());
	glDisable(GL_TEXTURE_1D);
}

void CGlSurface::DrawGL(BOOL bRel)
{
	ASSERT(wglGetCurrentContext() != NULL);

	if(!m_bVisible) return;

	if( m_srfData.IsEmpty() || m_tex1D.IsEmpty()) return;
	//if( !IsVisible() )	return;

	float*	fTex1D;
	double*	r;
	double	xm, ym, zm, dh;	// coordinates and colorIndex of middle point
	int		n00, n01, n11, n10;
	double  dz = m_dMax-m_dMin;
	double	t;

	if( !glIsTexture(m_nTex) ) 
		UpdateTexture();

	ASSERT( glIsTexture(m_nTex) );

	// GL hints
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_1D);

	if(m_fAlpha != 1.0f) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_BLEND);
	}

	glBindTexture(GL_TEXTURE_1D, m_nTex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	r = m_srfData.GetData();
	fTex1D = m_tex1D.GetData();
	
	if(m_bSolid)		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else	{
		glLineWidth(1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glColor4f(1.0f, 1.0f, 1.0f, m_fAlpha);
	for(int i=0; i<m_nRows-1; i ++) {
		for(int j=0; j<m_nCols-1; j++) {
			n00 = m_nSrfFormat*(i*m_nCols+j);
			n01 = m_nSrfFormat*(i*m_nCols+j+1);
			n11 = m_nSrfFormat*((i+1)*m_nCols+j+1);
			n10 = m_nSrfFormat*((i+1)*m_nCols+j);

			if(	r[n00+1]!=DUMMY && r[n01+1]!=DUMMY && r[n11+1]!=DUMMY && r[n10+1]!=DUMMY &&
				r[n00+6]!=DUMMY && r[n01+6]!=DUMMY && r[n11+6]!=DUMMY && r[n10+6]!=DUMMY) 
			{
				glBegin(GL_TRIANGLES);
					// middle point
					xm = ( r[n00 + 0] + r[n01 + 0] + r[n11 + 0] + r[n10 + 0] ) / 4.0f;
					ym = ( r[n00 + 1] + r[n01 + 1] + r[n11 + 1] + r[n10 + 1] ) / 4.0f;
					zm = ( r[n00 + 2] + r[n01 + 2] + r[n11 + 2] + r[n10 + 2] ) / 4.0f;
					// middle color
					dh = (r[n00 + 6] + r[n01 + 6] + r[n11 + 6] + r[n10 + 6] ) / 4.0f;

					// triangle 1
					t= (r[n00 + 6]-m_dMin)/dz;	glTexCoord1d(t);		
					glVertex3d(r[n00 + 0],	GetH(r[n00 + 1], r[n00 + 6], m_dMin, m_dMax),	r[n00 + 2]);
					t= (r[n01 + 6]-m_dMin)/dz;	glTexCoord1d(t);		
					glVertex3d(r[n01 + 0],	GetH(r[n01 + 1], r[n01 + 6], m_dMin, m_dMax),	r[n01 + 2]);
					t= (dh-m_dMin)/dz;	glTexCoord1d(t);				
					glVertex3d(xm,	GetH(ym, dh, m_dMin, m_dMax),	zm);

					// triangle 2
					t= (r[n01 + 6]-m_dMin)/dz;	glTexCoord1d(t);		
					glVertex3d(r[n01 + 0],	GetH(r[n01 + 1], r[n01 + 6], m_dMin, m_dMax),	r[n01 + 2]);
					t= (r[n11 + 6]-m_dMin)/dz;	glTexCoord1d(t);		
					glVertex3d(r[n11 + 0],	GetH(r[n11 + 1], r[n11 + 6], m_dMin, m_dMax),	r[n11 + 2]);
					t= (dh-m_dMin)/dz;	glTexCoord1d(t);				
					glVertex3d(xm,	GetH(ym, dh, m_dMin, m_dMax),	zm);

					// triangle 3
					t= (r[n11 + 6]-m_dMin)/dz;	glTexCoord1d(t);		
					glVertex3d(r[n11 + 0],	GetH(r[n11 + 1], r[n11 + 6], m_dMin, m_dMax),	r[n11 + 2]);
					t= (r[n10 + 6]-m_dMin)/dz;	glTexCoord1d(t);		
					glVertex3d(r[n10 + 0],	GetH(r[n10 + 1], r[n10 + 6], m_dMin, m_dMax),	r[n10 + 2]);
					t= (dh-m_dMin)/dz;	glTexCoord1d(t);				
					glVertex3d(xm,	GetH(ym, dh, m_dMin, m_dMax),	zm);

					// triangle 4
					t= (r[n10 + 6]-m_dMin)/dz;	glTexCoord1d(t);
					glVertex3d(r[n10 + 0],	GetH(r[n10 + 1], r[n10 + 6], m_dMin, m_dMax),	r[n10 + 2]);
					t= (r[n00 + 6]-m_dMin)/dz;	glTexCoord1d(t);
					glVertex3d(r[n00 + 0],	GetH(r[n00 + 1], r[n00 + 6], m_dMin, m_dMax),	r[n00 + 2]);
					t= (dh-m_dMin)/dz;	glTexCoord1d(t);
					glVertex3d(xm,	GetH(ym, dh, m_dMin, m_dMax),	zm);
				glEnd();
			}
		}
	}
	glDisable(GL_TEXTURE_1D);
}


void CGlSurface::DrawGlShade()
{
	ASSERT(wglGetCurrentContext() != NULL);

	if(!m_bVisible) return;

	if( m_srfData.IsEmpty() || m_tex1D.IsEmpty()) return;
	//if( !IsVisible() )	return;

	double*	r;
	double	xm, ym, zm, dh;			// coordinates and colorIndex of middle point
	double	nxm, nym, nzm;			// middle normal
	int		n00, n01, n11, n10;
	double  dz = m_dMax-m_dMin;

	// GL settings
	//glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(m_bSolid==1)	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLineWidth(1.0f);
	glColor4d(1.0, 1.0, 1.0, 1.0);

	float matEmF[] = {0.4f, 0.4f, 0.4f, m_fAlphaShade};
	float matEmB[] = {0.0f, 0.0f, 0.0f, m_fAlphaShade};
	float matAb[] = {0.2f, 0.2f, 0.2f, m_fAlphaShade};
	float matDf[] = {0.8f, 0.8f, 0.8f, m_fAlphaShade};
	float matSpec[4] = {0.7f, 0.7f, 0.7f, 0.1f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmF);
	//glMaterialfv(GL_BACK, GL_EMISSION, matEmB);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDf);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 120.0f);

	r = m_srfData.GetData();
	for(int i=0; i<m_nRows-1; i ++) {
		for(int j=0; j<m_nCols-1; j++) {
			n00 = m_nSrfFormat*(i*m_nCols+j);
			n01 = m_nSrfFormat*(i*m_nCols+j+1);
			n11 = m_nSrfFormat*((i+1)*m_nCols+j+1);
			n10 = m_nSrfFormat*((i+1)*m_nCols+j);

			if(	r[n00+1]!=DUMMY && r[n01+1]!=DUMMY && r[n11+1]!=DUMMY && r[n10+1]!=DUMMY &&
				r[n00+6]!=DUMMY && r[n01+6]!=DUMMY && r[n11+6]!=DUMMY && r[n10+6]!=DUMMY) 
			{
				glBegin(GL_TRIANGLES);
					// middle point
					xm = ( r[n00 + 0] + r[n01 + 0] + r[n11 + 0] + r[n10 + 0] ) / 4.0f;
					ym = ( r[n00 + 1] + r[n01 + 1] + r[n11 + 1] + r[n10 + 1] ) / 4.0f;
					zm = ( r[n00 + 2] + r[n01 + 2] + r[n11 + 2] + r[n10 + 2] ) / 4.0f;
					// middle normal
					nxm = (r[n00 + 3] + r[n01 + 3] + r[n11 + 3] + r[n10 + 3] ) / 4.0f;
					nym = (r[n00 + 4] + r[n01 + 4] + r[n11 + 4] + r[n10 + 4] ) / 4.0f;
					nzm = (r[n00 + 5] + r[n01 + 5] + r[n11 + 5] + r[n10 + 5] ) / 4.0f;
					dh = (r[n00 + 6] + r[n01 + 6] + r[n11 + 6] + r[n10 + 6] ) / 4.0f;

					// triangle 1
					glNormal3d(r[n00 + 3],	r[n00 + 4], r[n00 + 5] );		
					glVertex3d(r[n00 + 0],	GetH(r[n00 + 1], r[n00 + 6], m_dMin, m_dMax),	r[n00 + 2]);

					glNormal3d(r[n01 + 3],	r[n01 + 4], r[n01 + 5] );		
					glVertex3d(r[n01 + 0],	GetH(r[n01 + 1], r[n01 + 6], m_dMin, m_dMax),	r[n01 + 2]);

					glNormal3d(nxm,	nym, nzm );		
					glVertex3d(xm,	GetH(ym, dh, m_dMin, m_dMax),	zm);

					// triangle 2
					glNormal3d(r[n01 + 3],	r[n01 + 4], r[n01 + 5] );		
					glVertex3d(r[n01 + 0],	GetH(r[n01 + 1], r[n01 + 6], m_dMin, m_dMax),	r[n01 + 2]);
					glNormal3d(r[n11 + 3],	r[n11 + 4], r[n11 + 5] );		
					glVertex3d(r[n11 + 0],	GetH(r[n11 + 1], r[n11 + 6], m_dMin, m_dMax),	r[n11 + 2]);
					glNormal3d(nxm,	nym, nzm );		
					glVertex3d(xm,	GetH(ym, dh, m_dMin, m_dMax),	zm);

					// triangle 3
					glNormal3d(r[n11 + 3],	r[n11 + 4], r[n11 + 5] );		
					glVertex3d(r[n11 + 0],	GetH(r[n11 + 1], r[n11 + 6], m_dMin, m_dMax),	r[n11 + 2]);
					glNormal3d(r[n10 + 3],	r[n10 + 4], r[n10 + 5] );		
					glVertex3d(r[n10 + 0],	GetH(r[n10 + 1], r[n10 + 6], m_dMin, m_dMax),	r[n10 + 2]);
					glNormal3d(nxm,	nym, nzm );		
					glVertex3d(xm,	GetH(ym, dh, m_dMin, m_dMax),	zm);

					// triangle 4
					glNormal3d(r[n10 + 3],	r[n10 + 4], r[n10 + 5] );		
					glVertex3d(r[n10 + 0],	GetH(r[n10 + 1], r[n10 + 6], m_dMin, m_dMax),	r[n10 + 2]);
					glNormal3d(r[n00 + 3],	r[n00 + 4], r[n00 + 5] );		
					glVertex3d(r[n00 + 0],	GetH(r[n00 + 1], r[n00 + 6], m_dMin, m_dMax),	r[n00 + 2]);
					glNormal3d(nxm,	nym, nzm );		
					glVertex3d(xm,	GetH(ym, dh, m_dMin, m_dMax),	zm);
				glEnd();
			}
		}
	}
}
