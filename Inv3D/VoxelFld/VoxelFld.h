#pragma once

#include "Facet3Pt.h"

// CVoxelFld command target

class CVoxelFld : public CObject
{
public:
	CVoxelFld();
	virtual ~CVoxelFld();

protected:
	CFacet3Pt	fctT1, fctT2;
	CFacet3Pt	fctB1, fctB2;
	CFacet3Pt	fctE1, fctE2;
	CFacet3Pt	fctW1, fctW2;
	CFacet3Pt	fctN1, fctN2;
	CFacet3Pt	fctS1, fctS2;

	// voxel side lengths
	double dx;
	double dy;
	double dz;

	// voxel centroid
	double cx;
	double cy;
	double cz;

public:
	void Init();
	void Init(double dx, double dy, double dz);
	void SetCentroid(double x, double y, double z)		{cx=x; cy=y; cz=z;}
	void SetDimensions(double x, double y, double z)	{dx=x; dy=y; dz=z;}
	void FldGrav(double rx, double ry, double rz, double& gz);
};


