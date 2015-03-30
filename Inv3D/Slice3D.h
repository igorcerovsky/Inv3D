#pragma once

#include <afxtempl.h>


// CSlice3D command target

class CSlice3D : public CObject
{
public:
enum sliceTag {
	sliceXZ=0,			// slice parallel to the WORLD x axis ...
	sliceYZ=1,
	sliceXY=2
};

public:
	CSlice3D();
	virtual ~CSlice3D();

// variables
public:
	int nType;
	int nProf;
	float fAlpha;

// functions	
	int		GetType()				{return nType;}
	void	SetType(int type)		{nType=type;}

	int		GetProf()				{return nProf;}
	void	SetProf(int prof)		{nProf=prof;}

	float	GetAlpha()				{return fAlpha;}
	void	SetAlpha(float alpha)	{fAlpha=alpha;}
};

typedef CTypedPtrArray<CObArray, CSlice3D*> SlcPtrArray;


