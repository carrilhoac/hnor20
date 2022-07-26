
/////////////////////////////////////////////////////////////////////
//
// Autor    Andre Caceres Carrilho
// Data     25 jul 2022
//
/////////////////////////////////////////////////////////////////////

#ifndef __BICUBIC_H__
#define __BICUBIC_H__

#include "mat4.h"

struct Point {
	double x, y;	
};

struct CubicSpline {
public:
	Vector 		_parms;
	
	void 		Fit(const Point& p1, const Point& p2, const Point& p3, const Point& p4);
	double		Eval(double x) const;	
};

#endif 
