// field of bodies

#include "math.h"
#include "GravMagFormulae.h"

#define GRAVCONST 6.67259e-11
#define PI 3.1415926535897932384626433832795

//____________________________________________________________________________________________________
/*
	computes gravity field of a prism Gz
	z axis posive downward
	ht, hb: top and bottom of a prism
	xl, xr: left, right coordinate of a prism
	this version adds field to previous fld
*/
void PrismGrav2D_Gz(double& fld, double xPt, double zPt, double ht, double hb, double xl, double xr) 
{
	fld += 1.0e5*GRAVCONST*((xr-xPt)*log((xr-xPt)*(xr-xPt)+hb*hb)+2*hb*atan((xr-xPt)/hb)-
	   (xl-xPt)*log((xl-xPt)*(xl-xPt)+hb*hb)-2*hb*atan((xl-xPt)/hb)-
	   (xr-xPt)*log((xr-xPt)*(xr-xPt)+ht*ht)-2*ht*atan((xr-xPt)/ht)+
	   (xl-xPt)*log((xl-xPt)*(xl-xPt)+ht*ht)+2*ht*atan((xl-xPt)/ht));   
}

double PrismGrav2D_Gz(double xPt, double zPt, double ht, double hb, double xl, double xr) 
{
	double fld;
	fld = 1.0e5*GRAVCONST*((xr-xPt)*log((xr-xPt)*(xr-xPt)+hb*hb)+2*hb*atan((xr-xPt)/hb)-
	   (xl-xPt)*log((xl-xPt)*(xl-xPt)+hb*hb)-2*hb*atan((xl-xPt)/hb)-
	   (xr-xPt)*log((xr-xPt)*(xr-xPt)+ht*ht)-2*ht*atan((xr-xPt)/ht)+
	   (xl-xPt)*log((xl-xPt)*(xl-xPt)+ht*ht)+2*ht*atan((xl-xPt)/ht));   

	return fld;
}
//____________________________________________________________________________________________________


/*
	computes magnetic field of a prism Gz
	z axis posive downward
	ht, hb: top and bottom of a prism
	xl, xr: left, right coordinate of a prism
	this version adds field to previous fld
*/
// mg2DprdT(x:real;suszept:real;x1:real;x2:real;ht:real;h2:real;inkl:real;azim:real;T:real):real;  
double PrismMag2D_Gz(double xPt, double zPt, double ht, double hb, double xl, double xr, double incl, double azim, double indFld, double suscept=1.0) 
{
	double  r1, r2, r3, r4;
	double	theta1, theta2, theta3, theta4;
	double	c, MM, dZ, dH;
	double	fld;

	r1 = sqrt((xl-xPt)*(xl-xPt)+ht*ht); 
	r2 = sqrt((xr-xPt)*(xr-xPt)+ht*ht);
	r3 = sqrt((xl-xPt)*(xl-xPt)+hb*hb); 
	r4 = sqrt((xr-xPt)*(xr-xPt)+hb*hb);

	theta1 = atan((xl-xPt)/ht);   
	theta2 = atan((xr-xPt)/ht);
	theta3 = atan((xl-xPt)/hb);   
	theta4 = atan((xr-xPt)/hb);

	indFld = indFld/(400.0*PI);
	incl = incl*PI/180.0;
	azim = azim*PI/180.0;
	c = 4.0*PI*0.0000001/(4.0*PI);     
	MM = suscept*indFld;
	dZ = -2*c*MM*1000000000*((theta1-theta2-theta3+theta4)*sin(incl)+
		log((r1*r4)/(r2*r3))*cos(incl)*cos(azim));
	dH = -2*c*MM*1000000000*(log((r1*r4)/(r2*r3))*sin(incl)-
		(theta1-theta2-theta3+theta4)*cos(incl)*cos(azim)); 
	fld = dZ*sin(incl)+dH*cos(incl)*cos(azim);

	return fld;
}

void PrismMag2D_Gz(double& fld, double& Z, double& H, double xPt, double zPt, double ht, double hb, double xl, double xr, double incl, double azim, double indFld,  double suscept=1.0) 
{
	double  r1, r2, r3, r4;
	double	theta1, theta2, theta3, theta4;
	double	c, MM;

	r1 = sqrt((xl-xPt)*(xl-xPt)+ht*ht); 
	r2 = sqrt((xr-xPt)*(xr-xPt)+ht*ht);
	r3 = sqrt((xl-xPt)*(xl-xPt)+hb*hb); 
	r4 = sqrt((xr-xPt)*(xr-xPt)+hb*hb);

	theta1 = atan((xl-xPt)/ht);   
	theta2 = atan((xr-xPt)/ht);
	theta3 = atan((xl-xPt)/hb);   
	theta4 = atan((xr-xPt)/hb);

	indFld = indFld/(400.0*PI);
	incl = incl*PI/180.0;
	azim = azim*PI/180.0;
	c = 4.0*PI*0.0000001/(4.0*PI);     
	MM = suscept*indFld;
	Z = -2*c*MM*1000000000*((theta1-theta2-theta3+theta4)*sin(incl)+
		log((r1*r4)/(r2*r3))*cos(incl)*cos(azim));
	H = -2*c*MM*1000000000*(log((r1*r4)/(r2*r3))*sin(incl)-
		(theta1-theta2-theta3+theta4)*cos(incl)*cos(azim)); 
	fld = Z*sin(incl) + H*cos(incl)*cos(azim);
}
