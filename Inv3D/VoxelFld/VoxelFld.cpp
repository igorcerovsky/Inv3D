// C:\! Igor\Inv3D\Inv3D\VoxelFld\VoxelFld.cpp : implementation file
//

#include "stdafx.h"
#include "VoxelFld.h"
#include "point3d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CVoxelFld

CVoxelFld::CVoxelFld()
{
	// voxel side lengths
	dx = 0;
	dy = 0;
	dz = 0;

	// voxel centroid
	cx = 0;
	cy = 0;
	cz = 0;
}

CVoxelFld::~CVoxelFld()
{
}


void CVoxelFld::Init(double xSide, double ySide, double zSide)
{
	dx = xSide;
	dy = ySide;
	dz = zSide;
	Init();
}

// CVoxelFld member functions
void CVoxelFld::Init()
{
	// top facet 1
	fctT1.pts[0].x = cx - dx/2.0;
	fctT1.pts[0].y = cy - dy/2.0;
	fctT1.pts[0].z = cz;

	fctT1.pts[1].x = cx + dx/2.0;
	fctT1.pts[1].y = cy - dy/2.0;
	fctT1.pts[1].z = cz;

	fctT1.pts[2].x = cx + dx/2.0;
	fctT1.pts[2].y = cy + dy/2.0;
	fctT1.pts[2].z = cz;
	fctT1.Init();

	// top facet 2
	fctT2.pts[0].x = cx - dx/2.0;
	fctT2.pts[0].y = cy - dy/2.0;
	fctT2.pts[0].z = cz;

	fctT2.pts[1].x = cx + dx/2.0;
	fctT2.pts[1].y = cy + dy/2.0;
	fctT2.pts[1].z = cz;

	fctT2.pts[2].x = cx - dx/2.0;
	fctT2.pts[2].y = cy + dy/2.0;
	fctT2.pts[2].z = cz;
	fctT2.Init();

	// bottom facet 1
	fctB1.pts[0].x = cx - dx/2.0;
	fctB1.pts[0].y = cy - dy/2.0;
	fctB1.pts[0].z = cz - dz;

	fctB1.pts[1].x = cx + dx/2.0;
	fctB1.pts[1].y = cy + dy/2.0;
	fctB1.pts[1].z = cz - dz;

	fctB1.pts[2].x = cx + dx/2.0;
	fctB1.pts[2].y = cy - dy/2.0;
	fctB1.pts[2].z = cz - dz;
	fctB1.Init();

	// top facet 2
	fctB2.pts[0].x = cx - dx/2.0;
	fctB2.pts[0].y = cy - dy/2.0;
	fctB2.pts[0].z = cz - dz;

	fctB2.pts[1].x = cx - dx/2.0;
	fctB2.pts[1].y = cy + dy/2.0;
	fctB2.pts[1].z = cz - dz;
	fctB2.Init();

	fctB2.pts[2].x = cx + dx/2.0;
	fctB2.pts[2].y = cy + dy/2.0;
	fctB2.pts[2].z = cz - dz;
	fctB2.Init();

	// south facet 1 ____________
	fctS1.pts[0].x = cx - dx/2.0;
	fctS1.pts[0].y = cy - dy/2.0;
	fctS1.pts[0].z = cz;

	fctS1.pts[1].x = cx - dx/2.0;
	fctS1.pts[1].y = cy - dy/2.0;
	fctS1.pts[1].z = cz - dz;

	fctS1.pts[2].x = cx + dx/2.0;
	fctS1.pts[2].y = cy - dy/2.0;
	fctS1.pts[2].z = cz;
	fctS1.Init();

	// south facet 2
	fctS2.pts[0].x = cx - dx/2.0;
	fctS2.pts[0].y = cy - dy/2.0;
	fctS2.pts[0].z = cz - dz;

	fctS2.pts[1].x = cx + dx/2.0;
	fctS2.pts[1].y = cy - dy/2.0;
	fctS2.pts[1].z = cz - dz;

	fctS2.pts[2].x = cx + dx/2.0;
	fctS2.pts[2].y = cy - dy/2.0;
	fctS2.pts[2].z = cz;
	fctS2.Init();

	// north facet 1 _____________
	fctN1.pts[0].x = cx - dx/2.0;
	fctN1.pts[0].y = cy + dy/2.0;
	fctN1.pts[0].z = cz;

	fctN1.pts[2].x = cx - dx/2.0;
	fctN1.pts[2].y = cy + dy/2.0;
	fctN1.pts[2].z = cz - dz;

	fctN1.pts[1].x = cx + dx/2.0;
	fctN1.pts[1].y = cy + dy/2.0;
	fctN1.pts[1].z = cz;
	fctN1.Init();

	// north facet 2
	fctN2.pts[0].x = cx - dx/2.0;
	fctN2.pts[0].y = cy + dy/2.0;
	fctN2.pts[0].z = cz - dz;

	fctN2.pts[2].x = cx + dx/2.0;
	fctN2.pts[2].y = cy + dy/2.0;
	fctN2.pts[2].z = cz - dz;

	fctN2.pts[1].x = cx + dx/2.0;
	fctN2.pts[1].y = cy + dy/2.0;
	fctN2.pts[1].z = cz;
	fctN2.Init();

	// east facet 1 _____________
	fctE1.pts[0].x = cx + dx/2.0;
	fctE1.pts[0].y = cy - dy/2.0;
	fctE1.pts[0].z = cz;

	fctE1.pts[1].x = cx + dx/2.0;
	fctE1.pts[1].y = cy - dy/2.0;
	fctE1.pts[1].z = cz - dz;

	fctE1.pts[2].x = cx + dx/2.0;
	fctE1.pts[2].y = cy + dy/2.0;
	fctE1.pts[2].z = cz;
	fctE1.Init();

	// east facet 2
	fctE2.pts[0].x = cx + dx/2.0;
	fctE2.pts[0].y = cy - dy/2.0;
	fctE2.pts[0].z = cz - dz;

	fctE2.pts[1].x = cx + dx/2.0;
	fctE2.pts[1].y = cy - dy/2.0;
	fctE2.pts[1].z = cz - dz;

	fctE2.pts[2].x = cx + dx/2.0;
	fctE2.pts[2].y = cy + dy/2.0;
	fctE2.pts[2].z = cz;
	fctE2.Init();

	// west facet 1 _____________
	fctW1.pts[0].x = cx - dx/2.0;
	fctW1.pts[0].y = cy - dy/2.0;
	fctW1.pts[0].z = cz;

	fctW1.pts[1].x = cx - dx/2.0;
	fctW1.pts[1].y = cy + dy/2.0;
	fctW1.pts[1].z = cz;

	fctW1.pts[2].x = cx - dx/2.0;
	fctW1.pts[2].y = cy - dy/2.0;
	fctW1.pts[2].z = cz - dz;
	fctW1.Init();

	// west facet 2
	fctW2.pts[0].x = cx - dx/2.0;
	fctW2.pts[0].y = cy - dy/2.0;
	fctW2.pts[0].z = cz - dz;

	fctW2.pts[1].x = cx - dx/2.0;
	fctW2.pts[1].y = cy + dy/2.0;
	fctW2.pts[1].z = cz;

	fctW2.pts[2].x = cx - dx/2.0;
	fctW2.pts[2].y = cy - dy/2.0;
	fctW2.pts[2].z = cz - dz;
	fctW2.Init();
}

void CVoxelFld::FldGrav(double rx, double ry, double rz, double& gz)
{
	CPoint3D vr(rx, ry, rz);
	CPoint3D vg;

	fctT1.FldVlado( vr, vg );
	fctT2.FldVlado( vr, vg );
	fctB1.FldVlado( vr, vg );
	fctB2.FldVlado( vr, vg );
	fctS1.FldVlado( vr, vg );
	fctS2.FldVlado( vr, vg );
	fctN1.FldVlado( vr, vg );
	fctN2.FldVlado( vr, vg );
	fctE1.FldVlado( vr, vg );
	fctE2.FldVlado( vr, vg );
	fctW1.FldVlado( vr, vg );
	fctW2.FldVlado( vr, vg );

	gz += vg.z;
}

void CVoxelFld::FldGrav(double rx, double ry, double rz, double& gx, double& gy, double& gz)
{
	ASSERT(0);
}
